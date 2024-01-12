//
// Created by Ignorant on 2024/1/8.
//

#ifndef FINALPROJECT_GAME_H
#define FINALPROJECT_GAME_H

#include "common.h"

#define SCORE_X 20
#define SCORE_Y 15
#define NUM_OF_CHARACTERS 2
#define NUM_OF_COUNTERS 30
#define PAUSE_X 400
#define PAUSE_Y 360
#define PAUSE_GAP 115
#define GAP_X 200

int DoGameLogic();

static void DrawGame();
static int DrawPause();

static void UpdateScoreText(Text *scoreText);
static void UpdatePause(Text *choices, Image *pause, Image *arrow);

static void UpdateChoices(Text *choices);
static void UpdateArrow(Image *arrow, Text choice);

static void PrevPauseWidget();
static void NextPauseWidget();
static int ActPauseWidget();

static void InitPauseWidgets();
static int DoPauseWidgets();
static void QuitPauseWidgets();

static int ActionContinue();
static int ActionRestart();
static int ActionMainMenu();

#endif //FINALPROJECT_GAME_H
