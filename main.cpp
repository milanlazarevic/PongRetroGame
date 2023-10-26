#include <iostream>
#include "raylib.h"

Color Green = Color{38, 185, 154, 255};
Color DarkGreen = Color{20, 160, 133, 255};
Color LightGreen = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};
const int screenWidth = 1280;
const int screenHeight = 800;
int playerScore = 0;
int computerScore = 0;
class Ball
{
public:
    float x, y;
    int speedX, speedY;
    int radious;
    void Draw()
    {
        DrawCircle(x, y, radious, Yellow);
    }
    void Update()
    {
        x += speedX;
        y += speedY;

        // check for the collison with edges
        if (y + radious >= screenHeight || y - radious <= 0)
        {
            speedY *= -1;
        }
        if (x + radious >= screenWidth)
        {
            computerScore++;
            Reset();
        }
        if (x - radious <= 0)
        {
            playerScore++;
            Reset();
        }
    }
    void Reset()
    {
        x = screenWidth / 2;
        y = screenHeight / 2;

        int speedChoices[2] = {-1, 1};
        speedX *= speedChoices[GetRandomValue(0, 1)];
        speedY *= speedChoices[GetRandomValue(0, 1)];
    }
};

class Paddle
{
protected:
    void LimitMovement()
    {
        if (y <= 0)
        {
            y = 0;
        }
        if (y + height >= screenHeight)
        {
            y = screenHeight - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 30, 2, WHITE);
    }
    void Update()
    {
        if (IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }
        else if (IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        LimitMovement();
    }
};
class CPUPaddle : public Paddle
{
public:
    void Update(int ballY)
    {
        if (y + height / 2 > ballY)
        {
            y = y - speed;
        }
        if (y + height / 2 <= ballY)
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle player;
CPUPaddle computer;
int main(void)
{

    std::cout << "Pokrenuo sam" << std::endl;

    InitWindow(screenWidth, screenHeight, "Pong Game");
    SetTargetFPS(60);
    // BALL
    ball.radious = 20;
    ball.x = screenWidth / 2;
    ball.y = screenHeight / 2;
    ball.speedX = 10;
    ball.speedY = 10;

    // PLAYER
    player.width = 25;
    player.height = 120;
    player.x = screenWidth - player.width - 10;
    player.y = screenHeight / 2 - player.height / 2;
    player.speed = 10;

    // COMPUTER
    computer.height = 120;
    computer.width = 25;
    computer.x = 10;
    computer.y = screenHeight / 2 - computer.height / 2;
    computer.speed = 7;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        // Updating
        ball.Update();
        player.Update();
        computer.Update(ball.y);

        // Check for the collision
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radious, Rectangle{player.x, player.y, player.width, player.height}))
        {
            ball.speedX *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radious, Rectangle{computer.x, computer.y, computer.width, computer.height}))
        {
            ball.speedX *= -1;
        }

        // Draawing
        ClearBackground(DarkGreen);
        DrawRectangle(0, 0, screenWidth / 2, screenHeight, Green);
        DrawCircle(screenWidth / 2, screenHeight / 2, 150, LightGreen);
        ball.Draw();
        computer.Draw();
        player.Draw();
        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);
        DrawText(TextFormat("CPU: %d", computerScore), 30, 20, 50, WHITE);
        DrawText(TextFormat("Player: %d", playerScore), 3 * screenWidth / 4, 20, 50, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}