#include "GameState2.h"
#include "GameState.h"
#include "ShipControlerComponent.h"
#include "FollowEntityComponent.h"
#include "ShipControlerSystem.h"
#include <NDK/Application.hpp>
#include <NDK/StateMachine.hpp>
#include <NDK/World.hpp>
#include <NDK/Components.hpp>
#include <Nazara/Renderer.hpp>
#include <memory>

void initializeCustomComponentsAndSystems()
{
	Ndk::InitializeComponent<ShipControlerComponent>("001SCC");
	Ndk::InitializeComponent<FollowEntityComponent>("002FEC");

	Ndk::InitializeSystem<ShipControlerSystem>();
}

void initializeWorld(Ndk::World & world, const Nz::RenderTarget & window)
{
	Ndk::EntityHandle camera(world.CreateEntity());
	Ndk::NodeComponent& cameraNode = camera->AddComponent<Ndk::NodeComponent>();
	cameraNode.SetPosition(Nz::Vector3f(0, 0, 10));

	Ndk::CameraComponent& CameraComponent = camera->AddComponent<Ndk::CameraComponent>();
	CameraComponent.SetProjectionType(Nz::ProjectionType_Perspective);
	CameraComponent.SetTarget(&window);

	world.AddSystem<ShipControlerSystem>();
}

int main()
{
	Ndk::Application application;
	initializeCustomComponentsAndSystems();

	Nz::RenderWindow& mainWindow = application.AddWindow<Nz::RenderWindow>();
	mainWindow.Create(Nz::VideoMode(800, 600, 32), "Test");
	mainWindow.SetFramerateLimit(60);

	auto & world2D = application.AddWorld();
	auto & world3D = application.AddWorld();
	Ndk::StateMachine fsm{ std::shared_ptr<Ndk::State>() };

	fsm.ChangeState(std::make_shared<GameState>(Env(application, fsm, mainWindow, world2D, world3D)));

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