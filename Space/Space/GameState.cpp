#include "GameState.h"
#include "FollowEntityComponent.h"
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include <NDK/Components/LightComponent.hpp>
#include <NDK/Components/PhysicsComponent2D.hpp>
#include <NDK/Components/CollisionComponent2D.hpp>
#include <Nazara/Graphics/Model.hpp>
#include "ShipControlerComponent.h"
#include "ShipControlerSystem.h"

#include <NDK/Components/ParticleGroupComponent.hpp>
#include <NDK/Components/ParticleEmitterComponent.hpp>
#include <Nazara/Graphics/ParticleFunctionController.hpp>
#include <Nazara/Graphics/ParticleMapper.hpp>
#include <Nazara/Graphics/ParticleFunctionRenderer.hpp>
#include <Nazara/Graphics/ParticleStruct.hpp>

GameState::GameState(const Env & env)
	: m_env(env)
{

}

void GameState::Enter(Ndk::StateMachine& fsm)
{
	addPlayerShip();
	addCamera();
	addLight();
	m_env.world3D.AddSystem<ShipControlerSystem>();

	auto entity = m_env.world3D.CreateEntity();
	auto handle = entity->AddComponent<Ndk::ParticleGroupComponent>(100, Nz::ParticleDeclaration::Get(Nz::ParticleLayout_Billboard)).CreateHandle();
	handle->AddController(Nz::ParticleFunctionController::New([](Nz::ParticleGroup& group, Nz::ParticleMapper& mapper, unsigned int startId, unsigned int endId, float elapsedTime)
	{
		auto colorPtr = mapper.GetComponentPtr<Nz::Color>(Nz::ParticleComponent_Color);
		auto lifePtr = mapper.GetComponentPtr<float>(Nz::ParticleComponent_Life);
		auto positionPtr = mapper.GetComponentPtr<Nz::Vector3f>(Nz::ParticleComponent_Position);
		auto rotationPtr = mapper.GetComponentPtr<float>(Nz::ParticleComponent_Rotation);
		auto sizePtr = mapper.GetComponentPtr<Nz::Vector2f>(Nz::ParticleComponent_Size);
		auto velocityPtr = mapper.GetComponentPtr<Nz::Vector3f>(Nz::ParticleComponent_Velocity);

		float velFactor = std::pow(0.9f, elapsedTime / 0.1f);

		for (unsigned int i = startId; i <= endId; ++i)
		{
			colorPtr[i].a = static_cast<Nz::UInt8>(Nz::Clamp(lifePtr[i] * 255.f, 0.f, 255.f));
			lifePtr[i] -= elapsedTime;
			if (lifePtr[i] < 0.f)
				group.KillParticle(i);

			positionPtr[i] += velocityPtr[i];
		}

	}));

	Nz::MaterialRef fireMat = Nz::Material::New("Translucent3D");
	fireMat->EnableFaceCulling(true);
	fireMat->SetDiffuseMap("res/fire_particle.png");
	// Additive blending for fire
	fireMat->SetDstBlend(Nz::BlendFunc_One);
	fireMat->SetSrcBlend(Nz::BlendFunc_SrcAlpha);

	handle->SetRenderer(Nz::ParticleFunctionRenderer::New([fireMat](const Nz::ParticleGroup& group, const Nz::ParticleMapper& mapper, unsigned int startId, unsigned int endId, Nz::AbstractRenderQueue* renderQueue)
	{
		auto colorPtr = mapper.GetComponentPtr<const Nz::Color>(Nz::ParticleComponent_Color);
		auto posPtr = mapper.GetComponentPtr<const Nz::Vector3f>(Nz::ParticleComponent_Position);
		auto rotPtr = mapper.GetComponentPtr<const float>(Nz::ParticleComponent_Rotation);
		auto sizePtr = mapper.GetComponentPtr<const Nz::Vector2f>(Nz::ParticleComponent_Size);

		renderQueue->AddBillboards(0, fireMat, endId - startId + 1, posPtr, sizePtr, rotPtr, colorPtr);
	}));

	auto & emitter = m_shipEntity->AddComponent<Ndk::ParticleEmitterComponent>();
	emitter.SetEmissionCount(2);
	emitter.SetEmissionRate(100.f);
	emitter.SetSetupFunc([&gen = m_gen, handle](const Ndk::EntityHandle& entity, Nz::ParticleMapper& mapper, unsigned int count)
	{
		const float maxFireVel = 15.f;
		std::uniform_real_distribution<float> lifeDis(-0.5f, 0.5f);
		std::uniform_real_distribution<float> normalDis(-1.f, 1.f);
		std::uniform_real_distribution<float> posDis(-0.1f, 0.1f);
		std::uniform_real_distribution<float> rotDis(-180.f, 180.f);
		std::uniform_real_distribution<float> sizeDis(1.0f, 4.f);
		std::uniform_real_distribution<float> velDis(-maxFireVel, maxFireVel);

		Nz::Vector3f pos = entity->GetComponent<Ndk::NodeComponent>().GetPosition();

		Nz::ParticleStruct_Billboard* billboards = static_cast<Nz::ParticleStruct_Billboard*>(mapper.GetPointer());
		for (unsigned int i = 0; i < count; ++i)
		{
			billboards[i].color = Nz::Color::White;
			billboards[i].life = 1.f + lifeDis(gen);
			billboards[i].position = pos + Nz::Vector3f(posDis(gen), posDis(gen), posDis(gen));
			billboards[i].rotation = rotDis(gen);
			billboards[i].size = { 1.28f, 1.28f };
			billboards[i].size *= sizeDis(gen);
			billboards[i].velocity.Set(normalDis(gen), normalDis(gen), normalDis(gen));
			billboards[i].velocity.Normalize();
			billboards[i].velocity *= velDis(gen);
		}
	});

	handle->AddEmitter(m_shipEntity);
}

void GameState::Leave(Ndk::StateMachine& fsm)
{
	cleanEntities();
	m_env.world3D.RemoveSystem<ShipControlerSystem>();
}

bool GameState::Update(Ndk::StateMachine& fsm, float elapsedTime)
{
	return true;
}

void GameState::addCamera()
{
	Nz::Vector3f offset(0, 0, 5.0f);

	m_cameraEntity = m_env.world3D.CreateEntity();
	auto & nodeComponent = m_cameraEntity->AddComponent<Ndk::NodeComponent>();
	nodeComponent.SetPosition(offset);
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
	auto & controlerComponent = m_shipEntity->AddComponent<ShipControlerComponent>(ShipControlerComponent::Controls(Nz::Keyboard::Key::Up, Nz::Keyboard::Key::Down, Nz::Keyboard::Key::Left, Nz::Keyboard::Key::Right));
	
	controlerComponent.acceleration = 0.2f;
	controlerComponent.fluidDrag = 0.05f;

	controlerComponent.maxRotationSpeed = 600.0f;
	controlerComponent.rotationAcceleration = 200.0f;
	
	Ndk::PhysicsComponent2D& physicComponent = m_shipEntity->AddComponent<Ndk::PhysicsComponent2D>();
	Ndk::CollisionComponent2D & collisionComponent = m_shipEntity->AddComponent<Ndk::CollisionComponent2D>();
	Nz::CircleCollider2DRef collision = Nz::CircleCollider2D::New(1);
	collisionComponent.SetGeom(collision);

	auto ship = Nz::Model::New();

	const std::string shipDir = "res/Ship/space_frigate_6/space_frigate_6";
	Nz::ModelParameters params;
	params.mesh.texCoordScale.y = -1;
	params.mesh.optimizeIndexBuffers = false;
	params.mesh.matrix = Nz::Matrix4f::Rotate(Nz::Quaternionf(Nz::EulerAnglesf(90, 0, 180)));
	if (!ship->LoadFromFile(shipDir + ".obj", params))
	{
		NazaraWarning("Failed to load " + shipDir + ".obj");
	}
	ship->GetMesh()->GenerateNormalsAndTangents();
	auto mat = ship->GetMaterial(0);
	mat->SetDiffuseMap(shipDir + "_color.png");
	mat->SetEmissiveMap(shipDir + "_illumination.png");
	mat->SetSpecularMap(shipDir + "_specular.png");
	auto & box = ship->GetMesh()->GetAABB();
	auto scale = 1/std::max({ box.width, box.height, box.depth });
	nodeComponent.SetScale(scale, scale, scale);

	graphicComponent.Attach(ship);
}

void GameState::addLight()
{
	m_lightEntity = m_env.world3D.CreateEntity();
	auto & nodeComponent = m_lightEntity->AddComponent<Ndk::NodeComponent>();
	nodeComponent.SetRotation(Nz::Quaternionf(Nz::EulerAnglesf(-45, 0, 0)));
	auto & lightComponent = m_lightEntity->AddComponent<Ndk::LightComponent>(Nz::LightType::LightType_Directional);
	lightComponent.SetAmbientFactor(0.2f);
}

void GameState::cleanEntities()
{
	m_env.world3D.KillEntities(std::vector<Ndk::EntityHandle>{m_cameraEntity, m_shipEntity, m_lightEntity});
}