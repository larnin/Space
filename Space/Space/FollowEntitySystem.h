#pragma once

#include <NDK/System.hpp>
#include <Nazara/Utility/Window.hpp>

class FollowEntitySystem : public Ndk::System<FollowEntitySystem>
{
public:
	FollowEntitySystem(Nz::Window & window);
	inline FollowEntitySystem(const FollowEntitySystem&) { NazaraAssert(false, "Can't clone this system !"); }
	~FollowEntitySystem() = default;

	static Ndk::SystemIndex systemIndex;

protected:
	virtual void OnUpdate(float elapsedTime) override;

private:
	NazaraSlot(Nz::EventHandler, OnMouseMoved, mouseMovedEvent);
	NazaraSlot(Nz::EventHandler, OnMouseWheelMoved, mouseWheelMovedEvent);
	
	Nz::Vector2f m_normalizedMousePos;
	float m_zoomTime;
	int m_zoomLevel;
	float m_oldZoomLevel;
};

