//
// Created by Ignorant on 2023/12/24.
//

#ifndef FINALPROJECT_WIDGET_H
#define FINALPROJECT_WIDGET_H

#include "common.h"

typedef int (*Action)();

typedef struct Widget {
    char *text;
    int x;
    int y;
    Action action;
} Widget;

void InitWidgets(Widget *widgets, int numOfWidgets, int *selection, Action *actions, int x, int y, int gap, char *contents[]);
int DoWidgets(const Widget *widgets, int *selection, int numOfWidgets);
void QuitWidgets(Widget *widgets);
void PrevWidget(int *selection, int numOfWidgets);
void NextWidget(int *selection, int numOfWidgets);
int ActWidget(const Widget *widgets, const int *selection);

#endif //FINALPROJECT_WIDGET_H
