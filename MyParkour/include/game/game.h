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

void DoGameLogic();

static void DrawGame();

static void UpdateScoreText(Text *scoreText);

#endif //FINALPROJECT_GAME_H
