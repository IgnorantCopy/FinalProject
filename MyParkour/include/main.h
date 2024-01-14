//
// Created by Ignorant on 2023/12/24.
//

#ifndef FINALPROJECT_MAIN_H
#define FINALPROJECT_MAIN_H

#include <stdio.h>
#include <stdbool.h>
#include "common.h"
#include "menu.h"
#include "game.h"
#include "utils/input.h"
#include "utils/widget.h"
#include "utils/display.h"
#include "cJSON.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

static void InitApp();
static void QuitApp();

static void InitRenderer();
static void QuitRenderer();

static void InitWindow();
static void QuitWindow();

static void InitKeyboard();
static void QuitKeyboard();

#endif //FINALPROJECT_MAIN_H
