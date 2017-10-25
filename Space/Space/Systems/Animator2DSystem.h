#pragma once

#include <NDK/System.hpp>

class Animator2DSystem : public Ndk::System<Animator2DSystem>
{
public:
	Animator2DSystem();
	~Animator2DSystem() = default;

	static Ndk::SystemIndex systemIndex;

protected:
	virtual void OnUpdate(float elapsedTime) override;
};

