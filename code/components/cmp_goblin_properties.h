#pragma once
#include <ecm.h>
#include "engine.h"
#include <SFML/Audio.hpp>

class GoblinPropertiesComponent : public Component
{
protected:
	float _health = 3;
	float damageDelay = 0.2;
	float totalTime = 0;
	float attackPause = 0.f;
	float attackPauseDuration = 0.4f;
public:

	sf::SoundBuffer _bufferHit;

	sf::Sound _soundHit;


	sf::IntRect rect = sf::IntRect(0,0,100,5);
	float goblinDamage = 1;
	bool immortal = true;
	std::shared_ptr<Entity> _player;
	explicit GoblinPropertiesComponent(Entity* p);
	sf::IntRect checkHealth();
	GoblinPropertiesComponent() = delete;
	double getHealth();
	void takeDamage(double);
	void update(double dt);
	void render() override;
	void checkContact(double dt);
	bool canAttack() const { return attackPause <= 0.f; }
	void beginAttackPause() { attackPause = attackPauseDuration; }
};
