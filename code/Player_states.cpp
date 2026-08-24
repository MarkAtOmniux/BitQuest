#include "gavin_states.h"
#include "prefabs.h"
#include "components/cmp_sprite.h"
#include "components/cmp_player_physics.h"
#include "components/cmp_animation.h"
#include <iostream>
#include "Player_states.h"
#include "components/cmp_player_controller.h"
#include "components/cmp_hurt.h"
#include "GameState.h"
#include "SaveLoad.h"
#include "Controls.h"
#include "engine.h"

using namespace sf;
using namespace std;


void  Player_IdleState::execute(Entity *owner, double dt) noexcept
{
	auto me = owner->get_components<StateMachineComponent>()[0]; 
	auto p = owner->get_components<PlayerPhysicsComponent>()[0];

	if (Controls::isPressed("Attack"))
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("Attack");
	}

	if (Controls::isPressed("Block"))
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("block");
	}

	//dampen if not jumping
	if (p->isGrounded() == true)
	{
		p->dampen({ 0.7f , 0 });
	}
	
	if (Controls::isPressed("MoveLeft"))
	{
		me->changeState("walk_left");
	}
	if (Controls::isPressed("MoveRight"))
	{
		me->changeState("walk_right");
	}
	if (Controls::isPressed("Attack"))
	{
		me->changeState("Attack");
	}
	if (Controls::isPressed("Jump"))
	{
		me->changeState("jump");
	}

	if (owner->get_components<PlayerControlerComponent>()[0]->getHealth() <= 0)
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}
}

void  Player_MoveLeftState::execute(Entity *owner, double dt) noexcept
{

	owner->get_components<AnimationComponent>()[0]->faceRight = false;


	if (Controls::isPressed("Block"))
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("block");
	}

	if (Controls::isPressed("Attack"))
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("Attack");
	}

	auto p = owner->get_components<PlayerPhysicsComponent>()[0];
	

	if (!Controls::isPressed("MoveLeft"))
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("idle");
	}
	
	if (owner->get_components<PlayerControlerComponent>()[0]->getHealth() <= 0)
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}
}

void  Player_MoveRightState::execute(Entity *owner, double dt) noexcept
{

	owner->get_components<AnimationComponent>()[0]->faceRight = true;


	if (Controls::isPressed("Block"))
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("block");
	}

	if (Controls::isPressed("Attack"))
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("Attack");
	}

	if (!Controls::isPressed("MoveRight"))
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("idle");
	}

	auto p = owner->get_components<PlayerPhysicsComponent>()[0];
	
	if (owner->get_components<PlayerControlerComponent>()[0]->getHealth() <= 0)
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}
	
	if (Controls::isPressed("Attack"))
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("Attack");
	}

}

void  Player_AttackState::execute(Entity *owner, double dt) noexcept
{

	if (Engine::isMouseDown(sf::Mouse::Right) || Controls::isPressed("Block"))
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("block");
	}

	auto me = owner->get_components<StateMachineComponent>()[0];
	auto me_anim = owner->get_components<AnimationComponent>()[0];
	auto p = owner->get_components<PlayerPhysicsComponent>()[0];

	if (owner->get_components<PlayerControlerComponent>()[0]->sword == false)
	{

		if (me_anim->attackImgNo >= 6)
		{
			
			if (SaveLoad::arrows > 0)
			{
				makeArrow();
				SaveLoad::arrows--;
				
			}
			me_anim->attackImgNo = 0;
			me->changeState("idle");
		}

		if (Controls::isPressed("MoveLeft"))
		{
			me->changeState("walk_left");
		}
		if (Controls::isPressed("MoveRight"))
		{
			me->changeState("walk_right");
		}

	}
	else
	{
		if (me_anim->attackImgNo >= 6)
		{
			me_anim->attackImgNo = 0;
			me->changeState("idle");
		}

		if (Engine::isMouseDown(sf::Mouse::Left) || Controls::isPressed("Attack"))
		{
			owner->get_components<StateMachineComponent>()[0]->changeState("Attack");
		}

		if (Controls::isPressed("MoveLeft"))
		{
			me->changeState("walk_left");
		}
		if (Controls::isPressed("MoveRight"))
		{
			me->changeState("walk_right");
		}
		if (Controls::isPressed("Jump"))
		{
			me->changeState("jump");
		}
		if (owner->get_components<PlayerControlerComponent>()[0]->getHealth() <= 0)
		{
			me->changeState("dead");
		}
	}
}
void  Player_DeadState::execute(Entity *owner, double dt) noexcept
{

	auto me_anim = owner->get_components<AnimationComponent>()[0];

	if (me_anim->currentimage.x >= 5)
	{
		me_anim->pause = true;
	}


}
void  Player_BlockState::execute(Entity *owner, double dt) noexcept
{
	auto me_anim = owner->get_components<AnimationComponent>()[0];

	me_anim->currentimage.x = 3;
	
	me_anim->pause = true;
	owner->get_components<PlayerControlerComponent>()[0]->immortal = true;

	if(!Controls::isPressed("Block"))
	{
		me_anim->pause = false;
		owner->get_components<StateMachineComponent>()[0]->changeState("idle");
	}
	
}