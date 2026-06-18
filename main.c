#include <raylib.h>
#include <stdio.h>

typedef enum
{
    LEFT,
    RIGHT,
} Direction;

typedef struct
{
    Vector2 ballPos;
    float ballSpeed;
    Direction currentDir;
    float radius;
    Color color;
} Ball;

typedef struct
{
    Ball ball;
    int playerScore;
    int computerScore;
} GameState;

void movePlayer(struct Rectangle *rec, int upKey, int downKey);
void renderMiddleLine();
void renderScore(int playerScore, int computerScore);
void moveBall(Ball *ball);
void updateScore(int *score);
void resetBall(Vector2 *ball);
void checkCollisions(Rectangle *player, Rectangle *computer, GameState *game);

int main()
{
    // window
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 450;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");
    SetTargetFPS(120);

    // shapes
    const int RECTANGLE_WIDTH = 20;
    const int RECTANGLE_HEIGHT = 100;
    const int PLAYER_X = SCREEN_WIDTH - RECTANGLE_WIDTH;
    const int COMPUTER_X = 0;
    const int RECTANGLE_Y = SCREEN_HEIGHT / 2 - RECTANGLE_HEIGHT / 2;
    const Color RECTANGLE_COLOR = WHITE;
    const float BALL_RADIUS = 7;
    const Color BALL_COLOR = WHITE;
    const float BALL_SPEED = 300.0f;

    struct Rectangle player = {PLAYER_X, RECTANGLE_Y, RECTANGLE_WIDTH, RECTANGLE_HEIGHT};
    struct Rectangle computer = {COMPUTER_X, RECTANGLE_Y, RECTANGLE_WIDTH, RECTANGLE_HEIGHT};
    struct Vector2 ballPos = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};

    int playerScore = 0, computerScore = 0;
    Direction currentDir = RIGHT;

    Ball ball = {ballPos, BALL_SPEED, currentDir, BALL_RADIUS, BALL_COLOR};
    GameState game = {ball, playerScore, computerScore};

    while (!WindowShouldClose())
    {
        movePlayer(&player, KEY_UP, KEY_DOWN);
        movePlayer(&computer, KEY_W, KEY_S);
        moveBall(&ball);
        checkCollisions(&player, &computer, &game);

        BeginDrawing();

        // map setup
        ClearBackground(BLACK);
        renderMiddleLine();
        renderScore(playerScore, computerScore);

        DrawRectangleRec(computer, WHITE);
        DrawRectangleRec(player, WHITE);
        DrawCircleV(ball.ballPos, ball.radius, ball.color);

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

void moveBall(Ball *ball)
{
    float deltaTime = GetFrameTime();

    if (ball->currentDir == RIGHT)
    {
        ball->ballPos.x += ball->ballSpeed * deltaTime;
        ball->ballPos.y += ball->ballSpeed * deltaTime;
    }
    else
    {
        ball->ballPos.x -= ball->ballSpeed * deltaTime;
        ball->ballPos.y -= ball->ballSpeed * deltaTime;
    }
}

void checkCollisions(Rectangle *player, Rectangle *computer, GameState *game)
{
    bool ballHitRec;
    bool ballHitWall;

    if (game->ball.currentDir == RIGHT)
    {
        ballHitRec = CheckCollisionCircleRec(game->ball.ballPos, game->ball.radius, *player);
        if (ballHitRec)
        {
            game->ball.currentDir = LEFT;
        }

        if (game->ball.ballPos.x >= 800)
        {
            updateScore(&game->computerScore);
            resetBall(&game->ball.ballPos);
        }
    }
    else
    {
        ballHitRec = CheckCollisionCircleRec(game->ball.ballPos, game->ball.radius, *computer);
        if (ballHitRec)
        {
            game->ball.currentDir = RIGHT;
        }

        if (game->ball.ballPos.x <= 0)
        {
            updateScore(&game->playerScore);
            resetBall(&game->ball.ballPos);
        }
    }
}

void updateScore(int *score) { (*score)++; }

void resetBall(Vector2 *ball)
{
    ball->x = 400;
    ball->y = 225;
}
