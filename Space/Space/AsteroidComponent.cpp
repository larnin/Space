#include "AsteroidComponent.h"
#include <Nazara/Noise/Perlin.hpp>
#include <Nazara/Utility/Mesh.hpp>
#include <Nazara/Utility/StaticMesh.hpp>
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

	Nz::VertexMapper mapper(sphere);
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

	return comp;
}