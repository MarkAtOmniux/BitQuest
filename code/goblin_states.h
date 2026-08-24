#pragma once
#include "components/cmp_state_Machine.h"

class Goblin_IdleState : public State
{
private:
	std::shared_ptr<Entity> _player;

public:

	Goblin_IdleState(std::shared_ptr<Entity> player)
		: _player(player) {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};


class Goblin_AttackState : public State
{
private:
	std::shared_ptr<Entity> _player;

public:

	Goblin_AttackState(std::shared_ptr<Entity> player)
		: _player(player) {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};


class Goblin_ChaseState : public State
{
private:
	std::shared_ptr<Entity> _player;

public:

	Goblin_ChaseState(std::shared_ptr<Entity> player)
		: _player(player) {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};

class Goblin_DeadState : public State
{
private:

public:

	double attackRange;
	Goblin_DeadState() {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};