#include "GameState.h"
#include "ShipControlerComponent.h"
#include <NDK/Components.hpp>
#include <Nazara/Graphics/Model.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <iostream>

GameState::GameState(Ndk::World & world)
	: m_world(world)
{
	createShip();
	createBackground();
}

void GameState::Enter(Ndk::StateMachine& fsm)
{

}

void GameState::Leave(Ndk::StateMachine& fsm)
{

}

bool GameState::Update(Ndk::StateMachine& fsm, float elapsedTime)
{
	return true;
}

void GameState::createShip()
{
	Ndk::EntityHandle entity = m_world.CreateEntity();
	Ndk::NodeComponent& nodeComponent = entity->AddComponent<Ndk::NodeComponent>();
	nodeComponent.SetPosition(Nz::Vector3f(0, 0, 0));
	Ndk::GraphicsComponent& graphicComponent = entity->AddComponent<Ndk::GraphicsComponent>();
	Ndk::PhysicsComponent2D& physicComponent = entity->AddComponent<Ndk::PhysicsComponent2D>();
	
	Ndk::CollisionComponent2D & collisionComponent = entity->AddComponent<Ndk::CollisionComponent2D>();
	Nz::CircleCollider2DRef collision = Nz::CircleCollider2D::New(1);
	collisionComponent.SetGeom(collision);
	
	entity->AddComponent<ShipControlerComponent>(ShipControlerComponent::Controls(Nz::Keyboard::Key::Up, Nz::Keyboard::Key::Down, Nz::Keyboard::Key::Left, Nz::Keyboard::Key::Right));

	Nz::MeshRef mesh = Nz::Mesh::New();
	mesh->CreateStatic();
	Nz::SubMeshRef subMesh = mesh->BuildSubMesh(Nz::Primitive::Box(Nz::Vector3f(1, 0.5f, 1)));
	subMesh->SetMaterialIndex(0);
	mesh->AddSubMesh(subMesh);

	Nz::ModelRef model = Nz::Model::New();
	model->SetMesh(mesh);
	Nz::MaterialRef mat = Nz::Material::New();
	mat->SetDiffuseColor(Nz::Color::White);
	model->SetMaterial(0, mat);
	graphicComponent.Attach(model);

	m_entities.push_back(entity);
}

void GameState::createBackground()
{
	Ndk::EntityHandle entity = m_world.CreateEntity();
	Ndk::NodeComponent& nodeComponent = entity->AddComponent<Ndk::NodeComponent>();
	Ndk::GraphicsComponent& graphicComponent = entity->AddComponent<Ndk::GraphicsComponent>();

	Nz::SpriteRef sprite = Nz::Sprite::New();
	sprite->SetTexture("res/back.jpg");
	sprite->SetSize(100, 100);
	graphicComponent.Attach(sprite);
	nodeComponent.SetPosition(Nz::Vector3f(-sprite->GetSize().x/2.0f, sprite->GetSize().y/2.0f, -10));

	m_entities.push_back(entity);
}