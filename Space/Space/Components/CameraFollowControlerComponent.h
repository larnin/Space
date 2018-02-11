#pragma once

#include <NDK/Component.hpp>
#include <Nazara/Math/Vector2.hpp>
#include "Event/WindowEventArgs.h"
#include "Event/Event.h"

struct CommandsInfos
{
	int zoomOffset;
	Nz::Vector2f offset;
};

class CameraFollowControlerComponent : public Ndk::Component<CameraFollowControlerComponent>
{
public:
	CameraFollowControlerComponent();
	CameraFollowControlerComponent(const CameraFollowControlerComponent & c);
	CameraFollowControlerComponent(CameraFollowControlerComponent && c);
	virtual ~CameraFollowControlerComponent() = default;

	CommandsInfos evaluateControles();

	static Ndk::ComponentIndex componentIndex;

private:
	void onMouseMoved(const MouseMovedEvent & e);
	void onMouseWheelMoved(const MouseWheelMovedEvent & e);

	EventHolder<MouseMovedEvent> m_mouseMoveHolder;
	EventHolder<MouseWheelMovedEvent> m_mouseWhellMovedHolder;

	Nz::Vector2f m_offset;
	int m_zoomOffset;
};

