#include "ShipControlerComponent.h"
#include "Utilities/vect2ext.h"
#include <NDK/Components/PhysicsComponent2D.hpp>
#include <cmath>

Ndk::ComponentIndex ShipControlerComponent::componentIndex;

namespace
{
	float offsetAngle(float current, float target)
	{
		current = fmod(current, 360.0f);
		if (current < 0) current += 360.0f;
		target = fmod(target, 360.0f);
		if (target < 0)
			target += 360.0f;

		float angle = target - current;
		if (angle > 180.0f)
			return angle - 360.0f;
		if (angle < -180.0f)
			return 360.0f + angle;
		return angle;
	}
}

float sign(float value)
{
	return value >= 0 ? 1.0f : -1.0f;
}

ShipControlerComponent::ShipControlerComponent()
	: solidDrag(0)
	, fluidDrag(0)
	, acceleration(0)
	, maxRotationSpeed(0)
	, rotationAcceleration(0)
	, m_rotationSpeed(0)
{
}

void ShipControlerComponent::update(const Nz::Vector2f & constroles, float elapsedTime)
{
	auto & physics = GetEntity()->GetComponent<Ndk::PhysicsComponent2D>();
	float rotation = physics.GetRotation();

	if (abs(m_rotationSpeed) > maxRotationSpeed)
	{
		float newRotationSpeed = m_rotationSpeed - sign(m_rotationSpeed) * rotationAcceleration * elapsedTime;
		if (abs(newRotationSpeed) < maxRotationSpeed)
			newRotationSpeed = maxRotationSpeed * sign(newRotationSpeed);
		m_rotationSpeed = newRotationSpeed;
	}

	float currentAcceleration(0);
	float targetRotation = angle(constroles) * 180.0f / M_PI;
	float a = offsetAngle(rotation, targetRotation);

	if ((constroles.x != 0 || constroles.y != 0) && (sign(a) != sign(m_rotationSpeed) || abs(m_rotationSpeed * m_rotationSpeed / (rotationAcceleration*2.0f)) < abs(a)))
	{
		float currentRotationAcceleration = rotationAcceleration;
		if (abs(a) < 20 && sign(m_rotationSpeed) == sign(a))
			currentRotationAcceleration *= abs(a) / 20;

		float newRotationSpeed = m_rotationSpeed + sign(a) * currentRotationAcceleration * elapsedTime;
		if (abs(newRotationSpeed) > maxRotationSpeed)
			newRotationSpeed = maxRotationSpeed * sign(newRotationSpeed);

		if (currentRotationAcceleration < 0.1f && abs(newRotationSpeed) < 1.0f)
		{
			newRotationSpeed = 0;
			physics.SetRotation(targetRotation);
		}

		m_rotationSpeed = newRotationSpeed;

	}
	else if (m_rotationSpeed != 0)
	{
		float newRotationSpeed = m_rotationSpeed - sign(m_rotationSpeed) * rotationAcceleration * elapsedTime;
		if (sign(newRotationSpeed) != sign(m_rotationSpeed))
			newRotationSpeed = 0;
		m_rotationSpeed = newRotationSpeed;
	}

	if (constroles.x != 0 || constroles.y != 0)
		currentAcceleration = (1 - (a / 180.0f)*(a / 180.0f)) * acceleration;
	physics.SetRotation(physics.GetRotation() + m_rotationSpeed * elapsedTime);

	Nz::Vector2f accelerationDir(currentAcceleration * cos(physics.GetRotation() / 180.0f * float(M_PI)), currentAcceleration * sin(physics.GetRotation() / 180.0f * float(M_PI)));

	Nz::Vector2f velocity = physics.GetVelocity() + accelerationDir;
	float velocityNorm = velocity.GetLength();
	velocityNorm *= (1 - fluidDrag);
	velocityNorm = std::max(velocityNorm - solidDrag, 0.0f);
	velocity = velocity.Normalize() * velocityNorm;
	physics.SetVelocity(velocity);
}