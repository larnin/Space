#include "GameState.h"
#include "ShipControlerComponent.h"
#include "FollowEntityComponent.h"
#include "AsteroidComponent.h"
#include "ShipControlerSystem.h"
#include "FollowEntitySystem.h"
#include <NDK/Application.hpp>
#include <NDK/StateMachine.hpp>
#include <NDK/Components.hpp>
#include <Nazara/Renderer.hpp>
#include <memory>

#include <Nazara/Renderer/Shader.hpp>
#include <Nazara/Graphics/Material.hpp>

void initializeCustomComponentsAndSystems()
{
	Ndk::InitializeComponent<ShipControlerComponent>("001SCC");
	Ndk::InitializeComponent<FollowEntityComponent>("002FEC");
	Ndk::InitializeComponent<AsteroidComponent>("003AsC");

	Ndk::InitializeSystem<ShipControlerSystem>();
	Ndk::InitializeSystem<FollowEntitySystem>();
}
#include <iostream>
int main()
{
	Ndk::Application application;
	initializeCustomComponentsAndSystems();

	Nz::RenderWindow& mainWindow = application.AddWindow<Nz::RenderWindow>();
	mainWindow.Create(Nz::VideoMode(800, 600, 32), "Test");
	mainWindow.SetFramerateLimit(60);
	mainWindow.EnableVerticalSync(true);

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