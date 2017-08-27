#pragma once

#include <NDK/Component.hpp>
#include <Nazara/Math/Vector3.hpp>
#include <Nazara/Graphics/Model.hpp>

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
};

class AsteroidComponent : public Ndk::Component<AsteroidComponent>
{
public:
	AsteroidComponent();
	~AsteroidComponent() = default;

	static AsteroidComponent & create(Ndk::EntityHandle e, const AsteroidParameters & params);

	static Ndk::ComponentIndex componentIndex;

	inline Nz::ModelRef getModel() { return m_model; };

private:
	Nz::ModelRef m_model;
	unsigned int m_bufferSize;
	Nz::VertexStruct_XYZ_Normal_UV_Tangent* m_buffer;
};

