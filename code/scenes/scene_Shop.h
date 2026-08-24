#pragma once

#include "engine.h"
#include "../components/cmp_text.h"
#include <ecm.h>
#include <SFML/Audio.hpp>

class ShopScene : public Scene {
public:
	ShopScene() = default;
	~ShopScene() override = default;

	void Load() override;

	void UnLoad() override;

	void Update(const double& dt) override;

	void Render() override;

private:

	sf::SoundBuffer _bufferBuy;

	sf::Sound _soundBuy;

	std::shared_ptr<sf::Music>_musicShop;

	std::shared_ptr<Entity> _txtCoin;
	std::shared_ptr<Entity> _txtArrow;
	std::shared_ptr<Entity> _txtHam;
};