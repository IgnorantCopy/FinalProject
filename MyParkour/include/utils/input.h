//
// Created by Ignorant on 2023/12/24.
//

#ifndef FINALPROJECT_INPUT_H
#define FINALPROJECT_INPUT_H

#include "common.h"

static void Quit();
static void DoMouseButtonDown(SDL_MouseButtonEvent button);
static void DoMouseButtonUp(SDL_MouseButtonEvent button);
static void DoKeyDown(SDL_KeyboardEvent key);
static void DoKeyUp(SDL_KeyboardEvent key);

void DoEvent(SDL_Event event, int flag);

#endif //FINALPROJECT_INPUT_H
