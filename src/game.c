// game.c
#include "raylib.h"
#include "game.h"
#include "player.h"

#define MAX_PLATFORMS 10

static Player player;
static Rectangle platforms[MAX_PLATFORMS];
static int platformCount = 0;
static Texture2D playerTexture;

static Camera2D camera;

void InitGame(void)
{
    InitPlayer(&player);
    playerTexture = LoadTexture("assets/images/player.png");

    // Create the platforms
    platforms[0] = (Rectangle){0, 400, 1600, 50}; // ground
    platforms[1] = (Rectangle){1650, 400, 300, 50};
    platforms[2] = (Rectangle){300, 320, 100, 10};
    platforms[3] = (Rectangle){600, 260, 120, 10};
    platforms[4] = (Rectangle){1000, 300, 100, 10};
    platforms[5] = (Rectangle){1300, 200, 100, 10};
    platformCount = 6;

    // init the cam
    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){400, 300}; // Center of the screen. Half of the screen size
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void UpdateGame(void)
{
    float delta = GetFrameTime();

    UpdatePlayer(&player, delta, platforms, platformCount);

    // Updates the cam based on the player x
    // The y axys stays fixed
    camera.target = (Vector2){
        player.position.x + player.width / 2,
        300};

    // Draw the map
    BeginMode2D(camera);

    for (int i = 0; i < platformCount; i++)
    {
        DrawRectangleRec(platforms[i], DARKGRAY);
    }

    DrawPlayer(player, playerTexture);

    EndMode2D();

    DrawText("Use as setas para mover e ESPACO para pular", 10, 10, 18, DARKGRAY);
    DrawText(player.isOnGround ? "no chao" : "no ar", 10, 30, 20, RED);
    DrawText(TextFormat("Vel Y: %.2f", player.velocity.y), 10, 60, 20, BLUE);
    DrawText(TextFormat("Delta: %.4f", GetFrameTime()), 10, 80, 20, DARKGRAY);
    DrawText(TextFormat("POS Y: %.2f", player.position.y), 10, 100, 20, PINK);
    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 120, 20, YELLOW);
}

void CloseGame(void)
{
    UnloadTexture(playerTexture);
}