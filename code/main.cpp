#include "engine.h"
#include "gameState.h"
#include "scenes/scene_Level_1.h"
#include "scenes/scene_MainMenu.h"
#include "scenes/scene_Settings.h"
#include "scenes/scene_Settings_Audio.h"
#include "scenes/scene_Settings_Gameplay.h"
#include "scenes/scene_Settings_Graphics.h"
#include "scenes/scene_Tutorial.h"
#include "scenes/Scene_Game_Over.h"
#include "scenes/scene_Shop.h"
#include "scenes/scene_Boss_Battle.h"
#include "Audio.h"
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

using namespace std;

//List of all scenes
MainMenuScene menu;
SettingsScene settings;
SettingsAudioScene settingsAudio;
SettingsGraphicsScene settingsGraphics;
SettingsGameplayScene settingsGameplay;
TutorialScene tutorial;
Level1Scene level1;
GameOverScene gameOver;
ShopScene shop;
BossBattleScene bossBattle;

int ResolutionX;
int ResolutionY;
int FPS;

int main() {
	Engine::UseExecutableWorkingDirectory();

	ifstream Graphics("res/savestates/Graphics.txt");
	(Graphics >> ResolutionX >> ResolutionY >> FPS);

	Audio::ResetAudio();

	Engine::Start(game_width, game_heigth, "Bitquest!", &menu, frameRate);
}
