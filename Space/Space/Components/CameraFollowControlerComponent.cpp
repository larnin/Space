#include "CameraFollowControlerComponent.h"
#include "Env.h"

 Ndk::ComponentIndex CameraFollowControlerComponent::componentIndex;

CameraFollowControlerComponent::CameraFollowControlerComponent()
	: m_mouseMoveHolder(Event<MouseMovedEvent>::connect([this](auto e) {onMouseMoved(e); }))
	, m_mouseWhellMovedHolder(Event<MouseWheelMovedEvent>::connect([this](auto e) {onMouseWheelMoved(e); }))
	, m_offset(0, 0)
	, m_zoomOffset(0)
{
	
	
}

CameraFollowControlerComponent::CameraFollowControlerComponent(const CameraFollowControlerComponent & c)
	: m_mouseMoveHolder(Event<MouseMovedEvent>::connect([this](auto e) {onMouseMoved(e); }))
	, m_mouseWhellMovedHolder(Event<MouseWheelMovedEvent>::connect([this](auto e) {onMouseWheelMoved(e); }))
	, m_offset(c.m_offset)
	, m_zoomOffset(c.m_zoomOffset)
{

}

CameraFollowControlerComponent::CameraFollowControlerComponent(CameraFollowControlerComponent && c)
	: m_mouseMoveHolder(Event<MouseMovedEvent>::connect([this](auto e) {onMouseMoved(e); }))
	, m_mouseWhellMovedHolder(Event<MouseWheelMovedEvent>::connect([this](auto e) {onMouseWheelMoved(e); }))
	, m_offset(std::move(c.m_offset))
	, m_zoomOffset(std::move(c.m_zoomOffset))
{

}

CommandsInfos CameraFollowControlerComponent::evaluateControles()
{
	CommandsInfos value{ m_zoomOffset, m_offset };
	m_zoomOffset = 0;
	return value;
}
#include <iostream>
void CameraFollowControlerComponent::onMouseMoved(const MouseMovedEvent & e)
{
	const float maxDistance = 200;
	auto screenSize = Env::instance().window().GetSize();

	Nz::Vector2f offset((int)e.value.x - screenSize.x / 2.0f, (int)e.value.y - screenSize.y / 2.0f);
	if (offset.GetSquaredLength() > maxDistance * maxDistance)
		m_offset = offset.Normalize();
	else m_offset = offset / maxDistance;
}

void CameraFollowControlerComponent::onMouseWheelMoved(const MouseWheelMovedEvent & e)
{
	m_zoomOffset += e.value.delta;
}

