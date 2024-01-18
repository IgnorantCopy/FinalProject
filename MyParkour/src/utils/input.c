//
// Created by Ignorant on 2023/12/24.
//

#include "utils/input.h"

#include <stdio.h>

static void Quit() {
    app.keyboard[SDL_SCANCODE_ESCAPE] = true;
}

static void DoKeyDown(SDL_KeyboardEvent key) {
    app.keyboard[key.keysym.scancode] = true;
}

static void DoKeyUp(SDL_KeyboardEvent key) {
    app.keyboard[key.keysym.scancode] = false;
}

void DoEvent(SDL_Event event) {
    switch (event.type) {
        case SDL_QUIT:
            Quit();
            return;
        case SDL_KEYDOWN:
            DoKeyDown(event.key);
            break;
        case SDL_KEYUP:
            DoKeyUp(event.key);
            break;
        default:
            break;
    }
}
