#pragma once

#include <NDK/Component.hpp>
#include <NDK/Components/PhysicsComponent2D.hpp>
#include <Nazara/Math/Vector2.hpp>

class ShipControlerComponent : public Ndk::Component<ShipControlerComponent>
{
public:
	ShipControlerComponent();
	virtual ~ShipControlerComponent() = default;

	float solidDrag;
	float fluidDrag;
	float acceleration;

	void update(const Nz::Vector2f & constroles, Ndk::PhysicsComponent2D & physics);

	static Ndk::ComponentIndex componentIndex;

private:
	float m_rotationSpeed;
	float m_maxRotationSpeed;
	float m_rotationAcceleration;
};

