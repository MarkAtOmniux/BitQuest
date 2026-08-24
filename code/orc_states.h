#pragma once
#include "components/cmp_state_Machine.h"

class Orc_IdleState : public State
{
private:
	std::shared_ptr<Entity> _player;

public:

	Orc_IdleState(std::shared_ptr<Entity> player)
		: _player(player) {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};


class Orc_AttackState : public State
{
private:
	std::shared_ptr<Entity> _player;

public:

	Orc_AttackState(std::shared_ptr<Entity> player)
		: _player(player) {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};


class Orc_ChaseState : public State
{
private:
	std::shared_ptr<Entity> _player;

public:

	Orc_ChaseState(std::shared_ptr<Entity> player)
		: _player(player) {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};

class Orc_DeadState : public State
{
private:

public:

	double attackRange;
	Orc_DeadState() {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};