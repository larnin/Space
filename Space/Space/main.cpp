#include "GameState.h"
#include "ShipControlerComponent.h"
#include "FollowEntityComponent.h"
#include "AsteroidComponent.h"
#include "Animation2DComponent.h"
#include "ShipControlerSystem.h"
#include "FollowEntitySystem.h"
#include "Animation2DSystem.h"
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
	Ndk::InitializeComponent<Animation2DComponent>("004A2D");

	Ndk::InitializeSystem<ShipControlerSystem>();
	Ndk::InitializeSystem<FollowEntitySystem>();
	Ndk::InitializeSystem<Animation2DSystem>();
}

void initializeShaders()
{
	Nz::UberShaderPreprocessorRef blendShader(Nz::UberShaderPreprocessor::New());
	blendShader->SetShaderFromFile(Nz::ShaderStageType::ShaderStageType_Vertex, "blend.vert", "FLAG_INSTANCING TRANSFORM UNIFORM_VERTEX_DEPTH TEXTURE_MAPPING");
	blendShader->SetShaderFromFile(Nz::ShaderStageType::ShaderStageType_Fragment, "blend.frag", "ALPHA_TEST AUTO_TEXCOORDS DIFFUSE_MAPPING NORMAL_MAPPING SPECULAR_MAPPING");
	Nz::UberShaderLibrary::Register("blendAsteroid", blendShader);
}

int main()
{
	Ndk::Application application;
	initializeCustomComponentsAndSystems();
	initializeShaders();

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