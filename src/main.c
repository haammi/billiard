#include "game.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;

    Game g = {0};

    if (!game_init(&g)) {
        printf("Failed to init game\n");
        return 1;
    }

    game_run(&g);
    game_quit(&g);

    return 0;
}
