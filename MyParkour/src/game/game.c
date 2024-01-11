//
// Created by Ignorant on 2024/1/8.
//

#include "game/game.h"
#include "utils/input.h"
#include "utils/display.h"

extern const SDL_Color BLACK;
extern const SDL_Color GRAY;

void DoGameLogic() {
    int score = 0;
    int wayIndex = 1;
    int counter = 0;
    int speed = 5;
    bool isLoading1 = true;
    bool isLoading2 = true;
    bool isLoading3 = true;
    
    Image background;
    int backgroundX = 0;
    int backgroundY = -WINDOW_HEIGHT;
    InitImage(&background, "../image/background_1.png", 1);
    
    Image character[NUM_OF_CHARACTERS];
    int characterIndex = 0;
    InitImage(&character[0], "../image/character/character_1.png", 1);
    InitImage(&character[1], "../image/character/character_2.png", 1);
    for (int i = 0; i < NUM_OF_CHARACTERS; i++) {
        character[i].rect.x = (WINDOW_WIDTH - character[i].rect.w) / 2;
        character[i].rect.y = WINDOW_HEIGHT;
    }
    
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
    
    while (!app.keyboard[SDL_SCANCODE_ESCAPE]) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            DoEvent(event, 1);
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
            DisplayImage(&character[characterIndex / 10], character[characterIndex / 10].rect.x, character[characterIndex / 10].rect.y, 1);
            if (character[characterIndex / 10].rect.y > WINDOW_HEIGHT - 250) {
                character[0].rect.y -= 5;
                character[1].rect.y -= 5;
            } else {
                isLoading2 = false;
            }
        } else if (isLoading3) {
            DisplayImage(&background, 0, 0, 0);
            DisplayText(&scoreText, SCORE_X, SCORE_Y);
            DisplayImage(&character[characterIndex / 10], character[characterIndex / 10].rect.x, character[characterIndex / 10].rect.y, 1);
            DisplayImage(&prepares[counter / 10], prepares[counter / 10].rect.x, prepares[counter / 10].rect.y, 1);
            if (counter < 309) {
                counter++;
            } else {
                isLoading3 = false;
            }
        } else {
            DisplayImage(&background, 0, 0, 0);
            DisplayText(&scoreText, SCORE_X, SCORE_Y);
            DisplayImage(&character[characterIndex / 10], character[characterIndex / 10].rect.x, character[characterIndex / 10].rect.y, 1);
            
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
}

static void DrawGame() {

}

static void UpdateScoreText(Text *scoreText) {

}
