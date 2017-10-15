#include "Animation2DSystem.h"
#include "Animation2DComponent.h"

Ndk::SystemIndex Animation2DSystem::systemIndex;

Animation2DSystem::Animation2DSystem()
{
	Requires<Animation2DComponent>();
}

void Animation2DSystem::OnUpdate(float elapsedTime)
{
	for (const Ndk::EntityHandle& entity : GetEntities())
	{
		auto & animationComponent = entity->GetComponent<Animation2DComponent>();

		if (!animationComponent.update(elapsedTime))
			continue;

		const auto & f = animationComponent.getCurrentFrame();

		for (auto & sprite : animationComponent.m_sprites)
			Animation2DComponent::setFrame(sprite, f);
	}
}
