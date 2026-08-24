#include "gavin_states.h"
#include "prefabs.h"
#include "components/cmp_gavin_properties.h"
#include "components/cmp_sprite.h"
#include "components/cmp_gavin_physics.h"
#include "components/cmp_animation.h"
#include <iostream>


using namespace sf;
using namespace std;

static double attackDelay= 2;
static double totalTime = 2;
static double deathDelay = 5;
static double chaseRange = 1000;
static double attackRange = 700;

// Gavin ----------------------------------------------------------------------

void  Gavin_IdleState::execute(Entity *owner, double dt) noexcept
{

	// Don't move
	owner->get_components<GavinPhysicsComponent>()[0]->setVelocity(Vector2f(0, 0));
	
	 //Chase player when in range
	if(length(owner->getPosition() - _player->getPosition()) < chaseRange)
	{
		auto sm = owner->get_components<StateMachineComponent>()[0];

		if (length(owner->getPosition() - _player->getPosition()) < attackRange)
		{
			totalTime += dt;

				if(totalTime >= attackDelay)
				{
					totalTime -= attackDelay;
					sm->changeState("cast");
				}
			
		}
		else
		{
			sm->changeState("chase");
		}
		
	}
	if (owner->get_components<GavinPropertiesComponent>()[0]->getHealth() == 0)
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}
}

void  Gavin_ChaseState::execute(Entity *owner, double dt) noexcept
{
	//out of range
	if (!(length(owner->getPosition() - _player->getPosition()) < chaseRange))
	{
		auto sm = owner->get_components<StateMachineComponent>()[0];
		sm->changeState("idle");
	}

	auto g = owner->get_components<GavinPhysicsComponent>()[0];

	//follow right

	if (_player->getPosition().x > owner->getPosition().x)
	{
		owner->get_components<AnimationComponent>()[0]->faceRight = true;
		g->impulse({ 2.0f , 0.0f });
		g->dampen({ 0.7f , 1.0f });
	}

	//follow left
	if (_player->getPosition().x < owner->getPosition().x)
	{
		owner->get_components<AnimationComponent>()[0]->faceRight = false;
		g->impulse({ -2.0f , 0.0f });
		g->dampen({ 0.7f , 1.0f });
		
	}

	//in attack range
	if (length(owner->getPosition() - _player->getPosition()) < attackRange)
	{
		
		auto sm = owner->get_components<StateMachineComponent>()[0];
		sm->changeState("cast");

	}
	if (owner->get_components<GavinPropertiesComponent>()[0]->getHealth() == 0)
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}
}

void  Gavin_CastState::execute(Entity *owner, double dt) noexcept
{
	auto me_anim = owner->get_components<AnimationComponent>()[0];
	
	if (_player->getPosition().x > owner->getPosition().x)
	{
		me_anim->faceRight = true;
	}
	if (_player->getPosition().x < owner->getPosition().x)
	{
		me_anim->faceRight = false;
	}
	

		if (me_anim->attackImgNo == 7 )
		{
			GavinBlast();
			me_anim->attackImgNo = 0;
			owner->get_components<StateMachineComponent>()[0]->changeState("idle");
		}

		if (me_anim->attackImgNo == 0)
		{
			//out of attack range
			if (length(owner->getPosition() - _player->getPosition()) > 700)
			{
				auto sm = owner->get_components<StateMachineComponent>()[0];
				sm->changeState("idle");

			}
		}
		if (owner->get_components<GavinPropertiesComponent>()[0]->getHealth() == 0)
		{
			owner->get_components<StateMachineComponent>()[0]->changeState("dead");
		}
	
}

void  Gavin_DeadState::execute(Entity *owner, double dt) noexcept
{
	auto me_anim = owner->get_components<AnimationComponent>()[0];
	if(me_anim->currentimage.x == 6)
	{
		me_anim->pause = true;
	}
}