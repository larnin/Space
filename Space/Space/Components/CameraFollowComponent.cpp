#include "CameraFollowComponent.h"
#include "Env.h"
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/CameraComponent.hpp>

Ndk::ComponentIndex CameraFollowComponent::componentIndex;

std::vector<float> generateZoomLevels()
{
	return { 1.0f, 1.414f, 2.0f, 2.828f, 4.0f, 5.65f, 8.0f };
}

std::vector<float> CameraFollowComponent::m_zoomLevels(generateZoomLevels());

CameraFollowComponent::CameraFollowComponent(Ndk::EntityHandle target)
	: m_target(target)
	, m_currentZoom(1)
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
#include <iostream>
void CameraFollowComponent::update(const CommandsInfos & commands, float elapsedTime)
{
	const float offsetMultiplier = 200;

	auto & node = GetEntity()->GetComponent<Ndk::NodeComponent>();
	auto & camera = GetEntity()->GetComponent<Ndk::CameraComponent>();

	if (commands.zoomOffset != 0)
	{
		if ((int)m_currentZoom + commands.zoomOffset < 0)
			m_currentZoom = 0;
		else if (m_currentZoom + commands.zoomOffset >= m_zoomLevels.size())
			m_currentZoom = m_zoomLevels.size() - 1;
		else m_currentZoom += commands.zoomOffset;

		auto size = Env::instance().window().GetSize();
		camera.SetSize(size.x / m_zoomLevels[m_currentZoom], size.y / m_zoomLevels[m_currentZoom]);
	}

	auto & targetNode = m_target->GetComponent<Ndk::NodeComponent>();
	node.SetPosition(targetNode.GetPosition() - camera.GetSize() / 2.0f + commands.offset * offsetMultiplier / m_zoomLevels[m_currentZoom]);
}
