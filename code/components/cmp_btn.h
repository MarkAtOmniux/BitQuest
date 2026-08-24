#pragma once

#include <ecm.h>
#include "cmp_text.h"
#include "cmp_animation.h"
#include <SFML/Audio.hpp>

class BtnComponent : public Component
{
private:
	bool _isHighlighted = false;
	bool _isTouching = false;
	bool selected = false;
	std::shared_ptr<ShapeComponent> _shapeCmp;
	std::shared_ptr<TextComponent> _textCmp;
	std::shared_ptr<ShapeComponent> _underLine;
	sf::SoundBuffer _bufferClick;
	sf::Sound _soundClick;


public:
	BtnComponent() = delete;
	explicit BtnComponent(Entity* p, std::shared_ptr<ShapeComponent>, std::shared_ptr<TextComponent>, std::shared_ptr<ShapeComponent>);
	void update(double dt) override;
	void render() override {}
	void setHighlighted(bool, bool isClicked = false);
	void setFilled();
	bool isSelected();

};