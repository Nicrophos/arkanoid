#include "logics.h"
#include "sound.h"
#include "physics.h"
#include <math.h>
#include <time.h>

void update() {
    if (!ball.launched) {
        ball.rect.x = paddle.rect.x + paddle.rect.w / 2 - BALL_SIZE / 2; //м€ч в начале
        return;
    }
    ball.rect.x += ball.velX;
    ball.rect.y += ball.velY;

    if (ball.rect.x <= 0) {
        ball.rect.x = 0;
        ball.velX = -ball.velX;
        playBounceSound();
    }
    else if (ball.rect.x >= SCREEN_WIDTH - BALL_SIZE) {
        ball.rect.x = SCREEN_WIDTH - BALL_SIZE;
        ball.velX = -ball.velX;
        playBounceSound();
    }

    if (ball.rect.y <= 0) {
        ball.rect.y = 0;
        ball.velY = -ball.velY;
        playBounceSound();
    }

    if (ball.rect.y >= SCREEN_HEIGHT) {
        lives--;
        if (lives <= 0) {
            inMenu = true;
            updateMusic();
        }
        else {
            resetBall();
        }
    }
    for (int i = 0; i < MAX_BONUSES; i++) {
        if (bonuses[i].active) {
            bonuses[i].rect.y += bonuses[i].speed;
            if (SDL_HasIntersection(&bonuses[i].rect, &paddle.rect)) {
                activateBonus(bonuses[i].type);
                bonuses[i].active = false;
                playBounceSound();
            }
            if (bonuses[i].rect.y > SCREEN_HEIGHT) {
                bonuses[i].active = false;
            }
        }
    }
    checkCollisions();
    checkTimedEffects();
    if (blocksActive == 0) {
        if (currentLevel < 3) {
            loadLevel(currentLevel + 1);
        }
        else {
            inMenu = true;
            Mix_HaltMusic();
        }
    }
}

void checkCollisions() {
    if (SDL_HasIntersection(&ball.rect, &paddle.rect)) {
        float hitPos = (ball.rect.x + BALL_SIZE / 2) - (paddle.rect.x + paddle.rect.w / 2);
        float normalizedHitPos = hitPos / (paddle.rect.w / 2);
        normalizedHitPos = fmaxf(-0.9f, fminf(0.9f, normalizedHitPos));
        float angle = normalizedHitPos * (MAX_REFLECTION_ANGLE * M_PI / 180.0f);
        float speed = sqrt(ball.velX * ball.velX + ball.velY * ball.velY);
        ball.velX = sin(angle) * speed;
        ball.velY = -cos(angle) * speed;
        ball.velY = -fabs(ball.velY);
        playBounceSound();
    }
    bool collisionOccurred = false;
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i].active && SDL_HasIntersection(&ball.rect, &blocks[i].rect)) {
            collisionOccurred = true;
            blocks[i].hits--;

            if (blocks[i].hits <= 0) {
                blocks[i].active = false;
                blocksActive--;
                score += (blocks[i].type == STRONG) ? 200 : 100;
                if (blocks[i].type == BONUS || (rand() % 100) < 15) {
                    spawnBonus(blocks[i].rect.x + BLOCK_WIDTH / 2,
                        blocks[i].rect.y + BLOCK_HEIGHT);
                }
            }
            float speedBefore = sqrt(ball.velX * ball.velX + ball.velY * ball.velY);
            if (!ball.canPierce) {
                int ballCenterX = ball.rect.x + BALL_SIZE / 2;
                int ballCenterY = ball.rect.y + BALL_SIZE / 2;
                int blockCenterX = blocks[i].rect.x + BLOCK_WIDTH / 2;
                int blockCenterY = blocks[i].rect.y + BLOCK_HEIGHT / 2;
                int dx = ballCenterX - blockCenterX;
                int dy = ballCenterY - blockCenterY;
                float overlapX = (BALL_SIZE / 2 + BLOCK_WIDTH / 2) - abs(dx);
                float overlapY = (BALL_SIZE / 2 + BLOCK_HEIGHT / 2) - abs(dy);
                if (overlapX >= overlapY) {
                    ball.velY = -ball.velY;
                    if (dy > 0) {
                        ball.rect.y += overlapY;
                    }
                    else {
                        ball.rect.y -= overlapY;
                    }
                }
                else {
                    ball.velX = -ball.velX;
                    if (dx > 0) {
                        ball.rect.x += overlapX;
                    }
                    else {
                        ball.rect.x -= overlapX;
                    }
                }
            }

            float speedAfter = sqrt(ball.velX * ball.velX + ball.velY * ball.velY);
            if (speedAfter > 0) {
                float ratio = speedBefore / speedAfter;
                ball.velX *= ratio;
                ball.velY *= ratio;
            }

            ball.velX *= SPEED_INCREASE_FACTOR;
            ball.velY *= SPEED_INCREASE_FACTOR;
            normalizeBallSpeed();

            playBounceSound();

            if (!ball.canPierce) break;
        }
    }
}

void spawnBonus(int x, int y) {
    for (int i = 0; i < MAX_BONUSES; i++) {
        if (!bonuses[i].active) {
            bonuses[i].rect.x = x - 15;
            bonuses[i].rect.y = y;
            bonuses[i].rect.w = 30;
            bonuses[i].rect.h = 15;

            int r = rand() % 100;
            if (r < 30) {
                bonuses[i].type = WIDE_PADDLE;
            }
            else if (r < 60) {
                bonuses[i].type = FAST_PADDLE;
            }
            else if (r < 80) {
                bonuses[i].type = EXTRA_LIFE;
            }
            else {
                bonuses[i].type = PIERCE_BALL;
            }

            bonuses[i].speed = 3;
            bonuses[i].active = true;
            break;
        }
    }
}

void activateBonus(BonusType type) {
    Uint32 currentTime = SDL_GetTicks();
    switch (type) {
    case EXTRA_LIFE:
        lives++;
        break;
    case WIDE_PADDLE:
        paddle.wide = true;
        paddle.rect.w = paddle.originalWidth * 1.5;
        paddle.wideEndTime = currentTime + 6000;
        break;
    case FAST_PADDLE:
        paddle.fast = true;
        paddle.speed = 12;
        ball.velX *= 1.2;
        ball.velY *= 1.2;
        normalizeBallSpeed();
        paddle.fastEndTime = currentTime + FAST_EFFECT_DURATION;
        break;
    case PIERCE_BALL:
        ball.canPierce = true;
        ball.pierceEndTime = currentTime + PIERCE_EFFECT_DURATION;
        break;
    case NONE:
        break;
    }
}

void resetBall() {
    ball.rect.x = paddle.rect.x + paddle.rect.w / 2 - BALL_SIZE / 2;
    ball.rect.y = PADDLE_Y_POSITION - BALL_SIZE;
    ball.rect.w = BALL_SIZE;
    ball.rect.h = BALL_SIZE;
    ball.velX = 0;
    ball.velY = 0;
    ball.launched = false;
    ball.canPierce = false;
}
void resetPaddle() {
    paddle.rect.x = SCREEN_WIDTH / 2 - paddle.rect.w / 2;
    paddle.rect.y = PADDLE_Y_POSITION;

    if (paddle.wide) {
        paddle.rect.w = paddle.originalWidth;
        paddle.wide = false;
    }

    if (paddle.fast) {
        paddle.speed = 8;
        paddle.fast = false;
    }
}

void loadLevel(int level) {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        blocks[i].active = false;
    }

    blocksActive = 0;
    currentLevel = level;

    int blocksCount = 0;
    int cols = 0;
    int rows = 0;

    switch (level) {
    case 1:
        blocksCount = 48;
        cols = 8;
        rows = 6;
        break;
    case 2:
        blocksCount = 56;
        cols = 8;
        rows = 7;
        break;
    case 3:
        blocksCount = 64;
        cols = 8;
        rows = 8;
        break;
    }

    for (int i = 0; i < blocksCount; i++) {
        blocks[i].rect.w = BLOCK_WIDTH;
        blocks[i].rect.h = BLOCK_HEIGHT;

        int r = rand() % 100;
        if (r < 15) {
            blocks[i].type = BONUS;
            blocks[i].hits = 1;
        }
        else if (level == 1 || r < 70) {
            blocks[i].type = STANDARD;
            blocks[i].hits = 1;
        }
        else {
            blocks[i].type = STRONG;
            blocks[i].hits = (level == 2) ? 2 : 3;
        }

        blocks[i].active = true;
        blocksActive++;
    }

    adjustBlockPositions(blocksCount, cols);
    resetBall();
    resetPaddle();
    if (musicEnabled) {
        Mix_HaltMusic();
        switch (level) {
        case 1: Mix_PlayMusic(lvl1music, -1); break;
        case 2: Mix_PlayMusic(lvl2music, -1); break;
        case 3: Mix_PlayMusic(lvl3music, -1); break;
        }
    }
}

void checkTimedEffects() {
    Uint32 currentTime = SDL_GetTicks();

    if (paddle.wide && currentTime >= paddle.wideEndTime) {
        paddle.rect.w = paddle.originalWidth;
        paddle.wide = false;
    }

    if (paddle.fast && currentTime >= paddle.fastEndTime) {
        paddle.speed = 8;
        paddle.fast = false;

        float currentSpeed = sqrt(ball.velX * ball.velX + ball.velY * ball.velY);
        if (currentSpeed > BASE_BALL_SPEED * 1.2) {
            ball.velX *= 0.833f;
            ball.velY *= 0.833f;
            normalizeBallSpeed();
        }
    }

    if (ball.canPierce && currentTime >= ball.pierceEndTime) {
        ball.canPierce = false;
    }
}

void adjustBlockPositions(int blocksCount, int cols) {
    int totalBlockWidth = cols * BLOCK_WIDTH + (cols - 1) * 5;
    int startX = (SCREEN_WIDTH - totalBlockWidth) / 2;

    for (int i = 0; i < blocksCount; i++) {
        int row = i / cols;
        int col = i % cols;
        blocks[i].rect.x = startX + col * (BLOCK_WIDTH + 5);
        blocks[i].rect.y = 30 + row * (BLOCK_HEIGHT + 5);
    }
}