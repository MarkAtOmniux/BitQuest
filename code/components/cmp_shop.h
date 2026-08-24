#pragma once

#include <ecm.h>

#include "cmp_animation.h"
#include <SFML/Audio.hpp>

class ShopComponent : public Component
{
private:
	
	std::shared_ptr<Entity> _player;

public:
	ShopComponent() = delete;
	explicit ShopComponent(Entity* p);
	void update(double dt) override;
	void render() override {}

};