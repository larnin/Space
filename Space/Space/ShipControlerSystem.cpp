#include "ShipControlerSystem.h"
#include "ShipControlerComponent.h"
#include <NDK/Components/ParticleEmitterComponent.hpp>
#include <NDK/Components/PhysicsComponent2D.hpp>
#include <Nazara/Utility/Keyboard.hpp>
#include <algorithm>

Ndk::SystemIndex ShipControlerSystem::systemIndex;

float sign(float value)
{
	return value >= 0 ? 1.0f : -1.0f;
}

float signedAngle(const Nz::Vector2f & vect)
{
	return atan2(vect.y, vect.x) * 180.0f / float(M_PI);
}

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

ShipControlerSystem::ShipControlerSystem()
{
	Requires<Ndk::PhysicsComponent2D, Ndk::ParticleEmitterComponent, ShipControlerComponent>();
}

void ShipControlerSystem::OnUpdate(float elapsedTime)
{
	for (const Ndk::EntityHandle& entity : GetEntities())
	{
		auto & shipComponent = entity->GetComponent<ShipControlerComponent>();
		auto & physicComponent = entity->GetComponent<Ndk::PhysicsComponent2D>();
		auto & particleComponent = entity->GetComponent<Ndk::ParticleEmitterComponent>();
		auto & controls = shipComponent.getControls();
		
		float rotation = physicComponent.GetRotation();

		Nz::Vector2f dir(0, 0);
		if (Nz::Keyboard::IsKeyPressed(controls.left))
			dir.x--;
		if (Nz::Keyboard::IsKeyPressed(controls.right))
			dir.x++;
		if (Nz::Keyboard::IsKeyPressed(controls.up))
			dir.y++;
		if (Nz::Keyboard::IsKeyPressed(controls.down))
			dir.y--;

		if (abs(shipComponent.rotationSpeed) > shipComponent.maxRotationSpeed)
		{
			float newRotationSpeed = shipComponent.rotationSpeed - sign(shipComponent.rotationSpeed) * shipComponent.rotationAcceleration * elapsedTime;
			if (abs(newRotationSpeed) < shipComponent.maxRotationSpeed)
				newRotationSpeed = shipComponent.maxRotationSpeed * sign(newRotationSpeed);
			shipComponent.rotationSpeed = newRotationSpeed;
		}

		float acceleration(0);
		float targetRotation = signedAngle(dir);
		float a = offsetAngle(rotation, targetRotation);

		if ((dir.x != 0 || dir.y != 0) && (sign(a) != sign(shipComponent.rotationSpeed) || abs(shipComponent.rotationSpeed * shipComponent.rotationSpeed / (shipComponent.rotationAcceleration*2.0f)) < abs(a) ))
		{ 
			float rotationAcceleration = shipComponent.rotationAcceleration;
			if (abs(a) < 20 && sign(shipComponent.rotationSpeed) == sign(a))
				rotationAcceleration *= abs(a) / 20;

			float newRotationSpeed = shipComponent.rotationSpeed + sign(a) * rotationAcceleration * elapsedTime;
			if (abs(newRotationSpeed) > shipComponent.maxRotationSpeed)
				newRotationSpeed = shipComponent.maxRotationSpeed * sign(newRotationSpeed);

			if (rotationAcceleration < 0.1f && abs(newRotationSpeed) < 1.0f)
			{
				newRotationSpeed = 0;
				physicComponent.SetRotation(targetRotation);
			}

			shipComponent.rotationSpeed = newRotationSpeed;
			
		}
		else if(shipComponent.rotationSpeed != 0)
		{
			float newRotationSpeed = shipComponent.rotationSpeed - sign(shipComponent.rotationSpeed) * shipComponent.rotationAcceleration * elapsedTime;
			if (sign(newRotationSpeed) != sign(shipComponent.rotationSpeed))
				newRotationSpeed = 0;
			shipComponent.rotationSpeed = newRotationSpeed;
		}

		if (dir.x != 0 || dir.y != 0)
		{
			acceleration = (1 - (a / 180.0f)*(a / 180.0f)) * shipComponent.acceleration;
			particleComponent.Enable(true);
		}
		else particleComponent.Enable(false);

		physicComponent.SetRotation(physicComponent.GetRotation() + shipComponent.rotationSpeed * elapsedTime);

		Nz::Vector2f accelerationDir(acceleration * cos(physicComponent.GetRotation() / 180.0f * float(M_PI)), acceleration * sin(physicComponent.GetRotation() / 180.0f * float(M_PI)));

		Nz::Vector2f velocity = physicComponent.GetVelocity() + accelerationDir;
		float velocityNorm = velocity.GetLength();
		velocityNorm *= (1 - shipComponent.fluidDrag);
		velocityNorm = std::max(velocityNorm - shipComponent.solidDrag, 0.0f);
		velocity = velocity.Normalize() * velocityNorm;
		physicComponent.SetVelocity(velocity);
	}
}