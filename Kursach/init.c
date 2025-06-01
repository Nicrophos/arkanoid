#include "init.h"
#include <stdio.h>
#include <time.h>

void initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL init error: %s\n", SDL_GetError());
        return;
    }

    if (TTF_Init() == -1) {
        printf("TTF init error: %s\n", TTF_GetError());
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer init error: %s\n", Mix_GetError());
        return;
    }

    window = SDL_CreateWindow("Arkanoid",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window error: %s\n", SDL_GetError());
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer error: %s\n", SDL_GetError());
        return;
    }
    font = TTF_OpenFont("timesnewromanpsmt.ttf", 24);
    if (!font) {
        printf("Font error: %s\n", TTF_GetError());
        return;
    }
    rulesFont = TTF_OpenFont("timesnewromanpsmt.ttf", 18);
    if (!rulesFont) {
        rulesFont = font;
    }
    menuMusic = Mix_LoadMUS("menumusic.wav");
    lvl1music = Mix_LoadMUS("lvl1music.wav");
    lvl2music = Mix_LoadMUS("lvl2music.wav");
    lvl3music = Mix_LoadMUS("lvl3music.wav");
    if (musicEnabled) {
        updateMusic();
    }
    if (!menuMusic || !lvl1music || !lvl2music || !lvl3music) {
        printf("Failed to load music files! Error: %s\n", Mix_GetError());
    }
    bounceSound = Mix_LoadWAV("udar.wav");
    SDL_Surface* bgSurface = SDL_LoadBMP("fon1.bmp");
    if (bgSurface) {
        menuBackground = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
    }
    bgSurface = SDL_LoadBMP("fon2.bmp");
    if (bgSurface) {
        gameBackground = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
    }
    bgSurface = SDL_LoadBMP("fon2.bmp");
    if (bgSurface) {
        rulesBackground = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
    }
    paddle.rect.x = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2;
    paddle.rect.y = PADDLE_Y_POSITION;
    paddle.rect.w = PADDLE_WIDTH;
    paddle.rect.h = PADDLE_HEIGHT;
    paddle.originalWidth = PADDLE_WIDTH;
    paddle.speed = 8;
    paddle.wide = false;
    paddle.fast = false;
    paddle.fastEndTime = 0;

    resetBall();

    for (int i = 0; i < MAX_BLOCKS; i++) {
        blocks[i].active = false;
    }
    for (int i = 0; i < MAX_BONUSES; i++) {
        bonuses[i].active = false;
    }

    srand(time(NULL));
    loadLevel(1);
}

void cleanUp() {
    if (font) TTF_CloseFont(font);
    if (rulesFont && rulesFont != font) TTF_CloseFont(rulesFont);
    if (menuMusic) Mix_FreeMusic(menuMusic);
    if (lvl1music) Mix_FreeMusic(lvl1music);
    if (lvl2music) Mix_FreeMusic(lvl2music);
    if (lvl3music) Mix_FreeMusic(lvl3music);
    if (bounceSound) Mix_FreeChunk(bounceSound);
    if (menuBackground) SDL_DestroyTexture(menuBackground);
    if (gameBackground) SDL_DestroyTexture(gameBackground);
    if (rulesBackground) SDL_DestroyTexture(rulesBackground);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);

    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}