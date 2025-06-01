#include "render.h"
#include <stdio.h>

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (inMenu) {
        if (showRules) {
            renderRules();
        }
        else {
            renderMenu();
        }
    }
    else {
        renderGame();
    }

    SDL_RenderPresent(renderer);
}

void renderMenu() {
    if (menuBackground) {
        SDL_RenderCopy(renderer, menuBackground, NULL, NULL);
    }
    else {
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            int blue = 50 + (y * 100 / SCREEN_HEIGHT);
            SDL_SetRenderDrawColor(renderer, 0, 0, blue, 255);
            SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
        }
    }

    SDL_Color color = WHITE;
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect rect;

    const char* menuItems[] = {
        "1. New Game",
        "2. How to Play",
        "3. Toggle Music",
        "4. Toggle Sound",
        "ESC. Exit"
    };

    for (int i = 0; i < 5; i++) {
        surface = TTF_RenderText_Solid(font, menuItems[i], color);
        if (surface) {
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            rect.x = SCREEN_WIDTH / 2 - 100;
            rect.y = 200 + i * 40;
            rect.w = 200;
            rect.h = 30;
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
    }

    const char* musicStatus = musicEnabled ? "Music: ON" : "Music: OFF";
    const char* soundStatus = soundEnabled ? "Sound: ON" : "Sound: OFF";

    surface = TTF_RenderText_Solid(font, musicStatus, color);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect.x = SCREEN_WIDTH / 2 - 100;
        rect.y = 450;
        rect.w = 200;
        rect.h = 30;
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    surface = TTF_RenderText_Solid(font, soundStatus, color);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect.x = SCREEN_WIDTH / 2 - 100;
        rect.y = 480;
        rect.w = 200;
        rect.h = 30;
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}

void renderRules() {
    if (rulesBackground) {
        SDL_RenderCopy(renderer, rulesBackground, NULL, NULL);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
        SDL_RenderClear(renderer);
    }

    SDL_Color color = WHITE;
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect rect;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect overlay = { 50, 50, SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100 };
    SDL_RenderFillRect(renderer, &overlay);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    surface = TTF_RenderText_Solid(font, "HOW TO PLAY", color);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect.x = SCREEN_WIDTH / 2 - 100;
        rect.y = 70;
        rect.w = 200;
        rect.h = 30;
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    const char* rules[] = {
        "Goal: Destroy all bricks by bouncing the ball with your paddle.",
        "Controls:",
        "- Left/Right arrows - move paddle",
        "- Space - launch ball",
        "- P - pause game",
        "- ESC - return to menu",
        "- M - toggle music",
        "- S - toggle sound",
        "",
        "Features:",
        "- 3 difficulty levels",
        "- Different brick types: standard, strong, bonus",
        "- Power-ups:",
        "  * Red: Extra life",
        "  * Blue: Wider paddle",
        "  * Yellow: Speed boost (5 sec)",
        "  * Purple: Pierce shot (3 sec)",
        "",
        "Press 2 to return to menu"
    };

    for (int i = 0; i < 18; i++) {
        surface = TTF_RenderText_Solid(rulesFont, rules[i], color);
        if (surface) {
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            int textWidth, textHeight;
            SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);
            rect.x = 70;
            rect.y = 120 + i * 25;
            rect.w = textWidth;
            rect.h = textHeight;
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
    }
}

void renderGame() {
    if (gameBackground) {
        SDL_RenderCopy(renderer, gameBackground, NULL, NULL);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    // Paddle
    SDL_SetRenderDrawColor(renderer, GREEN.r, GREEN.g, GREEN.b, GREEN.a);
    SDL_RenderFillRect(renderer, &paddle.rect);

    // Ball
    if (ball.canPierce) {
        SDL_SetRenderDrawColor(renderer, PURPLE.r, PURPLE.g, PURPLE.b, PURPLE.a);
    }
    else {
        SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);
    }
    SDL_RenderFillRect(renderer, &ball.rect);

    // Bricks
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (blocks[i].active) {
            switch (blocks[i].type) {
            case STANDARD:
                SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b, RED.a);
                break;
            case STRONG:
                SDL_SetRenderDrawColor(renderer, ORANGE.r, ORANGE.g, ORANGE.b, ORANGE.a);
                break;
            case BONUS:
                SDL_SetRenderDrawColor(renderer, YELLOW.r, YELLOW.g, YELLOW.b, YELLOW.a);
                break;
            }
            SDL_RenderFillRect(renderer, &blocks[i].rect);

            SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);
            SDL_RenderDrawRect(renderer, &blocks[i].rect);

            if (blocks[i].type == STRONG) {
                char hitsText[10];
                snprintf(hitsText, 10, "%d", blocks[i].hits);
                SDL_Color textColor = WHITE;
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, hitsText, textColor);
                if (textSurface) {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                    SDL_Rect textRect = {
                        blocks[i].rect.x + BLOCK_WIDTH / 2 - 5,
                        blocks[i].rect.y + BLOCK_HEIGHT / 2 - 10,
                        20, 20
                    };
                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                    SDL_DestroyTexture(textTexture);
                    SDL_FreeSurface(textSurface);
                }
            }
        }
    }

    // Power-ups
    for (int i = 0; i < MAX_BONUSES; i++) {
        if (bonuses[i].active) {
            switch (bonuses[i].type) {
            case EXTRA_LIFE:
                SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b, RED.a);
                break;
            case WIDE_PADDLE:
                SDL_SetRenderDrawColor(renderer, BLUE.r, BLUE.g, BLUE.b, BLUE.a);
                break;
            case FAST_PADDLE:
                SDL_SetRenderDrawColor(renderer, YELLOW.r, YELLOW.g, YELLOW.b, YELLOW.a);
                break;
            case PIERCE_BALL:
                SDL_SetRenderDrawColor(renderer, PURPLE.r, PURPLE.g, PURPLE.b, PURPLE.a);
                break;
            case NONE:
                break;
            }
            SDL_RenderFillRect(renderer, &bonuses[i].rect);
        }
    }

    renderHUD();

    if (gamePaused) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_Rect overlay = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderFillRect(renderer, &overlay);

        SDL_Color color = WHITE;
        SDL_Surface* surface = TTF_RenderText_Solid(font, "PAUSED", color);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect rect = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 15, 100, 30 };
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
    }
}

void renderHUD() {
    SDL_Color color = WHITE;
    char text[50];
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect rect;

    // Create semi-transparent HUD background
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_Rect hudBackground = { 0, SCREEN_HEIGHT - 90, SCREEN_WIDTH, 90 };
    SDL_RenderFillRect(renderer, &hudBackground);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // Score
    snprintf(text, 50, "Score: %d", score);
    surface = TTF_RenderText_Solid(font, text, color);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect.x = 20;
        rect.y = SCREEN_HEIGHT - 80;
        rect.w = 150;
        rect.h = 30;
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    // Lives
    snprintf(text, 50, "Lives: %d", lives);
    surface = TTF_RenderText_Solid(font, text, color);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect.x = 180;
        rect.y = SCREEN_HEIGHT - 80;
        rect.w = 100;
        rect.h = 30;
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    // Level
    snprintf(text, 50, "Level: %d", currentLevel);
    surface = TTF_RenderText_Solid(font, text, color);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect.x = 290;
        rect.y = SCREEN_HEIGHT - 80;
        rect.w = 100;
        rect.h = 30;
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    // Music and sound status
    const char* musicStatus = musicEnabled ? "Music: ON (M)" : "Music: OFF (M)";
    const char* soundStatus = soundEnabled ? "Sound: ON (S)" : "Sound: OFF (S)";

    surface = TTF_RenderText_Solid(rulesFont, musicStatus, color);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect.x = SCREEN_WIDTH - 300;
        rect.y = SCREEN_HEIGHT - 80;
        rect.w = 140;
        rect.h = 20;
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    surface = TTF_RenderText_Solid(rulesFont, soundStatus, color);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect.x = SCREEN_WIDTH - 300;
        rect.y = SCREEN_HEIGHT - 60;
        rect.w = 140;
        rect.h = 20;
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    // Pause hint
    surface = TTF_RenderText_Solid(rulesFont, "Pause: P", color);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect.x = SCREEN_WIDTH - 150;
        rect.y = SCREEN_HEIGHT - 80;
        rect.w = 100;
        rect.h = 20;
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    // Menu hint
    surface = TTF_RenderText_Solid(rulesFont, "Menu: ESC", color);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect.x = SCREEN_WIDTH - 150;
        rect.y = SCREEN_HEIGHT - 60;
        rect.w = 100;
        rect.h = 20;
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    // Show active effects
    if (ball.canPierce) {
        surface = TTF_RenderText_Solid(rulesFont, "PIERCE MODE!", PURPLE);
        if (surface) {
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            rect.x = SCREEN_WIDTH / 2 - 60;
            rect.y = SCREEN_HEIGHT - 120;
            rect.w = 120;
            rect.h = 20;
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
    }
    if (paddle.fast) {
        surface = TTF_RenderText_Solid(rulesFont, "SPEED BOOST!", YELLOW);
        if (surface) {
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            rect.x = SCREEN_WIDTH / 2 - 60;
            rect.y = SCREEN_HEIGHT - 140;
            rect.w = 120;
            rect.h = 20;
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
    }
}