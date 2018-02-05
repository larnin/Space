
#include <NDK/Application.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include <Nazara/Graphics/ColorBackground.hpp>
#include "Utilities/asteroidrender.h"
#include "Components/AsteroidComponent.h"
#include "Utilities/AsteroidBaseInfos.h"
#include "Components/AsteroidComponent.h"
#include "Components/ShipCommandsComponent.h"
#include "Components/ShipControlerComponent.h"
#include "Systems/ShipControlerSystem.h"
#include "Event/WindowEventsHolder.h"
#include "Env.h"
#include "Ship.h"

#include <iostream>

void initializeSystemsAndComponents()
{
	Ndk::InitializeComponent<AsteroidComponent>("001ASC");
	Ndk::InitializeComponent<ShipCommandsComponent>("002SCC");
	Ndk::InitializeComponent<ShipControlerComponent>("003SCC");

	Ndk::InitializeSystem<ShipControlerSystem>();
}

int main()
{
	Ndk::Application application;
	initializeSystemsAndComponents();
	AsteroidBaseInfos::createLibrary();

	Nz::RenderWindow& mainWindow = application.AddWindow<Nz::RenderWindow>();
	mainWindow.SetFramerateLimit(60);
	Env env;
	env.window(mainWindow);
	env.application(application);
	WindowEventsHolder windowEventHolder(mainWindow.GetEventHandler());
	
	mainWindow.Create(Nz::VideoMode(800, 600, 32), "Space");
	mainWindow.SetFramerateLimit(60);
	mainWindow.EnableVerticalSync(true);

	auto & world = application.AddWorld();
	world.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());
	world.GetSystem<Ndk::RenderSystem>().SetDefaultBackground(Nz::ColorBackground::New(Nz::Color::White));

	world.AddSystem<ShipControlerSystem>();

	ShipInfos shipInfos{ "res/Ship/1.png" };
	shipInfos.solidDrag = 0.1f;
	shipInfos.fluidDrag = 0.2f;
	shipInfos.acceleration = 60.0;
	shipInfos.maxRotationSpeed = 1000.0f;
	shipInfos.rotationAcceleration = 300.0f;
	createShip(world, shipInfos, Nz::Vector2f(100, 100), 0);

	auto & camera = world.CreateEntity();
	auto & cameraComponent = camera->AddComponent<Ndk::CameraComponent>();
	cameraComponent.SetProjectionType(Nz::ProjectionType_Orthogonal);
	cameraComponent.SetSize(Nz::Vector2f(mainWindow.GetSize().x, mainWindow.GetSize().y));
	cameraComponent.SetTarget(&mainWindow);
	auto & cameraNodeComponent = camera->AddComponent<Ndk::NodeComponent>();
	cameraNodeComponent.SetPosition(-10.0f*cameraComponent.GetForward());

	//Nz::ImageParams params;
	//params.loadFormat = Nz::PixelFormatType::PixelFormatType_RGBA8;
	//Nz::Image imgAsteroid;
	//imgAsteroid.LoadFromFile("res/Asteroids/asteroids0.png", params);
	//Nz::Image imgBorder;
	//imgBorder.LoadFromFile("res/Asteroids/explosion.png", params);
	//Nz::Image imgExplosedRock;
	//imgExplosedRock.LoadFromFile("res/Asteroids/border.png", params);
	//Nz::Image targetImage;

	//Nz::Clock c;
	//auto data = createAsteroidData(imgAsteroid);
	//std::cout << c.GetMicroseconds() << std::endl;
	//c.Restart();
	//createRender(targetImage, imgAsteroid, imgExplosedRock, imgBorder, data, { {30, {100, 20}}, {30, {20, 25}}, { 15, {60, 15}} });
	//std::cout << c.GetMicroseconds() << std::endl;

	///*createHole(imgAsteroid, Nz::Vector2i(100, 20), 30, imgBorder);
	//createHole(imgAsteroid, Nz::Vector2i(20, 50), 30, imgBorder);*/

	//auto tex = Nz::Texture::New(targetImage);

	//auto entity = world.CreateEntity();
	//auto & node = entity->AddComponent<Ndk::NodeComponent>();
	//node.SetPosition(200, 200);
	//auto sprite = Nz::Sprite::New(Nz::Material::New("Translucent2D"));
	//auto & t = sprite->GetMaterial()->GetDiffuseSampler();
	//t.SetFilterMode(Nz::SamplerFilter_Nearest);

	//sprite->SetTexture(tex);
	//sprite->SetSize(2*tex->GetSize().x, 2*tex->GetSize().y);
	//auto & graph = entity->AddComponent<Ndk::GraphicsComponent>();
	//graph.Attach(sprite);

	AsteroidBaseInfos::registerImage("Asteroid", "res/Asteroids/asteroids0.png");
	AsteroidBaseInfos::registerImage("Border", "res/Asteroids/explosion.png");
	AsteroidBaseInfos::registerImage("Explosion", "res/Asteroids/border.png");

	Shape s{ {27, 0}, {60, 0}, {96, 11}, {114, 43}, {114, 67}, {106, 84}, {92, 94}, {48, 94}, {22, 88}, {0, 63 }, {0, 28}, {10, 7} };
	AsteroidBaseInfos::registerInfos("Asteroid0", "Asteroid", "Explosion", "Border", s);
	auto e = AsteroidComponent::create(world, AsteroidBaseInfos::getAsteroidInfos("Asteroid0"), Nz::Vector2f(200, 200), 0);
	auto & comp = e->GetComponent<AsteroidComponent>();
	comp.damage(Nz::Vector2f(250, 200), 30, 100000);
	auto& phys = e->GetComponent<Ndk::PhysicsComponent2D>();

	while (application.Run())
	{
		//std::cout << phys.GetVelocity().x << " " << phys.GetVelocity().y << " * " << phys.GetAngularVelocity() << std::endl;
		mainWindow.Display();
	}

	return EXIT_SUCCESS;
}