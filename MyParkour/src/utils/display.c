//
// Created by Ignorant on 2023/12/24.
//

#include "utils/display.h"

#include <stdio.h>

void DoFps() {
    fflush(stdout);
    SDL_RenderPresent(app.renderer);
    SDL_Delay(FRAME_LEN);
}

void InitImage(Image *image, const char *path, int flag) {
    if (!(image->surface = IMG_Load(path))) {
        HANDLE_ERROR("InitImage Surface");
    }
    image->texture = SDL_CreateTextureFromSurface(app.renderer, image->surface);
    if (flag) {
        SDL_QueryTexture(image->texture, NULL, NULL, &image->rect.w, &image->rect.h);
    }
}

void DisplayImage(Image *image, int x, int y, int flag) {
    if (flag) {
        image->rect.x = x;
        image->rect.y = y;
    }
    SDL_RenderCopy(app.renderer, image->texture, NULL, flag ? &image->rect : NULL);
}

void QuitImage(Image *image) {
    if (image->texture != NULL) {
        SDL_DestroyTexture(image->texture);
    }
    if (image->surface != NULL) {
        SDL_FreeSurface(image->surface);
    }
}

void InitText(Text *text, const char *font, int size, char *content, const SDL_Color *color) {
    if (!(text->font = TTF_OpenFont(font, size))) {
        HANDLE_ERROR("InitFont");
    }
    text->content = content;
    text->color = *color;
    text->surface = TTF_RenderText_Blended(text->font, text->content, *color);
    text->texture = SDL_CreateTextureFromSurface(app.renderer, text->surface);
    SDL_QueryTexture(text->texture, NULL, NULL, &text->rect.w, &text->rect.h);
}

void DisplayText(Text *text, int x, int y) {
    text->rect.x = x;
    text->rect.y = y;
    SDL_RenderCopy(app.renderer, text->texture, NULL, &text->rect);
}

void QuitText(Text *text) {
    if (text->texture != NULL) {
        SDL_DestroyTexture(text->texture);
    }
    if (text->surface != NULL) {
        SDL_FreeSurface(text->surface);
    }
}

