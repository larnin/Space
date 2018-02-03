#pragma once

#include <NDK/Application.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>

class Env
{
public:
	Env();
	~Env();
	Env(const Env &) = delete;
	Env & operator=(const Env &) = delete;

	void window(Nz::RenderWindow & w);
	Nz::RenderWindow & window();

	void application(Ndk::Application & app);
	Ndk::Application & application();

	static Env& instance();
private:
	Nz::RenderWindow* m_window;
	Ndk::Application* m_application;

	static Env* m_instance;
};