#include "scene_Settings_Graphics.h"
#include "../components/cmp_text.h"
#include "../components/cmp_animation.h"
#include "../components/cmp_sprite.h"
#include "SFML/Graphics.hpp"
#include "../GameState.h"
#include "levelsystem.h"
#include "../components/cmp_btn.h"
#include "../code/Prefabs.h"
#include "engine.h"
#include <fstream>

using namespace std;
using namespace sf;

static shared_ptr<Entity> btnBack;
static shared_ptr<Entity> btnDone;

static shared_ptr<Entity> ResLow;
static shared_ptr<Entity> ResMed;
static shared_ptr<Entity> ResHigh;
static shared_ptr<Entity> btnOn;
static shared_ptr<Entity> btnOff;
static shared_ptr<Entity> btn30;
static shared_ptr<Entity> btn60;


void SettingsGraphicsScene::Load()
{
	ls::loadLevelFile("res/tilemaps/backgrounds.txt", 240.f);

	{
		btnBack = makeButton("Back", Vector2f(150, 60));
		btnBack->setPosition(Vector2f(Engine::GetWindow().getSize().x / 7, 100.f));
	}

	{
		auto txtGraphics = makeEntity();
		auto t = txtGraphics->addComponent<TextComponent>("Graphics");
		t->getText().setOrigin(t->getText().getGlobalBounds().width / 2, t->getText().getGlobalBounds().height / 2);
		txtGraphics->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, 100.f));
	}

	{
		auto txtResolution = makeEntity();
		auto t = txtResolution->addComponent<TextComponent>("Resolution");
		t->getText().setOrigin(0, t->getText().getGlobalBounds().height / 2);
		txtResolution->setPosition(Vector2f(Engine::GetWindow().getSize().x / 6, 250.f));

		ResLow = makeButton("1280x720", Vector2f(150, 60));
		ResLow->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, 250.f));

		ResMed = makeButton("1600x900", Vector2f(150, 60));
		ResMed->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 + 230, 250.f));

		ResHigh = makeButton("1920x1080", Vector2f(150, 60));
		ResHigh->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 + 460, 250.f));
	}

	{
		auto txtFPS = makeEntity();
		auto t = txtFPS->addComponent<TextComponent>("FPS");
		t->getText().setOrigin(0, t->getText().getGlobalBounds().height / 2);
		txtFPS->setPosition(Vector2f(Engine::GetWindow().getSize().x / 6, 350.f));

		btn30 = makeButton("30 FPS", Vector2f(150, 60));
		btn30->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, 350.f));

		btn60 = makeButton("60 FPS", Vector2f(150, 60));
		btn60->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 + 230, 350.f));
	}


	{
		auto txtMode = makeEntity();
		auto t = txtMode->addComponent<TextComponent>("Vysnc");
		t->getText().setOrigin(0, t->getText().getGlobalBounds().height / 2);
		txtMode->setPosition(Vector2f(Engine::GetWindow().getSize().x / 6, 450.f));

		btnOn = makeButton("Fullscreen", Vector2f(150, 60));
		btnOn->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, 450.f));

		btnOff = makeButton("Windowed", Vector2f(150, 60));
		btnOff->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 + 230, 450.f));

	}

	{
		btnDone = makeButton("Done", Vector2f(150, 60));
		btnDone->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, 650.f));
	}
	setLoaded(true);
}

void SettingsGraphicsScene::UnLoad()
{
	ls::unload();
	Scene::UnLoad();
}

void SettingsGraphicsScene::Update(const double& dt)
{
	if (btnBack->get_components<BtnComponent>()[0]->isSelected())
	{
		Engine::ChangeScene((Scene*)&settings);
	}
	Scene::Update(dt);

	if (ResLow->get_components<BtnComponent>()[0]->isSelected())
	{
		game_width = 1280;
		game_heigth = 720;
	}

	if (ResMed->get_components<BtnComponent>()[0]->isSelected())
	{
		game_width = 1600;
		game_heigth = 900;
	}

	if (ResHigh->get_components<BtnComponent>()[0]->isSelected())
	{
		game_width = 1920;
		game_heigth = 1080;
	}

	if (btn60->get_components<BtnComponent>()[0]->isSelected())
	{
		frameRate = 60;
	}

	if (btn30->get_components<BtnComponent>()[0]->isSelected())
	{
		frameRate = 30;
	}

	if (btnOn->get_components<BtnComponent>()[0]->isSelected())
	{
		//windowMode = "Fullscreen";
	}

	if (btnOff->get_components<BtnComponent>()[0]->isSelected())
	{
		//windowMode = "Windowed";
	}

	if (btnDone->get_components<BtnComponent>()[0]->isSelected())
	{
		Engine::GetWindow().close();

		Engine::Start(game_width, game_heigth, "Bitquest!", &menu, frameRate);
	}
}

void SettingsGraphicsScene::Render()
{
	ls::render(Engine::GetWindow());
	Scene::Render();
}