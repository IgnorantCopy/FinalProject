//
// Created by Ignorant on 2023/12/24.
//

#ifndef FINALPROJECT_DISPLAY_H
#define FINALPROJECT_DISPLAY_H

#include "common.h"

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 950

#define FPS 90
#define FRAME_LEN (1000 / FPS)

void DoFps();

void InitImage(Image *image, const char *path, int flag);
void DisplayImage(Image *image, int x, int y, int flag);
void QuitImage(Image *image);

void InitText(Text *text, const char *font, int size, char *content, const SDL_Color *color);
void DisplayText(Text *text, int x, int y);
void QuitText(Text *text);

#endif //FINALPROJECT_DISPLAY_H
