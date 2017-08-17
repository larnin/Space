#include "ShipControlerSystem.h"
#include "ShipControlerComponent.h"
#include <NDK/Components/PhysicsComponent2D.hpp>
#include <Nazara/Utility/Keyboard.hpp>

Ndk::SystemIndex ShipControlerSystem::systemIndex;

float signedAngle(const Nz::Vector2f & vect)
{
	return atan2(vect.y, vect.x) * 180.0f / M_PI;
}

ShipControlerSystem::ShipControlerSystem()
{
	Requires<Ndk::PhysicsComponent2D, ShipControlerComponent>();
}

void ShipControlerSystem::OnUpdate(float elapsedTime)
{
	for (const Ndk::EntityHandle& entity : GetEntities())
	{
		auto & shipComponent = entity->GetComponent<ShipControlerComponent>();
		auto & physicComponent = entity->GetComponent<Ndk::PhysicsComponent2D>();
		auto & controls = shipComponent.getControls();
		Nz::Vector2f dir(0, 0);
		if (Nz::Keyboard::IsKeyPressed(controls.left))
			dir.x--;
		if (Nz::Keyboard::IsKeyPressed(controls.right))
			dir.x++;
		if (Nz::Keyboard::IsKeyPressed(controls.up))
			dir.y++;
		if (Nz::Keyboard::IsKeyPressed(controls.down))
			dir.y--;

		dir *= elapsedTime;
		physicComponent.SetVelocity(physicComponent.GetVelocity() + dir);
		physicComponent.SetRotation(signedAngle(physicComponent.GetVelocity()));
	}
}