#pragma once

#include <ecm.h>
#include "cmp_text.h"
#include "cmp_animation.h"
#include <SFML/Audio.hpp>

class ChestComponent : public Component
{
private:
	
	std::shared_ptr<Entity> _player;


public:
	ChestComponent() = delete;
	explicit ChestComponent(Entity* p);
	void update(double dt) override;
	void render() override {}
};