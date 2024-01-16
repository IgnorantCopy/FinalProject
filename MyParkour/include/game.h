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
#define NUM_OF_OVER_WIDGETS 2
#define NUM_OF_ITEMS 10
#define NUM_OF_MONEY_MAX 20
#define CHARACTER_Y 750
#define PAUSE_X 400
#define PAUSE_Y 360
#define PAUSE_GAP 115
#define GAP_X 250
#define DELTA 30
#define CHARACTER_MAX 164
#define CHARACTER_WIDTH 124
#define CHARACTER_MIN 94
#define NEW_HIGH_SCORE_X 0
#define NEW_HIGH_SCORE_Y 150

typedef struct Item {
    bool isActive;
    int wayIndex;
    int x;
    int y;
} Item;

int DoGameLogic();

static void InitItems(const Image *upImage, const Image *downImage, const Image *changeImage, const Image *moneyImage, const Image *defenceImage, const Image *pileImage);
static int DrawItems(Image *upImage, Image *downImage, Image *changeImage);
static int DrawMoney(Image *moneyImage);
static int DrawProp(Image *defenceImage, Image *pileImage);
static int DrawPause();
static int DrawOver(Image *background, Text *scoreText, Image *character, int characterX, int characterY);

static void UpdateCharacter(Image *character, int w, int h);
static void UpdateScoreText(Text *scoreText);
static void UpdatePause(Text *choices, Image *pause, Image *arrow);
static void UpdateOver(Text *choices, Image *over, Image *arrow, Image *background, Text *scoreText, Image *character, int characterX, int characterY, Image *newHighScore, bool flag);

static void UpdatePauseChoices(Text *choices);
static void UpdateOverChoices(Text *choices);
static void UpdatePauseArrow(Image *arrow, Text choice);
static void UpdateOverArrow(Image *arrow, Text choice);

static int ActPauseWidget();
static int ActOverWidget();

static int DoPauseWidgets();
static int DoOverWidgets();

static int ActionContinue();
static int ActionRestart();
static int ActionMainMenu();

#endif //FINALPROJECT_GAME_H
