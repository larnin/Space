#include "FollowEntitySystem.h"
#include "FollowEntityComponent.h"
#include "Utilities.h"
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/PhysicsComponent2D.hpp>

Ndk::SystemIndex FollowEntitySystem::systemIndex;

namespace
{
	const float maxZoomTime(0.5f);
	const float mouseDistanceMultiplier(0.5f);

	float zoomHeight(float level)
	{
		const float base(sqrt(2.0f));
		return pow(base, level);
	}
}

FollowEntitySystem::FollowEntitySystem(Nz::Window & window)
	: m_normalizedMousePos(0.0f, 0.0f)
	, m_zoomTime(0)
	, m_zoomLevel(3)
	, m_oldZoomLevel(3.0f)
{
	Requires<Ndk::NodeComponent, FollowEntityComponent>();

	auto & handler = window.GetEventHandler();
	mouseMovedEvent.Connect(handler.OnMouseMoved,
		[&normalizedMousePos = m_normalizedMousePos, &window](const Nz::EventHandler*, const Nz::WindowEvent::MouseMoveEvent& event)
	{
		normalizedMousePos = Nz::Vector2f(event.x / (float)window.GetWidth() - 0.5f, -(event.y / (float)window.GetHeight() - 0.5f));
		normalizedMousePos.x = sign(normalizedMousePos.x) * normalizedMousePos.x * normalizedMousePos.x * 4;
		normalizedMousePos.y = sign(normalizedMousePos.y) * normalizedMousePos.y * normalizedMousePos.y * 4;
	});

	mouseWheelMovedEvent.Connect(handler.OnMouseWheelMoved,
		[&oldZoomLevel = m_oldZoomLevel, &zoomLevel = m_zoomLevel, &zoomTime = m_zoomTime](const Nz::EventHandler*, const Nz::WindowEvent::MouseWheelEvent& event)
	{
		oldZoomLevel = lerp((float)zoomLevel, oldZoomLevel, normalize(zoomTime, maxZoomTime, 0));
		zoomLevel = Nz::Clamp(zoomLevel + (event.delta > 0 ? 1 : -1), 1, 9);
		zoomTime = maxZoomTime;
	});
}

void FollowEntitySystem::OnUpdate(float elapsedTime)
{
	m_zoomTime = std::max(m_zoomTime - elapsedTime, 0.0f);		
	
	Nz::Vector3f offset(0, 0, 0);
	offset.z = zoomHeight(lerp((float)m_zoomLevel, m_oldZoomLevel, normalize(m_zoomTime, maxZoomTime, 0)));
	offset.x = m_normalizedMousePos.x * offset.z * mouseDistanceMultiplier;
	offset.y = m_normalizedMousePos.y * offset.z * mouseDistanceMultiplier;

	for (const Ndk::EntityHandle& entity : GetEntities())
	{
		auto & followComponent = entity->GetComponent<FollowEntityComponent>();
		if (!followComponent.entity || !followComponent.entity->HasComponent<Ndk::NodeComponent>())
			continue;
		auto & entityNodeComponent = followComponent.entity->GetComponent<Ndk::NodeComponent>();;
		auto & nodeComponent = entity->GetComponent<Ndk::NodeComponent>();

		nodeComponent.SetPosition(entityNodeComponent.GetPosition() + offset);
	}
}