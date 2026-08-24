#pragma once

#pragma once
#include "ecm.h"
#include "engine.h"
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>
#include <../engine/src/System_Renderer.h>
#include <../engine/src/system_resources.h>


class ObjectAnimComponent : public Component {
protected:
	std::shared_ptr<sf::Sprite> _sprite;
	std::shared_ptr<sf::Texture> _texture;

public:

	double switchtime;
	sf::IntRect animUvRect;
	sf::Vector2u currentimage;
	double totaltime;
	sf::Vector2u imagecount;

	void Anim(double dt);

	void Animation(std::string a, sf::Vector2f b, sf::IntRect animUvRect, sf::Vector2u imagecount);

	ObjectAnimComponent() = delete;
	explicit ObjectAnimComponent(Entity* p);
	void update(double dt);
	void render() override;

	sf::Sprite& getSprite() const;

	template <typename... Targs> void setSprite(Targs... params) {
		_sprite.reset(new sf::Sprite(params...));
	}

};
