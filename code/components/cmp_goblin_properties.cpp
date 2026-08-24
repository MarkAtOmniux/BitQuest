#include "cmp_goblin_properties.h"
#include "cmp_physics.h"
#include "cmp_player_physics.h"
#include "cmp_player_controller.h"
#include "cmp_animation.h"
#include "../code/Audio.h"

using namespace std;
using namespace sf;

GoblinPropertiesComponent::GoblinPropertiesComponent(Entity* p)
	: Component(p)
{
	_player = _parent->scene->ents.find("player")[0];
};

void GoblinPropertiesComponent::takeDamage(double h)
{
	if (immortal == false)
	{

		Audio::LoadAudio();

		_bufferHit = *(Resources::get<SoundBuffer>("Goblin_Sounds/Goblin_Hit.wav"));
		_soundHit.setBuffer(_bufferHit);

		_soundHit.play();
		_soundHit.setVolume(Audio::sfxVolume);

		immortal = true;
		_health = _health - h;

		if (_player->getPosition().x < _parent->getPosition().x)
		{
			_parent->get_components<PhysicsComponent>()[0]->getFixture()->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(50.f , 0.f),true);
		}
		else
		{
			_parent->get_components<PhysicsComponent>()[0]->getFixture()->GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-50.f, 0.f), true);
		}
		this->checkHealth();
	}
}

double GoblinPropertiesComponent::getHealth()
{
	return _health;
}


void GoblinPropertiesComponent::update(double dt)
{
	if (length(_parent->getPosition() - _player->getPosition()) < 100)
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
void GoblinPropertiesComponent::render()
{

}
void GoblinPropertiesComponent::checkContact(double dt)
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
				if (_player->get_components<AnimationComponent>()[0]->attackImgNo >= 5)
				{
					this->takeDamage(ap->playerDamage);
				}
			}
			if (_parent->get_components<AnimationComponent>()[0]->attackImgNo >= 5)
			{
				ap->takeDamage(goblinDamage, dt);
			}
		}
	}

}

sf::IntRect GoblinPropertiesComponent::checkHealth()
{
	if (_health == 3)
	{
		rect = sf::IntRect(0, 0, 100, 5);
	}

	if (_health == 2)
	{
		rect = sf::IntRect(0, 0, 66, 5);
	}

	if (_health == 1)
	{
		rect = sf::IntRect(0, 0, 33, 5);
	}
	if (_health == 0)
	{
		rect = sf::IntRect(0, 0, 0, 5);
	}

	return rect; 

}