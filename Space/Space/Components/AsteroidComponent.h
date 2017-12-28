#pragma once
#include <NDK/Component.hpp>
#include <Nazara/Math/Vector2.hpp>

using Shape = std::vector<Nz::Vector2f>;

class AsteroidComponent : public Ndk::Component<AsteroidComponent>
{
public:
	AsteroidComponent();

	virtual ~AsteroidComponent() = default;

	void damage(const Nz::Vector2f & pos, float radius);

	static Ndk::ComponentIndex componentIndex;

private:
	Shape m_shape;

};

