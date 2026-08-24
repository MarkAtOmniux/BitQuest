#pragma once

#include "scenes/scene_Level_1.h"
#include "scenes/scene_MainMenu.h"
#include "scenes/scene_Settings.h"
#include "scenes/scene_Settings_Audio.h"
#include "scenes/scene_Settings_Gameplay.h"
#include "scenes/scene_Settings_Graphics.h"
#include "scenes/scene_Tutorial.h"
#include "scenes/scene_Boss_Battle.h"
#include "scenes/Scene_Game_Over.h"
#include "scenes/scene_Shop.h"

extern MainMenuScene menu;
extern SettingsScene settings;
extern SettingsAudioScene settingsAudio;
extern SettingsGraphicsScene settingsGraphics;
extern SettingsGameplayScene settingsGameplay;
extern TutorialScene tutorial;
extern Level1Scene level1;
extern GameOverScene gameOver;
extern ShopScene shop;
extern BossBattleScene bossBattle;
static bool usingKeyboard = true;

static uint16_t game_width = 1600;
static uint16_t game_heigth = 900;
static uint16_t frameRate = 60;