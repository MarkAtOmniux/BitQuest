#include "goblin_states.h"
#include "components/cmp_goblin_properties.h"
#include "components/cmp_physics.h"
#include "components/cmp_animation.h"
#include "components/cmp_hurt.h"
#include "Prefabs.h"

void  Goblin_IdleState::execute(Entity *owner, double dt) noexcept
{

	auto sm = owner->get_components<StateMachineComponent>()[0];


	if (owner->get_components<GoblinPropertiesComponent>()[0]->getHealth() <= 0)
	{
		owner->get_components<PhysicsComponent>()[0]->setVelocity(sf::Vector2f(0, 0));
		owner->get_components<AnimationComponent>()[0]->currentimage.x = 0;
		sm->changeState("dead");
	}


	// Don't move
	owner->get_components<PhysicsComponent>()[0]->setVelocity(sf::Vector2f(0, 0));


	if (_player->getPosition().x > owner->getPosition().x)
	{
		owner->get_components<AnimationComponent>()[0]->faceRight = true;
	}
	if (_player->getPosition().x < owner->getPosition().x)
	{
		owner->get_components<AnimationComponent>()[0]->faceRight = false;
	}

	//Chase player when in range
	if (length(owner->getPosition() - _player->getPosition()) < 800.0f)
	{
		if (length(owner->getPosition() - _player->getPosition()) < 200.0f)
		{
			if (owner->get_components<GoblinPropertiesComponent>()[0]->canAttack())
			{
				sm->changeState("Attack");
			}
		}
		else if (length(owner->getPosition() - _player->getPosition()) < 800.0f)
		{
			sm->changeState("chase");
		}
	}
	
}

void  Goblin_ChaseState::execute(Entity *owner, double dt) noexcept
{
	auto p = owner->get_components<PhysicsComponent>()[0];
	auto g = owner->get_components<PhysicsComponent>()[0];
	auto sm = owner->get_components<StateMachineComponent>()[0];

	if (owner->get_components<GoblinPropertiesComponent>()[0]->getHealth() <= 0)
	{
		owner->get_components<PhysicsComponent>()[0]->setVelocity(sf::Vector2f(0, 0));
		owner->get_components<AnimationComponent>()[0]->currentimage.x = 0;
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}


	if (length(owner->getPosition() - _player->getPosition()) < 200.0f)
	{
		if (owner->get_components<GoblinPropertiesComponent>()[0]->canAttack())
		{
			sm->changeState("Attack");
		}
	}

	if (_player->getPosition().x > owner->getPosition().x)
	{
		owner->get_components<AnimationComponent>()[0]->faceRight = true;
		g->impulse({ 5.0f , 0.0f });
		g->dampen({ 0.7f , 1.0f });
	}

	//follow left
	if (_player->getPosition().x < owner->getPosition().x)
	{
		owner->get_components<AnimationComponent>()[0]->faceRight = false;
		g->impulse({ -5.0f , 0.0f });
		g->dampen({ 0.7f , 1.0f });

	}
	
}


void  Goblin_AttackState::execute(Entity *owner, double dt) noexcept
{
	auto p = owner->get_components<PhysicsComponent>()[0];
	auto a = owner->get_components<AnimationComponent>()[0];
	auto sm = owner->get_components<StateMachineComponent>()[0];
	auto props = owner->get_components<GoblinPropertiesComponent>()[0];

	if (_player->getPosition().x > owner->getPosition().x)
	{
		a->faceRight = true;
		p->impulse({ 3.0f , 0.0f });
		p->dampen({ 0.7f , 1.0f });
	}

	if (_player->getPosition().x < owner->getPosition().x)
	{
		a->faceRight = false;
		p->impulse({ -3.0f , 0.0f });
		p->dampen({ 0.7f , 1.0f });
	}

	if (a->attackImgNo >= 6)
	{
		a->attackImgNo = 0;
		props->beginAttackPause();
		sm->changeState("idle");
	}

	if (length(owner->getPosition() - _player->getPosition()) > 200.0f)
	{
		sm->changeState("idle");
	}

	if (props->getHealth() <= 0)
	{
		a->currentimage.x = 0;
		p->setVelocity(sf::Vector2f(0, 0));
		sm->changeState("dead");
	}
}


void  Goblin_DeadState::execute(Entity *owner, double dt) noexcept
{
	auto me_anim = owner->get_components<AnimationComponent>()[0];
	if (me_anim->currentimage.x == 7)
	{
		makeCoin(owner->getPosition());
		makeCoin(owner->getPosition());
		
		me_anim->currentimage.x = 0;
		me_anim->pause = true;
		owner->setForDelete();
	}
}