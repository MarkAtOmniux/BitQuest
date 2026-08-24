#include "orc_states.h"
#include "components/cmp_orc_properties.h"
#include "components/cmp_physics.h"
#include "components/cmp_animation.h"
#include "Prefabs.h"
#include "components/cmp_hurt.h"


void  Orc_IdleState::execute(Entity *owner, double dt) noexcept
{
	
	auto p = owner->get_components<PhysicsComponent>()[0];
	auto sm = owner->get_components<StateMachineComponent>()[0];
	auto props = owner->get_components<OrcPropertiesComponent>()[0];

	p->dampen({ 0.7f , 1.0f });

	if (_player->getPosition().x < owner->getPosition().x)
	{
		owner->get_components<AnimationComponent>()[0]->faceRight = false;
	}

	if (_player->getPosition().x > owner->getPosition().x)
	{
		owner->get_components<AnimationComponent>()[0]->faceRight = true;
	}


	if (length(owner->getPosition() - _player->getPosition()) < 500.f)
	{
		if (length(owner->getPosition() - _player->getPosition()) < 200.f)
		{
			if (props->canAttack())
			{
				sm->changeState("Attack");
			}
		}
		else if(length(owner->getPosition() - _player->getPosition()) > 400.f)
		{
			sm->changeState("chase");
		}

	}

	if (props->getHealth() <= 0)
	{
		owner->get_components<AnimationComponent>()[0]->currentimage.x = 0;
		owner->get_components<PhysicsComponent>()[0]->setVelocity(sf::Vector2f(0, 0));
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}

}

void  Orc_ChaseState::execute(Entity *owner, double dt) noexcept
{
	auto sm = owner->get_components<StateMachineComponent>()[0];
	auto p = owner->get_components<PhysicsComponent>()[0];
	auto props = owner->get_components<OrcPropertiesComponent>()[0];

	if (length(owner->getPosition() - _player->getPosition()) > 500.f)
	{
		sm->changeState("idle");
	}


	if (length(owner->getPosition() - _player->getPosition()) < 200.f)
	{
		if (_player->getPosition().x > owner->getPosition().x)
		{
			owner->get_components<AnimationComponent>()[0]->faceRight = true;
		}
		if (_player->getPosition().x < owner->getPosition().x)
		{
			owner->get_components<AnimationComponent>()[0]->faceRight = false;
		}
		p->dampen({ 0.55f , 1.0f });
		if (props->canAttack())
		{
			sm->changeState("Attack");
		}
	}
	else if (_player->getPosition().x > owner->getPosition().x)
	{
		owner->get_components<AnimationComponent>()[0]->faceRight = true;
		p->impulse({ 3.0f , 0.0f });
		p->dampen({ 0.7f , 1.0f });
	}
	else if (_player->getPosition().x < owner->getPosition().x)
	{
		owner->get_components<AnimationComponent>()[0]->faceRight = false;
		p->impulse({ -3.0f , 0.0f });
		p->dampen({ 0.7f , 1.0f });
	}

	if (props->getHealth() <= 0)
	{
		owner->get_components<AnimationComponent>()[0]->currentimage.x = 0;
		owner->get_components<PhysicsComponent>()[0]->setVelocity(sf::Vector2f(0, 0));
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}

}


void Orc_AttackState::execute(Entity *owner, double dt) noexcept
{
	auto p = owner->get_components<PhysicsComponent>()[0];
	auto a = owner->get_components<AnimationComponent>()[0];
	auto sm = owner->get_components<StateMachineComponent>()[0];
	auto props = owner->get_components<OrcPropertiesComponent>()[0];

	
	if (_player->getPosition().x < owner->getPosition().x)
	{
		a->faceRight = false;
	}

	if (_player->getPosition().x > owner->getPosition().x)
	{
		a->faceRight = true;
	}
	p->dampen({ 0.85f , 1.0f });

	if (a->attackImgNo >= 6)
	{
		a->attackImgNo = 0;
		props->beginAttackPause();
		sm->changeState("idle");
	}


	if (length(owner->getPosition() - _player->getPosition()) > 400.f)
	{
		sm->changeState("chase");
	}

	if (props->getHealth() <= 0)
	{
		owner->get_components<AnimationComponent>()[0]->currentimage.x = 0;
		owner->get_components<PhysicsComponent>()[0]->setVelocity(sf::Vector2f(0, 0));
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}

}


void  Orc_DeadState::execute(Entity *owner, double dt) noexcept
{
	auto me_anim = owner->get_components<AnimationComponent>()[0];
	if (me_anim->currentimage.x == 7)
	{
		me_anim->currentimage.x = 0;
		makeCoin(owner->getPosition());
		makeCoin(owner->getPosition());
		makeCoin(owner->getPosition());
		makeCoin(owner->getPosition());
		makeCoin(owner->getPosition());
		me_anim->pause = true;
		owner->setForDelete();
	}
}

