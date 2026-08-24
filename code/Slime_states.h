#pragma once
#include "components/cmp_state_Machine.h"
#include "ecm.h"
#include <SFML/Audio.hpp>

class Slime_IdleState : public State
{
private:
	std::shared_ptr<Entity> _player;

public:

	Slime_IdleState(std::shared_ptr<Entity> player)
		: _player(player) {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};


class Slime_AttackState : public State
{
private:
	std::shared_ptr<Entity> _player;

public:

	Slime_AttackState(std::shared_ptr<Entity> player)
		: _player(player) {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};


class Slime_FleeState : public State
{
private:
	std::shared_ptr<Entity> _player;

public:

	Slime_FleeState(std::shared_ptr<Entity> player)
		: _player(player) {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};

class Slime_DeadState : public State
{

public:

	
	sf::SoundBuffer _bufferDeath;

	
	sf::Sound _soundDeath;

	double attackRange;
	Slime_DeadState() {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};