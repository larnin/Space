
#include "Systems/Animator2DSystem.h"
#include "Components/Animator2DComponent.h"

Ndk::SystemIndex Animator2DSystem::systemIndex;

Animator2DSystem::Animator2DSystem()
{
	Requires<Animator2DComponent>();
}

void Animator2DSystem::OnUpdate(float elapsedTime)
{
	for (const Ndk::EntityHandle& entity : GetEntities())
	{
		auto & animatorComponent = entity->GetComponent<Animator2DComponent>();

		if (!animatorComponent.update(elapsedTime))
			continue;

		const auto & f = animatorComponent.getCurrentFrame();

		for (auto & sprite : animatorComponent.m_sprites)
			Animator2DComponent::setFrame(sprite, f);
	}
}
