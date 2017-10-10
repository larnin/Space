#include "AsteroidCreator.h"
#include "AsteroidComponent.h"
#include <Nazara/Graphics/Model.hpp>
#include <Nazara/Utility/StaticMesh.hpp>
#include <Nazara/Utility/VertexDeclaration.hpp>
#include <Nazara/Utility/VertexStruct.hpp>
#include <Nazara/Utility/BufferMapper.hpp>
#include <Nazara/Utility/VertexMapper.hpp>
#include <Nazara/Core/SparsePtr.hpp>
#include <Nazara/Core/OffsetOf.hpp>
#include <Nazara/Utility/IndexMapper.hpp>
#include <Nazara/Utility/IndexIterator.hpp>
#include <Nazara/Noise/Perlin.hpp>
#include <Nazara/Core/Primitive.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/GraphicsComponent.hpp>

#include <iostream>

AsteroidCreator::AsteroidCreator(Ndk::World & world)
	: m_world(world)
{
	loadTextures();
	initializeMaterials();
}

Ndk::EntityHandle AsteroidCreator::create(const AsteroidParameters & params, const Nz::Vector3f & pos)
{
	auto mesh = Nz::Mesh::New();
	mesh->CreateStatic();
	Nz::MeshParams meshParams;
	meshParams.vertexDeclaration = AsteroidComponent::vertexDeclaration();
	auto sphere = mesh->BuildSubMesh(Nz::Primitive::CubicSphere(1, params.subdivisions), meshParams);
	auto sphereMesh = dynamic_cast<Nz::StaticMesh*>(sphere);
	NazaraAssert(sphereMesh != nullptr, "The sphere is not a staticmesh !");

	auto bufferSize = sphereMesh->GetVertexCount();

	Nz::VertexMapper mapper(sphereMesh);
	Nz::SparsePtr<Nz::Vector3f> position = mapper.GetComponentPtr<Nz::Vector3f>(Nz::VertexComponent_Position);

	struct PerlinData
	{
		PerlinData(const Nz::Perlin & _perlin, float _amplitude, float _scale) : perlin(_perlin), amplitude(_amplitude), scale(_scale) {}
		Nz::Perlin perlin;
		float amplitude;
		float scale;
	};

	std::vector<PerlinData> perlins;
	float totalAmplitude(0);
	{
		float amplitude(params.amplitude);
		float scale(params.scale);
		for (unsigned int i(0); i < params.steps; i++)
		{
			perlins.emplace_back(Nz::Perlin(params.seed + i), amplitude, scale);
			totalAmplitude += amplitude;
			amplitude *= params.amplitudeMultiplier;
			scale *= params.scaleMultiplier;
		}
	}

	for (unsigned int i(0); i < bufferSize; i++)
	{
		auto pos = *position;

		pos.x *= params.sphereScale.x;
		pos.y *= params.sphereScale.y;
		pos.z *= params.sphereScale.z;

		float offset(0);
		for (const auto & p : perlins)
			offset += p.perlin.Get(pos.x, pos.y, pos.z, p.scale) * p.amplitude;

		offset += totalAmplitude / 2;
		offset /= totalAmplitude;
		offset = std::pow(offset, params.amplitudeExp);
		offset *= totalAmplitude;
		offset -= totalAmplitude / 2;
		offset *= params.amplitude / totalAmplitude;

		pos = (pos.GetLength() + offset) * Nz::Vector3f::Normalize(pos);

		*position = pos;
		position++;
	}

	mapper.Unmap();
	sphereMesh->GenerateNormalsAndTangents();
	sphereMesh->GenerateAABB();

	sphere->SetMaterialIndex(0);
	auto lightModel = Nz::Model::New();
	lightModel->SetMesh(mesh);
	auto textureModel = Nz::Model::New();
	textureModel->SetMesh(mesh);

	lightModel->SetMaterial(0, m_lightMaterial);
	textureModel->SetMaterial(0, m_materials[params.oreType]);

	auto entity = m_world.CreateEntity();
	auto & comp = entity->AddComponent<AsteroidComponent>();
	auto & graphicComponent = entity->AddComponent<Ndk::GraphicsComponent>();
	graphicComponent.Attach(lightModel);
	graphicComponent.Attach(textureModel);

	auto & nodeComponent = entity->AddComponent<Ndk::NodeComponent>();
	nodeComponent.SetPosition(pos);

	/*comp.m_model = model;
	comp.m_bufferSize = bufferSize;
	comp.m_mesh = sphereMesh;
	comp.m_life = params.sphereScale.x * params.sphereScale.y * params.sphereScale.z;
	comp.m_damageResistance = params.damageResistance;*/

	return entity;
}

void AsteroidCreator::loadTextures()
{
	const std::string dir("res/Asteroids/");

	std::map<TextureIndex, std::string> texturesNames{
		{ TextureIndex_Stone, "stone" },
		{ TextureIndex_Lightstone, "lightstone" },
		{ TextureIndex_Sandstone, "sandstone" },
		{ TextureIndex_Bedrock, "bedrock" },
		{ TextureIndex_Iron2, "iron2" },
		{ TextureIndex_Clay, "clay" },
		{ TextureIndex_Diamond, "diamond" },
		{ TextureIndex_Emerald, "emerald" },
		{ TextureIndex_Gold, "gold" },
		{ TextureIndex_Iron, "iron" },
		{ TextureIndex_Lapis, "lapis" },
		{ TextureIndex_Netherrack, "netherrack" },
		{ TextureIndex_Obsidian, "obsidian" },
		{ TextureIndex_Redstone, "redstone" },
		{ TextureIndex_Yellow, "yellow" } };

	for (const auto & n : texturesNames)
	{
		auto t = Nz::Texture::New();
		if (t->LoadFromFile(dir + n.second + ".png"))
			m_textures.emplace(n.first, t);
	}
}

void AsteroidCreator::initializeMaterials()
{
	m_lightMaterial = Nz::Material::New();
	m_lightMaterial->SetShader("PhongLighting");
	m_lightMaterial->SetFaceFilling(Nz::FaceFilling_Fill);
	m_lightMaterial->EnableShadowCasting(true);
	m_lightMaterial->EnableShadowReceive(true);

	for (unsigned int i(0); i <= OreType_Max; i++)
	{
		auto mat = Nz::Material::New();
		mat->SetShader("blendAsteroid");
		mat->SetDiffuseMap(OreTypeToStoneTexture((OreType)i));
		mat->SetSpecularMap(OreTypeToOreTexture((OreType)i));
		mat->SetNormalMap(OreTypeToBrokenStoneTexture((OreType)i));
		mat->EnableBlending(true);
		mat->SetSrcBlend(Nz::BlendFunc_DestColor);
		mat->SetDstBlend(Nz::BlendFunc_Zero);
		mat->EnableDepthWrite(true);
		mat->SetDepthFunc(Nz::RendererComparison_Equal);
		m_materials.push_back(mat);
	}
}

Nz::TextureRef AsteroidCreator::OreTypeToStoneTexture(OreType type)
{
	switch (type)
	{
	case OreType_Clay:
	case OreType_Yellow:
	case OreType_Emerald:
		return m_textures[TextureIndex_Lightstone];
	case OreType_Gold:
	case OreType_Lapis:
		return m_textures[TextureIndex_Sandstone];
	case OreType_obsidian:
	case OreType_Iron:
	case OreType_Redstone:
	case OreType_Diamond:
	case OreType_Netherrack:
		return m_textures[TextureIndex_Stone];
	default:
		NazaraAssert(false, "No texture defined for this type.");
		break;
	}

	return Nz::TextureRef();
}

Nz::TextureRef AsteroidCreator::OreTypeToBrokenStoneTexture(OreType type)
{
	switch (type)
	{
	case OreType_Clay:
	case OreType_Yellow:
	case OreType_Emerald:
	case OreType_Gold:
	case OreType_Lapis:
		return m_textures[TextureIndex_Iron2];
	case OreType_obsidian:
	case OreType_Iron:
	case OreType_Redstone:
	case OreType_Diamond:
	case OreType_Netherrack:
		return m_textures[TextureIndex_Bedrock];
	default:
		NazaraAssert(false, "No texture defined for this type.");
		break;
	}

	return Nz::TextureRef();
}

Nz::TextureRef AsteroidCreator::OreTypeToOreTexture(OreType type)
{
	switch (type)
	{
	case OreType_Clay:
		return m_textures[TextureIndex_Clay];
	case OreType_Yellow:
		return m_textures[TextureIndex_Yellow];
	case OreType_Emerald:
		return m_textures[TextureIndex_Emerald];
	case OreType_Gold:
		return m_textures[TextureIndex_Gold];
	case OreType_Lapis:
		return m_textures[TextureIndex_Lapis];
	case OreType_obsidian:
		return m_textures[TextureIndex_Obsidian];
	case OreType_Iron:
		return m_textures[TextureIndex_Iron];
	case OreType_Redstone:
		return m_textures[TextureIndex_Redstone];
	case OreType_Diamond:
		return m_textures[TextureIndex_Diamond];
	case OreType_Netherrack:
		return m_textures[TextureIndex_Netherrack];
	default:
		NazaraAssert(false, "No texture defined for this type.");
		break;
	}

	return Nz::TextureRef();
}

