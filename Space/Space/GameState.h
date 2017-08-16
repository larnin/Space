#pragma once

#include <NDK/State.hpp>
#include <NDK/StateMachine.hpp>
#include <NDK/World.hpp>
#include <vector>

class GameState : public Ndk::State
{
public:
	GameState(Ndk::World & world);
	virtual ~GameState() = default;

	void Enter(Ndk::StateMachine& fsm) override;
	void Leave(Ndk::StateMachine& fsm) override;
	bool Update(Ndk::StateMachine& fsm, float elapsedTime) override;

private:
	void createShip();
	void createBackground();

	Ndk::World & m_world;
	std::vector<Ndk::EntityHandle> m_entities;
};

