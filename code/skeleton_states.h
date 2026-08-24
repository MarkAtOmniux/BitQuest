#pragma once
#include "components/cmp_state_Machine.h"

class Skeleton_IdleState : public State
{
private:
	std::shared_ptr<Entity> _player;

public:

	Skeleton_IdleState(std::shared_ptr<Entity> player)
		: _player(player) {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};


class Skeleton_AttackState : public State
{
private:
	std::shared_ptr<Entity> _player;

public:

	Skeleton_AttackState(std::shared_ptr<Entity> player)
		: _player(player) {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};


class Skeleton_ChaseState : public State
{
private:
	std::shared_ptr<Entity> _player;

public:

	Skeleton_ChaseState(std::shared_ptr<Entity> player)
		: _player(player) {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};

class Skeleton_DeadState : public State
{
private:

public:

	double attackRange;
	Skeleton_DeadState() {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};