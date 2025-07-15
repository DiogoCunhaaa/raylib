
/*-------------------------------------
* player.c
* HEADLINE
---------------------------------------*/

#include "player.h"
#include "utils.h"
#include <stdio.h>

#define GRAVITY 400.0f
#define MOVE_SPEED 200.0f
#define JUMP_FORCE -300.0f
#define WORLD_WIDTH 1950
#define WORLD_HEIGHT 450

// Internal functions, use restrictly by the player.c
static void HandleMovementInput(Player *player);
static void ApplyPhysics(Player *player, float delta, Rectangle *platforms, int platformCount);
static void UpdateState(Player *player, float delta);
static void UpdateAnimation(Player *player, float delta);

void InitPlayer(Player *player)
{
    player->position = (Vector2){100, 300};
    player->velocity = (Vector2){0, 0};

    player->width = 20;
    player->height = 40;

    player->isOnGround = false;
    player->state = PLAYER_IDLE;

    player->frame = 0;
    player->frameTime = 0.0f;
    player->frameSpeed = 4.0f; // default: idle
    player->maxFrames = 8;     // default: idle

    player->actionTimer = 0.0f;
}

// internal functions
//------------------------------------------------------------------------------------------------
static void HandleMovementInput(Player *player)
{
    if (player->state != PLAYER_ATTACK)
    {
        if (IsKeyDown(KEY_A))
        {
            player->velocity.x = -MOVE_SPEED; // make the player walk left by making his speed -200.0f
            player->directionFacing = true;   // turns the sprite left
        }
        else if (IsKeyDown(KEY_D))
        {
            player->velocity.x = MOVE_SPEED; // make the player walk right by making his speed 200.0f
            player->directionFacing = false; // turns the sprite right
        }
        else
        {
            player->velocity.x = 0; // makes sure the player stays stopped
        }

        if (player->isOnGround && IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W))
        {
            player->velocity.y = JUMP_FORCE; // makes the player jump
            player->isOnGround = false;      // makes sure the player can only jump when on ground
        }
    }
}

static void ApplyPhysics(Player *player, float delta, Rectangle *platforms, int platformCount)
{
    bool onGround = false;
    player->velocity.y += GRAVITY * delta;

    Rectangle playerRect = {
        player->position.x,
        player->position.y,
        player->width,
        player->height,
    };

    // Horizontal movement and collision
    player->position.x += player->velocity.x * delta;
    playerRect.x = player->position.x;

    for (int i = 0; i < platformCount; i++)
    {
        Rectangle plat = platforms[i];

        if (CheckCollisionRecs(playerRect, plat))
        {
            if (player->velocity.x > 0)
            {
                player->position.x = plat.x - player->width;
            }
            else if (player->velocity.x < 0)
            {
                player->position.x = plat.x + plat.width;
            }

            player->velocity.x = 0;
            playerRect.x = player->position.x;
        }
    }

    // Vertical movement and collision
    player->position.y += player->velocity.y * delta;
    playerRect.y = player->position.y;

    for (int i = 0; i < platformCount; i++)
    {
        Rectangle plat = platforms[i];

        if (CheckCollisionRecs(playerRect, plat))
        {
            float playerTop = player->position.y;
            float playerBottom = player->position.y + player->height;
            float platTop = plat.y;
            float platBottom = plat.y + plat.height;

            if (player->velocity.y > 0 && playerBottom > platTop)
            {
                player->position.y = plat.y - player->height;
                player->velocity.y = 0;
                onGround = true;
            }
            else if (player->velocity.y < 0 && playerTop < platBottom)
            {
                player->position.y = platBottom;
                player->velocity.y = 0;
            }

            playerRect.y = player->position.y;
        }
    }

    player->isOnGround = onGround;


    
    // Clamp to avoid the player from exiting the screen
    player->position.x = MyClamp(player->position.x, 0, WORLD_WIDTH - player->width);
    player->position.y = MyClamp(player->position.y, 0, WORLD_HEIGHT - player->height);
}

static void UpdateState(Player *player, float delta)
{
    // If attacking, ignores the rest
    if (player->state == PLAYER_ATTACK)
    {
        player->frameSpeed = 12.0f;
        player->maxFrames = 3;
        player->actionTimer -= delta;
        if (player->actionTimer <= 0)
        {
            player->state = PLAYER_IDLE;
        }
        return;
    }

    // State logic
    if (!player->isOnGround)
        player->state = PLAYER_JUMP;
    else if (player->velocity.x != 0)
        player->state = PLAYER_WALK;
    else
        player->state = PLAYER_IDLE;

    // Configuration by state
    switch (player->state)
    {
    case PLAYER_IDLE:
        player->frameSpeed = 8.0f;
        player->maxFrames = 6; // This is the quantity of frames the sprite has. Never forget to change it
        break;
    case PLAYER_WALK:
        player->frameSpeed = 4.0f;
        player->maxFrames = 8;
        break;
    default:
        break;
    case PLAYER_JUMP:
        player->frameSpeed = 4.0f;
        player->maxFrames = 6;
    }
}

static void UpdateAnimation(Player *player, float delta)
{
    if (player->frameSpeed <= 0)
        return;

    player->frameTime += delta;
    if (player->frameTime >= 1.0f / player->frameSpeed)
    {
        player->frameTime = 0;
        player->frame = (player->frame + 1) % player->maxFrames;
    }
}
//------------------------------------------------------------------------------------------------

// Update
void UpdatePlayer(Player *player, float delta, Rectangle *platforms, int platformCount)
{
    HandleMovementInput(player);
    ApplyPhysics(player, delta, platforms, platformCount);
    UpdateState(player, delta);
    UpdateAnimation(player, delta);
};

// Draw
void DrawPlayer(Player player, Texture2D texture)
{
    Rectangle source = {
        player.frame * player.width,
        player.state * player.height,
        player.directionFacing ? -player.width : player.width,
        player.height,
    };

    Vector2 position = {player.position.x, player.position.y};
    DrawTextureRec(texture, source, position, WHITE);
}
