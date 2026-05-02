#include "raylib.h"

#define LIGHT_GREEN (Color){ 125, 204, 184,255 }
#define LIGHT_BLUE (Color){ 102, 178, 255,255 }
#define DARK_BLUE (Color){ 51, 153, 255,255 }
#define DARK_GREEN (Color){ 20, 160, 133,255 }
#define MY_GREEN (Color){ 28, 185 ,154,  255 }

int player1Score = 0;
int player2Score = 0;
bool active = false;                // Game not running
bool pause = false;                 // Pause control flag

int main(void)
{
    InitWindow(1280, 800, "raylib example - basic window");

    const int screenWidth = 1280;
    const int screenHeight = 800;

    int ballRadius = 20;
    int ballSpeedX = 0; 
    int ballSpeedY = 0; 
    int ballPositionX = GetScreenWidth() / 2 ;
    int ballPositionY = GetScreenHeight() / 2;
    
    int player1PositionY = 400;
    int player2PositionY = 400;

// setuo the player paddle sizes
    Rectangle player1Rect = { 20, player1PositionY, 8, 100 };
    Rectangle player2Rect = { screenWidth - 20, player2PositionY, 8, 100 };
// Sound
    InitAudioDevice();
    Sound ping = LoadSound("resources/ping.mp3");
    // init ball speed
    ballSpeedX = GetRandomValue(5,-4);
    ballSpeedY = GetRandomValue(4,-3);


    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        if (IsKeyPressed(KEY_P)) pause = !pause;

        BeginDrawing();
            ClearBackground(DARK_BLUE);
            // DrawText(TextFormat("X: %d Y: %d", ballPositionX, ballPositionY) , screenWidth / 2 , 150, 50, WHITE);

            DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, LIGHT_BLUE);
//          void DrawCircle(int centerX, int centerY, float radius, Color color);  // Draw a color-filled circle  
            DrawCircle(screenWidth / 2, screenHeight / 2, 250  / 2, BLUE);
            DrawCircleLines(screenWidth / 2, screenHeight / 2, 250  / 2, WHITE);
            DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), WHITE);

            DrawText(TextFormat("%i", player1Score), screenWidth / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", player2Score), 3 * screenWidth / 4, 20, 80, WHITE);

            if (active == false) DrawText("Press SPACE BAR to start", 490 , 50, 25, WHITE);

            //player 1
            if (IsKeyDown(KEY_UP)) {
                player2Rect.y -= 7;
                if (player2Rect.y <= 0 ) player2Rect.y = 0;
            }

            if (IsKeyDown(KEY_DOWN)) {
                player2Rect.y += 7;
                if (player2Rect.y >=  700  ) player2Rect.y = 700;
            }
            // Player 2
            if (IsKeyDown(KEY_A)) {
                player1Rect.y -= 7;
                if (player1Rect.y <= 0 ) player1Rect.y = 0;
            }

            if (IsKeyDown(KEY_Z)) {
                player1Rect.y += 7;
                if (player1Rect.y >=  700  ) player1Rect.y = 700;
            }

            if (IsKeyPressed(KEY_SPACE)) {
                active = true;
                ballPositionX += ballSpeedX;
                ballPositionY += ballSpeedY;
            }

            if (active == true && !pause) {
                ballPositionX += ballSpeedX;
                ballPositionY += ballSpeedY;


                    if (ballPositionX >= GetScreenWidth()) {
                        ballPositionX = GetScreenWidth() / 2;
                        ballPositionY = GetScreenHeight() / 2;
                        player1Score += 1;
                        active = false;
                    }

                    if (ballPositionX <= 0) {
                        ballPositionX = GetScreenWidth() / 2;
                        ballPositionY = GetScreenHeight() / 2;
                        player2Score += 1;
                        active = false;
                    } 
                // bool CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec); // Check collision between circle and rectangle
                // Check collision the ball and the paddles
                if (CheckCollisionCircleRec((Vector2) { ballPositionX , ballPositionY  }, 20.0f, player1Rect ) == true ) {
                    PlaySound(ping);
                    ballSpeedX = 4; 
                    ballPositionX += ballSpeedX;
                }

                if (CheckCollisionCircleRec((Vector2) { ballPositionX , ballPositionY  }, 20.0f, player2Rect ) == true ) {
                    PlaySound(ping);
                    ballSpeedX = -4; 
                    ballPositionX += ballSpeedX;
                }

            }
            if (ballPositionY <= 0 + ballRadius / 2) ballSpeedY = 6;
            if (ballPositionY >= GetScreenHeight() - ballRadius / 2) ballSpeedY = -6;

            DrawCircle( ballPositionX + ballSpeedX, ballPositionY + ballSpeedY,  ballRadius, YELLOW);
            // void DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color); // Draw rectangle with rounded edges
            DrawRectangleRounded(player1Rect, 2.0f, 5, WHITE);
            DrawRectangleRounded(player2Rect, 2.0f, 5, WHITE);

        EndDrawing();
    }

    CloseWindow();

    UnloadSound(ping);

    return 0;
}

