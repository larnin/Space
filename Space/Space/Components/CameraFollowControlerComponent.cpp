#include "CameraFollowControlerComponent.h"

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
	return { m_zoomOffset, m_offset };
}
#include <iostream>
void CameraFollowControlerComponent::onMouseMoved(const MouseMovedEvent & e)
{
	std::cout << e.value.x << " " << e.value.y << " * " << e.value.deltaX << " " << e.value.deltaY << std::endl;
}

void CameraFollowControlerComponent::onMouseWheelMoved(const MouseWheelMovedEvent & e)
{
	std::cout << e.value.delta << std::endl;
}

