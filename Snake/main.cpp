#include <iostream>
#include <raylib.h>
#include "objects.h"

using namespace std;

static bool allowMove = false; // Определение переменной для хранения информации о разрешении движения


class Game // Определение класса игры
{
public:
    Snake snake = Snake(); // Определение экземпляра класса змейки
    Food food = Food(snake.body); // Определение экземпляра класса еды
    bool running = true; // Определение переменной для хранения информации о запущенности игры
    int score = 0; // Определение переменной для хранения информации о счете
    float speed = 0.2; // Определение переменной для хранения информации о скорости
    Sound eatSound; // Определение переменной для хранения информации о звуке поедания
    Sound wallSound; // Определение переменной для хранения информации о звуке столкновения

    Game() // Конструктор класса игры
    {
        InitAudioDevice(); // Инициализация аудиоустройства
        eatSound = LoadSound("Sounds/eat.mp3"); // Загрузка звука поедания
        wallSound = LoadSound("Sounds/wall.mp3"); // Загрузка звука столкновения
    }

    ~Game() // Деструктор класса игры
    {
        UnloadSound(eatSound); // Освобождение памяти, занимаемой звуком поедания
        UnloadSound(wallSound); // Освобождение памяти, занимаемой звуком столкновения
        CloseAudioDevice(); // Закрытие аудиоустройства
    }

    void Draw() // Функция рисования игры
    {
        food.Draw(); // Рисование еды
        snake.Draw(); // Рисование змейки
    }

    void Update() // Функция обновления игры
    {
        if (running)
        {
            snake.Update(); // Обновление статуса змейки
            CheckCollisionWithFood(); // Проверка на столкновение с едой
            CheckCollisionWithEdges(); // Проверка на столкновение с краями
            CheckCollisionWithTail(); // Проверка на столкновение с хвостом
        }
    }

    void CheckCollisionWithFood() // Функция проверки на столкновение с едой
    {
        if (Vector2Equals(snake.body[0], food.position)) // Если голова змейки пересекается с едой
        {
            food.position = food.GenerateRandomPos(snake.body); // Генерация новой позиции еды
            snake.addSegment = true; // Добавление нового сегмента в змею
            score++; // Увеличение счета
            speed = speed - 0.001; // Увеличение скорости
            PlaySound(eatSound); // Воспроизведение звука поедания
        }
    }

    void CheckCollisionWithEdges() // Функция проверки на столкновение с краями
    {
        if (snake.body[0].x == cellCount || snake.body[0].x == -1) // Если голова змейки выходит за правый или левый край
        {
            GameOver(); // Завершение игры
        }
        if (snake.body[0].y == cellCount || snake.body[0].y == -1) // Если голова змейки выходит за верхний или нижний край
        {
            GameOver(); // Завершение игры
        }
    }

    void GameOver() // Функция завершения игры
    {
        snake.Reset(); // Сброс змейки
        food.position = food.GenerateRandomPos(snake.body); // Генерация новой позиции еды
        running = false; // Остановка игры
        score = 0; // Сброс счета
        PlaySound(wallSound); // Воспроизведение звука столкновения
    }

    void CheckCollisionWithTail() // Функция проверки на столкновение с хвостом
    {
        deque<Vector2> headlessBody = snake.body; // Копия тела змейки без головы
        headlessBody.pop_front(); // Удаление головы из копии тела
        if (ElementInDeque(snake.body[0], headlessBody)) // Если голова змейки пересекается с хвостом
        {
            GameOver(); // Завершение игры
        }
    }
};


int main() // Главная функция
{
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Snake"); // Инициализация окна
    SetTargetFPS(360); // Установка целевой частоты кадров

    Game game = Game(); // Создание экземпляра класса игры

    while (WindowShouldClose() == false) // Пока окно не закрыто
    {
        BeginDrawing(); // Начало рисования

        if (EventTriggered(game.speed)) // Если наступило событие обновления
        {
            allowMove = true; // Разрешение движения
            game.Update(); // Обновление игры
        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1 && allowMove) // Если нажата клавиша вверх, голова змейки не движется вниз и разрешено движение
        {
            game.snake.direction = { 0, -1 }; // Движение головы змейки вверх
            game.running = true; // Запуск игры
            allowMove = false; // Запрет движения
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1 && allowMove) // Если нажата клавиша вниз, голова змейки не движется вверх и разрешено движение
        {
            game.snake.direction = { 0, 1 }; // Движение головы змейки вниз
            game.running = true; // Запуск игры
            allowMove = false; // Запрет движения
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1 && allowMove) // Если нажата клавиша влево, голова змейки не движется вправо и разрешено движение
        {
            game.snake.direction = { -1, 0 }; // Движение головы змейки влево
            game.running = true; // Запуск игры
            allowMove = false; // Запрет движения
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1 && allowMove) // Если нажата клавиша вправо, голова змейки не движется влево и разрешено движение
        {
            game.snake.direction = { 1, 0 }; // Движение головы змейки вправо
            game.running = true; // Запуск игры
            allowMove = false; // Запрет движения
        }

        ClearBackground(WHITE); // Очистка фона
        DrawRectangleLinesEx(Rectangle{ (float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10 }, 5, violet); // Рисование рамки
        DrawText("Snake", (offset + 1) * 5, 20, 40, BLACK); // Рисование текста "Snake"
        DrawText(TextFormat("Score: %i", game.score), offset - 5, offset + cellSize * cellCount + 10, 40, violet); // Рисование текста со счетом
        game.Draw(); // Рисование игры

        EndDrawing(); // Конец рисования
    }
    CloseWindow(); // Закрытие окна
    return 0; // Возврат 0
}