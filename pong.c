#include "raylib.h"
#include "raymath.h"

// -------------------------------------------------------------
// Simple pong game in Raylib and C
//
//  MIT License
//  Copyright (c) 2026 Ira Theobold
//
// Based on https://github.com/bi3mer/raylib-pong
// -------------------------------------------------------------
#define LIGHT_BLUE (Color){ 102, 178, 255,255 }
#define DARK_BLUE (Color){ 51, 153, 255,255 }
#define GAME_OVER "GAME OVER!"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int main(void)
{
    // setup game state

    const int screenWidth = 1280;
    const int screenHeight = 800;
    const int playStartHeight = 50;


    int player1Score = 0;
    int player2Score = 0;

    bool paused = false;
    bool active = false;

    // setup the player paddles
    Rectangle playerLeftPaddle =
    { 
      .x = 0.05f * screenWidth,
      .y = 0.40f * screenHeight,
      .width = 0.01f * screenWidth,
      .height = 0.2f * screenHeight,
    };

    Rectangle playerRightPaddle =
    { 
      .x = 0.95f * screenWidth,
      .y = 0.40f * screenHeight,
      .width = 0.01f * screenWidth,
      .height = 0.2f * screenHeight,
    };

    Vector2 ballPosition = 
    {
        .x = 0.5f * screenWidth,
        .y = 0.5f * screenHeight,
    };

    float ballRadius = screenHeight * 0.015f;

    Vector2 ballSpeed = 
    {
        .x = -300.0f,
        .y = 0.0f,
    };

    
    // Run the game
    InitWindow(screenWidth , screenHeight, "raylib pong");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        if (IsKeyPressed(KEY_P))
        {
            paused = !paused;
        }

        if (!paused)
        {
            // Handle key presses
            const float paddleMod = 10.0f;
            //player 1
            if (IsKeyDown(KEY_W)) {
                playerLeftPaddle.y = MAX(playStartHeight, playerLeftPaddle.y - paddleMod);
            }
            if (IsKeyDown(KEY_S))
            {
                playerLeftPaddle.y = MIN(screenHeight - playerLeftPaddle.height, playerLeftPaddle.y + paddleMod);
            }

            if (IsKeyDown(KEY_UP))
            {
                playerRightPaddle.y = MAX(playStartHeight, playerRightPaddle.y - paddleMod);
            }

            if (IsKeyDown(KEY_DOWN))
            {
                playerRightPaddle.y =
                    MIN(screenHeight - playerRightPaddle.height, playerRightPaddle.y + paddleMod);
            }

            const float dt = GetFrameTime();

            ballPosition.x += ballSpeed.x * dt;
            ballPosition.y += ballSpeed.y * dt;

           // handle collisions
            if (ballSpeed.x < 0)
            {
                if (CheckCollisionCircleRec(ballPosition, ballRadius, playerLeftPaddle))
                {
                    float hitPos = (ballPosition.y - playerLeftPaddle.y) / playerLeftPaddle.height;
                    hitPos = Clamp(hitPos, 0.0f, 1.0f);

                    const float a = (hitPos - 0.5f) * PI / 4;
                    const float speed = Vector2Length(ballSpeed) * 1.05f;
                    ballSpeed.x = speed * cosf(a);
                    ballSpeed.y = speed * sinf(a);
                }
            }
            else
            {
                if (CheckCollisionCircleRec(ballPosition, ballRadius, playerRightPaddle))
                {
                    float hitPos = (ballPosition.y - playerRightPaddle.y) / playerRightPaddle.height;
                    hitPos = Clamp(hitPos, 0.0f, 1.0f);

                    const float a = PI - (hitPos - 0.5f) * PI / 4;
                    const float speed = Vector2Length(ballSpeed) * 1.05f;
                    ballSpeed.x = speed * cosf(a);
                    ballSpeed.y = speed * sinf(a);
                }
            }

            // collision with top and bottom wall
            if (ballSpeed.y > 0)
            {
                if (ballPosition.y >= screenHeight - ballRadius)
                {
                    ballSpeed.y *= -1;
                    ballPosition.y = screenHeight - ballRadius;
                }
            }
            else
            {
                if (ballPosition.y <= playStartHeight + ballRadius)
                {
                    ballSpeed.y *= -1;
                    ballPosition.y = playStartHeight + ballRadius;
                }
            }

            // handle scoring
            if (ballPosition.x < 0)
            {
                ++player2Score;

                ballSpeed.x = -300.0f;
                ballSpeed.y = 0.0f;
                ballPosition.x = 0.5f * screenWidth;
                ballPosition.y = 0.5f * screenHeight;
            }
            else if (ballPosition.x > screenWidth)
            {
                ++player1Score;

                ballSpeed.x = 300.0f;
                ballSpeed.y = 0.0f;
                ballPosition.x = 0.5f * screenWidth;
                ballPosition.y = 0.5f * screenHeight;
            }
        }

        // Render the game screen
        BeginDrawing();

            ClearBackground(DARK_BLUE);

            DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, LIGHT_BLUE);
            DrawCircle(screenWidth / 2, screenHeight / 2, 150  / 2, BLUE);
            DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);

            DrawRectangleRec(playerLeftPaddle, WHITE);
            DrawRectangleRec(playerRightPaddle, WHITE);
            DrawCircleV(ballPosition, ballRadius, YELLOW);

            DrawText(TextFormat("%i", player1Score), screenWidth / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", player2Score), 3 * screenWidth / 4, 20, 80, WHITE);

            // Start of game instructions
            if (active == false)
            {
                // pause at start

                paused = true;

                const int startFontSize = 50;
                const char *startText = "Press ENTER to start game";
                const char *keysText = "Player controls: W/S and UP/DOWN keys";
                const int startTextWidth = MeasureText(startText, startFontSize);
                const int keysTextWidth = MeasureText(keysText, startFontSize);
                const int sX = (screenWidth - startTextWidth) / 2;
                const int kX = (screenWidth - keysTextWidth) / 2;

                DrawText(startText, sX, screenHeight / 4 - 20, startFontSize, WHITE);
                DrawText(keysText, kX, screenHeight / 4 + startFontSize, startFontSize, WHITE);

                    if(IsKeyDown(KEY_ENTER))
                    {
                        active = true;
                        paused = false;

                    }
            }

           // render pause, if necessary
           if (paused)
           { 
            const int pausedFontSize = 80;
            const char *pauseText = "Paused";
            const int pauseWidth = MeasureText(pauseText, pausedFontSize);
            const int pX = (screenWidth - pauseWidth) / 2;

            const Rectangle rec = {
                .x = pX - 10,
                .y = screenHeight / 2 - 30,
                .width = pauseWidth + 20,
                .height = 100,
            };

            DrawRectangleRec(rec, WHITE);
            DrawText(pauseText, pX, screenHeight / 2 - 20, pausedFontSize, BLACK);
            }  


        EndDrawing();
    }

    CloseWindow();
    return 0;
}

