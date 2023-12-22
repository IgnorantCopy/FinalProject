//
// Created by Ignorant on 2023/12/21.
//


#include <stdio.h>
#include "SDL2/SDL.h"
#include <math.h>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

#define PI 3.1415926
#define WIDTH 800
#define HEIGHT 600
#undef main

int main() {
    // 初始化
    // 1.初始化sdl视频界面和计时器
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    unsigned long long startup = SDL_GetTicks();
    // 2.创建窗口
    SDL_Window *window = SDL_CreateWindow("Hello World!", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    // 3.创建渲染器
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // 4.设置背景色
    // 设置渲染颜色
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    // 将设定好的颜色填充到整个渲染器
    SDL_RenderClear(renderer);
    // 将渲染器中的内容渲染到屏幕上,相当于update
    SDL_RenderPresent(renderer);
    // 5.填充画布
    // 画线
    SDL_Delay(1000);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, 0, 0, 800, 600);
    SDL_RenderPresent(renderer);
    // 画矩形
    SDL_Delay(1000);
    SDL_Rect rect = {.x = 100, .y = 100, .w = 100, .h = 100};
    // 空心
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
    // 实心
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
    // 动态效果
    for (double theta = 0; theta < 4 * PI; theta += 0.01) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_Rect newRect = {.x = (int)(300.0 + 100 * cos(theta)), .y = (int)(300.0 + 100 * sin(theta)), .w = 100, .h = 100};
        SDL_RenderFillRect(renderer, &newRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(5);
    }
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    
    // 6.绘制图片
    // 初始化
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    // 创建图片渲染器
    SDL_Surface *surfaceBackground0;
    if (!(surfaceBackground0 = IMG_Load("../image/background_0.jpg"))) {
        printf("Failed to load background: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Texture *textureBackground0 = SDL_CreateTextureFromSurface(renderer, surfaceBackground0);
    // 渲染到整个画布
    SDL_RenderCopy(renderer, textureBackground0, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
    // 渲染到部分画布
    SDL_Rect rectBackground0 = {.x = 0, .y = 100};
    SDL_QueryTexture(textureBackground0, NULL, NULL, &rectBackground0.w, &rectBackground0.h);   // 获取图片宽高
    SDL_RenderCopy(renderer, textureBackground0, NULL, &rectBackground0);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
    
    // 7.使用字体
    // 初始化
    TTF_Init();
    // 创建字体
    TTF_Font *fontTitle;
    if (!(fontTitle = TTF_OpenFont("../fonts/arial.ttf", 40))) {
        printf("Failed to load the font: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Color colorFontTitle = {0, 0, 0, SDL_ALPHA_OPAQUE};
    SDL_Surface *surfaceTitle = TTF_RenderText_Blended(fontTitle, "Tap ENTER To Start!", colorFontTitle);
    SDL_Texture *textureTitle = SDL_CreateTextureFromSurface(renderer, surfaceTitle);
    
    SDL_Rect rectTitle = {.x = 200, .y = 250};
    SDL_QueryTexture(textureTitle, NULL, NULL, &rectTitle.w, &rectTitle.h);
    SDL_RenderCopy(renderer, textureTitle, NULL, &rectTitle);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
    printf("RunTime: %lld\n", SDL_GetTicks() - startup);
    // 8.处理事件
    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                // 退出
                case SDL_QUIT:
                    puts("CLOSE");
                    goto CLOSE;
                // 按下鼠标
                case SDL_MOUSEBUTTONDOWN:
                    // 获取鼠标坐标
                    printf("mouse button down at (%d, %d)\n", event.button.x, event.button.y);
                    break;
                    // 抬起鼠标
                case SDL_MOUSEBUTTONUP:
                    printf("mouse button up at (%d, %d)\n", event.button.x, event.button.y);
                    break;
                // 按下键盘
                case SDL_KEYDOWN:
                    // 获取按键名
                    printf("key down: %s\n", SDL_GetKeyName(event.key.keysym.sym));
                    break;
                    // 抬起键盘
                case SDL_KEYUP:
                    printf("key up: %s\n", SDL_GetKeyName(event.key.keysym.sym));
                    // 按下esc退出程序
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        puts("CLOSE by key esc.\n");
                        goto CLOSE;
                    }
                    break;
                default:
                    break;
            }
            fflush(stdout);
        }
        SDL_Delay(5);
    }
    CLOSE:
    // 9.播放音频
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
    // 音效:一次可以放多个
    Mix_Chunk *sound;
    if (!(sound = Mix_LoadWAV("../audio/finish.MP3"))) {
        printf("Failed to load the music: %s\n", SDL_GetError());
        return 1;
    }
    // 音乐:一次只能放一首
    Mix_Music *finish;
    if (!(finish = Mix_LoadMUS("../audio/finish.MP3"))) {
        printf("Failed to load the music: %s\n", SDL_GetError());
        return 1;
    }
    // channel: -1 -- 自动选择音轨
    // loops: 循环次数
//    Mix_PlayChannel(-1, sound, 0);
    Mix_PlayMusic(finish, 0);
    // 暂停音效: Mix_HaltChannel(int channel);
    // 暂停音乐: Mix_HaltMusic();
    SDL_Delay(3000);
    
    // 10.关闭资源
    // 关闭音频
    Mix_CloseAudio();
    Mix_FreeChunk(sound);
    Mix_FreeMusic(finish);
    // 关闭字体
    TTF_CloseFont(fontTitle);
    // 退出字体渲染
    SDL_FreeSurface(surfaceTitle);
    SDL_DestroyTexture(textureTitle);
    TTF_Quit();
    // 关闭图片渲染器
    SDL_DestroyTexture(textureBackground0);
    SDL_FreeSurface(surfaceBackground0);
    // 关闭窗口
    SDL_DestroyWindow(window);
    // 关闭渲染器
    SDL_DestroyRenderer(renderer);
    
    SDL_Quit();
    
    return 0;
}
