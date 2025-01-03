#ifndef DEFINITION_H
#define DEFINITION_H

#include "./PhysicsEngine/Managers/ComponentEntityManager.h"
#include "./PhysicsEngine/Components/BoxCollider.h"
#include "./PhysicsEngine/Components/RigidBody.h"
#include "./PhysicsEngine/Components/SpriteRenderer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <string>
#include <iomanip>
#include <memory>
#include <unordered_map>
#include <assert.h>
#include "./Menu/marioGameManager.h"

#define BLOCK_WIDTH 64
#define BLOCK_HEIGHT 64
#define BACKGROUND "resource/Sprite/background.png"
#define MARIO "resource/Sprite/Mario1.png"
#define SUPERMARIO "resource/Sprite/MarioSuper2.png"
#define LUIGI "resource/Sprite/Luigi1.png"
#define SUPERLUIGI "resource/Sprite/LuigiSuper2.png"
#define ENEMY "resource/Sprite/Enemies.png"
#define ITEM "resource/Sprite/Items.png"
#define EXIT_BUTTON "resource/Menu/ExitButton.png"
#define MAP_LOG "resource/Log/game_log.txt"
#define MARIO_LOG "resource/Log/game_state.txt"
#define TILES "resource/Sprite/TilesBackup.png"
#define HEART_ICON "resource/Sprite/Heart.png"
#define SOUNDBAR_IMG_PATH "resource/Menu/Settings/sound_bar/sb_0"
#define SOUND_CONTROLS_BG "resource/Menu/Settings/CHARACTER_SELECTION.png"
#define INC_VOLUME "resource/Menu/Settings/IncreaseVolume.png"
#define DEC_VOLUME "resource/Menu/Settings/DecreaseVolume.png"
#define SAVE_BUTTON "resource/Menu/Settings/SaveBut.png"

#define SCOREBOARD_BG "resource/Menu/ScoreboardBG.png"
#define HIGHSCORE_FILE "resource/Log/highscore.txt"
#define ARROW "resource/Menu/Settings/ARROW.png"

#define TOTAL_NUM_LEVEL 3
#define TOTAL_HIGHSCORES 5
#define VOLUME_UNIT 10
#endif