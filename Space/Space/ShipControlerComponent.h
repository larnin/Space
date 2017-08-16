#pragma once
#include <NDK/Component.hpp>

class ShipControlerComponent : public Ndk::Component<ShipControlerComponent>
{
public:
	ShipControlerComponent();
	virtual ~ShipControlerComponent() = default;
	
	static Ndk::ComponentIndex componentIndex;
};

