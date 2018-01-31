#pragma once

#include <Nazara/Platform/Keyboard.hpp>
#include <Nazara/Platform/Mouse.hpp>
#include <vector>
#include <array>

enum class InputName
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	COUNT
};

class InputConfigs
{
	struct InputValue
	{
		enum class InputType
		{
			KEYBOARD_KEY,
			MOUSE_BUTTON,
			JOYSTICK_AXIS,
			JOYSTICK_BUTTON
		};

		InputType type;

		union InputValues
		{
			Nz::Keyboard::Key keyboardKey;
			Nz::Mouse::Button mouseButton;
			//add joystick
		};

		InputValues inputValues;
	};

public:
	using InputKeys = std::vector<InputValue>;

	InputConfigs();
	~InputConfigs() = default;

private:
	void generateDefaultConfigs();

	std::array<InputKeys, static_cast<size_t>(InputName::COUNT)> m_keys;
};

