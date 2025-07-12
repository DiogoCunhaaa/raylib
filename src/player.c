/*-------------------------------------
* I know it should be player-> instead of (*player)
* but i started this way and its too much work to do if i wanted to refactor the entire code
---------------------------------------*/

#include "player.h"
#include "utils.h"
#include <stdio.h>

#define GRAVITY 400.0f
#define MOVE_SPEED 200.0f
#define JUMP_FORCE -250.0f
#define WORLD_WIDTH 1950
#define WORLD_HEIGHT 450

void InitPlayer(Player *player)
{
    (*player).position = (Vector2){100, 300};
    (*player).velocity = (Vector2){0, 0};
    (*player).width = 20;
    (*player).height = 40;
    (*player).isOnGround = false;
    (*player).frame = 0;
    (*player).frameTime = 0.0f;
    (*player).frameSpeed = 6.0f; // Frames per second
    (*player).maxFrames = 8;     // Total number of frames
}

// Update
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

    if ((*player).isOnGround && IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) // CHANGE TO if ((*player).isOnGround && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W))) AFTER TESTS
    {
        (*player).velocity.y = JUMP_FORCE; // makes the player jump
        (*player).isOnGround = false;      // makes sure the player can only jump when on ground
    }

    Rectangle playerRect = {
        (*player).position.x,
        (*player).position.y,
        (*player).width,
        (*player).height,
    };

    // moves the player based on his actual position
    // On the x axys
    (*player).position.x += (*player).velocity.x * delta; // Moves horizontaly

    // HORIZONTAL MOVEMENT + LATERAL COLLISION
    playerRect.x = (*player).position.x;
    playerRect.y = (*player).position.y;

    for (int i = 0; i < platformCount; i++)
    {
        Rectangle plat = platforms[i];

        if (CheckCollisionRecs(playerRect, plat))
        {
            if ((*player).velocity.x > 0)
            {
                (*player).position.x = plat.x - (*player).width;
            }
            else if ((*player).velocity.x < 0)
            {
                (*player).position.x = plat.x + plat.width;
            }

            (*player).velocity.x = 0;

            // Updates the rect
            playerRect.x = (*player).position.x;
        }
    };

    // VERTICAL MOVEMENT + VERTICAL COLLISION
    // Moves the player on his y axys
    (*player).position.y += (*player).velocity.y * delta;

    playerRect.y = (*player).position.y;

    onGround = false;

    for (int i = 0; i < platformCount; i++)
    {
        Rectangle plat = platforms[i];

        if (CheckCollisionRecs(playerRect, plat))
        {
            // Collision variables
            float playerTop = (*player).position.y;
            float playerBottom = (*player).position.y + (*player).height;
            float platTop = plat.y;
            float platBottom = plat.y + plat.height;

            if ((*player).velocity.y > 0 && playerBottom > platTop)
            {
                // Feet collision with the platform
                (*player).position.y = plat.y - (*player).height;
                (*player).velocity.y = 0;
                onGround = true;
            }
            else if ((*player).velocity.y < 0 && playerTop < platBottom)
            {
                // Head collision with the platform
                (*player).position.y = platBottom;
                (*player).velocity.y = 0;
            }

            // Updates the rect
            playerRect.y = (*player).position.y;
        }
    }

    (*player).isOnGround = onGround;

    // Clamp to avoid the player from exiting the screen
    (*player).position.x = MyClamp((*player).position.x, 0, WORLD_WIDTH - (*player).width);
    (*player).position.y = MyClamp((*player).position.y, 0, WORLD_HEIGHT - (*player).height);

    (*player).frameTime += delta;
    if ((*player).velocity.x != 0)
    {
        if ((*player).frameTime >= 1.0f / (*player).frameSpeed)
        {
            (*player).frameTime = 0;
            (*player).frame = ((*player).frame + 1) % (*player).maxFrames;
        }
    }
    else
    {
        (*player).frame = 0;
    }
};

// Draw
void DrawPlayer(Player player, Texture2D texture)
{
    Rectangle source = {(player).frame * 20, 0, 20, 40};
    Vector2 position = {player.position.x, player.position.y};
    DrawTextureRec(texture, source, position, WHITE);
}
