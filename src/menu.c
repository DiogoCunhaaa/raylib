//menu.c
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "menu.h"
#include "text_utils.h"

void InitMenu(void) {}

int UpdateMenu(void)
{
    //This calculates the center x position of the screen
    int posX = GetTextCenteredX("Bem-Vindo ao jogo", 20, GetScreenWidth());
    //This calculates the center y position of the screen
    int posY = GetTextCenteredY("Bem-Vindo ao jogo", 20, GetScreenHeight());

    DrawText("Bem-Vindo ao jogo!", posX, 100, 20, DARKGRAY);

    if (GuiButton((Rectangle){300, 200, 200, 40}, "JOGAR"))
    {
        return 1;
    }

    return 0;
}