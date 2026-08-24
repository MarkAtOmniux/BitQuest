
#include "cmp_physics.h"
#include "cmp_player_physics.h"
#include "cmp_player_controller.h"
#include "cmp_orc_properties.h"
#include "../code/Audio.h"
#include "cmp_animation.h"

using namespace std;
using namespace sf;

OrcPropertiesComponent::OrcPropertiesComponent(Entity* p)
	: Component(p)
{
	_player = _parent->scene->ents.find("player")[0];
};

void OrcPropertiesComponent::takeDamage(double h)
{
	if (immortal == false)
	{
		Audio::LoadAudio();

		_bufferHit = *(Resources::get<SoundBuffer>("orc_Sounds/orc_Hit.wav"));
		_soundHit.setBuffer(_bufferHit);

		_soundHit.play();
		_soundHit.setVolume(Audio::sfxVolume);


		_parent->get_components<PhysicsComponent>()[0]->knockBackFrom(_player->getPosition(), 14.f);
		_parent->get_components<AnimationComponent>()[0]->flashHit();
		immortal = true;
		_health = _health - h;
		this->checkHealth();
	}
}

double OrcPropertiesComponent::getHealth()
{
	return _health;
}

void OrcPropertiesComponent::update(double dt)
{
	if (length(_parent->getPosition() - _player->getPosition()) < 300)
	{
		this->checkContact(dt);
	}

	_parent->get_components<SpriteComponent>()[0]->Sprite("EnemyHealth.png", rect);

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

void OrcPropertiesComponent::render()
{

}

void OrcPropertiesComponent::checkContact(double dt)
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
				ap->takeDamage(orcDamage, dt);
			}
		}
	}

}
sf::IntRect OrcPropertiesComponent::checkHealth()
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