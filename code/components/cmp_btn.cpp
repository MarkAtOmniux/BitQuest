#include "cmp_btn.h"
#include "engine.h"
#include "../code/Audio.h"
#include <SFML/Window/Mouse.hpp>

using namespace std;
using namespace sf;

BtnComponent::BtnComponent(Entity* p, shared_ptr<ShapeComponent> s, shared_ptr<TextComponent> t, shared_ptr<ShapeComponent> u)
	: _shapeCmp(s), _textCmp(t), _underLine(u), Component(p)
{
		setHighlighted(false, true);

		Audio::LoadAudio();
}

void BtnComponent::update(double dt)
{
	auto mousePos = Engine::GetWindow().mapPixelToCoords(Mouse::getPosition(Engine::GetWindow()));
	if (_shapeCmp->getShape().getGlobalBounds().contains(mousePos))
	{
		setHighlighted(true);

		_isTouching = true;
	}
	else
	{
		setHighlighted(false);

		_isTouching = false;
	}
}

void BtnComponent::setHighlighted(bool h, bool isClicked)
{
	if (h != _isHighlighted || isClicked)
	{
		_isHighlighted = h;
		if (h)
		{
			_textCmp->getText().setFillColor(Color(240, 178, 0));
			_underLine->getShape().setFillColor(Color(240, 178, 0));

			_bufferClick = *(Resources::get<SoundBuffer>("Game_Sounds/Click.ogg"));
			_soundClick.setBuffer(_bufferClick);

			_soundClick.play();
			_soundClick.setVolume(Audio::sfxVolume);
		}
		else
		{
			_textCmp->getText().setFillColor(Color(255, 255, 255));
			_underLine->getShape().setFillColor(Color(128, 128, 128));
		}
	}
}

void BtnComponent::setFilled()
{
	_textCmp->getText().setFillColor(Color(240, 178, 0));
	_underLine->getShape().setFillColor(Color(240, 178, 0));
}

bool BtnComponent::isSelected()
{
	if (_isHighlighted)
	{
		if (Engine::isMouseDown(Mouse::Left))
		{
			_textCmp->getText().setFillColor(Color(240, 178, 0));
			_underLine->getShape().setFillColor(Color(240, 178, 0));
			return true;
		}
	}
	return false;
}
