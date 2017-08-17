#pragma once

#include <NDK/System.hpp>

class ShipControlerSystem : public Ndk::System<ShipControlerSystem>
{
public:
	ShipControlerSystem();
	virtual ~ShipControlerSystem() = default;

	static Ndk::SystemIndex systemIndex;

protected:
	virtual void OnUpdate(float elapsedTime) override;

private:

};

