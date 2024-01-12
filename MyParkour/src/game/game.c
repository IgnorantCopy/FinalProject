//
// Created by Ignorant on 2024/1/8.
//

#include "game/game.h"
#include "utils/input.h"
#include "utils/display.h"
#include "menu.h"

extern const SDL_Color BLACK;
extern const SDL_Color GRAY;

static Widget *pauseWidgets;
static int selectionPause;
static bool isPause = false;

int DoGameLogic() {
    int score = 0;
    int wayIndex = 0;
    int counter = 0;
    int speed = 5;
    int flag = 1;
    bool isLoading1 = true;
    bool isLoading2 = true;
    bool isLoading3 = true;
    bool isLeft = false;
    bool isRight = false;
    
    Image background;
    int backgroundX = 0;
    int backgroundY = -WINDOW_HEIGHT;
    InitImage(&background, "../image/background_1.png", 1);
    
    Image character[NUM_OF_CHARACTERS];
    int characterIndex = 0;
    InitImage(&character[0], "../image/character/character_1.png", 1);
    InitImage(&character[1], "../image/character/character_2.png", 1);
    int characterX = (WINDOW_WIDTH - character[0].rect.w) / 2;
    int characterY = WINDOW_HEIGHT;
    
    Image prepares[NUM_OF_COUNTERS];
    for (int i = 0; i < NUM_OF_COUNTERS; i++) {
        char barBasePath1[30] = "../image/counter/counter-";
        const char *barBasePath2 = ".png";
        char *barBasePath3 = malloc(3 * sizeof(*barBasePath3));
        sprintf(barBasePath3, "%d", i + 1);
        strcat(barBasePath1, barBasePath3);
        strcat(barBasePath1, barBasePath2);
        InitImage(&prepares[i], barBasePath1, 1);
        free(barBasePath3);
        prepares[i].rect.x = (WINDOW_WIDTH - prepares[i].rect.w) / 2;
        prepares[i].rect.y = (WINDOW_HEIGHT - prepares[i].rect.h) / 2;
    }
    
    Text scoreText;
    int scoreTextY = backgroundY + SCORE_Y;
    char temp[100] = "Score: ";
    itoa(score, temp + 7, 10);
    InitText(&scoreText, "../fonts/arialbi.ttf", 70, temp, &BLACK);
    
    Mix_Music *bgm;
    if (!(bgm = Mix_LoadMUS("../audio/bgm.MP3"))) {
        HANDLE_ERROR("InitAudio bgm0");
    }
    Mix_PlayMusic(bgm, 100);
    
    while (!app.keyboard[SDL_SCANCODE_ESCAPE] && flag == 1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            DoEvent(event);
            if (app.keyboard[SDL_SCANCODE_SPACE] && !isPause) {
                isPause = true;
            }
            if (app.keyboard[SDL_SCANCODE_LEFT] && wayIndex > -1) {
                wayIndex--;
                isLeft = true;
                isRight = false;
            } else if (app.keyboard[SDL_SCANCODE_RIGHT] && wayIndex < 1) {
                wayIndex++;
                isRight = true;
                isLeft = false;
            }
        }
        if (isLoading1) {
            DisplayImage(&background, backgroundX, backgroundY, 1);
            DisplayText(&scoreText, SCORE_X, scoreTextY);
            if (backgroundY < 0) {
                backgroundY += 10;
                scoreTextY += 10;
            } else {
                isLoading1 = false;
            }
        } else if (isLoading2) {
            DisplayImage(&background, 0, 0, 0);
            DisplayText(&scoreText, SCORE_X, SCORE_Y);
            DisplayImage(&character[characterIndex / 10], characterX, characterY, 1);
            if (characterY > WINDOW_HEIGHT - 250) {
                characterY-= 5;
            } else {
                isLoading2 = false;
            }
        } else if (isLoading3) {
            DisplayImage(&background, 0, 0, 0);
            DisplayText(&scoreText, SCORE_X, SCORE_Y);
            DisplayImage(&character[characterIndex / 10], characterX, characterY, 1);
            DisplayImage(&prepares[counter / 10], prepares[counter / 10].rect.x, prepares[counter / 10].rect.y, 1);
            if (counter < 309) {
                counter++;
            } else {
                isLoading3 = false;
            }
        } else if (!isPause){
            UpdateScoreText(&scoreText);
            DisplayImage(&background, 0, 0, 0);
            DisplayText(&scoreText, SCORE_X, SCORE_Y);
            DisplayImage(&character[characterIndex / 10], characterX, characterY, 1);
            DrawGame();
        } else if (isPause) {
            flag = DrawPause();
        }
        characterIndex = (characterIndex + 1) % (NUM_OF_CHARACTERS * 10);
        DoFps();
    }
    
    for (int i = 0; i < NUM_OF_COUNTERS; i++) {
        QuitImage(&prepares[i]);
    }
    for (int i = 0; i < NUM_OF_CHARACTERS; i++) {
        QuitImage(&character[i]);
    }
    QuitText(&scoreText);
    QuitImage(&background);
    return app.keyboard[SDL_SCANCODE_ESCAPE] ? 0 : flag;
}

static void DrawGame() {

}

static int DrawPause() {
    Image pause;
    InitImage(&pause, "../image/pause.png", 1);
    InitPauseWidgets();
    
    Text choices[NUM_OF_WIDGETS];
    for (int i = 0; i < NUM_OF_WIDGETS; i++) {
        InitText(&choices[i], "../fonts/arial.ttf", 50, pauseWidgets[i].text, &BLACK);
    }
    Image arrow;
    InitImage(&arrow, "../image/choose.png", 1);
    arrow.rect.x = PAUSE_X - arrow.rect.w;
    
    SDL_Event event;
    int flag = 0;
    
    UpdatePause(choices, &pause, &arrow);
    while (!app.keyboard[SDL_SCANCODE_ESCAPE] && SDL_WaitEvent(&event) && flag == 0) {
        UpdatePause(choices, &pause, &arrow);
        DoEvent(event);
        flag = DoPauseWidgets();
    }
    
    for (int i = 0; i < NUM_OF_WIDGETS; i++) {
        QuitText(&choices[i]);
    }
    QuitPauseWidgets();
    QuitImage(&pause);
    return app.keyboard[SDL_SCANCODE_ESCAPE] ? 0 : flag;
}

static void UpdateScoreText(Text *scoreText) {

}

static void UpdatePause(Text *choices, Image *pause, Image *arrow) {
    UpdateChoices(choices);
    UpdateArrow(arrow, choices[0]);
    DisplayImage(pause, (WINDOW_WIDTH - pause->rect.w) / 2, (WINDOW_HEIGHT - pause->rect.h) / 2, 1);
    DisplayImage(arrow, arrow->rect.x, arrow->rect.y, 1);
    for (int i = 0; i < NUM_OF_WIDGETS; i++) {
        DisplayText(&choices[i], pauseWidgets[i].x, pauseWidgets[i].y);
    }
    DoFps();
}

static void UpdateChoices(Text *choices) {
    for (int i = 0; i < NUM_OF_WIDGETS; i++) {
        choices[i].color = selectionPause == i ? GRAY : BLACK;
        choices[i].surface = TTF_RenderText_Blended(choices[i].font, pauseWidgets[i].text, choices[i].color);
        choices[i].texture = SDL_CreateTextureFromSurface(app.renderer, choices[i].surface);
    }
}

static void UpdateArrow(Image *arrow, Text choice) {
    arrow->rect.y = PAUSE_Y + selectionPause * PAUSE_GAP + choice.rect.h / 2 - arrow->rect.h / 2;
}

static void InitPauseWidgets() {
    pauseWidgets = malloc(NUM_OF_WIDGETS * sizeof(Widget));
    pauseWidgets[0] = (Widget) {"Continue", PAUSE_X, PAUSE_Y, ActionContinue};
    pauseWidgets[1] = (Widget) {"Restart", PAUSE_X, PAUSE_Y + PAUSE_GAP, ActionRestart};
    pauseWidgets[2] = (Widget) {"Main Menu", PAUSE_X, PAUSE_Y + 2 * PAUSE_GAP, ActionMainMenu};
    selectionPause = 0;
}

static void PrevPauseWidget() {
    selectionPause = (selectionPause - 1 + NUM_OF_WIDGETS) % NUM_OF_WIDGETS;
}

static void NextPauseWidget() {
    selectionPause = (selectionPause + 1) % NUM_OF_WIDGETS;
}

static int ActPauseWidget() {
    Action action = pauseWidgets[selectionPause].action;
    isPause = false;
    app.keyboard[SDL_SCANCODE_RETURN] = false;
    if (action != NULL) {
        return action();
    }
    return -1;
}

static int DoPauseWidgets() {
    if (app.keyboard[SDL_SCANCODE_UP]) {
        PrevPauseWidget();
    } else if (app.keyboard[SDL_SCANCODE_DOWN]) {
        NextPauseWidget();
    } else if (app.keyboard[SDL_SCANCODE_RETURN]) {
        return ActPauseWidget();
    }
    return 0;
}

static void QuitPauseWidgets() {
    free(pauseWidgets);
}

static int ActionContinue() {
    return 1;
}

static int ActionRestart() {
    return 2;
}

static int ActionMainMenu() {
    DisplayMenu();
    app.keyboard[SDL_SCANCODE_RETURN] = false;
    return 3;
}
