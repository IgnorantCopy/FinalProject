//
// Created by Ignorant on 2024/1/8.
//

#include "game.h"
#include "utils/input.h"
#include "utils/display.h"
#include "utils/widget.h"
#include "menu.h"

extern const SDL_Color BLACK;
extern const SDL_Color GRAY;
const SDL_Color RED = {255, 0, 0, SDL_ALPHA_OPAQUE};

static Widget *pauseWidgets;
static int selectionPause;
static Widget *overWidgets;
static int selectionOver;

static Item ups[NUM_OF_ITEMS];
static Item downs[NUM_OF_ITEMS];
static Item changes[NUM_OF_ITEMS];
static Item money[NUM_OF_MONEY_MAX];
static Item defence;
static Item pile;

static bool isPause;
static bool isOver;
static bool isUp;
static bool isDown;
static bool isExist;
static bool isDefensive;
static int upsIndex;
static int downsIndex;
static int changesIndex;
static int moneyIndex;
static int wayIndex;
static int score;
static int speed;
static int bonus;

int DoGameLogic() {
    wayIndex = 0;
    score = 0;
    speed = 5;
    bonus = 0;
    int scoreCount = 0;
    int counter = 0;
    int flag = 1;
    bool isLoading1 = true;
    bool isLoading2 = true;
    bool isLoading3 = true;
    bool isLeft = false;
    bool isRight = false;
    bool isMax = false;
    bool isMin = false;
    isPause = false;
    isOver = false;
    isUp = false;
    isDown = false;
    isExist = false;
    isDefensive = false;
    upsIndex = 0;
    downsIndex = 0;
    changesIndex = 0;
    moneyIndex = 0;
    
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
    int characterW = character[0].rect.w;
    int characterH = character[0].rect.h;
    int target = characterX;
    
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
    
    Image upImage;
    Image downImage;
    Image changeImage;
    Image moneyImage;
    InitImage(&upImage, "../image/up.png", 1);
    InitImage(&downImage, "../image/down.png", 1);
    InitImage(&changeImage, "../image/change.png", 1);
    InitImage(&moneyImage, "../image/money.png", 1);
    
    Image defenceImage;
    Image pileImage;
    Image defensive;
    InitImage(&defenceImage, "../image/defence.png", 1);
    InitImage(&pileImage, "../image/pile.png", 1);
    InitImage(&defensive, "../image/defensive.png", 1);
    
    Text scoreText;
    int scoreTextY = backgroundY + SCORE_Y;
    InitText(&scoreText, "../fonts/arialbi.ttf", 70, "Score: 0", &BLACK);
    
    Mix_Music *bgm;
    if (!(bgm = Mix_LoadMUS("../audio/bgm.MP3"))) {
        HANDLE_ERROR("InitAudio bgm0");
    }
    Mix_PlayMusic(bgm, 100);
    
    Mix_Chunk *getScore;
    if (!(getScore = Mix_LoadWAV("../audio/getScore.MP3"))) {
        HANDLE_ERROR("InitSound getScore");
    }
    Mix_Chunk *peng;
    if (!(peng = Mix_LoadWAV("../audio/peng.MP3"))) {
        HANDLE_ERROR("InitSound peng");
    }
    
    srand(time(NULL));
    InitItems(&upImage, &downImage, &changeImage, &moneyImage, &defenceImage, &pileImage);
    while (!app.keyboard[SDL_SCANCODE_ESCAPE] && flag == 1) {
        SDL_Event event;
        bonus = bonus > 10 ? 10 : score / 1000;
        UpdateScoreText(&scoreText);
        while (SDL_PollEvent(&event)) {
            DoEvent(event);
            if (app.keyboard[SDL_SCANCODE_SPACE] && !isPause) {
                isPause = true;
            }
            if (app.keyboard[SDL_SCANCODE_LEFT]) {
                target = (WINDOW_WIDTH - character[0].rect.w) / 2 + GAP_X * (wayIndex > -1 ? (wayIndex - 1) : wayIndex);
                isLeft = true;
                isRight = false;
            } else if (app.keyboard[SDL_SCANCODE_RIGHT]) {
                target = (WINDOW_WIDTH - character[0].rect.w) / 2 + GAP_X * (wayIndex < 1 ? (wayIndex + 1) : wayIndex);
                isRight = true;
                isLeft = false;
            }
            if (app.keyboard[SDL_SCANCODE_UP]) {
                isUp = true;
                isDown = false;
            } else if (app.keyboard[SDL_SCANCODE_DOWN]) {
                isDown = true;
                isUp = false;
            }
        }
        if (characterX >= (WINDOW_WIDTH - character[0].rect.w) / 2 - GAP_X && characterX < (WINDOW_WIDTH - character[0].rect.w) / 2 - GAP_X / 2) {
            wayIndex = -1;
        } else if (characterX >= (WINDOW_WIDTH - character[0].rect.w) / 2 - GAP_X / 2 && characterX < (WINDOW_WIDTH - character[0].rect.w) / 2 + GAP_X / 2) {
            wayIndex = 0;
        } else if (characterX >= (WINDOW_WIDTH - character[0].rect.w) / 2 + GAP_X / 2 && characterX <= (WINDOW_WIDTH - character[0].rect.w) / 2 + GAP_X) {
            wayIndex = 1;
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
            if (characterY > CHARACTER_Y) {
                characterY -= 5;
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
        } else if (isOver) {
            flag = DrawOver(&background, &scoreText, &character[characterIndex / 10], characterX, characterY);
        } else if (isPause){
            flag = DrawPause();
        } else {
            DisplayImage(&background, 0, 0, 0);
            DisplayText(&scoreText, SCORE_X, SCORE_Y);
            
            if (characterX > target) {
                characterX -= speed + bonus;
                if (isRight) {
                    isRight = false;
                }
            } else if (characterX < target) {
                characterX += speed + bonus;
                if (isLeft) {
                    isLeft = false;
                }
            }
            if (isUp) {
                if (!isMax) {
                    if (characterW < CHARACTER_MAX) {
                        characterW++;
                        characterH++;
                    } else if (characterW == CHARACTER_MAX) {
                        isMax = true;
                    }
                } else {
                    if (characterW > CHARACTER_WIDTH) {
                        characterW--;
                        characterH--;
                    } else if (characterW == CHARACTER_WIDTH) {
                        isUp = false;
                        isMax = false;
                    }
                }
            } else if (isDown) {
                if (!isMin) {
                    if (characterW > CHARACTER_MIN) {
                        characterW--;
                        characterH--;
                    } else if (characterW == CHARACTER_MIN) {
                        isMin = true;
                    }
                } else {
                    if (characterW < CHARACTER_WIDTH) {
                        characterW++;
                        characterH++;
                    } else if (characterW == CHARACTER_WIDTH) {
                        isDown = false;
                        isMin = false;
                    }
                }
            }
            UpdateCharacter(character, characterW, characterH);
            if (isDefensive) {
                DisplayImage(&defensive, characterX + (characterW - defensive.rect.w) / 2, characterY, 1);
            }
            DisplayImage(&character[characterIndex / 10], characterX, characterY, 1);
            
            if (DrawMoney(&moneyImage)) {
                Mix_PlayChannel(-1, getScore, 0);
            }
            if (score >= 2000 && DrawProp(&defenceImage, &pileImage)) {
                Mix_PlayChannel(-1, getScore, 2);
            }
            if (DrawItems(&upImage, &downImage, &changeImage)) {
                Mix_PlayChannel(-1, peng, 0);
            }
            
            if (scoreCount % 5 == 0) {
                score++;
            }
            scoreCount = (scoreCount + 1) % 100;
        }
        
        characterIndex = (characterIndex + 1) % (NUM_OF_CHARACTERS * 10);
        DoFps();
    }
    
    Mix_FreeChunk(peng);
    Mix_FreeChunk(getScore);
    Mix_FreeMusic(bgm);
    QuitImage(&defensive);
    QuitImage(&pileImage);
    QuitImage(&defenceImage);
    QuitImage(&upImage);
    QuitImage(&downImage);
    QuitImage(&changeImage);
    QuitImage(&moneyImage);
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

static void InitItems(const Image *upImage, const Image *downImage, const Image *changeImage, const Image *moneyImage, const Image *defenceImage, const Image *pileImage) {
    for (int i = 0; i < NUM_OF_ITEMS; i++) {
        ups[i].isActive = false;
        ups[i].y = -upImage->rect.h;
        downs[i].isActive = false;
        downs[i].y = -downImage->rect.h;
        changes[i].isActive = false;
        changes[i].y = -changeImage->rect.h;
        money[i].isActive = false;
        money[i].y = -moneyImage->rect.h;
    }
    defence.isActive = false;
    defence.y = -defenceImage->rect.h;
    pile.isActive = false;
    pile.y = -pileImage->rect.h;
}

static int DrawItems(Image *upImage, Image *downImage, Image *changeImage) {
    int moveRandom = rand() % 3;
    int wayRandom = rand() % 3;
    int itemRandom = rand() % 50;
    int flag = 0;
    int isDanger = 0;
    if (itemRandom == 20) {
        switch (moveRandom) {
            case 0:
                ups[upsIndex].wayIndex = wayRandom - 1;
                for (int i = 0; i < NUM_OF_ITEMS; i++) {
                    if (downs[i].isActive && downs[i].wayIndex == ups[upsIndex].wayIndex) {
                        isDanger = 1;
                        break;
                    }
                }
                if (!isDanger) {
                    ups[upsIndex].isActive = true;
                    ups[upsIndex].x = (WINDOW_WIDTH - upImage->rect.w) / 2 + ups[upsIndex].wayIndex * GAP_X;
                    upImage->rect.y = -upImage->rect.h;
                    upsIndex = (upsIndex + 1) % NUM_OF_ITEMS;
                }
                break;
            case 1:
                downs[downsIndex].wayIndex = wayRandom - 1;
                for (int i = 0; i < NUM_OF_ITEMS; i++) {
                    if (ups[i].isActive && ups[i].wayIndex == downs[downsIndex].wayIndex) {
                        isDanger = 1;
                    }
                }
                if (!isDanger) {
                    downs[downsIndex].isActive = true;
                    downs[downsIndex].x = (WINDOW_WIDTH - downImage->rect.w) / 2 + downs[downsIndex].wayIndex * GAP_X;
                    downImage->rect.y = -downImage->rect.h;
                    downsIndex = (downsIndex + 1) % NUM_OF_ITEMS;
                }
                break;
            case 2:
                changes[changesIndex].wayIndex = wayRandom - 1;
                int temp = changes[changesIndex].wayIndex;
                int left = 0;
                int middle = 0;
                int right = 0;
                for (int i = 0; i < NUM_OF_ITEMS; i++) {
                    switch (changes[i].wayIndex) {
                        case -1:
                            left++;
                            break;
                        case 0:
                            middle++;
                            break;
                        case 1:
                            right++;
                            break;
                        default:
                            break;
                    }
                }
                if ((temp == -1 && middle && right) || (temp == 0 && left && right) || (temp == 1 && left && middle)) {
                    break;
                }
                changes[changesIndex].isActive = true;
                changes[changesIndex].x = (WINDOW_WIDTH - changeImage->rect.w) / 2 + changes[changesIndex].wayIndex * GAP_X;
                changeImage->rect.y = -changeImage->rect.h;
                changesIndex = (changesIndex + 1) % NUM_OF_ITEMS;
                break;
            default:
                break;
        }
    }
    
    for (int i = 0; i < NUM_OF_ITEMS; i++) {
        if (ups[i].isActive) {
            DisplayImage(upImage, ups[i].x, ups[i].y, 1);
            ups[i].y += speed + bonus;
            if (ups[i].wayIndex == wayIndex && ups[i].y + upImage->rect.h >= CHARACTER_Y && ups[i].y <= CHARACTER_Y + DELTA + bonus && !isUp) {
                if (!isDefensive) {
                    isOver = true;
                } else {
                    isDefensive = false;
                    ups[i].isActive = false;
                    ups[i].y = -upImage->rect.h;
                }
                flag = 1;
            }
            if (ups[i].y >= WINDOW_HEIGHT + upImage->rect.h) {
                ups[i].isActive = false;
                ups[i].y = -upImage->rect.h;
            }
        }
        if (downs[i].isActive) {
            DisplayImage(downImage, downs[i].x, downs[i].y, 1);
            downs[i].y += speed + bonus;
            if (downs[i].wayIndex == wayIndex && downs[i].y + downImage->rect.h >= CHARACTER_Y && downs[i].y <= CHARACTER_Y + DELTA + bonus && !isDown) {
                if (!isDefensive) {
                    isOver = true;
                } else {
                    isDefensive = false;
                    downs[i].isActive = false;
                    downs[i].y = -downImage->rect.h;
                }
                flag = 1;
            }
            if (downs[i].y >= WINDOW_HEIGHT + downImage->rect.h) {
                downs[i].isActive = false;
                downs[i].y = -downImage->rect.h;
            }
        }
        if (changes[i].isActive) {
            DisplayImage(changeImage, changes[i].x, changes[i].y, 1);
            changes[i].y += speed + bonus;
            if (changes[i].wayIndex == wayIndex && changes[i].y + changeImage->rect.h >= CHARACTER_Y && changes[i].y <= CHARACTER_Y + DELTA + bonus) {
                if (!isDefensive) {
                    isOver = true;
                } else {
                    changes[i].isActive = false;
                    changes[i].y = -changeImage->rect.h;
                }
                flag = 1;
            }
            if (changes[i].y >= WINDOW_HEIGHT + changeImage->rect.h) {
                changes[i].isActive = false;
                changes[i].y = -changeImage->rect.h;
            }
        }
    }
    return flag;
}

static int DrawMoney(Image *moneyImage) {
    int wayRandom = rand() % 3;
    int moneyRandom = rand() % 20;
    int flag = 0;
    if (moneyRandom == 3) {
        money[moneyIndex].isActive = true;
        money[moneyIndex].wayIndex = wayRandom - 1;
        money[moneyIndex].x = (WINDOW_WIDTH - moneyImage->rect.w) / 2 + money[moneyIndex].wayIndex * GAP_X;
        money[moneyIndex].y = -moneyImage->rect.h;
        moneyIndex = (moneyIndex + 1) % NUM_OF_MONEY_MAX;
    }
    for (int i = 0; i < NUM_OF_MONEY_MAX; i++) {
        if (money[i].isActive) {
            DisplayImage(moneyImage, money[i].x, money[i].y, 1);
            money[i].y += speed + bonus;
            if (money[i].wayIndex == wayIndex && money[i].y + moneyImage->rect.h >= CHARACTER_Y && money[i].y <= CHARACTER_Y + DELTA + bonus) {
                money[i].isActive = false;
                score += 50;
                flag = 1;
            }
            if (money[i].y >= WINDOW_HEIGHT + moneyImage->rect.h) {
                money[i].isActive = false;
            }
        }
    }
    return flag;
}

static int DrawProp(Image *defenceImage, Image *pileImage) {
    int wayRandom = rand() % 3;
    int propRandom = rand() % 500;
    int clazz = rand() % 2;
    int flag = 0;
    if (propRandom == 10 && !isExist) {
        switch (clazz) {
            case 0:
                defence.isActive = true;
                defence.wayIndex = wayRandom - 1;
                defence.x = (WINDOW_WIDTH - defenceImage->rect.w) / 2 + GAP_X * defence.wayIndex;
                defence.y = -defenceImage->rect.h;
                break;
            case 1:
                pile.isActive = true;
                pile.wayIndex = wayRandom - 1;
                pile.x = (WINDOW_WIDTH - pileImage->rect.w) / 2 + GAP_X * pile.wayIndex;
                pile.y = -pileImage->rect.h;
                break;
            default:
                break;
        }
        isExist = true;
    } else if (isExist) {
        if (defence.isActive) {
            DisplayImage(defenceImage, defence.x, defence.y, 1);
            defence.y += speed + bonus;
            if (defence.wayIndex == wayIndex && defence.y + defenceImage->rect.h >= CHARACTER_Y && pile.y <= CHARACTER_Y + DELTA + bonus) {
                isDefensive = true;
                defence.isActive = false;
                isExist = false;
            }
            if (defence.y >= WINDOW_HEIGHT + defenceImage->rect.h) {
                defence.isActive = false;
                isExist = false;
            }
        } else if (pile.isActive) {
            DisplayImage(pileImage, pile.x, pile.y, 1);
            pile.y += speed + bonus;
            if (pile.wayIndex == wayIndex && pile.y + pileImage->rect.h >= CHARACTER_Y && pile.y <= CHARACTER_Y + DELTA + bonus) {
                score += 500;
                flag = 1;
                pile.isActive = false;
                isExist = false;
            }
            if (pile.y >= WINDOW_HEIGHT + pileImage->rect.h) {
                pile.isActive = false;
                isExist = false;
            }
        }
    }
    return flag;
}

static int DrawPause() {
    Image pause;
    InitImage(&pause, "../image/pause.png", 1);
    
    Action pauseActions[] = {ActionContinue, ActionRestart, ActionMainMenu};
    char *pauseContents[] = {
            "Continue",
            "Restart",
            "Main Menu"
    };
    pauseWidgets = malloc(NUM_OF_WIDGETS * sizeof(Widget));
    InitWidgets(pauseWidgets, NUM_OF_WIDGETS, &selectionPause, pauseActions, PAUSE_X, PAUSE_Y, PAUSE_GAP, pauseContents);
    
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
    QuitWidgets(pauseWidgets);
    QuitImage(&pause);
    return app.keyboard[SDL_SCANCODE_ESCAPE] ? 0 : flag;
}

static int DrawOver(Image *background, Text *scoreText, Image *character, int characterX, int characterY) {
    Image over;
    InitImage(&over, "../image/over.png", 1);
    Image newHighScore;
    InitImage(&newHighScore, "../image/newHighScore.png", 1);
    
    Action overActions[] = {ActionRestart, ActionMainMenu};
    char *overContents[] = {
            "Restart",
            "Main Menu"
    };
    overWidgets = malloc(NUM_OF_OVER_WIDGETS * sizeof(Widget));
    InitWidgets(overWidgets, NUM_OF_OVER_WIDGETS, &selectionOver, overActions, PAUSE_X, PAUSE_Y + PAUSE_GAP, PAUSE_GAP, overContents);
    
    Text choices[NUM_OF_OVER_WIDGETS];
    for (int i = 0; i < NUM_OF_OVER_WIDGETS; i++) {
        InitText(&choices[i], "../fonts/arial.ttf", 50, overWidgets[i].text, &BLACK);
    }
    Image arrow;
    InitImage(&arrow, "../image/choose.png", 1);
    arrow.rect.x = PAUSE_X - arrow.rect.w;
    
    SDL_Event event;
    int flag = 0;
    
    bool isNewHigh = false;
    int maxScore = 0;
    int averageScore = 0;
    int roundNumber = 0;
    FILE *file1 = fopen("../data/data.json", "r");
    if (file1 == NULL) {
        isNewHigh = true;
        maxScore = score;
        averageScore = score;
        roundNumber++;
    } else {
        fseek(file1, 0, SEEK_END);
        int fileSize = ftell(file1);
        rewind(file1);
        char *str = cJSON_malloc(fileSize * sizeof(*str));
        memset(str, 0, fileSize);
        fread(str, sizeof(*str), fileSize, file1);
        cJSON *root = cJSON_Parse(str);
        const cJSON *node1 = cJSON_GetObjectItem(root, "Max Score");
        const cJSON *node2 = cJSON_GetObjectItem(root, "Average Score");
        const cJSON *node3 = cJSON_GetObjectItem(root, "Round Number");
        maxScore = atoi(node1->valuestring);
        averageScore = atoi(node2->valuestring);
        roundNumber = atoi(node3->valuestring);
        if (maxScore <= score) {
            maxScore = score;
            isNewHigh = true;
        }
        averageScore = (int)(((double)roundNumber / (roundNumber + 1)) * averageScore + (double)score / (roundNumber + 1));
        roundNumber++;
        cJSON_Delete(root);
        fclose(file1);
    }
    
    FILE *file2 = fopen("../data/data.json", "w");
    if (file2 != NULL) {
        cJSON *root = cJSON_CreateObject();
        char max[100] = {0};
        char average[100] = {0};
        char round[100] = {0};
        itoa(maxScore, max, 10);
        itoa(averageScore, average, 10);
        itoa(roundNumber, round, 10);
        cJSON_AddStringToObject(root, "Max Score", max);
        cJSON_AddStringToObject(root, "Average Score", average);
        cJSON_AddStringToObject(root, "Round Number", round);
        const char *buffer = cJSON_Print(root);
        fwrite(buffer, strlen(buffer), 1, file2);
        cJSON_Delete(root);
        fclose(file2);
    }
    
    Mix_Chunk *applause;
    if (!(applause = Mix_LoadWAV("../audio/applause.MP3"))) {
        HANDLE_ERROR("InitSound applause");
    }
    if (isNewHigh) {
        Mix_PlayChannel(-1, applause, 0);
    }
    
    UpdateOver(choices, &over, &arrow, background, scoreText, character, characterX, characterY, &newHighScore, isNewHigh);
    while (!app.keyboard[SDL_SCANCODE_ESCAPE] && SDL_WaitEvent(&event) && flag == 0) {
        UpdateOver(choices, &over, &arrow, background, scoreText, character, characterX, characterY, &newHighScore, isNewHigh);
        DoEvent(event);
        flag = DoOverWidgets();
    }
    
    for (int i = 0; i < NUM_OF_OVER_WIDGETS; i++) {
        QuitText(&choices[i]);
    }
    Mix_FreeChunk(applause);
    QuitWidgets(overWidgets);
    QuitImage(&newHighScore);
    QuitImage(&over);
    return app.keyboard[SDL_SCANCODE_ESCAPE] ? 0 : flag;
}

static void UpdateCharacter(Image *character, int w, int h) {
    character[0].rect.w = w;
    character[0].rect.h = h;
    character[0].rect.x = (WINDOW_WIDTH - character[0].rect.w) / 2 + wayIndex * GAP_X;
    character[1].rect.w = w;
    character[1].rect.h = h;
    character[1].rect.x = (WINDOW_WIDTH - character[1].rect.w) / 2 + wayIndex * GAP_X;
}

static void UpdateScoreText(Text *scoreText) {
    char temp[100] = "Score: ";
    itoa(score, temp + 7, 10);
    InitText(scoreText, "../fonts/arialbi.ttf", 70, temp, &BLACK);
}

static void UpdatePause(Text *choices, Image *pause, Image *arrow) {
    UpdatePauseChoices(choices);
    UpdatePauseArrow(arrow, choices[0]);
    DisplayImage(pause, (WINDOW_WIDTH - pause->rect.w) / 2, (WINDOW_HEIGHT - pause->rect.h) / 2, 1);
    DisplayImage(arrow, arrow->rect.x, arrow->rect.y, 1);
    for (int i = 0; i < NUM_OF_WIDGETS; i++) {
        DisplayText(&choices[i], pauseWidgets[i].x, pauseWidgets[i].y);
    }
    DoFps();
}

static void UpdateOver(Text *choices, Image *over, Image *arrow, Image *background, Text *scoreText, Image *character, int characterX, int characterY, Image *newHighScore, bool flag) {
    UpdateOverChoices(choices);
    UpdateOverArrow(arrow, choices[0]);
    DisplayImage(background, 0, 0, 0);
    DisplayText(scoreText, SCORE_X, SCORE_Y);
    DisplayImage(character, characterX, characterY, 1);
    DisplayImage(over, (WINDOW_WIDTH - over->rect.w) / 2, (WINDOW_HEIGHT - over->rect.h) / 3, 1);
    DisplayImage(arrow, arrow->rect.x, arrow->rect.y, 1);
    if (flag) {
        DisplayImage(newHighScore, NEW_HIGH_SCORE_X, NEW_HIGH_SCORE_Y, 1);
    }
    for (int i = 0; i < NUM_OF_OVER_WIDGETS; i++) {
        DisplayText(&choices[i], overWidgets[i].x, overWidgets[i].y);
    }
    DoFps();
}

static void UpdatePauseChoices(Text *choices) {
    for (int i = 0; i < NUM_OF_WIDGETS; i++) {
        choices[i].color = selectionPause == i ? GRAY : BLACK;
        choices[i].surface = TTF_RenderText_Blended(choices[i].font, pauseWidgets[i].text, choices[i].color);
        choices[i].texture = SDL_CreateTextureFromSurface(app.renderer, choices[i].surface);
    }
}

static void UpdateOverChoices(Text *choices) {
    for (int i = 0; i < NUM_OF_OVER_WIDGETS; i++) {
        choices[i].color = selectionOver == i ? RED : BLACK;
        choices[i].surface = TTF_RenderText_Blended(choices[i].font, overWidgets[i].text, choices[i].color);
        choices[i].texture = SDL_CreateTextureFromSurface(app.renderer, choices[i].surface);
    }
}

static void UpdatePauseArrow(Image *arrow, Text choice) {
    arrow->rect.y = PAUSE_Y + selectionPause * PAUSE_GAP + choice.rect.h / 2 - arrow->rect.h / 2;
}

static void UpdateOverArrow(Image *arrow, Text choice) {
    arrow->rect.y = PAUSE_Y + (selectionOver + 1) * PAUSE_GAP + choice.rect.h / 2 - arrow->rect.h / 2;
}

static int ActPauseWidget() {
    isPause = false;
    return ActWidget(pauseWidgets, &selectionPause);
}

static int ActOverWidget() {
    isOver = false;
    return ActWidget(overWidgets, &selectionOver);
}

static int DoPauseWidgets() {
    if (app.keyboard[SDL_SCANCODE_UP]) {
        PrevWidget(&selectionPause, NUM_OF_WIDGETS);
    } else if (app.keyboard[SDL_SCANCODE_DOWN]) {
        NextWidget(&selectionPause, NUM_OF_WIDGETS);
    } else if (app.keyboard[SDL_SCANCODE_RETURN]) {
        return ActPauseWidget();
    }
    return 0;
}

static int DoOverWidgets() {
    if (app.keyboard[SDL_SCANCODE_UP]) {
        PrevWidget(&selectionOver, NUM_OF_OVER_WIDGETS);
    } else if (app.keyboard[SDL_SCANCODE_DOWN]) {
        NextWidget(&selectionOver, NUM_OF_OVER_WIDGETS);
    } else if (app.keyboard[SDL_SCANCODE_RETURN]) {
        return ActOverWidget();
    }
    return 0;
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
