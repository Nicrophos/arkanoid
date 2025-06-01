#define _CRT_SECURE_NO_WARNINGS
#include "init.h"
#include "input.h"
#include "update.h"
#include "render.h"

int main(int argc, char* argv[]) {
    initialize();

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