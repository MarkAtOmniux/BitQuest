#pragma once
#include <ecm.h>
#include "engine.h"
#include <SFML/Audio.hpp>

class SkeletonPropertiesComponent : public Component
{
protected:
	float _health = 2;
	float damageDelay = 0.2;
	float totalTime = 0;
	float attackPause = 0.f;
	float attackPauseDuration = 0.85f;

public:

	sf::SoundBuffer _bufferHit;

	sf::Sound _soundHit;


	sf::IntRect rect;
	bool immortal = true;
	float skeletonDamage = 1;
	std::shared_ptr<Entity> _player;
	explicit SkeletonPropertiesComponent(Entity* p);
	void checkHealth();
	SkeletonPropertiesComponent() = delete;
	double getHealth();
	void takeDamage(double);
	void update(double dt);
	void render() override;
	void checkContact(double dt);
	bool canAttack() const { return attackPause <= 0.f; }
	void beginAttackPause() { attackPause = attackPauseDuration; }
};