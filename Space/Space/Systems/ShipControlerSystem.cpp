#include "ShipControlerSystem.h"
#include "Components/ShipCommandsComponent.h"
#include "Components/ShipControlerComponent.h"
#include <Ndk/Components/PhysicsComponent2D.hpp>

Ndk::SystemIndex ShipControlerSystem::systemIndex;

ShipControlerSystem::ShipControlerSystem()
{
	Requires<Ndk::PhysicsComponent2D, ShipCommandsComponent, ShipControlerComponent>();
}

void ShipControlerSystem::OnUpdate(float elapsedTime)
{
	for (const Ndk::EntityHandle& entity : GetEntities())
	{
		auto & shipComponent = entity->GetComponent<ShipControlerComponent>();
		auto & shipCommandsComponent = entity->GetComponent<ShipCommandsComponent>();
		auto & physicComponent = entity->GetComponent<Ndk::PhysicsComponent2D>();

		shipComponent.update(shipCommandsComponent.evaluateControles(), physicComponent, elapsedTime);
	}
}