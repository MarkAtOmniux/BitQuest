#pragma once
#include <ecm.h>
#include "engine.h"
#include <SFML/Audio.hpp>

class OrcPropertiesComponent : public Component
{
protected:
	float _health = 4;
	float damageDelay = 0.2;
	float totalTime = 0;
	float attackPause = 0.f;
	float attackPauseDuration = 1.4f;
public:

	sf::SoundBuffer _bufferHit;

	sf::Sound _soundHit;

	sf::IntRect rect = sf::IntRect(0, 0, 100, 5);
	float orcDamage = 1;
	bool immortal = true;
	std::shared_ptr<Entity> _player;
	explicit OrcPropertiesComponent(Entity* p);
	sf::IntRect checkHealth();
	OrcPropertiesComponent() = delete;
	double getHealth();
	void takeDamage(double);
	void update(double dt);
	void render() override;
	void checkContact(double dt);
	bool canAttack() const { return attackPause <= 0.f; }
	void beginAttackPause() { attackPause = attackPauseDuration; }
};