#include "FollowEntitySystem.h"
#include "FollowEntityComponent.h"
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/PhysicsComponent2D.hpp>

#include <iostream>

Ndk::SystemIndex FollowEntitySystem::systemIndex;

FollowEntitySystem::FollowEntitySystem(Nz::Window & window)
{
	Requires<Ndk::NodeComponent, FollowEntityComponent>();

	auto & handler = window.GetEventHandler();
	mouseMovedEvent.Connect(handler.OnMouseMoved,
		[](const Nz::EventHandler*, const Nz::WindowEvent::MouseMoveEvent& event)
	{
		std::cout << event.x << " " << event.y << " " << event.x << " " << event.y << std::endl;
	});
}

void FollowEntitySystem::OnUpdate(float elapsedTime)
{
	for (const Ndk::EntityHandle& entity : GetEntities())
	{
		auto & followComponent = entity->GetComponent<FollowEntityComponent>();
		if (!followComponent.entity || !followComponent.entity->HasComponent<Ndk::NodeComponent>())
			continue;
		auto & entityNodeComponent = followComponent.entity->GetComponent<Ndk::NodeComponent>();;
		auto & nodeComponent = entity->GetComponent<Ndk::NodeComponent>();

		nodeComponent.SetPosition(entityNodeComponent.GetPosition() + followComponent.offset);
	}
}
