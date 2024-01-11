//
// Created by Ignorant on 2023/12/23.
//

#include "main.h"

#undef main

App app;

int main() {
    InitApp();
    InitWindow();
    InitRenderer();
    InitKeyboard();
    
    atexit(&QuitKeyboard);
    atexit(&QuitRenderer);
    atexit(&QuitWindow);
    atexit(&QuitApp);
    
    StartUp();
    DisplayMenu();
    DoGameLogic();
    
    return 0;
}

static void InitApp() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        HANDLE_ERROR("InitApp SDL");
    }
    if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)) {
        HANDLE_ERROR("InitApp Image");
    }
    if (TTF_Init() == -1) {
        HANDLE_ERROR("InitApp TTF");
    }
    if (!Mix_Init(MIX_INIT_MP3)) {
        HANDLE_ERROR("InitApp Mixer");
    }
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
}

static void QuitApp() {
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

static void InitWindow() {
    app.window = SDL_CreateWindow("Parkour", 400, 50, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
}

static void QuitWindow() {
    SDL_DestroyWindow(app.window);
}

static void InitRenderer() {
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
}

static void QuitRenderer() {
    SDL_DestroyRenderer(app.renderer);
}

static void InitKeyboard() {
    app.keyboard = calloc(SDL_NUM_SCANCODES, sizeof(bool));
}

static void QuitKeyboard() {
    free(app.keyboard);
}

