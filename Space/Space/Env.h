#pragma once

#include <NDK/StateMachine.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>

struct Env
{
	inline Env(Ndk::StateMachine &_fsm, Nz::RenderWindow &_window)
		: fsm(_fsm), window(_window)
	{ }
	Ndk::StateMachine & fsm;
	Nz::RenderWindow & window;
};