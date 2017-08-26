#pragma once

#include "Env.h"
#include <NDK/World.hpp>
#include <NDK/State.hpp>
#include <NDK/Components/ParticleGroupComponent.hpp>
#include <random>

class GameState : public Ndk::State
{
public:
	GameState(const Env & env);
	~GameState() = default;

	void Enter(Ndk::StateMachine& fsm) override;
	void Leave(Ndk::StateMachine& fsm) override;
	bool Update(Ndk::StateMachine& fsm, float elapsedTime) override;

private:
	void addCamera();
	void addPlayerShip();
	void addLight();
	void addBasicBackground();
	void createParticleHandle();
	void addAsteroid(unsigned int count);

	Env m_env;
	Ndk::EntityHandle m_shipEntity;
	std::mt19937 m_gen;

	Ndk::World m_world2D;
	Ndk::World m_world3D;
	
	Ndk::ParticleGroupComponentHandle m_shipParticleHandle;
};

