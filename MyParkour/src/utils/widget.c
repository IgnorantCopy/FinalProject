//
// Created by Ignorant on 2023/12/24.
//

#include "utils/widget.h"

void InitWidgets(Widget *widgets, int numOfWidgets, int *selection, Action *actions, int x, int y, int gap, char *contents[]) {
    for (int i = 0; i < numOfWidgets; i++) {
        widgets[i] = (Widget) {contents[i], x, y + i * gap, actions[i]};
    }
    *selection = 0;
}

int DoWidgets(const Widget *widgets, int *selection, int numOfWidgets) {
    if (app.keyboard[SDL_SCANCODE_UP]) {
        PrevWidget(selection, numOfWidgets);
    } else if (app.keyboard[SDL_SCANCODE_DOWN]) {
        NextWidget(selection, numOfWidgets);
    } else if (app.keyboard[SDL_SCANCODE_RETURN]) {
        return ActWidget(widgets, selection);
    }
    return -1;
}

void QuitWidgets(Widget *widgets) {
    free(widgets);
}

void PrevWidget(int *selection, int numOfWidgets) {
    *selection = (*selection - 1 + numOfWidgets) % numOfWidgets;
}

void NextWidget(int *selection, int numOfWidgets) {
    *selection = (*selection + 1) % numOfWidgets;
}

int ActWidget(const Widget *widgets, const int *selection) {
    app.keyboard[SDL_SCANCODE_RETURN] = false;
    Action action = widgets[*selection].action;
    if (action != NULL) {
        return action();
    }
    return -1;
}
