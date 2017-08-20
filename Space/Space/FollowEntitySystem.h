#pragma once

#include <NDK/System.hpp>

class FollowEntitySystem : public Ndk::System<FollowEntitySystem>
{
public:
	FollowEntitySystem();
	~FollowEntitySystem() = default;

	static Ndk::SystemIndex systemIndex;

protected:
	virtual void OnUpdate(float elapsedTime) override;

private:

};

