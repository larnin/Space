
#include <NDK/Application.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include <Nazara/Graphics/ColorBackground.hpp>
#include "Utilities/asteroidrender.h"

int main()
{
	Ndk::Application application;

	Nz::RenderWindow& mainWindow = application.AddWindow<Nz::RenderWindow>();
	mainWindow.Create(Nz::VideoMode(800, 600, 32), "Space");
	mainWindow.SetFramerateLimit(60);
	mainWindow.EnableVerticalSync(true);

	auto & world = application.AddWorld();
	world.GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());
	world.GetSystem<Ndk::RenderSystem>().SetDefaultBackground(Nz::ColorBackground::New(Nz::Color::White));

	auto & camera = world.CreateEntity();
	auto & cameraComponent = camera->AddComponent<Ndk::CameraComponent>();
	cameraComponent.SetProjectionType(Nz::ProjectionType_Orthogonal);
	cameraComponent.SetSize(Nz::Vector2f(mainWindow.GetSize().x, mainWindow.GetSize().y));
	cameraComponent.SetTarget(&mainWindow);
	auto & cameraNodeComponent = camera->AddComponent<Ndk::NodeComponent>();
	cameraNodeComponent.SetPosition(-10.0f*cameraComponent.GetForward());

	Nz::Image imgAsteroid;
	imgAsteroid.LoadFromFile("res/Asteroids/asteroids0.png");
	Nz::Image imgBorder;
	imgBorder.LoadFromFile("res/Asteroids/explosion.png");
	createHole(imgAsteroid, Nz::Vector2i(180, 90), 30, imgBorder);

	auto tex = Nz::Texture::New(imgAsteroid);

	auto entity = world.CreateEntity();
	auto & node = entity->AddComponent<Ndk::NodeComponent>();
	node.SetPosition(200, 200);
	auto sprite = Nz::Sprite::New(Nz::Material::New("Translucent2D"));
	auto & t = sprite->GetMaterial()->GetDiffuseSampler();
	t.SetFilterMode(Nz::SamplerFilter_Nearest);

	sprite->SetTexture(tex);
	sprite->SetTextureRect(Nz::Rectui(107, 75, 115, 95));
	sprite->SetSize(115*2, 95*2);
	auto & graph = entity->AddComponent<Ndk::GraphicsComponent>();
	graph.Attach(sprite);


	while (application.Run())
	{

		mainWindow.Display();
	}

	return EXIT_SUCCESS;
}