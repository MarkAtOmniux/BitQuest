#include "scene_level_1.h"
#include "../GameState.h"
#include "../code/Prefabs.h"
#include "../code/components/cmp_btn.h"
#include "../code/SaveLoad.h"
#include "../code/Audio.h"
#include <levelsystem.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> gavin;
static shared_ptr<Entity> Save;
static shared_ptr<Entity> Quit;
static shared_ptr<Entity> Resume;
static shared_ptr<Entity> CoinText;
Vector2f view_center;


void Level1Scene::Load() {

	Audio::LoadAudio();

	ls::loadLevelFile("res/Tilemaps/Level_1.txt", 240.f);  //the test environment is designed to push the game to it's limit

	_musicLevel1 = Resources::get<Music>("Level_music.ogg");
	_musicLevel1->play();
	_musicLevel1->setLoop(true);
	_musicLevel1->setVolume(Audio::musicVolume);

	TilePhysics();

	makeTorches();

	player = makePlayer(Vector2f(SaveLoad::positionX, SaveLoad::positionY));

	view_center = player->getPosition();

	makeChests();
	
	gavin = makeGavin();

	makeEnemies();

	addUI("");

	makeShops();

	setLoaded(true);
}

void Level1Scene::UnLoad() {
	player.reset();
	_musicLevel1->stop();
	_musicLevel1.reset();

	ls::unload();
	Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {

	if (ls::getTileAt(player->getPosition()) == ls::END) {
		Engine::ChangeScene((Scene*)&menu);
	}


	View view(FloatRect(0, 0, Engine::GetWindow().getSize().x, Engine::GetWindow().getSize().y));
	float view_player_distance = sqrt(((player->getPosition().x - view_center.x) * (player->getPosition().x - view_center.x)) + ((player->getPosition().y - view_center.y) * (player->getPosition().y - view_center.y)));
	if (view_player_distance > 40.f)
		view_center += (player->getPosition() - view_center) *(float)dt * 4.f;
	view.setCenter(view_center);

	Engine::GetWindow().setView(view);

	
	
	if (player->get_components<StateMachineComponent>()[0]->currentState() == "dead")
	{
		Vector2f currentPos = player->getPosition();

		int posX = currentPos.x;
		int posY = currentPos.y;

		Vector2u saveCoords = Vector2u(((posX + 240 / 2) / 240) * 240, ((posY + 240) / 240) * 240);

		totalTime += dt;

		if (totalTime >= holdTime)
		{			
			SaveLoad::positionX = saveCoords.x;
			SaveLoad::positionY = saveCoords.y - 240;

			SaveLoad::SaveGame();

			Engine::ChangeScene(&gameOver);
		}

	}
	if (gavin->get_components<StateMachineComponent>()[0]->currentState() == "dead")
	{
		totalTime += dt;

		if (totalTime >= holdTime)
		{
			Engine::ChangeScene(&menu);
		}
	}

	Scene::Update(dt);

	if (Engine::isKeyDown(Keyboard::Escape))
	{

		Vector2f currentPos = player->getPosition();

		int posX = currentPos.x;
		int posY = currentPos.y;

		Vector2u saveCoords = Vector2u(((posX + 240 / 2) / 240) * 240, ((posY + 240) / 240) * 240);

		SaveLoad::positionX = saveCoords.x;
		SaveLoad::positionY = saveCoords.y - 240;

		SaveLoad::SaveGame();

		Engine::ChangeScene(&menu);
	}
}

void Level1Scene::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}

