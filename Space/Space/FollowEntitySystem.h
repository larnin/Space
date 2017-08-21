#pragma once

#include <NDK/System.hpp>
#include <Nazara/Utility/Window.hpp>

class FollowEntitySystem : public Ndk::System<FollowEntitySystem>
{
public:
	FollowEntitySystem(Nz::Window & window);
	~FollowEntitySystem() = default;

	static Ndk::SystemIndex systemIndex;

protected:
	virtual void OnUpdate(float elapsedTime) override;

private:
	NazaraSlot(Nz::EventHandler, OnMouseMoved, mouseMovedEvent);
};

