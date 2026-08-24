#include "ghost_states.h"
#include "prefabs.h"
#include "components/cmp_ghost_properties.h"
#include "components/cmp_sprite.h"
#include "components/cmp_physics.h"
#include "components/cmp_animation.h"
#include <iostream>


using namespace sf;
using namespace std;

double attackDelayGH = 2;
double totalTimeGH = 2;
double deathDelayGH = 5;
double chaseRangeGH = 1000;
double attackRangeGH = 600;

// Gavin ----------------------------------------------------------------------

void  Ghost_IdleState::execute(Entity *owner, double dt) noexcept
{

	// Don't move
	owner->get_components<PhysicsComponent>()[0]->setVelocity(Vector2f(0, 0));
	
	 //Chase player when in range
	if(length(owner->getPosition() - _player->getPosition()) < chaseRangeGH)
	{
		auto sm = owner->get_components<StateMachineComponent>()[0];

		if (length(owner->getPosition() - _player->getPosition()) < attackRangeGH)
		{
			totalTimeGH += dt;

				if(totalTimeGH >= attackDelayGH)
				{
					totalTimeGH -= attackDelayGH;
					sm->changeState("cast");
				}
			
		}
		else
		{
			sm->changeState("chase");
		}
		
	}
	if (owner->get_components<GhostPropertiesComponent>()[0]->getHealth() == 0)
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}
}

void  Ghost_ChaseState::execute(Entity *owner, double dt) noexcept
{
	//out of range
	if (!(length(owner->getPosition() - _player->getPosition()) < chaseRangeGH))
	{
		auto sm = owner->get_components<StateMachineComponent>()[0];
		sm->changeState("idle");
	}

	auto g = owner->get_components<PhysicsComponent>()[0];

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
	if (length(owner->getPosition() - _player->getPosition()) < attackRangeGH)
	{
		
		auto sm = owner->get_components<StateMachineComponent>()[0];
		sm->changeState("cast");

	}
	if (owner->get_components<GhostPropertiesComponent>()[0]->getHealth() == 0)
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}
}

void  Ghost_CastState::execute(Entity *owner, double dt) noexcept
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
	
	owner->get_components<PhysicsComponent>()[0]->setVelocity(Vector2f(0, 0));


		if (me_anim->attackImgNo == 7 )
		{
			ghostBlast();
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
		if (owner->get_components<GhostPropertiesComponent>()[0]->getHealth() == 0)
		{
			owner->get_components<StateMachineComponent>()[0]->changeState("dead");
		}
	
}

void  Ghost_DeadState::execute(Entity *owner, double dt) noexcept
{
	auto me_anim = owner->get_components<AnimationComponent>()[0];
	if(me_anim->currentimage.x == 6)
	{
		owner->setForDelete();
		me_anim->pause = true;
	}
}