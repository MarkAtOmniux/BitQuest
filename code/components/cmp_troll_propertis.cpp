#include "cmp_troll_properties.h"
#include "cmp_player_physics.h"
#include "cmp_player_controller.h"
#include <iostream>
#include "../code/GameState.h"
#include "../code/Audio.h"

using namespace sf;
using namespace std;

TrollPropertiesComponent::TrollPropertiesComponent(Entity* p)
	: Component(p)
{
	_player = _parent->scene->ents.find("player")[0];
	_trolls = _parent->scene->ents.find("troll");
};


void TrollPropertiesComponent::takeDamage(double h)
{

	if (immortal == false)
	{
		Audio::LoadAudio();
		
		_bufferHit = *(Resources::get<SoundBuffer>("Troll_Sounds/Troll_Hit.wav"));
		_soundHit.setBuffer(_bufferHit);

		_soundHit.play();
		_soundHit.setVolume(Audio::sfxVolume);

		std::cout << _health << std::endl;
		immortal = true;
		this->_health = _health - h;

	}

}

double TrollPropertiesComponent::getHealth()
{
	return _health;
}

void TrollPropertiesComponent::update(double dt)
{

	//only check when near player (saves performance evaluation of position Runs in Constant time loop runs in liniar time avoid where possible)
	if (length(_parent->getPosition() - _player->getPosition()) > 50)
	{
		checkContact(dt);
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

	if (attackPause > 0.f)
	{
		attackPause -= static_cast<float>(dt);
	}
}

void TrollPropertiesComponent::render()
{

}

void TrollPropertiesComponent::checkContact(double dt)
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
				if (_player->get_components<AnimationComponent>()[0]->attackImgNo >= 6)
				{
					this->takeDamage(ap->playerDamage);
				}
				
			}	
			if (_parent->get_components<AnimationComponent>()[0]->attackImgNo >= 5)
			{
				ap->takeDamage(trollDamage, dt);
			}
				
			

		}
	}

}

sf::IntRect TrollPropertiesComponent::checkHealth()
{

		if (_health == 10)
		{
			rect = sf::IntRect(0, 0, 100, 5);
		}

		if (_health == 9)
		{
			sf::IntRect(0, 0, 90, 5);
		
		}
		if (_health == 8)
		{
			rect = sf::IntRect(0, 0, 80, 5);
			
		}

		if (_health == 7)
		{
			rect = sf::IntRect(0, 0, 70, 5);
		}
		if (_health == 6)
		{
			rect = sf::IntRect(0, 0, 60, 5);
		}

		if (_health == 5)
		{
			rect = sf::IntRect(0, 0, 50, 5);
		}
		if (_health == 4)
		{
			rect = sf::IntRect(0, 0, 40, 5);
		}

		if (_health == 3)
		{
			rect = sf::IntRect(0, 0, 30, 5);
		}

		if (_health == 2)
		{
			rect = sf::IntRect(0, 0, 20, 5);
		}

		if (_health == 1)
		{
			rect = sf::IntRect(0, 0, 10, 5);
		}
		if (_health == 0)
		{
			rect = sf::IntRect(0, 0, 0, 5);
		}
	
		return rect;

}