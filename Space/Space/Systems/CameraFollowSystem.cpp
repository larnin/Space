#include "CameraFollowSystem.h"
#include "Components/CameraFollowComponent.h"
#include "Components/CameraFollowControlerComponent.h"
#include <NDK/Components/NodeComponent.hpp>

Ndk::SystemIndex CameraFollowSystem::systemIndex;

CameraFollowSystem::CameraFollowSystem()
{
	Requires<Ndk::NodeComponent, CameraFollowComponent, CameraFollowControlerComponent>();
}

void CameraFollowSystem::OnUpdate(float elapsedTime)
{
	for (const Ndk::EntityHandle& entity : GetEntities())
	{
		auto controles = entity->GetComponent<CameraFollowControlerComponent>().evaluateControles();
		auto & followComponent = entity->GetComponent<CameraFollowComponent>();
		followComponent.update(controles, elapsedTime);
	}
}
