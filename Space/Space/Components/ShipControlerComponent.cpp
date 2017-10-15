#include "Components/ShipControlerComponent.h"

Ndk::ComponentIndex ShipControlerComponent::componentIndex;

ShipControlerComponent::ShipControlerComponent(const Controls & c)
	: m_controls(c)
	, solidDrag(0)
	, fluidDrag(0)
	, acceleration(0)
	, rotationSpeed(0)
	, maxRotationSpeed(0)
	, rotationAcceleration(0)
{

}
