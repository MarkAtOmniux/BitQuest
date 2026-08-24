#include "scene_Settings_Audio.h"
#include "../components/cmp_text.h"
#include "../components/cmp_animation.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_btn.h"
#include "../code/Prefabs.h"
#include "SFML/Graphics.hpp"
#include "../GameState.h"
#include "../code/Audio.h"
#include "levelsystem.h"

using namespace std;
using namespace sf;

static shared_ptr<Entity> btnBack;
static shared_ptr<Entity> btnDone;
static shared_ptr<Entity> btnMusicLow;
static shared_ptr<Entity> btnMusicMed;
static shared_ptr<Entity> btnMusicHigh;
static shared_ptr<Entity> btnSFXLow;
static shared_ptr<Entity> btnSFXMed;
static shared_ptr<Entity> btnSFXHigh;

void SettingsAudioScene::Load()
{

	Audio::LoadAudio();

	ls::loadLevelFile("res/tilemaps/backgrounds.txt", 240.f);

	//Music
	_musicMenu = Resources::get<Music>("Menu_Music.ogg");
	_musicMenu->play();
	_musicMenu->setLoop(true);
	_musicMenu->setVolume(Audio::musicVolume);


	{
		//Back button
		auto txtAudio = makeEntity();
		auto t = txtAudio->addComponent<TextComponent>("Audio");
		t->getText().setOrigin(t->getText().getGlobalBounds().width / 2, t->getText().getGlobalBounds().height / 2);
		txtAudio->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, 100.f));

		btnBack = makeButton("Back", Vector2f(150, 60));
		btnBack->setPosition(Vector2f(Engine::GetWindow().getSize().x / 7, 100.f));
	}

	{
		//Music audio slider

		auto txtMusic = makeEntity();
		auto t = txtMusic->addComponent<TextComponent>("Music Volume");
		t->getText().setOrigin(0, t->getText().getGlobalBounds().height / 2);
		txtMusic->setPosition(Vector2f(Engine::GetWindow().getSize().x / 6, 250.f));

		btnMusicLow = makeButton("Off", Vector2f(150, 60));
		btnMusicLow->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, 250.f));

		btnMusicMed = makeButton("Low", Vector2f(150, 60));
		btnMusicMed->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 + 230, 250.f));

		btnMusicHigh = makeButton("High", Vector2f(150, 60));
		btnMusicHigh->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 + 460, 250.f));

		
	}

	{
		//SFX audio slider

		auto txtSFX = makeEntity();
		auto t = txtSFX->addComponent<TextComponent>("SFX Audio");
		t->getText().setOrigin(0, t->getText().getGlobalBounds().height / 2);
		txtSFX->setPosition(Vector2f(Engine::GetWindow().getSize().x / 6, 450.f));

		btnSFXLow = makeButton("Off", Vector2f(150, 60));
		btnSFXLow->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, 450.f));

		btnSFXMed = makeButton("Low", Vector2f(150, 60));
		btnSFXMed->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 + 230, 450.f));

		btnSFXHigh = makeButton("High", Vector2f(150, 60));
		btnSFXHigh->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 + 460, 450.f));
	}

	{
		//Save settings

		btnDone = makeButton("Done", Vector2f(150, 60));
		btnDone->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, 650.f));

	}

	setLoaded(true);
}

void SettingsAudioScene::UnLoad() {
	_musicMenu->stop();
	_musicMenu.reset();

	ls::unload();
	Scene::UnLoad();
}

void SettingsAudioScene::Update(const double& dt)
{
		
	if (btnBack->get_components<BtnComponent>()[0]->isSelected())
	{
		Engine::ChangeScene((Scene*)&settings);
	}

	if (btnMusicLow->get_components<BtnComponent>()[0]->isSelected())
	{

		Audio::musicVolume = 0;

		btnMusicLow->get_components<BtnComponent>()[0]->setHighlighted(true);
	}

	if (btnMusicMed->get_components<BtnComponent>()[0]->isSelected())
	{
		Audio::musicVolume = 50;
	}

	if (btnMusicHigh->get_components<BtnComponent>()[0]->isSelected())
	{
		Audio::musicVolume = 100;
	}

	if (btnSFXLow->get_components<BtnComponent>()[0]->isSelected())
	{
		Audio::sfxVolume = 0;
	}

	if (btnSFXMed->get_components<BtnComponent>()[0]->isSelected())
	{
		Audio::sfxVolume = 50;
	}

	if (btnSFXHigh->get_components<BtnComponent>()[0]->isSelected())
	{
		Audio::sfxVolume = 100;
	}

	if (btnDone->get_components<BtnComponent>()[0]->isSelected())
	{
		Audio::SaveAudio;

		Engine::ChangeScene(&settings);
	}

	Scene::Update(dt);
}

void SettingsAudioScene::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}