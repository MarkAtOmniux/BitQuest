#include "scene_Settings_Gameplay.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "SFML/Graphics.hpp"
#include "../GameState.h"
#include "../code/Prefabs.h"
#include "../components/cmp_btn.h"
#include "../code/Controls.h"
#include "levelsystem.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace sf;


void SettingsGameplayScene::Load()
{

	ls::loadLevelFile("res/tilemaps/Backgrounds.txt", 240.f);

	_btns.clear();
	_changingControl.reset();

	_btn_Back = makeButton("Back", Vector2f(150, 60));
	_btn_Back->setPosition(Vector2f(Engine::GetWindow().getSize().x / 7, 100.f));

	_btn_ControlsAction.reset();
	_btn_ControlsAction = makeButton("Action", Vector2f(40, 40));
	_btn_ControlsAction->setPosition(Vector2f(Engine::GetWindow().getSize().x / 8, 300.f));
	_btns.push_back(_btn_ControlsAction);
	_controlsBtns[_btn_ControlsAction] = "Action";


	_btn_ControlsLeft.reset();
	_btn_ControlsLeft = makeButton("Left", Vector2f(40, 40));
	_btn_ControlsLeft->setPosition(Vector2f(Engine::GetWindow().getSize().x / 8 + 150 , 300.f));
	_btns.push_back(_btn_ControlsLeft);
	_controlsBtns[_btn_ControlsLeft] = "MoveLeft";


	_btn_ControlsRight.reset();
	_btn_ControlsRight = makeButton("Right", Vector2f(40, 40));
	_btn_ControlsRight->setPosition(Vector2f(Engine::GetWindow().getSize().x / 8 + 300, 300.f));
	_btns.push_back(_btn_ControlsRight);
	_controlsBtns[_btn_ControlsRight] = "MoveRight";


	_btn_ControlsJump.reset();
	_btn_ControlsJump = makeButton("Jump", Vector2f(40, 40));
	_btn_ControlsJump->setPosition(Vector2f(Engine::GetWindow().getSize().x / 8 + 450, 300.f));
	_btns.push_back(_btn_ControlsJump);
	_controlsBtns[_btn_ControlsJump] = "Jump";


	_btn_ControlsAttack.reset();
	_btn_ControlsAttack = makeButton("Attack", Vector2f(40, 40));
	_btn_ControlsAttack->setPosition(Vector2f(Engine::GetWindow().getSize().x / 8 + 600, 300.f));
	_btns.push_back(_btn_ControlsAttack);
	_controlsBtns[_btn_ControlsAttack] = "Attack";


	_btn_ControlsBlock.reset();
	_btn_ControlsBlock = makeButton("Block", Vector2f(40, 40));
	_btn_ControlsBlock->setPosition(Vector2f(Engine::GetWindow().getSize().x / 8 + 750, 300.f));
	_btns.push_back(_btn_ControlsBlock);
	_controlsBtns[_btn_ControlsBlock] = "Block";


	_btn_ControlsSword.reset();
	_btn_ControlsSword = makeButton("Sword", Vector2f(40, 40));
	_btn_ControlsSword->setPosition(Vector2f(Engine::GetWindow().getSize().x / 8 + 900, 300.f));
	_btns.push_back(_btn_ControlsSword);
	_controlsBtns[_btn_ControlsSword] = "Sword";


	_btn_ControlsBow.reset();
	_btn_ControlsBow = makeButton("Bow", Vector2f(40, 40));
	_btn_ControlsBow->setPosition(Vector2f(Engine::GetWindow().getSize().x / 8 + 1050, 300.f));
	_btns.push_back(_btn_ControlsBow);
	_controlsBtns[_btn_ControlsBow] = "Bow";


	_btn_Done = makeButton("Done", Vector2f(150, 60));
	_btn_Done->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, 650.f));


	_clickCooldown = 0.2f;


	Engine::GetWindow().setView(Engine::GetWindow().getDefaultView());

	setLoaded(true);
}

void SettingsGameplayScene::UnLoad() {
	ls::unload();
	Scene::UnLoad();
}

void SettingsGameplayScene::Update(const double& dt)
{

	if (_clickCooldown >= 0.0f) _clickCooldown -= dt;

	if (_clickCooldown < 0.0f)
	{

		if (_changingControl == nullptr)
		{
			for (auto b : _controlsBtns)
			{
				if (b.first->GetCompatibleComponent<BtnComponent>()[0]->isSelected())
				{
					_changingControl = b.first;
					b.first->get_components<TextComponent>()[0]->getText().setColor(Color(254, 203, 82));
				}
			}
		}
	}

	if (_changingControl != nullptr)
	{
		for (auto k = Keyboard::Unknown; k != Keyboard::Pause; k = static_cast<Keyboard::Key>(k + 1))
		{
			Controls::SetKeyboardButton(_controlsBtns[_changingControl], k);
			_changingControl->get_components<TextComponent>()[0]->getText().setColor(Color(198, 152, 127));
			_changingControl = nullptr;
		}
	}

	if (_btn_Back->get_components<BtnComponent>()[0]->isSelected())
	{
		Engine::ChangeScene((Scene*)&settings);
	}


	if (_btn_Done->get_components<BtnComponent>()[0]->isSelected())
	{
		Engine::ChangeScene((Scene*)&settings);
	}


Scene::Update(dt);
}


void SettingsGameplayScene::Render()
{
	ls::render(Engine::GetWindow());
	Scene::Render();
}