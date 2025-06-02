#include "input.h"
#include "sound.h"
#include "logics.h"

void processInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            gameRunning = false;
        }
        if (e.type == SDL_KEYDOWN) {
            if (inMenu) {
                switch (e.key.keysym.sym) {
                case SDLK_1: 
                    inMenu = false;
                    score = 0;
                    lives = 3;
                    loadLevel(1);
                    if (musicEnabled) {
                        Mix_HaltMusic();
                        Mix_PlayMusic(lvl1music, -1);
                    }
                    break;
                case SDLK_2:
                    showRules = !showRules;
                    break;
                case SDLK_3:
                    toggleMusic();
                    break;
                case SDLK_4:
                    toggleSound();
                    break;
                case SDLK_ESCAPE:
                    gameRunning = false;
                    break;
                }
            }
            else {
                switch (e.key.keysym.sym) {
                case SDLK_SPACE:
                    if (!ball.launched) {
                        ball.launched = true;
                        ball.velX = (rand() % 2) ? BASE_BALL_SPEED : -BASE_BALL_SPEED;
                        ball.velY = -BASE_BALL_SPEED;
                        playBounceSound();
                    }
                    break;
                case SDLK_ESCAPE:
                    inMenu = true;
                    updateMusic();
                    break;
                case SDLK_m:
                    toggleMusic();
                    break;
                case SDLK_s:
                    toggleSound();
                    break;
                case SDLK_p:
                    gamePaused = !gamePaused;
                    if (gamePaused) {
                        Mix_PauseMusic();
                    }
                    else {
                        if (musicEnabled) Mix_ResumeMusic();
                    }
                    break;
                }
            }
        }
    }
    if (!inMenu && !gamePaused) {
        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        if (keystates[SDL_SCANCODE_LEFT]) { 
            paddle.rect.x -= paddle.speed;
            if (paddle.rect.x < 0) paddle.rect.x = 0;
        }
        if (keystates[SDL_SCANCODE_RIGHT]) {
            paddle.rect.x += paddle.speed;
            if (paddle.rect.x > SCREEN_WIDTH - paddle.rect.w) {
                paddle.rect.x = SCREEN_WIDTH - paddle.rect.w;
            }
        }
    }
}