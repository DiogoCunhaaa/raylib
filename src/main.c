//main.c
#include "raylib.h"
#include "menu.h"
#include "game.h"

typedef enum GameScreen
{
    SCREEN_MENU,
    SCREEN_GAME
} GameScreen;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Jogo");

    GameScreen currentScreen = SCREEN_GAME;

    InitGame();

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Draw
        //--------------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentScreen)
        {
        case SCREEN_MENU:
            if (UpdateMenu() == 1)
                currentScreen = SCREEN_GAME;
            break;
        case SCREEN_GAME:
            UpdateGame();
            break;
        }
        EndDrawing();
        //--------------------------------------------------------------------------------------
    }

    // De-initialization
    //--------------------------------------------------------------------------------------
    CloseGame();
    CloseWindow();

    return 0;
}