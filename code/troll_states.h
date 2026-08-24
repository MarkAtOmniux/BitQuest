
#pragma once
#include "components/cmp_state_Machine.h"

class Troll_IdleState : public State
{
private:
	std::shared_ptr<Entity> _player;

public:

	Troll_IdleState(std::shared_ptr<Entity> player)
		: _player(player) {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};


class Troll_AttackState : public State
{
private:
	std::shared_ptr<Entity> _player;

public:

	Troll_AttackState(std::shared_ptr<Entity> player)
		: _player(player) {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};


class Troll_ChaseState : public State
{
private:
	std::shared_ptr<Entity> _player;

public:

	Troll_ChaseState(std::shared_ptr<Entity> player)
		: _player(player) {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};

class Troll_DeadState : public State
{
public:
	Troll_DeadState() {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};