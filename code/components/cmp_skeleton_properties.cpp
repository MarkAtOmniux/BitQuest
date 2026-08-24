#include "cmp_skeleton_properties.h"
#include "cmp_player_physics.h"
#include "cmp_player_controller.h"
#include "cmp_physics.h"
#include "cmp_animation.h"
#include "../code/GameState.h"
#include "../code/Audio.h"
#include <iostream>

using namespace std;
using namespace sf;

SkeletonPropertiesComponent::SkeletonPropertiesComponent(Entity* p)
	: Component(p)
{
	_player = _parent->scene->ents.find("player")[0];

}


void SkeletonPropertiesComponent::takeDamage(double h)
{

	if (immortal == false)
	{
		Audio::LoadAudio();

		_bufferHit = *(Resources::get<SoundBuffer>("Skeleton_Sounds/Skeleton_Hit.wav"));
		_soundHit.setBuffer(_bufferHit);

		_soundHit.play();
		_soundHit.setVolume(Audio::sfxVolume);

		_parent->get_components<PhysicsComponent>()[0]->knockBackFrom(_player->getPosition(), 15.f);
		_parent->get_components<AnimationComponent>()[0]->flashHit();
		immortal = true;
		_health = _health - h;

	}

}

double SkeletonPropertiesComponent::getHealth()
{
	return _health;
}

void SkeletonPropertiesComponent::update(double dt)
{

	//only check when near player (saves performance evaluation of position Runs in Constant time loop runs in liniar time avoid where possible)
	if (length(_parent->getPosition() - _player->getPosition()) > 50)
	{
		this->checkContact(dt);
	}



	_parent->get_components<SpriteComponent>()[0]->Sprite("EnemyHealth.png", rect);



	checkHealth();
	if (immortal == true)
	{
		totalTime += dt;

		if (totalTime >= damageDelay)
		{
			totalTime -= damageDelay;
			immortal = false;
		}
	}

	if (attackPause > 0.f)
	{
		attackPause -= static_cast<float>(dt);
	}
}

void SkeletonPropertiesComponent::render()
{

}

void SkeletonPropertiesComponent::checkContact(double dt)
{

	auto cs = _parent->get_components<PhysicsComponent>()[0]->getTouching();

	auto ap = _player->get_components<PlayerControlerComponent>()[0];
	auto playerPhys = _player->get_components<PlayerPhysicsComponent>()[0];

	for (auto c : cs)
	{
		if (c->GetFixtureA() == playerPhys->getFixture() || c->GetFixtureB() == playerPhys->getFixture())
		{
			if (_player->get_components<StateMachineComponent>()[0]->currentState() == "Attack")
			{
				if (_player->get_components<AnimationComponent>()[0]->attackImgNo >= 2)
				{
					this->takeDamage(ap->playerDamage);
				}
			}	
			if (_parent->get_components<AnimationComponent>()[0]->attackImgNo >= 5)
			{
				ap->takeDamage(skeletonDamage, dt);
			}
				
			

		}
	}

}

void SkeletonPropertiesComponent::checkHealth()
{
	if (_health == 2)
	{
		rect = sf::IntRect(0, 0, 100, 5);
	}

	if (_health == 1)
	{
		rect = sf::IntRect(0, 0, 50, 5);
	}
	if (_health == 0)
	{
		rect = sf::IntRect(0, 0, 0, 5);
	}

}