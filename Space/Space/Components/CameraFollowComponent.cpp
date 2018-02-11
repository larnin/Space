#include "CameraFollowComponent.h"

Ndk::ComponentIndex CameraFollowComponent::componentIndex;

CameraFollowComponent::CameraFollowComponent(Ndk::EntityHandle target)
	: m_target(target)
{
}

void CameraFollowComponent::setTarget(Ndk::EntityHandle target)
{
	m_target = target;
}

Ndk::EntityHandle CameraFollowComponent::getTarget() const
{
	return m_target;
}

void CameraFollowComponent::update(const CommandsInfos & commands, float elapsedTime)
{
}
