#include "GameState.h"
#include "ShipControlerComponent.h"
#include "FollowEntityComponent.h"
#include "ShipControlerSystem.h"
#include "FollowEntitySystem.h"
#include <NDK/Application.hpp>
#include <NDK/StateMachine.hpp>
#include <NDK/Components.hpp>
#include <Nazara/Renderer.hpp>
#include <memory>

void initializeCustomComponentsAndSystems()
{
	Ndk::InitializeComponent<ShipControlerComponent>("001SCC");
	Ndk::InitializeComponent<FollowEntityComponent>("002FEC");

	Ndk::InitializeSystem<ShipControlerSystem>();
	Ndk::InitializeSystem<FollowEntitySystem>();
}

int main()
{
	Ndk::Application application;
	initializeCustomComponentsAndSystems();

	Nz::RenderWindow& mainWindow = application.AddWindow<Nz::RenderWindow>();
	mainWindow.Create(Nz::VideoMode(800, 600, 32), "Test");
	mainWindow.SetFramerateLimit(60);

	Ndk::StateMachine fsm{ std::shared_ptr<Ndk::State>() };

	fsm.ChangeState(std::make_shared<GameState>(Env(fsm, mainWindow)));

	while (application.Run())
	{
		if (!fsm.Update(application.GetUpdateTime()))
		{
			NazaraError("Failed to update state machine.");
			return EXIT_FAILURE;
		}

		mainWindow.Display();
	}

	return EXIT_SUCCESS;
}