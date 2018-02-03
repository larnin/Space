#include "Env.h"

Env* Env::m_instance = nullptr;

Env::Env()
	: m_window(nullptr)
	, m_application(nullptr)
{
	NazaraAssert(!m_instance, "Two instance of Env instanciated !");
	m_instance = this;
}

Env::~Env()
{
	m_instance = nullptr;
}

void Env::window(Nz::RenderWindow & w)
{
	m_window = &w;
}

Nz::RenderWindow & Env::window()
{
	NazaraAssert(m_window, "No window.");
	return *m_window;
}

void Env::application(Ndk::Application & app)
{
	m_application = &app;
}

Ndk::Application & Env::application()
{
	NazaraAssert(m_application, "No application");
	return *m_application;
}

Env& Env::instance()
{
	NazaraAssert(m_instance, "The instance is null !");
	return *m_instance;
}
