#include <iostream>
#include <raylib.h>
#include "objects.h"

using namespace std;

static bool allowMove = false; // ����������� ���������� ��� �������� ���������� � ���������� ��������


class Game // ����������� ������ ����
{
public:
    Snake snake = Snake(); // ����������� ���������� ������ ������
    Food food = Food(snake.body); // ����������� ���������� ������ ���
    bool running = true; // ����������� ���������� ��� �������� ���������� � ������������ ����
    int score = 0; // ����������� ���������� ��� �������� ���������� � �����
    float speed = 0.2; // ����������� ���������� ��� �������� ���������� � ��������
    Sound eatSound; // ����������� ���������� ��� �������� ���������� � ����� ��������
    Sound wallSound; // ����������� ���������� ��� �������� ���������� � ����� ������������

    Game() // ����������� ������ ����
    {
        InitAudioDevice(); // ������������� ���������������
        eatSound = LoadSound("Sounds/eat.mp3"); // �������� ����� ��������
        wallSound = LoadSound("Sounds/wall.mp3"); // �������� ����� ������������
    }

    ~Game() // ���������� ������ ����
    {
        UnloadSound(eatSound); // ������������ ������, ���������� ������ ��������
        UnloadSound(wallSound); // ������������ ������, ���������� ������ ������������
        CloseAudioDevice(); // �������� ���������������
    }

    void Draw() // ������� ��������� ����
    {
        food.Draw(); // ��������� ���
        snake.Draw(); // ��������� ������
    }

    void Update() // ������� ���������� ����
    {
        if (running)
        {
            snake.Update(); // ���������� ������� ������
            CheckCollisionWithFood(); // �������� �� ������������ � ����
            CheckCollisionWithEdges(); // �������� �� ������������ � ������
            CheckCollisionWithTail(); // �������� �� ������������ � �������
        }
    }

    void CheckCollisionWithFood() // ������� �������� �� ������������ � ����
    {
        if (Vector2Equals(snake.body[0], food.position)) // ���� ������ ������ ������������ � ����
        {
            food.position = food.GenerateRandomPos(snake.body); // ��������� ����� ������� ���
            snake.addSegment = true; // ���������� ������ �������� � ����
            score++; // ���������� �����
            speed = speed - 0.001; // ���������� ��������
            PlaySound(eatSound); // ��������������� ����� ��������
        }
    }

    void CheckCollisionWithEdges() // ������� �������� �� ������������ � ������
    {
        if (snake.body[0].x == cellCount || snake.body[0].x == -1) // ���� ������ ������ ������� �� ������ ��� ����� ����
        {
            GameOver(); // ���������� ����
        }
        if (snake.body[0].y == cellCount || snake.body[0].y == -1) // ���� ������ ������ ������� �� ������� ��� ������ ����
        {
            GameOver(); // ���������� ����
        }
    }

    void GameOver() // ������� ���������� ����
    {
        snake.Reset(); // ����� ������
        food.position = food.GenerateRandomPos(snake.body); // ��������� ����� ������� ���
        running = false; // ��������� ����
        score = 0; // ����� �����
        PlaySound(wallSound); // ��������������� ����� ������������
    }

    void CheckCollisionWithTail() // ������� �������� �� ������������ � �������
    {
        deque<Vector2> headlessBody = snake.body; // ����� ���� ������ ��� ������
        headlessBody.pop_front(); // �������� ������ �� ����� ����
        if (ElementInDeque(snake.body[0], headlessBody)) // ���� ������ ������ ������������ � �������
        {
            GameOver(); // ���������� ����
        }
    }
};


int main() // ������� �������
{
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Snake"); // ������������� ����
    SetTargetFPS(360); // ��������� ������� ������� ������

    Game game = Game(); // �������� ���������� ������ ����

    while (WindowShouldClose() == false) // ���� ���� �� �������
    {
        BeginDrawing(); // ������ ���������

        if (EventTriggered(game.speed)) // ���� ��������� ������� ����������
        {
            allowMove = true; // ���������� ��������
            game.Update(); // ���������� ����
        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1 && allowMove) // ���� ������ ������� �����, ������ ������ �� �������� ���� � ��������� ��������
        {
            game.snake.direction = { 0, -1 }; // �������� ������ ������ �����
            game.running = true; // ������ ����
            allowMove = false; // ������ ��������
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1 && allowMove) // ���� ������ ������� ����, ������ ������ �� �������� ����� � ��������� ��������
        {
            game.snake.direction = { 0, 1 }; // �������� ������ ������ ����
            game.running = true; // ������ ����
            allowMove = false; // ������ ��������
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1 && allowMove) // ���� ������ ������� �����, ������ ������ �� �������� ������ � ��������� ��������
        {
            game.snake.direction = { -1, 0 }; // �������� ������ ������ �����
            game.running = true; // ������ ����
            allowMove = false; // ������ ��������
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1 && allowMove) // ���� ������ ������� ������, ������ ������ �� �������� ����� � ��������� ��������
        {
            game.snake.direction = { 1, 0 }; // �������� ������ ������ ������
            game.running = true; // ������ ����
            allowMove = false; // ������ ��������
        }

        ClearBackground(WHITE); // ������� ����
        DrawRectangleLinesEx(Rectangle{ (float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10 }, 5, violet); // ��������� �����
        DrawText("Snake", (offset + 1) * 5, 20, 40, BLACK); // ��������� ������ "Snake"
        DrawText(TextFormat("Score: %i", game.score), offset - 5, offset + cellSize * cellCount + 10, 40, violet); // ��������� ������ �� ������
        game.Draw(); // ��������� ����

        EndDrawing(); // ����� ���������
    }
    CloseWindow(); // �������� ����
    return 0; // ������� 0
}