#include "ShipControlerComponent.h"

Ndk::ComponentIndex ShipControlerComponent::componentIndex;

ShipControlerComponent::ShipControlerComponent()
	: solidDrag(0)
	, fluidDrag(0)
	, acceleration(0)
	, m_rotationSpeed(0)
	, m_maxRotationSpeed(0)
	, m_rotationAcceleration(0)
{
}

void ShipControlerComponent::update(const Nz::Vector2f & constroles, Ndk::PhysicsComponent2D & physics)
{
	//do stuff
}