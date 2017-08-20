#pragma once
#include <NDK/Component.hpp>
#include <Nazara/Utility/Keyboard.hpp>

class ShipControlerComponent : public Ndk::Component<ShipControlerComponent>
{
public:
	
	struct Controls
	{
		Controls(Nz::Keyboard::Key _up, Nz::Keyboard::Key _down, Nz::Keyboard::Key _left, Nz::Keyboard::Key _right)
			: up(_up), down(_down), left(_left), right(_right){}

		Nz::Keyboard::Key up;
		Nz::Keyboard::Key down;
		Nz::Keyboard::Key left;
		Nz::Keyboard::Key right;
	};

	ShipControlerComponent(const Controls & c);
	virtual ~ShipControlerComponent() = default;

	static Ndk::ComponentIndex componentIndex;

	inline const Controls & getControls() const { return m_controls; }

	float solidDrag;
	float fluidDrag;
	float acceleration;

	float rotationSpeed;
	float maxRotationSpeed;
	float rotationAcceleration;
private:
	Controls m_controls;
};

