#define _CRT_SECURE_NO_WARNINGS
#include "initialization.h"
#include "input.h"
#include "logics.h"
#include "render.h"

int main(int argc, char* argv[]) {
    initialize();
    inMenu = true; 
    updateMusic();
    while (gameRunning) {
        processInput();

        if (!inMenu && !gamePaused) {
            update();
        }

        render();
        SDL_Delay(16);
    }

    cleanUp();
    return 0;
}