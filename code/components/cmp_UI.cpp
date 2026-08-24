#include "cmp_UI.h"
#include "engine.h"
#include "cmp_state_machine.h"
#include "System_Renderer.h"
#include "system_resources.h"
#include "../code/Prefabs.h"
#include "../code/SaveLoad.h"
#include "cmp_text.h"
#include "../code/GameState.h"

using namespace std;
using namespace sf;

UIComponent::UIComponent(Entity* p, shared_ptr<TextComponent> c, shared_ptr<TextComponent> a, shared_ptr<TextComponent> h, shared_ptr<TextComponent> cutsceneText, shared_ptr<ShapeComponent> b)
	: _coinTxt(c), _arrowTxt(a), _hamTxt(h), _cutsceneTxt(cutsceneText), _cutsceneBar(b), Component(p)
{

	_player = _parent->scene->ents.find("player")[0];

	_texHeartUI = Resources::load<Texture>("heart.png");
	_texCoinUI = Resources::load<Texture>("spritesheets/Coin_spritesheet.png");
	_texArrowUI = Resources::load<Texture>("arrowUI.png");
	_texSwordUI = Resources::load<Texture>("SwordUI.png");
	_texBowUI = Resources::load<Texture>("BowUI.png");
	_texHamUI = Resources::load<Texture>("HamUI.png");

	heartUI = Sprite(*_texHeartUI);
	heartUI.setTextureRect(IntRect(0, 0, 240, 60));

	coinUI = Sprite(*_texCoinUI);
	coinUI.setTextureRect(IntRect(0, 0, 60, 60));

	arrowUI = Sprite(*_texArrowUI);
	arrowUI.setTextureRect(IntRect(0, 0, 60, 60));

	weaponUI = Sprite(*_texSwordUI);
	weaponUI.setTextureRect(IntRect(0, 0, 60, 60));

	hamUI = Sprite(*_texHamUI);
	hamUI.setTextureRect(IntRect(0, 0, 60, 60));

}

void UIComponent::update(double dt)
{
	_parent->setPosition({
		Engine::GetWindow().getView().getCenter().x,
		Engine::GetWindow().getView().getCenter().y });


	if (Engine::isKeyDown(Keyboard::Num1))
	{
		auto p = _player->get_components<AnimationComponent>()[0];
		p->Animation("spritesheets/Bob_archer_spritesheetV2.png", Vector2f(0, 120), IntRect(0, 0, 240, 240), Vector2u(8, 8));
		p->getSprite().setOrigin(p->getSprite().getGlobalBounds().width / 2, p->getSprite().getGlobalBounds().height / 2);;
		weaponUI = Sprite(*_texBowUI);
		sword = false;
	}

	if (Engine::isKeyDown(Keyboard::Num2))
	{
		auto p = _player->get_components<AnimationComponent>()[0];
		_player->get_components<AnimationComponent>()[0]->Animation("spritesheets/Bob_spritesheet.png", Vector2f(0, 120), IntRect(0, 0, 240, 240), Vector2u(8, 8));
		p->getSprite().setOrigin(p->getSprite().getGlobalBounds().width / 2, p->getSprite().getGlobalBounds().height / 2);
		sword = true;
		weaponUI = Sprite(*_texSwordUI);
	}

	heartUI.setPosition(
		_parent->getPosition().x - Engine::GetWindow().getSize().x / 2 + 30.f,
		_parent->getPosition().y - Engine::GetWindow().getSize().y / 2 + 10.f);

	coinUI.setPosition(
		_parent->getPosition().x + Engine::GetWindow().getSize().x / 3,
		_parent->getPosition().y - Engine::GetWindow().getSize().y / 2 + 10.f);

	arrowUI.setPosition(
		_parent->getPosition().x + Engine::GetWindow().getSize().x / 3,
		_parent->getPosition().y + Engine::GetWindow().getSize().y / 2.7);

	weaponUI.setPosition(
		_parent->getPosition().x - Engine::GetWindow().getSize().x / 50,
		_parent->getPosition().y + Engine::GetWindow().getSize().y / 2.7);

	hamUI.setPosition(
		_parent->getPosition().x - Engine::GetWindow().getSize().x / 2.3,
		_parent->getPosition().y + Engine::GetWindow().getSize().y / 2.7);

	_coinTxt->getText().setPosition(
		coinUI.getPosition().x + 100, coinUI.getPosition().y + 30);

	_coinTxt->getText().setString(to_string(SaveLoad::coins));
	_arrowTxt->getText().setString(to_string(SaveLoad::arrows));
	_hamTxt->getText().setString(to_string(SaveLoad::hams));

	_arrowTxt->getText().setPosition(
		arrowUI.getPosition().x + 100, arrowUI.getPosition().y + 30);

	_hamTxt->getText().setPosition(
		hamUI.getPosition().x + 100, hamUI.getPosition().y + 30);

	_cutsceneBar->getShape().setPosition(_parent->getPosition().x - Engine::GetWindow().getSize().x / 2,
		_parent->getPosition().y + Engine::GetWindow().getSize().y / 3);

	_cutsceneTxt->getText().setPosition(
		hamUI.getPosition().x + 200, hamUI.getPosition().y + 30);

}

void UIComponent::render()
{
	Renderer::queue(&weaponSelection);
	Renderer::queue(&heartUI);
	Renderer::queue(&coinUI);
	Renderer::queue(&arrowUI);
	Renderer::queue(&weaponUI);
	Renderer::queue(&hamUI);
}

void UIComponent::setHealthDisplay(sf::IntRect s)
{
	heartUI.setTextureRect(s);
}

void UIComponent::cutSceneMode(bool isCutscene)
{
	if (isCutscene == true)
	{
		_cutsceneBar->getShape().setFillColor(Color::Black);
		hamUI.setColor(Color::Transparent);
		arrowUI.setColor(Color::Transparent);
	}
	else
	{
		_cutsceneBar->getShape().setFillColor(Color::Transparent);
	}
}