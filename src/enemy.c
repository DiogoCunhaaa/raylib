// enemy.c

#include "enemy.h"
#include "utils.h"

#define GRAVITY 400.0f
#define MOVE_SPEED 200.0f
#define JUMP_FORCE -300.0f
#define WORLD_WIDTH 1950
#define WORLD_HEIGHT 450

//internal functions, used restrictly by the enemy.c
static void ApplyPhysics(Enemy *enemy, float delta, Rectangle *platforms, int platformCount);

void InitEnemy(Enemy *enemy)
{
    enemy->position = (Vector2){150, 300};
    enemy->velocity = (Vector2){0, 0};

    enemy->width = 10;
    enemy->height = 20;

    enemy->isOnGround = false;
}

// internal functions
//------------------------------------------------------------------------------------------------
static void ApplyPhysics(Enemy *enemy, float delta, Rectangle *platforms, int platformCount)
{
    bool onGround = false;
    enemy->velocity.y += GRAVITY * delta;

    Rectangle enemyRect = {
        enemy->position.x,
        enemy->position.y,
        enemy->width,
        enemy->height,
    };

    // Horizontal movement and collision
    enemy->position.x += enemy->velocity.x * delta;
    enemyRect.x = enemy->position.x;

    for (int i = 0; i < platformCount; i++)
    {
        Rectangle plat = platforms[i];

        if (CheckCollisionRecs(enemyRect, plat))
        {
            if (enemy->velocity.x > 0)
            {
                enemy->position.x = plat.x - enemy->width;
            }
            else if (enemy->velocity.x < 0)
            {
                enemy->position.x = plat.x + plat.width;
            }

            enemy->velocity.x = 0;
            enemyRect.x = enemy->position.x;
        }
    }

    // Vertical movement and collision
    enemy->position.y += enemy->velocity.y * delta;
    enemyRect.y = enemy->position.y;

    for (int i = 0; i < platformCount; i++)
    {
        Rectangle plat = platforms[i];

        if (CheckCollisionRecs(enemyRect, plat))
        {
            float enemyTop = enemy->position.y;
            float enemyBottom = enemy->position.y + enemy->height;
            float platTop = plat.y;
            float platBottom = plat.y + plat.height;

            if (enemy->velocity.y > 0 && enemyBottom > platTop)
            {
                enemy->position.y = plat.y - enemy->height;
                enemy->velocity.y = 0;
                onGround = true;
            }
            else if (enemy->velocity.y < 0 && enemyTop < platBottom)
            {
                enemy->position.y = platBottom;
                enemy->velocity.y = 0;
            }

            enemyRect.y = enemy->position.y;
        }
    }

    enemy->isOnGround = onGround;

    // Clamp to avoid the enemy from exiting the screen
    enemy->position.x = MyClamp(enemy->position.x, 0, WORLD_WIDTH - enemy->width);
    enemy->position.y = MyClamp(enemy->position.y, 0, WORLD_HEIGHT - enemy->height);
}

//------------------------------------------------------------------------------------------------

void UpdateEnemy(Enemy *enemy, float delta, Rectangle *platforms, int platformCount)
{
    ApplyPhysics(enemy, delta, platforms, platformCount);

}

void DrawEnemy(Enemy enemy, Texture2D texture)
{
    Rectangle source = {
        enemy.position.x,
        enemy.position.y,
        enemy.width,
        enemy.height};

    Vector2 position = {enemy.position.x, enemy.position.y};
    DrawTextureRec(texture, source, position, WHITE);
}
