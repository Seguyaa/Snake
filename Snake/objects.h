#pragma once
#include <raylib.h> // ����������� ���������� raylib ��� ������ � ��������, ������ � ������
#include <deque> // ����������� ���������� deque ��� ������ � ������������� ���������
#include <raymath.h> // ����������� ���������� raymath ��� ������ � ��������������� ���������

Color violet = { 199, 21, 133, 255 }; // ����������� ����� ����������� ��� ������������� � ����

using namespace std; // ������������� ������������ ���� std ��� ��������� ��������� ����

int cellSize = 30, cellCount = 25, offset = 75; // ����������� ������� ������, ���������� ������ � �������� �� ���� ����

double lastUpdateTime = 0; // ����������� ���������� ��� �������� ������� ���������� ����������

bool ElementInDeque(Vector2 element, deque<Vector2> deque) // ������� �������� ������� �������� � ������������ �������
{
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element)) // ��������� �������� � ������ ��������� �������
        {
            return true; // ������� true, ���� ������� ������
        }
    }
    return false; // ������� false, ���� ������� �� ������
}

bool EventTriggered(double interval) // ������� �������� ����������� ������� ����� �������� ��������
{
    double currentTime = GetTime(); // ��������� �������� �������
    if (currentTime - lastUpdateTime >= interval) // ��������� ������� ����� ������� � ���������� �������� � �������� ����������
    {
        lastUpdateTime = currentTime; // ���������� ���������� ���������� �������
        return true; // ������� true, ���� ������� ���������
    }
    return false; // ������� false, ���� ������� ��� �� ���������
}

class Snake // ����������� ������ ������
{
public:
    deque<Vector2> body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} }; // ����������� ���� ������ � ���� ������������ ������� �� ��������
    Vector2 direction = { 1, 0 }; // ����������� ����������� �������� ������
    bool addSegment = false; // ����������� ���������� ��� �������� ���������� � ������������� ���������� ��������

    void Draw() // ������� ��������� ������
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x; // ��������� ���������� x ��������
            float y = body[i].y; // ��������� ���������� y ��������
            Rectangle segment = Rectangle{ offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize }; // ����������� �������������� ��������
            DrawRectangleRounded(segment, 0.5, 6, violet); // ��������� �������������� � ������������� ������ � ���������� ������
        }
    }

    void Update() // ������� ���������� ������
    {
        body.push_front(Vector2Add(body[0], direction)); // ���������� ������ �������� � ������ �������
        if (addSegment == true)
        {
            addSegment = false; // ����� ����� ���������� ��������
        }
        else
        {
            body.pop_back(); // �������� ���������� �������� �� �������
        }
    }

    void Reset() // ������� ������ ������
    {
        body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} }; // ������� ���� ������ � �������� ���������
        direction = { 1, 0 }; // ������� ����������� �������� ������ � �������� ���������
    }
};

class Food // ����������� ������ ���
{

public:
    Vector2 position; // ����������� ���������� ��� �������� ������� ���
    Texture2D texture; // ����������� ���������� ��� �������� �������� ���

    Food(deque<Vector2> snakeBody) // ����������� ������ ���
    {
        Image image = LoadImage("Graphics/food.png"); // �������� ����������� ���
        texture = LoadTextureFromImage(image); // �������� �������� �� �����������
        UnloadImage(image); // ������������ ������, ���������� ������������
        position = GenerateRandomPos(snakeBody); // ��������� ��������� ������� ���
    }

    ~Food() // ���������� ������ ���
    {
        UnloadTexture(texture); // ������������ ������, ���������� ���������
    }

    void Draw() // ������� ��������� ���
    {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE); // ��������� �������� ��� � �������� �������
    }

    Vector2 GenerateRandomCell() // ������� ��������� ��������� ������
    {
        float x = GetRandomValue(0, cellCount - 1); // ��������� ���������� �������� ���������� x
        float y = GetRandomValue(0, cellCount - 1); // ��������� ���������� �������� ���������� y
        return Vector2{ x, y }; // ������� ������� � ������������ x � y
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody) // ������� ��������� ��������� ������� ���
    {
        Vector2 position = GenerateRandomCell(); // ��������� ��������� ������
        while (ElementInDeque(position, snakeBody)) // ���� ������ ������������ � ����� ������
        {
            position = GenerateRandomCell(); // ��������� ����� ������
        }
        return position; // ������� ������� � ������������ x � y
    }
};