//game.c
#include "raylib.h"
#include "game.h"
#include "player.h"

#define MAX_PLATFORMS 10

static Player player;
static Rectangle platforms[MAX_PLATFORMS];
static int platformCount = 0;

void InitGame(void) {
    InitPlayer(&player);

    //Create the platforms
    platforms[0] = (Rectangle){ 0, 400, 800, 50 }; // ground
    platforms[1] = (Rectangle){ 200, 300, 100, 10}; //small platform
    platforms[2] = (Rectangle){ 400, 250, 120, 10}; //bigger platform
    platformCount = 3;
}

void UpdateGame(void)
{
    float delta = GetFrameTime(); 

    UpdatePlayer(&player, delta, platforms, platformCount);

    DrawText("Use as setas para mover e ESPACO para pular", 10, 10, 18, DARKGRAY);
    DrawText(player.isOnGround ? "no chao" : "no ar",10, 30, 20, RED);
    DrawText(TextFormat("Vel Y: %.2f", player.velocity.y), 10, 60, 20, BLUE);
    DrawText(TextFormat("Delta: %.4f", GetFrameTime()), 10, 80, 20, DARKGRAY);
    DrawText(TextFormat("POS Y: %.2f", player.position.y), 10, 100, 20, PINK);


    for (int i = 0; i < platformCount; i++) {
        DrawRectangleRec(platforms[i], DARKGRAY);
    }

    DrawPlayer(player);
}

void CloseGame(void) {}