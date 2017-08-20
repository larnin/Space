#include "FollowEntitySystem.h"
#include "FollowEntityComponent.h"
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/PhysicsComponent2D.hpp>

Ndk::SystemIndex FollowEntitySystem::systemIndex;

FollowEntitySystem::FollowEntitySystem()
{
	Requires<Ndk::NodeComponent, FollowEntityComponent>();
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