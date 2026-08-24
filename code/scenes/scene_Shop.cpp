#include "scene_Shop.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_animation.h"
#include "../components/cmp_btn.h"
#include "../GameState.h"
#include "../code/Prefabs.h"
#include "../code/SaveLoad.h"
#include "engine.h"
#include "levelsystem.h"
#include "../code/Audio.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace sf;

static shared_ptr<Entity> btnDamage;
static shared_ptr<Entity> btnHealth;
static shared_ptr<Entity> btnHam;
static shared_ptr<Entity> btnArrow;
static shared_ptr<Entity> btnBack;

using namespace std;
using namespace sf;


void ShopScene::Load()
{
	ls::loadLevelFile("res/tilemaps/backgrounds.txt", 240.f);

	SaveLoad::LoadGame;

	_bufferBuy = *(Resources::get<SoundBuffer>("Game_Sounds/buySound.ogg"));
	_soundBuy.setBuffer(_bufferBuy);

	//Music
	_musicShop = Resources::get<Music>("Shop_Music.ogg");
	_musicShop->play();
	_musicShop->setLoop(true);
	_musicShop->setVolume(Audio::musicVolume);


	auto coin= makeEntity();
	auto c = coin->addComponent<SpriteComponent>();
	c->Sprite("Spritesheets/Coin_spritesheet.png", IntRect(0, 0, 60, 60));
	c->getSprite().setOrigin(c->getSprite().getLocalBounds().width / 2, c->getSprite().getLocalBounds().height / 2);
	coin->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 + 650 ,30));

	_txtCoin = makeEntity();
	auto t = _txtCoin->addComponent<TextComponent>("0");
	t->getText().setOrigin(t->getText().getGlobalBounds().width / 2, t->getText().getGlobalBounds().height / 2);
	_txtCoin->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 + 720, 30));


	auto arrow = makeEntity();
	auto a = arrow->addComponent<SpriteComponent>();
	a->Sprite("arrowUI.png", IntRect(0, 0, 60, 60));
	a->getSprite().setOrigin(c->getSprite().getLocalBounds().width / 2, c->getSprite().getLocalBounds().height / 2);
	arrow->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 + 650, 100));

	_txtArrow = makeEntity();
	auto ta = _txtArrow->addComponent<TextComponent>("0");
	ta->getText().setOrigin(ta->getText().getGlobalBounds().width / 2, ta->getText().getGlobalBounds().height / 2);
	_txtArrow->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 + 720, 100));


	auto ham = makeEntity();
	auto h = ham->addComponent<SpriteComponent>();
	h->Sprite("hamUI.png", IntRect(0, 0, 60, 60));
	h->getSprite().setOrigin(h->getSprite().getLocalBounds().width / 2, h->getSprite().getLocalBounds().height / 2);
	ham->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 + 650, 170));

	 _txtHam = makeEntity();
	auto th = _txtHam->addComponent<TextComponent>("0");
	th->getText().setOrigin(th->getText().getGlobalBounds().width / 2, th->getText().getGlobalBounds().height / 2);
	_txtHam->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2 + 720, 170));


	{
		auto txtShop = makeEntity();
		auto t = txtShop->addComponent<TextComponent>("Ye Olde Tesco");
		t->getText().setOrigin(t->getText().getGlobalBounds().width / 2, t->getText().getGlobalBounds().height / 2);
		txtShop->setPosition(Vector2f(Engine::GetWindow().getSize().x / 2, 100.f));
		

		btnBack = makeButton("Back", Vector2f(150, 60));
		btnBack->setPosition(Vector2f(Engine::GetWindow().getSize().x / 7, 100.f));
	}


	{
		auto Damage = makeEntity();
		auto d = Damage->addComponent<SpriteComponent>();
		d->Sprite("Shop_Upgrades/DamageUpgrade.png", IntRect(0, 0, 360, 407));
		d->getSprite().setOrigin(d->getSprite().getLocalBounds().width / 2, d->getSprite().getLocalBounds().height / 2);
		Damage->setPosition(Vector2f(Engine::GetWindow().getSize().x / 5, Engine::GetWindow().getSize().y / 2));

		auto txtDamage = makeEntity();
		auto dt = txtDamage->addComponent<TextComponent>("250 Gold");
		dt->getText().setFillColor(Color(240, 178, 0));
		dt->getText().setOrigin(dt->getText().getGlobalBounds().width / 2, dt->getText().getGlobalBounds().height / 2);
		txtDamage->setPosition(Vector2f(Engine::GetWindow().getSize().x / 5, Engine::GetWindow().getSize().y / 2 * 0.5));


		btnDamage = makeButton("Buy Upgrade!", Vector2f(230, 60));
		btnDamage->setPosition(Vector2f(Engine::GetWindow().getSize().x / 5, Engine::GetWindow().getSize().y / 2 * 1.6));
	}

	{
		auto Health = makeEntity();
		auto h = Health->addComponent<SpriteComponent>();
		h->Sprite("Shop_Upgrades/HealthUpgrade.png", IntRect(0, 0, 360, 407));
		h->getSprite().setOrigin(h->getSprite().getLocalBounds().width / 2, h->getSprite().getLocalBounds().height / 2);
		Health->setPosition(Vector2f(Engine::GetWindow().getSize().x / 5 * 2, Engine::GetWindow().getSize().y / 2));

		auto txtHealth = makeEntity();
		auto dt = txtHealth->addComponent<TextComponent>("250 Gold");
		dt->getText().setFillColor(Color(240, 178, 0));
		dt->getText().setOrigin(dt->getText().getGlobalBounds().width / 2, dt->getText().getGlobalBounds().height / 2);
		txtHealth->setPosition(Vector2f(Engine::GetWindow().getSize().x / 5*2, Engine::GetWindow().getSize().y / 2 * 0.5));

		btnHealth = makeButton("Buy Upgrade!", Vector2f(230, 60));
		btnHealth->setPosition(Vector2f(Engine::GetWindow().getSize().x / 5 * 2, Engine::GetWindow().getSize().y / 2 * 1.6));
	}

	{
		auto Ham = makeEntity();
		auto ha = Ham->addComponent<SpriteComponent>();
		ha->Sprite("Shop_Upgrades/HealthFull.png", IntRect(0, 0, 360, 407));
		ha->getSprite().setOrigin(ha->getSprite().getLocalBounds().width / 2, ha->getSprite().getLocalBounds().height / 2);
		Ham->setPosition(Vector2f(Engine::GetWindow().getSize().x / 5 * 3, Engine::GetWindow().getSize().y / 2));

		auto txtHam = makeEntity();
		auto dt = txtHam->addComponent<TextComponent>("25 Gold");
		dt->getText().setFillColor(Color(240, 178, 0));
		dt->getText().setOrigin(dt->getText().getGlobalBounds().width / 2, dt->getText().getGlobalBounds().height / 2);
		txtHam->setPosition(Vector2f(Engine::GetWindow().getSize().x / 5*3, Engine::GetWindow().getSize().y / 2 * 0.5));

		btnHam = makeButton("Buy Ham!", Vector2f(230, 60));
		btnHam->setPosition(Vector2f(Engine::GetWindow().getSize().x / 5 * 3, Engine::GetWindow().getSize().y / 2 * 1.6));
	}

	{
		auto Arrow = makeEntity();
		auto a = Arrow->addComponent<SpriteComponent>();
		a->Sprite("Shop_Upgrades/Arrow.png", IntRect(0, 0, 360, 407));
		a->getSprite().setOrigin(a->getSprite().getLocalBounds().width / 2, a->getSprite().getLocalBounds().height / 2);
		Arrow->setPosition(Vector2f(Engine::GetWindow().getSize().x / 5 * 4, Engine::GetWindow().getSize().y / 2));

		auto txtArrow = makeEntity();
		auto dt = txtArrow->addComponent<TextComponent>("5 Gold");
		dt->getText().setFillColor(Color(240, 178, 0));
		dt->getText().setOrigin(dt->getText().getGlobalBounds().width / 2, dt->getText().getGlobalBounds().height / 2);
		txtArrow->setPosition(Vector2f(Engine::GetWindow().getSize().x / 5*4, Engine::GetWindow().getSize().y / 2 * 0.5));

		btnArrow = makeButton("Buy Arrow!", Vector2f(230, 60));
		btnArrow->setPosition(Vector2f(Engine::GetWindow().getSize().x / 5 * 4, Engine::GetWindow().getSize().y / 2 * 1.6));
	}

	Engine::GetWindow().setView(Engine::GetWindow().getDefaultView());

	setLoaded(true);
}


void ShopScene::UnLoad() {
	_musicShop->stop();
	_musicShop.reset();

	ls::unload();
	Scene::UnLoad();
}

void ShopScene::Update(const double& dt)
{

	_txtCoin->get_components<TextComponent>()[0]->getText().setString(to_string(SaveLoad::coins));
	_txtArrow->get_components<TextComponent>()[0]->getText().setString(to_string(SaveLoad::arrows));
	_txtHam->get_components<TextComponent>()[0]->getText().setString(to_string(SaveLoad::hams));


	if (btnDamage->get_components<BtnComponent>()[0]->isSelected())
	{

		sf::sleep(milliseconds(100));

		if (SaveLoad::coins >= 250)
		{

			_soundBuy.play();
			_soundBuy.setVolume(Audio::sfxVolume);


			SaveLoad::playerDamage = 2;
			SaveLoad::coins = SaveLoad::coins - 250;
		}
	}

	if (btnHealth->get_components<BtnComponent>()[0]->isSelected())
	{

		sf::sleep(milliseconds(100));

		if (SaveLoad::coins >= 250)
		{

			_soundBuy.play();
			_soundBuy.setVolume(Audio::sfxVolume);

			SaveLoad::playerMaxHealth = 10;
			SaveLoad::health = 10;

			SaveLoad::coins = SaveLoad::coins - 250;
		}
	}

	if (btnHam->get_components<BtnComponent>()[0]->isSelected())
	{

		sf::sleep(milliseconds(100));

		if (SaveLoad::coins >= 25)
		{
			_soundBuy.play();
				_soundBuy.setVolume(Audio::sfxVolume);

			SaveLoad::hams++;

			SaveLoad::coins = SaveLoad::coins - 25;
		}
	}

	if (btnArrow->get_components<BtnComponent>()[0]->isSelected())
	{

		sf::sleep(milliseconds(100));

		if (SaveLoad::coins >= 5)
		{

			_soundBuy.play();
				_soundBuy.setVolume(Audio::sfxVolume);

			SaveLoad::arrows++;

			SaveLoad::coins = SaveLoad::coins - 5;
		}
	}

	if (btnBack->get_components<BtnComponent>()[0]->isSelected())
	{
		SaveLoad::SaveGame;

		SaveLoad::LoadGame;
		Engine::ChangeScene(&level1);
	}


	Scene::Update(dt);
}


void ShopScene::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}