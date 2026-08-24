#pragma once
#include "components/cmp_state_machine.h"
#include <SFML/Audio.hpp>

//Player -----------------------------------------------------------------

class Player_IdleState : public State
{

public:

	Player_IdleState() {};
		
	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};

class Player_MoveLeftState : public State
{

public:

	Player_MoveLeftState(){}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};

class Player_MoveRightState : public State
{

public:

	Player_MoveRightState() {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};

class Player_AttackState : public State
{

public:
	float _attackHold = 0.8;

	Player_AttackState() {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};

class Player_DeadState : public State
{

public:
	
	Player_DeadState() {}

	double totalTime = 0;
	double deathHold = 1;


	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};

class Player_BlockState : public State
{

public:

	Player_BlockState() {}

	void enterState(Entity*) noexcept override {};
	void execute(Entity*, double) noexcept override;
};