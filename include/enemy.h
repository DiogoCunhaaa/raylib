#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

typedef struct enemy
{
    Vector2 position;
    Vector2 velocity;
    float width;
    float height;
    bool isOnGround;
} Enemy;

// Enemy related functions
void InitEnemy(Enemy *enemy);
void UpdateEnemy(Enemy *enemy, float delta, Rectangle *platforms, int platformCount);
void DrawEnemy(Enemy enemy, Texture2D texture);

#endif
