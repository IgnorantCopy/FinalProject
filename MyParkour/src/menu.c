//
// Created by Ignorant on 2023/12/24.
//

#include "menu.h"
#include "utils/input.h"
#include "utils/display.h"
#include "utils/widget.h"

static Widget *widgets;
static int selection;

const SDL_Color BLACK = {0, 0, 0, SDL_ALPHA_OPAQUE};
const SDL_Color GRAY = {192, 192, 192, SDL_ALPHA_OPAQUE};

void StartUp() {
    Image background;
    Text title;
    InitImage(&background, "../image/background_0.png", 0);
    InitText(&title, "../fonts/CURLZ___.TTF", 200, "Parkour", &BLACK);
    
    Image bars[33];
    for (int i = 0; i < NUM_OF_BARS; i++) {
        char barBasePath1[30] = "../image/bar/bar-";
        const char *barBasePath2 = ".png";
        char *barBasePath3 = malloc(3 * sizeof(*barBasePath3));
        sprintf(barBasePath3, "%d", i + 1);
        strcat(barBasePath1, barBasePath3);
        strcat(barBasePath1, barBasePath2);
        InitImage(&bars[i], barBasePath1, 1);
        free(barBasePath3);
    }
    
    while (!app.keyboard[SDL_SCANCODE_ESCAPE]) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            DoEvent(event);
        }
        if (DrawStart(&background, &title, bars)) {
            DisplayImage(&background, 0, 0, 0);
            DoFps();
            break;
        }
        DoFps();
    }
    
    for (int i = 0; i < NUM_OF_BARS; i++) {
        QuitImage(&bars[i]);
    }
    QuitText(&title);
    QuitImage(&background);
}

static int DrawStart(Image *background, Text *title, Image *bars) {
    if (app.keyboard[SDL_SCANCODE_ESCAPE]) {
        return 0;
    }
    static bool isLoading1 = true;
    static bool isLoading2 = true;
    static double alphaBackgroundRate = 0;
    static int isLoadingCounter = 0;
    int titleX = (WINDOW_WIDTH - title->rect.w) / 2;
    int titleY = (WINDOW_HEIGHT - title->rect.h) / 2;
    if (isLoading1) {
        SDL_SetTextureBlendMode(background->texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(background->texture, (Uint8)(SDL_ALPHA_OPAQUE * alphaBackgroundRate));
        DisplayImage(background, 0, 0, 0);
        DisplayText(title, titleX, titleY);
        if (alphaBackgroundRate < 0.03) {
            alphaBackgroundRate += 0.0001;
        } else {
            isLoading1 = false;
            alphaBackgroundRate = 0;
            SDL_SetTextureBlendMode(background->texture, SDL_BLENDMODE_NONE);
        }
    } else if (isLoading2) {
        SDL_SetTextureBlendMode(bars[isLoadingCounter / 10].texture, SDL_BLENDMODE_BLEND);
        if (alphaBackgroundRate < 1) {
            alphaBackgroundRate += 0.02;
        }
        SDL_SetTextureAlphaMod(bars[isLoadingCounter / 10].texture, (Uint8)(SDL_ALPHA_OPAQUE * alphaBackgroundRate));
        DisplayImage(background, 0, 0, 0);
        DisplayText(title, titleX, titleY);
        DisplayImage(&bars[isLoadingCounter / 10], (WINDOW_WIDTH - bars[0].rect.w) / 2, WINDOW_HEIGHT - 250, 1);
        isLoadingCounter++;
        if (isLoadingCounter >= 330) {
            isLoading2 = false;
            alphaBackgroundRate = 1;
            SDL_SetTextureBlendMode(bars[NUM_OF_BARS - 1].texture, SDL_BLENDMODE_NONE);
        }
    }
    if (!isLoading1 && !isLoading2) {
        return 1;
    } else {
        return 0;
    }
}

void DisplayMenu() {
    Image background;
    Image arrow;
    Text title;
    InitImage(&background, "../image/background_0.png", 0);
    InitText(&title, "../fonts/CURLZ___.TTF", 200, "Parkour", &BLACK);
    
    Action actions[] = {ActionStart, ActionData, ActionQuit};
    char *contents[] = {
            "Start",
            "Data",
            "Quit"
    };
    widgets = malloc(NUM_OF_WIDGETS * sizeof(Widget));
    InitWidgets(widgets, NUM_OF_WIDGETS, &selection, actions, WIDGET_X, WIDGET_Y, GAP_Y, contents);
    
    Mix_Music *bgm;
    if (!(bgm = Mix_LoadMUS("../audio/bgm0.MP3"))) {
        HANDLE_ERROR("InitAudio bgm0");
    }
    Mix_PlayMusic(bgm, 5);
    
    Text choices[NUM_OF_WIDGETS];
    for (int i = 0; i < NUM_OF_WIDGETS; i++) {
        InitText(&choices[i], "../fonts/CURLZ___.TTF", 100, widgets[i].text, &BLACK);
    }
    InitImage(&arrow, "../image/choose.png", 1);
    arrow.rect.x = WIDGET_X - arrow.rect.w - 20;
    
    SDL_Event event;
    DrawMenu(&background, &arrow, &title, choices);
    DoFps();
    while (!app.keyboard[SDL_SCANCODE_ESCAPE] && SDL_WaitEvent(&event)) {
        DoEvent(event);
        int flag = DoWidgets(widgets, &selection, NUM_OF_WIDGETS);
        if (flag == 0 || flag == 2) {
            break;
        } else if (flag == 1) {
            ShowData();
        }
        DrawMenu(&background, &arrow, &title, choices);
        DoFps();
    }
    Mix_FreeMusic(bgm);
    for (int i = 0; i < NUM_OF_WIDGETS; i++) {
        QuitText(&choices[i]);
    }
    QuitWidgets(widgets);
    QuitText(&title);
    QuitImage(&arrow);
    QuitImage(&background);
}

static void DrawMenu(Image *background, Image *arrow, Text *title, Text *choices) {
    UpdateArrow(arrow, choices[0]);
    UpdateChoices(choices);
    DisplayImage(background, 0, 0, 0);
    DisplayText(title, (WINDOW_WIDTH - title->rect.w) / 2, (WINDOW_HEIGHT - title->rect.h) / 4);
    DisplayImage(arrow, arrow->rect.x, arrow->rect.y, 1);
    for (int i = 0; i < NUM_OF_WIDGETS; i++) {
        DisplayText(&choices[i], widgets[i].x, widgets[i].y);
    }
}

static void UpdateChoices(Text *choices) {
    for (int i = 0; i < NUM_OF_WIDGETS; i++) {
        choices[i].color = selection == i ? GRAY : BLACK;
        choices[i].surface = TTF_RenderText_Blended(choices[i].font, widgets[i].text, choices[i].color);
        choices[i].texture = SDL_CreateTextureFromSurface(app.renderer, choices[i].surface);
    }
}

static void UpdateArrow(Image *arrow, Text choice) {
    arrow->rect.y = WIDGET_Y + selection * GAP_Y + choice.rect.h / 2 - arrow->rect.h / 2;
}

static int ActionStart() {
    return 0;
}

static int ActionData() {
    return 1;
}

static int ActionQuit() {
    app.keyboard[SDL_SCANCODE_ESCAPE] = true;
    return 2;
}

// TODO
void ShowData() {

}