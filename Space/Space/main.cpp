#include "States/GameState.h"
#include "Components/ShipControlerComponent.h"
#include "Components/FollowEntityComponent.h"
#include "Components/AsteroidComponent.h"
#include "Components/Animation2DComponent.h"
#include "Components/Animator2DComponent.h"
#include "Systems/ShipControlerSystem.h"
#include "Systems/FollowEntitySystem.h"
#include "Systems/Animation2DSystem.h"
#include "Systems/Animator2DSystem.h"
#include <NDK/Application.hpp>
#include <NDK/StateMachine.hpp>
#include <NDK/Components.hpp>
#include <Nazara/Renderer.hpp>
#include <memory>

#include <Nazara/Renderer/Shader.hpp>
#include <Nazara/Graphics/Material.hpp>
#include <Nazara/Platform/Keyboard.hpp>

void initializeCustomComponentsAndSystems()
{
	Ndk::InitializeComponent<ShipControlerComponent>("001SCC");
	Ndk::InitializeComponent<FollowEntityComponent>("002FEC");
	Ndk::InitializeComponent<AsteroidComponent>("003AsC");
	Ndk::InitializeComponent<Animation2DComponent>("004A2D");
	Ndk::InitializeComponent<Animator2DComponent>("005A2D");

	Ndk::InitializeSystem<ShipControlerSystem>();
	Ndk::InitializeSystem<FollowEntitySystem>();
	Ndk::InitializeSystem<Animation2DSystem>();
	Ndk::InitializeSystem<Animator2DSystem>();
}

void initializeShaders()
{
	Nz::UberShaderPreprocessorRef blendShader(Nz::UberShaderPreprocessor::New());
	blendShader->SetShaderFromFile(Nz::ShaderStageType::ShaderStageType_Vertex, "blend.vert", "FLAG_INSTANCING TRANSFORM UNIFORM_VERTEX_DEPTH TEXTURE_MAPPING");
	blendShader->SetShaderFromFile(Nz::ShaderStageType::ShaderStageType_Fragment, "blend.frag", "ALPHA_TEST AUTO_TEXCOORDS DIFFUSE_MAPPING NORMAL_MAPPING SPECULAR_MAPPING");
	Nz::UberShaderLibrary::Register("blendAsteroid", blendShader);
}

/*int main()
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
}*/

#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include "Components/Animation2DComponent.h"
#include "Animation2D/Animation2D.h"

int main()
{
	Ndk::Application application;
	initializeCustomComponentsAndSystems();

	Nz::RenderWindow& mainWindow = application.AddWindow<Nz::RenderWindow>();
	mainWindow.Create(Nz::VideoMode(800, 600, 32), "Test");
	mainWindow.SetFramerateLimit(60);
	mainWindow.EnableVerticalSync(true);

	auto & world = application.AddWorld();
	world.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());
	world.AddSystem<Animator2DSystem>();

	auto & camera = world.CreateEntity();
	auto & cameraComponent = camera->AddComponent<Ndk::CameraComponent>();
	cameraComponent.SetProjectionType(Nz::ProjectionType_Orthogonal);
	cameraComponent.SetSize(Nz::Vector2f(mainWindow.GetSize().x, mainWindow.GetSize().y));
	cameraComponent.SetTarget(&mainWindow);
	auto & cameraNodeComponent = camera->AddComponent<Ndk::NodeComponent>();
	cameraNodeComponent.SetPosition(-10.0f*cameraComponent.GetForward());
	
	auto & entity = world.CreateEntity();
	entity->AddComponent<Ndk::NodeComponent>();
	auto sprite = Nz::Sprite::New(Nz::Material::New("Translucent2D"));
	sprite->SetTexture("res/walkingdead.png");
	auto & graph = entity->AddComponent<Ndk::GraphicsComponent>();
	graph.Attach(sprite);

	auto anim = Animation2D::New("");
	for (unsigned int i(0); i < 10; i++)
		anim->addFrame(Frame(0.1f, Nz::Rectui(i*200, 0, 200, 312), Nz::Vector2f(0, 0), false, true));

	auto animator = Animator2D::New();
	animator->addAnimation(anim);
	auto & state1 = animator->addState("left", anim, 1, false, true);
	auto & state2 = animator->addState("right", anim, 1, true, true);
	state1.addTransition(Animation2DTransition(state2, std::make_unique<PropertyEqualCondition>("turn", 1)));
	state2.addTransition(Animation2DTransition(state1, std::make_unique<PropertyEqualCondition>("turn", 0)));
	animator->setDefaultStateName("left");

	auto & animatorComponent = entity->AddComponent<Animator2DComponent>(animator);
	animatorComponent.attach(sprite);

	while (application.Run())
	{
		if (Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Left))
			animatorComponent.setProperty("turn", 0);
		else if (Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Right))
			animatorComponent.setProperty("turn", 1);

		mainWindow.Display();
	}

	return EXIT_SUCCESS;
}