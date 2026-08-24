#include "skeleton_states.h"
#include "components/cmp_skeleton_properties.h"
#include "components/cmp_physics.h"
#include "components/cmp_animation.h"
#include "components/cmp_hurt.h"
#include "Prefabs.h"

void  Skeleton_IdleState::execute(Entity *owner, double dt) noexcept
{

	auto sm = owner->get_components<StateMachineComponent>()[0];


	if (owner->get_components<SkeletonPropertiesComponent>()[0]->getHealth() <= 0)
	{
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
			sm->changeState("Attack");
		}
		else if (length(owner->getPosition() - _player->getPosition()) < 800.0f)
		{
			sm->changeState("chase");
		}
	}

}

void  Skeleton_ChaseState::execute(Entity *owner, double dt) noexcept
{
	auto p = owner->get_components<PhysicsComponent>()[0];
	auto g = owner->get_components<PhysicsComponent>()[0];
	auto sm = owner->get_components<StateMachineComponent>()[0];

	if (owner->get_components<SkeletonPropertiesComponent>()[0]->getHealth() <= 0)
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}


	if (length(owner->getPosition() - _player->getPosition()) < 200.0f)
	{
		sm->changeState("Attack");
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


void Skeleton_AttackState::execute(Entity *owner, double dt) noexcept
{
	auto p = owner->get_components<PhysicsComponent>()[0];


	if (_player->getPosition().x > owner->getPosition().x)
	{
		owner->get_components<AnimationComponent>()[0]->faceRight = true;
		p->impulse({ 2.0f , 0.0f });
		p->dampen({ 0.7f , 0.0f });
	}

	//follow left
	if (_player->getPosition().x < owner->getPosition().x)
	{
		owner->get_components<AnimationComponent>()[0]->faceRight = false;
		p->impulse({ -2.0f , 0.0f });
		p->dampen({ 0.7f , 0.0f });

	}


	if (owner->get_components<SkeletonPropertiesComponent>()[0]->getHealth() <= 0)
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}

	if (length(owner->getPosition() - _player->getPosition()) > 200.0f)
	{
		auto sm = owner->get_components<StateMachineComponent>()[0];
		sm->changeState("chase");
	}

	if (owner->get_components<SkeletonPropertiesComponent>()[0]->getHealth() <= 0)
	{
		owner->get_components<StateMachineComponent>()[0]->changeState("dead");
	}
}


void  Skeleton_DeadState::execute(Entity *owner, double dt) noexcept
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

