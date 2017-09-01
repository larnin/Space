#include "AsteroidComponent.h"
#include <Nazara/Noise/Perlin.hpp>
#include <Nazara/Utility/Mesh.hpp>
#include <Nazara/Utility/BufferMapper.hpp>
#include <Nazara/Utility/VertexMapper.hpp>
#include <Nazara/Core/SparsePtr.hpp>
#include <vector>

#include <iostream>

Ndk::ComponentIndex AsteroidComponent::componentIndex;

AsteroidComponent::AsteroidComponent()
{
}

AsteroidComponent & AsteroidComponent::create(Ndk::EntityHandle e, const AsteroidParameters & params)
{
	auto mesh = Nz::Mesh::New();
	mesh->CreateStatic();
	auto sphere = mesh->BuildSubMesh(Nz::Primitive::IcoSphere(1, params.subdivisions));
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
	auto model = Nz::Model::New();
	model->SetMesh(mesh);

	auto mat = model->GetMaterial(0);
	mat->SetShader("PhongLighting");
	mat->SetFaceFilling(Nz::FaceFilling_Fill);

	auto & comp = e->AddComponent<AsteroidComponent>();
	comp.m_model = model;
	comp.m_bufferSize = bufferSize;
	comp.m_mesh = sphereMesh;
	comp.m_life = params.sphereScale.x * params.sphereScale.y * params.sphereScale.z;
	comp.m_damageResistance = params.damageResistance;

	return comp;
}

#include <iostream>
void AsteroidComponent::damage(const Nz::Vector3f & relativePos, float value)
{
	value /= m_damageResistance;
	m_life -= value;

	damageModel(relativePos, value);
}

void AsteroidComponent::damageModel(const Nz::Vector3f & relativePos, float radius)
{
	Nz::VertexMapper mapper(m_mesh);
	Nz::SparsePtr<Nz::Vector3f> position = mapper.GetComponentPtr<Nz::Vector3f>(Nz::VertexComponent_Position);

	for (unsigned int i(0); i < m_bufferSize; i++)
	{
		auto pos = *position;

		float angle = relativePos.AngleBetween(pos) / radius;
		if (angle < 1.0f)
		{
			angle *= angle;
			angle = (angle - 1) / 3;

			pos = (pos.GetLength() + angle) * Nz::Vector3f::Normalize(pos);
			*position = pos;
		}
		position++;
	}

	mapper.Unmap();
	m_mesh->GenerateNormalsAndTangents();
	m_mesh->GenerateAABB();
}

#include <Nazara/Utility/VertexDeclaration.hpp>
#include <Nazara/Utility/VertexStruct.hpp>
#include <Nazara/Core/OffsetOf.hpp>
#include <Nazara/Utility/IndexMapper.hpp>
#include <Nazara/Utility/IndexIterator.hpp>

struct CustomVertex : public Nz::VertexStruct_XYZ_Normal_UV_Tangent
{
	Nz::Vector2f factors; //x = destruction, y = ore
};

Nz::ModelRef createThing()
{
	auto declaration = Nz::VertexDeclaration::New();
	declaration->EnableComponent(Nz::VertexComponent_Position, Nz::ComponentType_Float3, NazaraOffsetOf(CustomVertex, position));
	declaration->EnableComponent(Nz::VertexComponent_Normal, Nz::ComponentType_Float3, NazaraOffsetOf(CustomVertex, normal));
	declaration->EnableComponent(Nz::VertexComponent_TexCoord, Nz::ComponentType_Float2, NazaraOffsetOf(CustomVertex, uv));
	declaration->EnableComponent(Nz::VertexComponent_Tangent, Nz::ComponentType_Float3, NazaraOffsetOf(CustomVertex, tangent));

	declaration->EnableComponent(Nz::VertexComponent_Userdata0, Nz::ComponentType_Float2, NazaraOffsetOf(CustomVertex, factors));

	NazaraAssert(declaration->GetStride() == sizeof(CustomVertex), "Invalid stride for declaration CustomVertex");

	auto indexBuffer= Nz::IndexBuffer::New(false, 36, Nz::DataStorage_Hardware, 0);
	auto vertexBuffer= Nz::VertexBuffer::New(declaration, 8, Nz::DataStorage_Hardware, 0);

	Nz::VertexMapper vertexMapper(vertexBuffer, Nz::BufferAccess_WriteOnly);

	Nz::SparsePtr<Nz::Vector3f> position = vertexMapper.GetComponentPtr<Nz::Vector3f>(Nz::VertexComponent_Position);
	Nz::SparsePtr<Nz::Vector2f> factors = vertexMapper.GetComponentPtr<Nz::Vector2f>(Nz::VertexComponent_Userdata0);
	// and others

	position[0].Set(-1, -1, -1);
	position[1].Set(-1, -1,  1);
	position[2].Set(-1,  1, -1);
	position[3].Set(-1,  1,  1);
	position[4].Set( 1, -1, -1);
	position[5].Set( 1, -1,  1);
	position[6].Set( 1,  1, -1);
	position[7].Set( 1,  1,  1);

	for (int i = 0; i < 8; i++)
		factors[i].Set(0, 0);
	factors[0].x = 1;
	factors[7].y = 1;

	Nz::IndexMapper indexMapper(indexBuffer, Nz::BufferAccess_WriteOnly);
	auto i = indexMapper.begin();

	*i++ = 0; *i++ = 3; *i++ = 2;
	*i++ = 0; *i++ = 1; *i++ = 3;

	*i++ = 0; *i++ = 5; *i++ = 6;
	*i++ = 0; *i++ = 4; *i++ = 5;

	*i++ = 0; *i++ = 2; *i++ = 6;
	*i++ = 0; *i++ = 6; *i++ = 4;

	*i++ = 7; *i++ = 5; *i++ = 4;
	*i++ = 7; *i++ = 4; *i++ = 6;

	*i++ = 7; *i++ = 6; *i++ = 2;
	*i++ = 7; *i++ = 2; *i++ = 3;

	*i++ = 7; *i++ = 3; *i++ = 1;
	*i++ = 7; *i++ = 1; *i++ = 5;

	vertexMapper.Unmap();
	indexMapper.Unmap();

	auto mesh = Nz::Mesh::New();
	auto subMesh = Nz::StaticMesh::New(mesh);
	if (!subMesh->Create(vertexBuffer))
		assert(false);

	subMesh->GenerateAABB();
	subMesh->SetIndexBuffer(indexBuffer);
	subMesh->GenerateNormalsAndTangents();
	subMesh->SetPrimitiveMode(Nz::PrimitiveMode_TriangleList);
	subMesh->SetMaterialIndex(0);

	mesh->CreateStatic();
	mesh->AddSubMesh(subMesh);

	auto model = Nz::Model::New();
	model->SetMesh(mesh);

	auto mat = model->GetMaterial(0);
	mat->SetFaceCulling(Nz::FaceSide_FrontAndBack);
	mat->SetFaceFilling(Nz::FaceFilling_Fill);

	return model;
}