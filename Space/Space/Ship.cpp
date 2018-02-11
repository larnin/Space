#include "Ship.h"
#include "Components/ShipCommandsComponent.h"
#include "Components/ShipControlerComponent.h"
#include "Components/CameraFollowComponent.h"
#include "Components/CameraFollowControlerComponent.h"
#include "Env.h"
#include <NDK/Components/PhysicsComponent2D.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/CollisionComponent2D.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include <Nazara/Physics2D/Collider2D.hpp>
#include <Nazara/Graphics/Sprite.hpp>

Ndk::EntityHandle createShip(Ndk::World & w, const ShipInfos & infos, const Nz::Vector2f & pos, float rotation)
{
	auto e = w.CreateEntity();
	e->AddComponent<Ndk::NodeComponent>();
	e->AddComponent<Ndk::CollisionComponent2D>(Nz::BoxCollider2D::New(Nz::Vector2f(50, 50)));
	auto & physics = e->AddComponent<Ndk::PhysicsComponent2D>();
	physics.SetPosition(pos);
	physics.SetRotation(rotation);

	auto & graphics = e->AddComponent<Ndk::GraphicsComponent>();
	auto sprite = Nz::Sprite::New(Nz::Material::New("Translucent2D"));
	auto & sampler = sprite->GetMaterial()->GetDiffuseSampler();
	sampler.SetFilterMode(Nz::SamplerFilter_Nearest);
	sprite->SetTexture(infos.imageName);
	auto size = sprite->GetMaterial()->GetDiffuseMap()->GetSize();
	sprite->SetOrigin(Nz::Vector3f(size.x, size.y, 0));
	sprite->SetSize(size.x * 2, size.y * 2);
	graphics.Attach(sprite, Nz::Matrix4f::Rotate(Nz::Quaternionf(Nz::EulerAnglesf(0, 0, 90))));

	e->AddComponent<ShipCommandsComponent>();
	auto & ship = e->AddComponent<ShipControlerComponent>();
	ship.solidDrag = infos.solidDrag;
	ship.fluidDrag = infos.fluidDrag;
	ship.acceleration = infos.acceleration;
	ship.maxRotationSpeed = infos.maxRotationSpeed;
	ship.rotationAcceleration = infos.rotationAcceleration;

	return e;
}

Ndk::EntityHandle createShipCamera(Ndk::World & w, const Ndk::EntityHandle & target)
{
	auto e = w.CreateEntity();
	auto & camera = e->AddComponent<Ndk::CameraComponent>();
	auto & node = e->AddComponent<Ndk::NodeComponent>();
	e->AddComponent<CameraFollowComponent>(target);
	e->AddComponent<CameraFollowControlerComponent>();

	camera.SetProjectionType(Nz::ProjectionType_Orthogonal);
	auto & window = Env::instance().window();
	camera.SetSize(Nz::Vector2f(window.GetSize().x, window.GetSize().y));
	camera.SetTarget(&window);

	node.SetPosition(-10.0f*camera.GetForward());
	return e;
}