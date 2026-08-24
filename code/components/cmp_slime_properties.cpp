#include "cmp_slime_properties.h"
#include "cmp_player_physics.h"
#include "cmp_player_controller.h"
#include "cmp_physics.h"
#include "cmp_animation.h"
#include <ecm.h>
#include <SFML/Audio.hpp>
#include "../GameState.h"
#include "../code/Audio.h"
#include <iostream>

using namespace std;
using namespace sf;

SlimePropertiesComponent::SlimePropertiesComponent(Entity* p)
	: Component(p)
{
	_player = _parent->scene->ents.find("player")[0];
}


void SlimePropertiesComponent::takeDamage(double h)
{

	if (immortal == false)
	{
		Audio::LoadAudio();

		_bufferHit = *(Resources::get<SoundBuffer>("Slime_Sounds/Slime_Hit.wav"));
		_soundHit.setBuffer(_bufferHit);

		_soundHit.play();
		_soundHit.setVolume(Audio::sfxVolume);

		immortal = true;
		this->_health = _health - h;
		_parent->get_components<PhysicsComponent>()[0]->knockBackFrom(_player->getPosition(), 12.f);
		_parent->get_components<AnimationComponent>()[0]->flashHit();

	}

}

double SlimePropertiesComponent::getHealth()
{
	return _health;
}

void SlimePropertiesComponent::update(double dt)
{

	//only check when near player (saves performance evaluation of position Runs in Constant time loop runs in liniar time avoid where possible)
	if (length(_parent->getPosition() - _player->getPosition()) > 50)
	{
		this->checkContact(dt);
	}


	_parent->get_components<SpriteComponent>()[0]->Sprite("EnemyHealth.png", rect);


	this->checkHealth();
	if (immortal == true)
	{
		totalTime += dt;

		if (totalTime >= damageDelay)
		{
			totalTime -= damageDelay;
			immortal = false;
		}
	}
}

void SlimePropertiesComponent::render()
{

}

void SlimePropertiesComponent::checkContact(double dt)
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

		}
	}

}

sf::IntRect SlimePropertiesComponent::checkHealth()
{

	
	if (_health == 4)
	{
		rect = sf::IntRect(0, 0, 100, 5);
	}

	if (_health == 3)
	{
		rect = sf::IntRect(0, 0, 75, 5);
	}

	if (_health == 2)
	{
		rect = sf::IntRect(0, 0, 50, 5);
	}

	if (_health == 1)
	{
		rect = sf::IntRect(0, 0, 25, 5);
	}
	if (_health == 0)
	{
		rect = sf::IntRect(0, 0, 0, 5);
	}

	return rect;

}