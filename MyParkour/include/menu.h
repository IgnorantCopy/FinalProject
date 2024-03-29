//
// Created by Ignorant on 2023/12/24.
//

#ifndef FINALPROJECT_MENU_H
#define FINALPROJECT_MENU_H

#include "common.h"

#define NUM_OF_BARS 33
#define NUM_OF_WIDGETS 3
#define WIDGET_X 375
#define WIDGET_Y 500
#define GAP_Y 100
#define DATA_X 100
#define DATA_Y 100

void StartUp();
static int DrawStart(Image *background, Text *title, Image *bars);

void DisplayMenu();
static void DrawMenu(Image *background, Image *arrow, Text *title, Text *choices);

static void UpdateChoices(Text *choices);
static void UpdateArrow(Image *arrow, Text choice);

static int ActionStart();
static int ActionData();
static int ActionQuit();

void ShowData();

#endif //FINALPROJECT_MENU_H
