#include <raylib.h>
#include <stdio.h>

typedef enum
{
    LEFT,
    RIGHT,
} Direction;

void movePlayer(struct Rectangle *rec, int upKey, int downKey);
void renderMiddleLine();
void renderScore(int playerScore, int computerScore);
void moveBall(Vector2 *ball, Direction dir, float ballSpeed);
void updateScore(int *score);
void resetBall(Vector2 *ball);

int main()
{
    // window
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 450;

    // shapes
    const int RECTANGLE_WIDTH = 20;
    const int RECTANGLE_HEIGHT = 100;
    const int PLAYER_X = SCREEN_WIDTH - RECTANGLE_WIDTH;
    const int COMPUTER_X = 0;
    const int RECTANGLE_Y = SCREEN_HEIGHT / 2 - RECTANGLE_HEIGHT / 2;
    const Color RECTANGLE_COLOR = WHITE;
    const float CIRCLE_RADIUS = 7;
    const Color CIRCLE_COLOR = WHITE;

    struct Rectangle player = {PLAYER_X, RECTANGLE_Y, RECTANGLE_WIDTH, RECTANGLE_HEIGHT};
    struct Rectangle computer = {COMPUTER_X, RECTANGLE_Y, RECTANGLE_WIDTH, RECTANGLE_HEIGHT};
    struct Vector2 ball = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};

    int playerScore = 0, computerScore = 0;
    Direction currentDir = RIGHT;
    const float BALL_SPEED = 300.0f;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        movePlayer(&player, KEY_UP, KEY_DOWN);
        movePlayer(&computer, KEY_W, KEY_S);
        moveBall(&ball, currentDir, BALL_SPEED);

        bool hasCollided;

        if (currentDir == RIGHT)
        {
            hasCollided = CheckCollisionCircleRec(ball, CIRCLE_RADIUS, player);
            if (hasCollided)
            {
                currentDir = LEFT;
            }

            if (ball.x >= 800)
            {
                updateScore(&computerScore);
                resetBall(&ball);
            }
        }
        else
        {
            hasCollided = CheckCollisionCircleRec(ball, CIRCLE_RADIUS, computer);
            if (hasCollided)
            {
                currentDir = RIGHT;
            }

            if (ball.x <= 0)
            {
                updateScore(&playerScore);
                resetBall(&ball);
            }
        }

        BeginDrawing();

        // map setup
        ClearBackground(BLACK);
        renderMiddleLine();
        renderScore(playerScore, computerScore);

        DrawRectangleRec(computer, WHITE);
        DrawRectangleRec(player, WHITE);
        DrawCircleV(ball, CIRCLE_RADIUS, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void renderMiddleLine()
{
    int middleLineY = 0;
    while (middleLineY < 450)
    {
        DrawText("|", 400, middleLineY, 20, WHITE);
        middleLineY += 25;
    }
}

void renderScore(int playerScore, int computerScore)
{
    char playerText[32];
    char computerText[32];

    sprintf(playerText, "%d", playerScore);
    sprintf(computerText, "%d", computerScore);

    DrawText(playerText, 500, 50, 80, WHITE);
    DrawText(computerText, 270, 50, 80, WHITE);
}

void movePlayer(Rectangle *rec, int upKey, int downKey)
{
    if (IsKeyDown(upKey) && rec->y > 0)
        rec->y -= 5;
    else if (IsKeyDown(downKey) && rec->y < 350)
        rec->y += 5;
}

void moveBall(Vector2 *ball, Direction dir)
{

    float deltaTime = GetFrameTime();

    if (dir == RIGHT)
        ball->x += BALL_SPEED * deltaTime;
    else
        ball->x -= BALL_SPEED * deltaTime;
}

void updateScore(int *score) { (*score)++; }

void resetBall(Vector2 *ball)
{
    ball->x = 400;
    ball->y = 225;
}
