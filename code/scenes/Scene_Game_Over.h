#pragma once

#include "engine.h"
#include <SFML/Audio.hpp>

class GameOverScene : public Scene {
public:
	GameOverScene() = default;
	~GameOverScene() override = default;

	void Load() override;

	void UnLoad() override;

	void Update(const double& dt) override;

	void Render() override;

private:

	std::shared_ptr<sf::Music>_musicDeath;

};