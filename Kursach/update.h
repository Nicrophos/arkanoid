#pragma once
#ifndef UPDATE_H
#define UPDATE_H

#include "game.h"

void update();
void checkCollisions();
void spawnBonus(int x, int y);
void activateBonus(BonusType type);
void resetBall();
void resetPaddle();
void loadLevel(int level);
void checkTimedEffects();
void adjustBlockPositions(int blocksCount, int cols);

#endif