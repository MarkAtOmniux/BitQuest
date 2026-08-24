#include "troll_states.h"
#include "components/cmp_troll_properties.h"
#include "components/cmp_animation.h"
#include "Prefabs.h"
#include "components/cmp_physics.h"


void  Troll_IdleState::execute(Entity *owner, double dt) noexcept
{
	auto p = owner->get_components<PhysicsComponent>()[0];
	auto sm = owner->get_components<StateMachineComponent>()[0];
	auto props = owner->get_components<TrollPropertiesComponent>()[0];

	if (length(owner->getPosition() - _player->getPosition()) < 800.f)
	{
		if (length(owner->getPosition() - _player->getPosition()) < 200.f)
		{
			if (props->canAttack())
			{
				sm->changeState("Attack");
			}
		}
		else
		{
			sm->changeState("chase");
		}
		
	}

	p->dampen({ 0.7f , 1.0f });

	if (props->getHealth() <= 0)
	{
		owner->get_components<PhysicsComponent>()[0]->setVelocity(sf::Vector2f(0, 0));
		owner->get_components<AnimationComponent>()[0]->currentimage.x = 0;
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}
}

void  Troll_ChaseState::execute(Entity *owner, double dt) noexcept
{
	auto sm = owner->get_components<StateMachineComponent>()[0];
	auto p = owner->get_components<PhysicsComponent>()[0];
	auto props = owner->get_components<TrollPropertiesComponent>()[0];

	if (length(owner->getPosition() - _player->getPosition()) > 800.f)
	{
		sm->changeState("idle");
	}
	
	if (length(owner->getPosition() - _player->getPosition()) < 200.f)
	{
		if (props->canAttack())
		{
			sm->changeState("Attack");
		}
	}

	if (props->getHealth() <= 0)
	{
		owner->get_components<PhysicsComponent>()[0]->setVelocity(sf::Vector2f(0, 0));
		owner->get_components<AnimationComponent>()[0]->currentimage.x = 0;
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}

	if (_player->getPosition().x > owner->getPosition().x)
	{
		owner->get_components<AnimationComponent>()[0]->faceRight = true;
		p->impulse({ 2.0f , 0.0f });
		p->dampen({ 0.7f , 1.0f });
	}

	//follow left
	if (_player->getPosition().x < owner->getPosition().x)
	{
		owner->get_components<AnimationComponent>()[0]->faceRight = false;
		p->impulse({ -2.0f , 0.0f });
		p->dampen({ 0.7f , 1.0f });

	}
}


void  Troll_AttackState::execute(Entity *owner, double dt) noexcept
{
	auto a = owner->get_components<AnimationComponent>()[0];
	auto sm = owner->get_components<StateMachineComponent>()[0];
	auto props = owner->get_components<TrollPropertiesComponent>()[0];

	
	if (a->attackImgNo >= 6)
	{
		a->attackImgNo = 0;
		props->beginAttackPause();
		sm->changeState("idle");
	}


	if (length(owner->getPosition() - _player->getPosition()) > 200.f)
	{
		sm->changeState("chase");
	}

	if (props->getHealth() <= 0)
	{
		owner->get_components<AnimationComponent>()[0]->currentimage.x = 0;
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}
	
}


void  Troll_DeadState::execute(Entity *owner, double dt) noexcept
{
	auto me_anim = owner->get_components<AnimationComponent>()[0];
	me_anim->currentimage.x = 3;
	auto p = owner->get_components<PhysicsComponent>()[0];
	
	me_anim->pause = true;
	owner->setForDelete();
	
}