#pragma once

#include <NDK/System.hpp>

class Animation2DSystem : public Ndk::System<Animation2DSystem>
{
public:
	Animation2DSystem();
	~Animation2DSystem() = default;

	static Ndk::SystemIndex systemIndex;

protected:
	virtual void OnUpdate(float elapsedTime) override;
};

