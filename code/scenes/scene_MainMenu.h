#pragma once
#include "engine.h"
#include "SFML/Audio.hpp"

class MainMenuScene : public Scene {
public:
	MainMenuScene() = default;
	~MainMenuScene() override = default;

	void Load() override;

	void UnLoad() override;

	void Update(const double& dt) override;

	void Render() override;


private:

	std::shared_ptr<sf::Music>_music_menu;

};