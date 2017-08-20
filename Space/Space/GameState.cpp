#include "GameState.h"
#include "FollowEntityComponent.h"
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include <NDK/Components/LightComponent.hpp>
#include <Nazara/Graphics/Model.hpp>

GameState::GameState(const Env & env)
	: m_env(env)
{
}

void GameState::Enter(Ndk::StateMachine& fsm)
{
	addPlayerShip();
	addCamera();
	addLight();
}

void GameState::Leave(Ndk::StateMachine& fsm)
{
	cleanEntities();
}

bool GameState::Update(Ndk::StateMachine& fsm, float elapsedTime)
{
	return true;
}


void GameState::addCamera()
{
	Nz::Vector3f offset(0, 10.0f, 0);

	m_cameraEntity = m_env.world3D.CreateEntity();
	auto & nodeComponent = m_cameraEntity->AddComponent<Ndk::NodeComponent>();
	nodeComponent.SetPosition(offset);
	nodeComponent.SetRotation(Nz::Quaternionf(Nz::EulerAnglesf(-90, 0, 0)));
	auto & cameraComponent = m_cameraEntity->AddComponent<Ndk::CameraComponent>();
	cameraComponent.SetProjectionType(Nz::ProjectionType_Perspective);
	cameraComponent.SetTarget(&m_env.window);
	m_cameraEntity->AddComponent<FollowEntityComponent>(m_shipEntity, offset);
}

void GameState::addPlayerShip()
{
	m_shipEntity = m_env.world3D.CreateEntity();
	auto & nodeComponent = m_shipEntity->AddComponent<Ndk::NodeComponent>();
	nodeComponent.SetPosition(0, 0, 0);
	auto & graphicComponent = m_shipEntity->AddComponent<Ndk::GraphicsComponent>();

	auto ship = Nz::Model::New();

	const std::string shipDir = "res/Ship/space_frigate_6/space_frigate_6";
	Nz::ModelParameters params;
	params.mesh.texCoordScale.y = -1;
	if (!ship->LoadFromFile(shipDir + ".obj", params))
	{
		NazaraWarning("Failed to load " + shipDir + ".obj");
	}
	auto mat = ship->GetMaterial(0);
	mat->SetDiffuseMap(shipDir + "_color.png");
	mat->SetEmissiveMap(shipDir + "_illumination.png");
	mat->SetSpecularMap(shipDir + "_specular.png");

	nodeComponent.SetScale(0.2f, 0.2f, 0.2f);

	graphicComponent.Attach(ship);
}

void GameState::addLight()
{
	m_lightEntity = m_env.world3D.CreateEntity();
	auto & nodeComponent = m_lightEntity->AddComponent<Ndk::NodeComponent>();
	nodeComponent.SetRotation(Nz::Quaternionf(Nz::EulerAnglesf(-50, 45, 0)));
	auto & lightComponent = m_lightEntity->AddComponent<Ndk::LightComponent>(Nz::LightType::LightType_Directional);
	lightComponent.SetAmbientFactor(0.2f);
}

void GameState::cleanEntities()
{
	m_env.world3D.KillEntities(std::vector<Ndk::EntityHandle>{m_cameraEntity, m_shipEntity, m_lightEntity});
}