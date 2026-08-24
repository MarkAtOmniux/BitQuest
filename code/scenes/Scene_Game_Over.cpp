#include "Scene_Game_Over.h"
#include "../GameState.h"
#include "../code/Prefabs.h"
#include "../components/cmp_btn.h"
#include "../code/components/cmp_text.h"
#include "engine.h"
#include "../code/SaveLoad.h"
#include "levelsystem.h"
#include "../code/Audio.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

static shared_ptr<Entity> btnExit;
static shared_ptr<Entity> btnRetry;

void GameOverScene::Load()
{
	ls::loadLevelFile("res/tilemaps/DeadBackground.txt", 240.f);

	Audio::LoadAudio();

	//Music
	_musicDeath= Resources::get<Music>("Death_Music.ogg");
	_musicDeath->play();
	_musicDeath->setLoop(true);
	_musicDeath->setVolume(Audio::musicVolume);


	{
		auto txtDeath = makeEntity();
		auto t = txtDeath->addComponent<TextComponent>("Game Over, That was absolutely terrible. The worst thing I have ");
		auto t2 = txtDeath->addComponent<TextComponent>("ever seen, If I had eyeballs I would quit my job as a Death Scene");
		auto t3 = txtDeath->addComponent<TextComponent>(" so I would never have to look at travesties such as you.");
		t->getText().setOrigin(t->getText().getGlobalBounds().width / 2, t->getText().getGlobalBounds().height / 2);
		t2->getText().setOrigin(t2->getText().getGlobalBounds().width / 2, t2->getText().getGlobalBounds().height / 2 - 50);
		t3->getText().setOrigin(t3->getText().getGlobalBounds().width / 2, t3->getText().getGlobalBounds().height / 2 - 100);
		txtDeath->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, 100.f));
	}

	{
		btnRetry = makeButton("Try Again", Vector2f(250, 60));
		btnRetry->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, 500.f));

		btnExit = makeButton("Exit", Vector2f(250, 60));
		btnExit->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2,600.f));
	}

	Engine::GetWindow().setView(Engine::GetWindow().getDefaultView());

	setLoaded(true);
}

void GameOverScene::UnLoad()
{
	_musicDeath->stop();
	_musicDeath.reset();

	ls::unload();
	Scene::UnLoad();
}

void GameOverScene::Update(const double& dt)
{

	if (btnRetry->get_components<BtnComponent>()[0]->isSelected())
	{
		SaveLoad::ResetGame();
		Engine::ChangeScene((Scene*)&level1);
	}

	if (btnExit->get_components<BtnComponent>()[0]->isSelected())
	{
		SaveLoad::ResetGame();
		Engine::ChangeScene((Scene*)&menu);
	}

	Scene::Update(dt);
}

void GameOverScene::Render()
{
	ls::render(Engine::GetWindow());
	Scene::Render();
}



