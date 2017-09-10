#pragma once

#include <NDK/Component.hpp>
#include <Nazara/Math/Vector3.hpp>
#include <Nazara/Graphics/Model.hpp>
#include <Nazara/Utility/StaticMesh.hpp>
#include <Nazara/Utility/VertexDeclaration.hpp>
#include <Nazara/Utility/VertexStruct.hpp>
#include <map>

struct AsteroidParameters
{
	unsigned int seed = 0;
	unsigned int subdivisions = 3;
	Nz::Vector3f sphereScale = Nz::Vector3f(1, 1, 1);
	float period = 2;
	float amplitude = 1.0f;
	float amplitudeMultiplier = 0.5f;
	float amplitudeExp = 1.0f;
	int steps = 3;
	float scale = 1.0f;
	float scaleMultiplier = 2.0f;
	float damageResistance = 1.0f;
	std::string rockName;
	std::string brokenRockName;
	std::string oreName;
};

struct AsteroidVertexStruct : public Nz::VertexStruct_XYZ_Normal_UV_Tangent
{
	Nz::Vector2f factors; //x = destruction, y = ore
};

class AsteroidComponent : public Ndk::Component<AsteroidComponent>
{
public:
	AsteroidComponent();
	~AsteroidComponent() = default;

	static AsteroidComponent & create(Ndk::EntityHandle e, const AsteroidParameters & params);

	inline Nz::ModelRef getModel() { return m_model; };

	void damage(const Nz::Vector3f & relativePos, float value);

	static Ndk::ComponentIndex componentIndex;

private:
	void damageModel(const Nz::Vector3f & relativePos, float radius);

	static Nz::VertexDeclarationRef asteroidVertexDeclaration();
	static std::map<std::string, Nz::TextureRef> texturesList(const std::vector<std::string> & names);

	Nz::ModelRef m_lightModel;
	Nz::ModelRef m_blendModel;

	Nz::ModelRef m_model;
	unsigned int m_bufferSize;
	Nz::StaticMesh* m_mesh;
	float m_life;
	float m_damageResistance;
	unsigned int m_baseSubdivision;

	static Nz::VertexDeclarationRef m_vertexDeclariation;
	static std::map<std::string, Nz::TextureRef> m_rockTextures;
	static std::map<std::string, Nz::TextureRef> m_brokenRockTextures;
	static std::map<std::string, Nz::TextureRef> m_oreTextures;
};

