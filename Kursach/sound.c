#include "sound.h"

void playBounceSound() {
    if (soundEnabled && bounceSound) {
        Mix_PlayChannel(-1, bounceSound, 0);
    }
}

void updateMusic() {
    if (!musicEnabled) {
        Mix_HaltMusic();
        return;
    }

    // ���������� ������� ������ ����� �������������
    Mix_HaltMusic();

    if (inMenu) {
        Mix_PlayMusic(menuMusic, -1);  // -1 ��� ������������
    }
    else {
        switch (currentLevel) {
        case 1: Mix_PlayMusic(lvl1music, -1); break;
        case 2: Mix_PlayMusic(lvl2music, -1); break;
        case 3: Mix_PlayMusic(lvl3music, -1); break;
        default: Mix_PlayMusic(lvl1music, -1); break;
        }
    }
}

void toggleMusic() {
    musicEnabled = !musicEnabled;
    updateMusic();  // ���������� ������ ������� ����������
}

void toggleSound() {
    soundEnabled = !soundEnabled;
}   