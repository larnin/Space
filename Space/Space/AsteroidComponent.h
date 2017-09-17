#pragma once

#include <NDK/Component.hpp>
#include <Nazara/Math/Vector3.hpp>
#include <Nazara/Utility/StaticMesh.hpp>
#include <Nazara/Utility/VertexStruct.hpp>
#include <map>

struct AsteroidVertexStruct : public Nz::VertexStruct_XYZ_Normal_UV_Tangent
{
	Nz::Vector2f factors; //x = destruction, y = ore
};

class AsteroidComponent : public Ndk::Component<AsteroidComponent>
{
public:
	AsteroidComponent();
	~AsteroidComponent() = default;

	void damage(const Nz::Vector3f & relativePos, float value);

	static inline Nz::VertexDeclarationRef vertexDeclaration() { return m_vertexDeclariation; }
	static Ndk::ComponentIndex componentIndex;

private:
	void damageModel(const Nz::Vector3f & relativePos, float radius);

	static Nz::VertexDeclarationRef asteroidVertexDeclaration();
	
	unsigned int m_bufferSize;
	Nz::StaticMesh* m_mesh;
	float m_life;
	float m_damageResistance;
	unsigned int m_baseSubdivision;

	static Nz::VertexDeclarationRef m_vertexDeclariation;
};

