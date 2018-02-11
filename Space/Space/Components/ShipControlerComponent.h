#pragma once

#include <NDK/Component.hpp>
#include <Nazara/Math/Vector2.hpp>

class ShipControlerComponent : public Ndk::Component<ShipControlerComponent>
{
public:
	ShipControlerComponent();
	virtual ~ShipControlerComponent() = default;

	float solidDrag;
	float fluidDrag;
	float acceleration;
	float maxRotationSpeed;
	float rotationAcceleration;

	void update(const Nz::Vector2f & constroles, float elapsedTime);

	static Ndk::ComponentIndex componentIndex;

private:
	float m_rotationSpeed;
};

