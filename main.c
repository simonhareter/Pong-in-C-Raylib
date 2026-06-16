#include <raylib.h>
#include <stdio.h>

void move(struct Rectangle *rec, int upKey, int downKey);
void renderMiddleLine();
void renderScore(int playerScore, int computerScore);

int main()
{
    // window
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 450;

    // shapes
    const int RECTANGLE_WIDTH = 20;
    const int RECTANGLE_HEIGHT = 100;
    const Color RECTANGLE_COLOR = WHITE;
    const float CIRCLE_RADIUS = 7;
    const Color CIRCLE_COLOR = WHITE;

    struct Rectangle player = {780, 175, RECTANGLE_WIDTH, RECTANGLE_HEIGHT};
    struct Rectangle computer = {0, 175, RECTANGLE_WIDTH, RECTANGLE_HEIGHT};
    struct Vector2 circle = {400, 225};

    int playerScore = 0, computerScore = 0;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        move(&player, KEY_UP, KEY_DOWN);
        move(&computer, KEY_W, KEY_S);

        BeginDrawing();

        // map setup
        ClearBackground(BLACK);
        renderMiddleLine();
        renderScore(playerScore, computerScore);

        DrawRectangleRec(computer, WHITE);
        DrawRectangleRec(player, WHITE);
        DrawCircleV(circle, CIRCLE_RADIUS, WHITE);

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

void move(Rectangle *rec, int upKey, int downKey)
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
