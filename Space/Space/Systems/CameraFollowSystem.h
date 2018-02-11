#pragma once

#include <NDK/System.hpp>

class CameraFollowSystem : public Ndk::System<CameraFollowSystem>
{
public:
	CameraFollowSystem();
	virtual ~CameraFollowSystem() = default;

	static Ndk::SystemIndex systemIndex;

protected:
	virtual void OnUpdate(float elapsedTime) override;
};

