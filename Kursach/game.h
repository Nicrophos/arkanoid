#pragma once
#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdbool.h>

/* GAME CONSTANTS */
#define MIN_BALL_SPEED 7.0f
#define MAX_BALL_SPEED 10.0f
#define SPEED_INCREASE_FACTOR 1.05f
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 20
#define PADDLE_Y_POSITION (SCREEN_HEIGHT - 100)
#define BALL_SIZE 15
#define BLOCK_WIDTH 70
#define BLOCK_HEIGHT 30
#define MAX_BLOCKS 64
#define MAX_BONUSES 10
#define BASE_BALL_SPEED 4
#define MAX_REFLECTION_ANGLE 60.0f
#define FAST_EFFECT_DURATION 5000
#define PIERCE_EFFECT_DURATION 3000
extern const SDL_Color WHITE;
extern const SDL_Color BLACK;
extern const SDL_Color RED;
extern const SDL_Color GREEN;
extern const SDL_Color BLUE;
extern const SDL_Color YELLOW;
extern const SDL_Color ORANGE;
extern const SDL_Color PURPLE;
typedef enum { STANDARD, STRONG, BONUS } BlockType;
typedef enum { EXTRA_LIFE, WIDE_PADDLE, FAST_PADDLE, PIERCE_BALL, NONE } BonusType;
typedef struct {
    SDL_Rect rect;
    BlockType type;
    int hits;
    bool active;
} Block;
typedef struct {
    SDL_Rect rect;
    BonusType type;
    bool active;
    int speed;
} Bonus;
typedef struct {
    SDL_Rect rect;
    float velX, velY;
    bool launched;
    bool canPierce;
    int pierceEndTime;
} Ball;
typedef struct {
    SDL_Rect rect;
    int originalWidth;
    int speed;
    bool wide;
    bool fast;
    Uint32 fastEndTime;
    Uint32 wideEndTime;
} Paddle;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern TTF_Font* font;
extern TTF_Font* rulesFont;
extern Mix_Music* menuMusic;
extern Mix_Music* lvl1music;
extern Mix_Music* lvl2music;
extern Mix_Music* lvl3music;
extern Mix_Chunk* bounceSound;
extern SDL_Texture* menuBackground;
extern SDL_Texture* gameBackground;
extern SDL_Texture* rulesBackground;
extern Block blocks[MAX_BLOCKS];
extern Bonus bonuses[MAX_BONUSES];
extern Ball ball;
extern Paddle paddle;
extern int score;
extern int lives;
extern int currentLevel;
extern int blocksActive;
extern bool gameRunning;
extern bool gamePaused;
extern bool inMenu;
extern bool showRules;
extern bool musicEnabled;
extern bool soundEnabled;
#endif