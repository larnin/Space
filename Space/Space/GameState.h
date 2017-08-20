#pragma once

#include "Env.h"
#include <NDK/State.hpp>
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
	void cleanEntities();

	Env m_env;
	Ndk::EntityHandle m_cameraEntity;
	Ndk::EntityHandle m_shipEntity;
	Ndk::EntityHandle m_lightEntity;
	std::mt19937 m_gen;
};

