// game.c
#include <math.h>
#include "raylib.h"
#include "game.h"
#include "player.h"
#include "enemy.h"

#define MAX_PLATFORMS 100

static Player player;
static Enemy enemy;

static Rectangle platforms[MAX_PLATFORMS];
static int platformCount = 0;

static Texture2D playerTexture;
static Texture2D enemyTexture;
static Texture2D farBackgroundTexture;
static Texture2D frontBackgroundTexture;

static Camera2D camera;

// internal functions, used restrictly by the game.c
void DrawParallaxBackground(Texture2D texture, Camera2D camera, float parallaxFactor);
void CheckCollisionPlayerEnemy(Player *player, Enemy *enemy);

void InitGame(void)
{
    InitPlayer(&player);
    InitEnemy(&enemy);
    playerTexture = LoadTexture("assets/images/player.png");
    enemyTexture = LoadTexture("assets/images/enemy.png");
    farBackgroundTexture = LoadTexture("assets/images/farBackground.png");
    frontBackgroundTexture = LoadTexture("assets/images/frontBackground.png");

    // Create the platforms
    platforms[0] = (Rectangle){0, 400, 1600, 50}; // ground
    platforms[1] = (Rectangle){1650, 400, 300, 50};
    platforms[2] = (Rectangle){300, 320, 100, 10};
    platforms[3] = (Rectangle){600, 260, 120, 10};
    platforms[4] = (Rectangle){1000, 300, 100, 10};
    platforms[5] = (Rectangle){1300, 200, 100, 10};

    platformCount = 10;

    // init the cam
    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){400, 300}; // Center of the screen. Half of the screen size
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

// Internal functions
//------------------------------------------------------------------------------------------------
void DrawParallaxBackground(Texture2D texture, Camera2D camera, float parallaxFactor)
{
    int bgWidth = texture.width;
    int screenWidth = GetScreenWidth();

    // Calculate the base position for the image
    float viewLeft = camera.target.x * parallaxFactor - camera.offset.x;

    // rest of 0 and bgWidth
    float offset = (fmod(viewLeft, (float)bgWidth)) / 4;
    if (offset < 0)
    {
        offset += bgWidth;
    }

    // Initial point, can be negative
    float firstTileX = -offset;

    // Calculates how much we need to draw
    int tilesToDraw = screenWidth;

    for (int i = 0; i < tilesToDraw; i++)
    {
        DrawTexture(texture, (int)(firstTileX + i * bgWidth), 0, WHITE);
    }
}

void CheckCollisionPlayerEnemy(Player *player, Enemy *enemy)
{
    // Ignore if already taken damage
    if (player->isHurt) return;

    Rectangle playerRect = {
        player->position.x,
        player->position.y,
        player->width,
        player->height,
    };

    Rectangle enemyRect = {
        enemy->position.x,
        enemy->position.y,
        enemy->width,
        enemy->height,
    };

   if  (CheckCollisionRecs(playerRect, enemyRect))
   {
        // Check if the player comes from above (stomp)
        if (player->velocity.y > 0 && playerRect.y + playerRect.height < enemyRect.y + enemyRect.height/2)
        {            
            //Bounce player up and a slightly knockback
            player->velocity.x = (playerRect.x < enemyRect.x) ? -75 : 75;
            player->velocity.y = -300;
            player->isOnGround = false;

            // Here we should add some mechanic for damaging or killing the enemy
        }
        else // Collision from side or below
        {
            if (player->lifeLeft > 1)
            {
                player->lifeLeft -= 1;
                //knockback 
                player->velocity.x = (playerRect.x < enemyRect.x) ? -100 : 100;
                player->velocity.y = -200;
                player->isOnGround = false;
            } else // For handling death here
            {
                player->lifeLeft = 0;
                // This makes a strong knockback for death
                player->velocity.x = (playerRect.x < enemyRect.x) ? -200 : 200;
                player->velocity.y = -250;
                player->isOnGround = false;

                // Here we should add some game over handling
            }
        }

        // Create the delay for invunerability
        player->isHurt = true;
        player->hurtTimer = 0.8f;
        
        TraceLog(LOG_INFO, "Player hit! Life left: %d, vel.x: %.2f", player->lifeLeft, player->velocity.x);
   };
}
//---------------------------------------------------------------------------,---------------------

void UpdateGame(void)
{
    float delta = GetFrameTime();

    UpdatePlayer(&player, delta, platforms, platformCount);
    UpdateEnemy(&enemy, delta, platforms, platformCount);
    CheckCollisionPlayerEnemy(&player, &enemy);

    // Updates the cam based on the player x
    // The y axys stays fixed
    camera.target = (Vector2){
        player.position.x + player.width / 2,
        300};

    // Draw the map
    BeginMode2D(camera);
    DrawParallaxBackground(farBackgroundTexture, camera, 0.2f);
    DrawParallaxBackground(frontBackgroundTexture, camera, 0.8f);

    for (int i = 0; i < platformCount; i++)
    {
        DrawRectangleRec(platforms[i], DARKGRAY);
    }

    DrawPlayer(player, playerTexture);
    DrawEnemy(enemy, enemyTexture);

    EndMode2D();

    DrawText("Use as setas para mover e ESPACO para pular", 10, 10, 18, DARKGRAY);
    DrawText(player.isOnGround ? "no chao" : "no ar", 10, 30, 20, RED);
    DrawText(TextFormat("Vel Y: %.2f", player.velocity.y), 10, 60, 20, BLUE);
    DrawText(TextFormat("Delta: %.4f", GetFrameTime()), 10, 80, 20, DARKGRAY);
    DrawText(TextFormat("POS Y: %.2f", player.position.y), 10, 100, 20, PINK);
    DrawText(TextFormat("POS X: %.2f", player.position.x), 10, 120, 20, PINK);
    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 140, 20, YELLOW);
    DrawText(TextFormat("Life: %d", player.lifeLeft), 10, 160, 20, BLUE);
}

void CloseGame(void)
{
    UnloadTexture(playerTexture);
    UnloadTexture(farBackgroundTexture);
    UnloadTexture(frontBackgroundTexture);
    UnloadTexture(enemyTexture);
}
