/*-------------------------------------
* I know it should be player-> instead of (*player)
* but i started this way and its too much work to do if i wanted to refactor the entire code
---------------------------------------*/

#include "player.h"
#include <stdio.h>

#define GRAVITY 400.0f
#define MOVE_SPEED 200.0f
#define JUMP_FORCE -250.0f

void InitPlayer(Player *player)
{
    (*player).position = (Vector2){100, 300};
    (*player).velocity = (Vector2){0, 0};
    (*player).width = 20;
    (*player).height = 40;
    (*player).isOnGround = false;
}

void UpdatePlayer(Player *player, float delta, Rectangle *platforms, int platformCount)
{
    bool onGround = false;

    (*player).velocity.y += GRAVITY * delta;

    if (IsKeyDown(KEY_A))
    {
        (*player).velocity.x = -MOVE_SPEED; // make the player walk left by making his speed -200.0f
    }
    else if (IsKeyDown(KEY_D))
    {
        (*player).velocity.x = MOVE_SPEED; // make the player walk right by making his speed 200.0f
    }
    else
    {
        (*player).velocity.x = 0; // makes sure the player stays stopped
    }

    if ((*player).isOnGround && IsKeyPressed(KEY_SPACE))
    {
        (*player).velocity.y = JUMP_FORCE; // makes the player jump
        (*player).isOnGround = false;      // makes sure the player can only jump when on ground
    }

    // if (IsKeyPressed(KEY_SPACE))
    // {
    //     (*player).velocity.y = JUMP_FORCE;
    // }

    // moves the player based on his actual position
    (*player).position.x += (*player).velocity.x * delta;
    (*player).position.y += (*player).velocity.y * delta;

    // This makes impossiple to the player fall infinitly
    if ((*player).position.y + (*player).height >= 400)
    {
        (*player).position.y = 400 - (*player).height;
        (*player).velocity.y = 0;
        (*player).isOnGround = true;
    };

    Rectangle playerRect = {
        (*player).position.x,
        (*player).position.y,
        (*player).width,
        (*player).height,
    };

    for (int i = 0; i < platformCount; i++)
    {
        Rectangle plat = platforms[i];

        if (CheckCollisionRecs(playerRect, plat))
        {
            float playerBottom = (*player).position.y + (*player).height;

            // Checks fall collision
            if (playerBottom <= plat.y + 5 && (*player).velocity.y >=0) {
                (*player).position.y = plat.y - (*player).height;
                (*player).velocity.y = 0;
                onGround = true;
            }
        }
    }
};

void DrawPlayer(Player player)
{
    DrawRectangle(player.position.x, player.position.y, player.width, player.height, BLUE);
}
