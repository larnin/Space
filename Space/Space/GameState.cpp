#include "GameState.h"
#include "FollowEntityComponent.h"
#include "ShipControlerComponent.h"
#include "FollowEntitySystem.h"
#include "ShipControlerSystem.h"
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include <NDK/Components/LightComponent.hpp>
#include <NDK/Components/PhysicsComponent2D.hpp>
#include <NDK/Components/CollisionComponent2D.hpp>
#include <NDK/Components/ParticleGroupComponent.hpp>
#include <NDK/Components/ParticleEmitterComponent.hpp>
#include <Nazara/Graphics/ParticleFunctionController.hpp>
#include <Nazara/Graphics/ParticleMapper.hpp>
#include <Nazara/Graphics/ParticleFunctionRenderer.hpp>
#include <Nazara/Graphics/ParticleStruct.hpp>
#include <Nazara/Graphics/Model.hpp>
#include <Nazara/Graphics/Sprite.hpp>

GameState::GameState(const Env & env)
	: m_env(env)
{

}

void GameState::Enter(Ndk::StateMachine& fsm)
{
	createParticleHandle();
	addPlayerShip();
	addCamera();
	addLight();
	addBasicBackground();
	m_world3D.AddSystem<ShipControlerSystem>();
	m_world3D.AddSystem<FollowEntitySystem>(m_env.window);
}

void GameState::Leave(Ndk::StateMachine& fsm)
{

}

bool GameState::Update(Ndk::StateMachine& fsm, float elapsedTime)
{
	m_world2D.Update(elapsedTime);
	m_world3D.Update(elapsedTime);

	return true;
}

void GameState::addCamera()
{
	auto cameraEntity = m_world3D.CreateEntity();
	auto & nodeComponent = cameraEntity->AddComponent<Ndk::NodeComponent>();
	auto & cameraComponent = cameraEntity->AddComponent<Ndk::CameraComponent>();
	cameraComponent.SetProjectionType(Nz::ProjectionType_Perspective);
	cameraComponent.SetTarget(&m_env.window);
	auto & followComponent = cameraEntity->AddComponent<FollowEntityComponent>(m_shipEntity);
}

void GameState::addPlayerShip()
{
	m_shipEntity = m_world3D.CreateEntity();
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

	auto & emitter = m_shipEntity->AddComponent<Ndk::ParticleEmitterComponent>();
	emitter.SetEmissionCount(2);
	emitter.SetEmissionRate(300.0f);
	emitter.Enable(false);
	
	emitter.SetSetupFunc([&gen = m_gen, &nodeComponent](const Ndk::EntityHandle& entity, Nz::ParticleMapper& mapper, unsigned int count)
	{
		Nz::Vector3f dir = nodeComponent.GetRotation() * Nz::Vector3f(-1.0f, 0, 0);
		Nz::Vector3f pos = dir / 2 + nodeComponent.GetPosition();

		std::uniform_real_distribution<float> lifeDis(-0.2f, 0.2f);
		std::uniform_real_distribution<float> posDis(-0.05f, 0.05f);
		std::uniform_real_distribution<float> rotDis(-180.f, 180.f);
		std::uniform_real_distribution<float> velDis(0.0f, 0.025f);

		Nz::ParticleStruct_Billboard* billboards = static_cast<Nz::ParticleStruct_Billboard*>(mapper.GetPointer());
		for (unsigned int i = 0; i < count; ++i)
		{
			billboards[i].color = Nz::Color(62, 172, 236);
			billboards[i].life = 0.4f + lifeDis(gen);
			billboards[i].position = pos + Nz::Vector3f(posDis(gen), posDis(gen), posDis(gen));
			billboards[i].rotation = rotDis(gen);
			billboards[i].size = { 0.2f, 0.2f };
			billboards[i].velocity = dir * velDis(gen);
		}
	});

	m_shipParticleHandle->AddEmitter(m_shipEntity);
}

void GameState::addLight()
{
	auto lightEntity = m_world3D.CreateEntity();
	auto & nodeComponent = lightEntity->AddComponent<Ndk::NodeComponent>();
	nodeComponent.SetRotation(Nz::Quaternionf(Nz::EulerAnglesf(-45, 0, 0)));
	auto & lightComponent = lightEntity->AddComponent<Ndk::LightComponent>(Nz::LightType::LightType_Directional);
	lightComponent.SetAmbientFactor(0.2f);
}

void  GameState::addBasicBackground()
{
	Ndk::EntityHandle entity = m_world3D.CreateEntity();
	Ndk::NodeComponent& nodeComponent = entity->AddComponent<Ndk::NodeComponent>();
	Ndk::GraphicsComponent& graphicComponent = entity->AddComponent<Ndk::GraphicsComponent>();

	Nz::SpriteRef sprite = Nz::Sprite::New();
	sprite->SetTexture("res/back.jpg");
	sprite->SetSize(100, 100);
	graphicComponent.Attach(sprite);
	nodeComponent.SetPosition(Nz::Vector3f(-sprite->GetSize().x / 2.0f, sprite->GetSize().y / 2.0f, -10));
}

void GameState::createParticleHandle()
{
	auto entity = m_world3D.CreateEntity();
	m_shipParticleHandle = entity->AddComponent<Ndk::ParticleGroupComponent>(1000, Nz::ParticleDeclaration::Get(Nz::ParticleLayout_Billboard)).CreateHandle();
	m_shipParticleHandle->AddController(Nz::ParticleFunctionController::New([](Nz::ParticleGroup& group, Nz::ParticleMapper& mapper, unsigned int startId, unsigned int endId, float elapsedTime)
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
			float normalizedLife(lifePtr[i] / 0.6f);
			const Nz::Color startColor(62, 172, 236);
			const Nz::Color endColor(255, 95, 13);
			Nz::Color current(startColor.r * normalizedLife + endColor.r * (1 - normalizedLife)
							, startColor.g * normalizedLife + endColor.g * (1 - normalizedLife)
							, startColor.b * normalizedLife + endColor.b * (1 - normalizedLife)
							, static_cast<Nz::UInt8>(Nz::Clamp(lifePtr[i] * 255.f, 0.f, 255.f)));
			colorPtr[i] = current;
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

	m_shipParticleHandle->SetRenderer(Nz::ParticleFunctionRenderer::New([fireMat](const Nz::ParticleGroup& group, const Nz::ParticleMapper& mapper, unsigned int startId, unsigned int endId, Nz::AbstractRenderQueue* renderQueue)
	{
		auto colorPtr = mapper.GetComponentPtr<const Nz::Color>(Nz::ParticleComponent_Color);
		auto posPtr = mapper.GetComponentPtr<const Nz::Vector3f>(Nz::ParticleComponent_Position);
		auto rotPtr = mapper.GetComponentPtr<const float>(Nz::ParticleComponent_Rotation);
		auto sizePtr = mapper.GetComponentPtr<const Nz::Vector2f>(Nz::ParticleComponent_Size);

		renderQueue->AddBillboards(0, fireMat, endId - startId + 1, posPtr, sizePtr, rotPtr, colorPtr);
	}));
}
