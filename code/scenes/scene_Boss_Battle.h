#pragma once

#include "engine.h"
#include "SFML/Audio.hpp"


class BossBattleScene : public Scene {
public:
	double holdTime = 4;
	double totalTime = 0;

	void Load() override;

	void UnLoad() override;

	void Update(const double& dt) override;

	void Render() override;

	std::shared_ptr<sf::Music>_musicLevel1;

};