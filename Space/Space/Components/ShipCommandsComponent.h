#pragma once

#include <NDK/Component.hpp>
#include <Nazara/Math/Vector2.hpp>

class ShipCommandsComponent : public Ndk::Component<ShipCommandsComponent>
{
public:
	ShipCommandsComponent();
	virtual ~ShipCommandsComponent() = default;

	Nz::Vector2f evaluateControles();

	static Ndk::ComponentIndex componentIndex;

private:
};

