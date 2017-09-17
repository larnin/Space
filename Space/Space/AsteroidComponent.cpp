#include "AsteroidComponent.h"
#include <Nazara/Utility/Mesh.hpp>
#include <Nazara/Utility/BufferMapper.hpp>
#include <Nazara/Utility/VertexMapper.hpp>
#include <Nazara/Core/SparsePtr.hpp>
#include <Nazara/Core/OffsetOf.hpp>
#include <Nazara/Utility/IndexMapper.hpp>
#include <Nazara/Utility/IndexIterator.hpp>
#include <vector>

#include <iostream>

Ndk::ComponentIndex AsteroidComponent::componentIndex;
Nz::VertexDeclarationRef AsteroidComponent::m_vertexDeclariation(AsteroidComponent::asteroidVertexDeclaration());

AsteroidComponent::AsteroidComponent()
	: m_bufferSize(0)
	, m_mesh(nullptr)
	, m_life(0)
	, m_damageResistance(1)
	, m_baseSubdivision(0)
{
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

Nz::VertexDeclarationRef AsteroidComponent::asteroidVertexDeclaration()
{
	auto declaration = Nz::VertexDeclaration::New();
	declaration->EnableComponent(Nz::VertexComponent_Position, Nz::ComponentType_Float3, NazaraOffsetOf(AsteroidVertexStruct, position));
	declaration->EnableComponent(Nz::VertexComponent_Normal, Nz::ComponentType_Float3, NazaraOffsetOf(AsteroidVertexStruct, normal));
	declaration->EnableComponent(Nz::VertexComponent_TexCoord, Nz::ComponentType_Float2, NazaraOffsetOf(AsteroidVertexStruct, uv));
	declaration->EnableComponent(Nz::VertexComponent_Tangent, Nz::ComponentType_Float3, NazaraOffsetOf(AsteroidVertexStruct, tangent));

	declaration->EnableComponent(Nz::VertexComponent_Userdata0, Nz::ComponentType_Float2, NazaraOffsetOf(AsteroidVertexStruct, factors));

	NazaraAssert(declaration->GetStride() == sizeof(CustomVertex), "Invalid stride for declaration CustomVertex");

	return declaration;
}

struct CustomVertex : public Nz::VertexStruct_XYZ_Normal_UV_Tangent
{
	Nz::Vector2f factors; //x = destruction, y = ore
};