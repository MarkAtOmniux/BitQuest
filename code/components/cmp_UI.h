#pragma once

#include <ecm.h>
#include "SFML/Graphics.hpp"
#include "cmp_text.h"
#include "cmp_animation.h"

class UIComponent : public Component
{
private:

	sf::RectangleShape weaponSelection;

	std::shared_ptr<sf::Texture> _texHeartUI;
	std::shared_ptr<sf::Texture> _texCoinUI;
	std::shared_ptr<sf::Texture> _texArrowUI;
	std::shared_ptr<sf::Texture> _texSwordUI;
	std::shared_ptr<sf::Texture> _texBowUI;
	std::shared_ptr<sf::Texture> _texSword;
	std::shared_ptr<sf::Texture> _texBow;
	std::shared_ptr<sf::Texture> _texHamUI;

	sf::Sprite heartUI;
	sf::Sprite coinUI;
	sf::Sprite arrowUI;
	sf::Sprite weaponUI;
	sf::Sprite hamUI;

	std::shared_ptr<TextComponent> _coinTxt;
	std::shared_ptr<TextComponent> _arrowTxt;
	std::shared_ptr<TextComponent> _hamTxt;
	std::shared_ptr<TextComponent> _cutsceneTxt;
	std::shared_ptr<ShapeComponent> _cutsceneBar;

	std::shared_ptr<Entity> _player;

public:
	UIComponent() = delete;
	explicit UIComponent(Entity* p, std::shared_ptr<TextComponent>, std::shared_ptr<TextComponent>, std::shared_ptr<TextComponent>, std::shared_ptr<TextComponent>, std::shared_ptr<ShapeComponent>);
	bool sword = true;
	void setHealthDisplay(sf::IntRect s);
	void cutSceneMode(bool isCutscene);
	void update(double dt) override;
	void render() override;
};