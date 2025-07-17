// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef enum
{
    PLAYER_IDLE,
    PLAYER_WALK,
    PLAYER_JUMP,
    PLAYER_ATTACK,
    PLAYER_STATE_COUNT, // Always last
} PlayerState;

// Structure that represents the player
typedef struct Player
{
    Vector2 position;
    Vector2 velocity;
    float width;
    float height;
    bool isOnGround;
    PlayerState state;

    int frame;
    float frameTime;
    float frameSpeed;  // Velocity the sprite updates
    int maxFrames;     // Frames the sprite has
    float actionTimer; // In the future, to set timer to actions like attack or dashes

    bool directionFacing; // True = left, False = right

    int lifeLeft;
    bool isHurt;
    float hurtTimer;
} Player;

// Player related functions
void InitPlayer(Player *player);                                                         // iniciates the player
void UpdatePlayer(Player *player, float delta, Rectangle *platforms, int platformCount); // update the player
void DrawPlayer(Player player, Texture2D texture);                                       // draw the player on screen

#endif
