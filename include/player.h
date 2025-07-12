#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

//Structure that represents the player 
typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    float width;
    float height;
    bool isOnGround;
    int frame;
    float frameTime;
    float frameSpeed;
    int maxFrames;
} Player;

//Player related functions 
void InitPlayer(Player *player); //iniciates the player 
void UpdatePlayer(Player *player, float delta, Rectangle *platforms, int platformCount); //update the player 
void DrawPlayer(Player player, Texture2D texture); // draw the player on screen

#endif