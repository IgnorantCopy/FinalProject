//
// Created by Ignorant on 2023/12/24.
//

#ifndef FINALPROJECT_COMMON_H
#define FINALPROJECT_COMMON_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include "utils/cJSON.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#define HANDLE_ERROR(msg) printf(msg "SDL: %s\n", SDL_GetError()); \
    exit(EXIT_FAILURE)

typedef struct App {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool *keyboard;
} App;

extern App app;

typedef struct Image {
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;
} Image;

typedef struct Text {
    char *content;
    TTF_Font *font;
    SDL_Color color;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;
} Text;


#endif //FINALPROJECT_COMMON_H
