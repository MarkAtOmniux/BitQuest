#pragma once

#include "engine.h"
#include <SFML/Audio.hpp>

class SettingsAudioScene : public Scene {
public:
	void Load() override;

	void UnLoad() override;

	void Update(const double& dt) override;

	void Render() override;

	std::shared_ptr<sf::Music>_musicMenu;
};