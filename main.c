#include <raylib.h>
#include <stdio.h>

typedef struct
{
    int screenWidth;
    int screenHeight;
    int fps;
    const char *name;
} GameConfig;

typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NONE
} Direction;

typedef struct
{
    Vector2 ballPos;
    float ballSpeed;
    Direction xDir;
    Direction yDir;
    float radius;
    Color color;
} Ball;

typedef struct
{
    Rectangle player;
    Rectangle computer;
    Ball ball;
    int playerScore;
    int computerScore;
} GameState;

typedef struct
{
    bool hitPlayer;
    bool hitComputer;
    bool hitWall;
    bool hitPlayerGoal;
    bool hitComputerGoal;
} CollisionResult;

typedef enum
{
    PLAYER,
    COMPUTER,
} Side;

typedef enum
{
    HORIZONTAL,
    VERTICAL,
} Component;

void movePlayer(struct Rectangle *rec, int upKey, int downKey);
void renderMiddleLine(int screenWidth, int screenHeight);
void renderScore(int playerScore, int computerScore);
void renderDirections(GameState state);
void moveBall(Ball *ball);
void updateBallTrajectory(CollisionResult result, GameState *state, int recHeight);
void updateScore(int *playerScore, int *computerScore, CollisionResult result);
void resetBall(GameState *state, CollisionResult result);
CollisionResult checkCollisions(Rectangle *player, Rectangle *computer, GameState *state, GameConfig gameConfig);
bool checkCollisionCircleWall(Ball ball, GameConfig gameConfig);
bool checkCollisionCircleGoal(Ball ball, GameConfig gameConfig, Side side);
void changeDirectionAxis(Ball *ball, Component axis);
void changeDirectionDir(Ball *ball, Direction dir);

int main()
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 450;
    const int FPS = 120;
    const char *NAME = "Pong";
    GameConfig gameConfig = {SCREEN_WIDTH, SCREEN_HEIGHT, FPS, NAME};

    InitWindow(gameConfig.screenWidth, gameConfig.screenHeight, gameConfig.name);
    SetTargetFPS(gameConfig.fps);

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
    Direction X_DIR = RIGHT;
    Direction Y_DIR = NONE;

    Ball ball = {ballPos, BALL_SPEED, X_DIR, Y_DIR, BALL_RADIUS, BALL_COLOR};
    GameState state = {player, computer, ball, playerScore, computerScore};

    while (!WindowShouldClose())
    {
        movePlayer(&player, KEY_UP, KEY_DOWN);
        movePlayer(&computer, KEY_W, KEY_S);
        moveBall(&state.ball);
        CollisionResult result = checkCollisions(&player, &computer, &state, gameConfig);
        updateBallTrajectory(result, &state, RECTANGLE_HEIGHT);
        updateScore(&state.playerScore, &state.computerScore, result);
        resetBall(&state, result);

        BeginDrawing();

        // map setup
        ClearBackground(BLACK);
        renderMiddleLine(SCREEN_WIDTH, SCREEN_HEIGHT);
        renderScore(state.playerScore, state.computerScore);
        renderDirections(state);

        DrawRectangleRec(computer, WHITE);
        DrawRectangleRec(player, WHITE);
        DrawCircleV(state.ball.ballPos, state.ball.radius, state.ball.color);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void renderDirections(GameState gameState)
{
    char xDir[64];
    char yDir[64];

    snprintf(xDir, sizeof(xDir), "%d\n", gameState.ball.xDir);
    snprintf(yDir, sizeof(yDir), "%d\n", gameState.ball.yDir);
    DrawText(xDir, 300, 200, 20, WHITE);
    DrawText(yDir, 360, 200, 20, WHITE);
}

void renderMiddleLine(int screenWidth, int screenHeight)
{
    int posY = 0;
    int fontSize = 20;

    while (posY < screenHeight)
    {
        DrawText("|", screenWidth / 2, posY, fontSize, WHITE);
        posY += 25;
    }
}

void renderScore(int playerScore, int computerScore)
{
    char playerText[32];
    char computerText[32];
    int playerScorePosX = 465, computerScorePosX = 300;
    int posY = 50;
    int fontSize = 80;

    sprintf(playerText, "%d", playerScore);
    sprintf(computerText, "%d", computerScore);

    DrawText(playerText, playerScorePosX, posY, fontSize, WHITE);
    DrawText(computerText, computerScorePosX, posY, fontSize, WHITE);
}

void movePlayer(Rectangle *rec, int upKey, int downKey)
{
    if (IsKeyDown(upKey) && rec->y > 0)
    {
        rec->y -= 5;
    }
    else if (IsKeyDown(downKey) && rec->y < 350)
    {
        rec->y += 5;
    }
}

void moveBall(Ball *ball)
{
    float deltaTime = GetFrameTime();
    float distance = ball->ballSpeed * deltaTime;

    if (ball->xDir == RIGHT)
    {
        ball->ballPos.x += distance;
    }
    else
    {
        ball->ballPos.x -= distance;
    }

    if (ball->yDir == NONE)
    {
        return;
    }

    if (ball->yDir == UP)
    {
        ball->ballPos.y -= distance;
    }
    else
    {
        ball->ballPos.y += distance;
    }
}

CollisionResult checkCollisions(Rectangle *player, Rectangle *computer, GameState *gameState, GameConfig gameConfig)
{
    CollisionResult result;

    result.hitWall = checkCollisionCircleWall(gameState->ball, gameConfig);
    result.hitPlayerGoal = checkCollisionCircleGoal(gameState->ball, gameConfig, PLAYER);
    result.hitComputerGoal = checkCollisionCircleGoal(gameState->ball, gameConfig, COMPUTER);
    result.hitPlayer = CheckCollisionCircleRec(gameState->ball.ballPos, gameState->ball.radius, *player);
    result.hitComputer = CheckCollisionCircleRec(gameState->ball.ballPos, gameState->ball.radius, *computer);

    return result;
}

void changeDirectionAxis(Ball *ball, Component axis)
{
    if (axis == HORIZONTAL)
    {
        if (ball->xDir == RIGHT)
        {
            ball->xDir = LEFT;
        }
        else
        {
            ball->xDir = RIGHT;
        }
    }
    else
    {
        if (ball->yDir == UP)
        {
            ball->yDir = DOWN;
        }
        else
        {
            ball->yDir = UP;
        }
    }
}

void changeDirectionDir(Ball *ball, Direction dir)
{
    if (dir == UP)
    {
        ball->yDir = UP;
    }
    else
    {
        ball->yDir = DOWN;
    }
}

bool checkCollisionCircleWall(Ball ball, GameConfig gameConfig)
{
    return (ball.ballPos.y <= 0 || ball.ballPos.y >= 450);
}

bool checkCollisionCircleGoal(Ball ball, GameConfig gameConfig, Side side)
{
    if (side == PLAYER)
    {
        return ball.ballPos.x >= 800;
    }
    else
    {
        return ball.ballPos.x <= 0;
    }
}

void updateBallTrajectory(CollisionResult result, GameState *state, int recHeight)
{
    if (result.hitWall)
    {
        changeDirectionAxis(&state->ball, VERTICAL);
    }
    else if (result.hitPlayer || result.hitComputer)
    {
        if (state->ball.xDir == RIGHT)
        {
            if (state->ball.ballPos.y >= state->player.y &&
                state->ball.ballPos.y < state->player.y + (float)recHeight / 2 - 5)
            {
                changeDirectionDir(&state->ball, UP);
            }
            else if (state->ball.ballPos.y > state->player.y + (float)recHeight / 2 + 5 &&
                     state->ball.ballPos.y <= state->player.y + recHeight)
            {
                changeDirectionDir(&state->ball, DOWN);
            }
        }
        else
        {
            if (state->ball.ballPos.y >= state->computer.y &&
                state->ball.ballPos.y < state->computer.y + ((float)recHeight / 2 + 5))
            {
                changeDirectionDir(&state->ball, UP);
            }
            else if (state->ball.ballPos.y > state->computer.y + ((float)recHeight / 2 + 5) &&
                     state->ball.ballPos.y <= state->computer.y + recHeight)
            {
                changeDirectionDir(&state->ball, DOWN);
            }
        }

        changeDirectionAxis(&state->ball, HORIZONTAL);
    }
}

void updateScore(int *playerScore, int *computerScore, CollisionResult result)
{
    if (result.hitPlayerGoal)
    {
        (*computerScore)++;
    }
    else if (result.hitComputerGoal)
    {
        (*playerScore)++;
    }
}

void resetBall(GameState *state, CollisionResult result)
{
    if (result.hitComputerGoal || result.hitPlayerGoal)
    {
        state->ball.ballPos.x = 400;
        state->ball.ballPos.y = 225;
        state->ball.yDir = NONE;
    }
}
