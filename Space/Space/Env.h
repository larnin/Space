#pragma once

#include <NDK/Application.hpp>
#include <NDK/World.hpp>
#include <NDK/StateMachine.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>

struct Env
{
	inline Env(Ndk::Application &_app, Ndk::StateMachine &_fsm, Nz::RenderWindow &_window, Ndk::World & _world2D, Ndk::World & _world3D)
		: app(_app), fsm(_fsm), window(_window), world2D(_world2D), world3D(_world3D)
	{ }
	Ndk::Application & app;
	Ndk::StateMachine & fsm;
	Nz::RenderWindow & window;
	Ndk::World & world2D;
	Ndk::World & world3D;
};