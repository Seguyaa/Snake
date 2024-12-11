#pragma once
#include <raylib.h> // Подключение библиотеки raylib для работы с графикой, звуком и вводом
#include <deque> // Подключение библиотеки deque для работы с двусторонними очередями
#include <raymath.h> // Подключение библиотеки raymath для работы с математическими функциями

Color violet = { 199, 21, 133, 255 }; // Определение цвета фиолетового для использования в игре

using namespace std; // Использование пространства имен std для упрощения написания кода

int cellSize = 30, cellCount = 25, offset = 75; // Определение размера клетки, количества клеток и смещения от края окна

double lastUpdateTime = 0; // Определение переменной для хранения времени последнего обновления

bool ElementInDeque(Vector2 element, deque<Vector2> deque) // Функция проверки наличия элемента в двусторонней очереди
{
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element)) // Сравнение элемента с каждым элементом очереди
        {
            return true; // Возврат true, если элемент найден
        }
    }
    return false; // Возврат false, если элемент не найден
}

bool EventTriggered(double interval) // Функция проверки наступления события через заданный интервал
{
    double currentTime = GetTime(); // Получение текущего времени
    if (currentTime - lastUpdateTime >= interval) // Сравнение разницы между текущим и предыдущим временем с заданным интервалом
    {
        lastUpdateTime = currentTime; // Обновление переменной последнего времени
        return true; // Возврат true, если событие наступило
    }
    return false; // Возврат false, если событие еще не наступило
}

class Snake // Определение класса змейки
{
public:
    deque<Vector2> body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} }; // Определение тела змейки в виде двусторонней очереди из векторов
    Vector2 direction = { 1, 0 }; // Определение направления движения змейки
    bool addSegment = false; // Определение переменной для хранения информации о необходимости добавления сегмента

    void Draw() // Функция рисования змейки
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x; // Получение координаты x сегмента
            float y = body[i].y; // Получение координаты y сегмента
            Rectangle segment = Rectangle{ offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize }; // Определение прямоугольника сегмента
            DrawRectangleRounded(segment, 0.5, 6, violet); // Рисование прямоугольника с закругленными углами и фиолетовым цветом
        }
    }

    void Update() // Функция обновления змейки
    {
        body.push_front(Vector2Add(body[0], direction)); // Добавление нового сегмента в начало очереди
        if (addSegment == true)
        {
            addSegment = false; // Сброс флага добавления сегмента
        }
        else
        {
            body.pop_back(); // Удаление последнего сегмента из очереди
        }
    }

    void Reset() // Функция сброса змейки
    {
        body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} }; // Возврат тела змейки в исходное состояние
        direction = { 1, 0 }; // Возврат направления движения змейки в исходное состояние
    }
};

class Food // Определение класса еды
{

public:
    Vector2 position; // Определение переменной для хранения позиции еды
    Texture2D texture; // Определение переменной для хранения текстуры еды

    Food(deque<Vector2> snakeBody) // Конструктор класса еды
    {
        Image image = LoadImage("Graphics/food.png"); // Загрузка изображения еды
        texture = LoadTextureFromImage(image); // Создание текстуры из изображения
        UnloadImage(image); // Освобождение памяти, занимаемой изображением
        position = GenerateRandomPos(snakeBody); // Получение случайной позиции еды
    }

    ~Food() // Деструктор класса еды
    {
        UnloadTexture(texture); // Освобождение памяти, занимаемой текстурой
    }

    void Draw() // Функция рисования еды
    {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE); // Рисование текстуры еды в заданной позиции
    }

    Vector2 GenerateRandomCell() // Функция генерации случайной клетки
    {
        float x = GetRandomValue(0, cellCount - 1); // Получение случайного значения координаты x
        float y = GetRandomValue(0, cellCount - 1); // Получение случайного значения координаты y
        return Vector2{ x, y }; // Возврат вектора с координатами x и y
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody) // Функция генерации случайной позиции еды
    {
        Vector2 position = GenerateRandomCell(); // Получение случайной клетки
        while (ElementInDeque(position, snakeBody)) // Пока клетка пересекается с телом змейки
        {
            position = GenerateRandomCell(); // Генерация новой клетки
        }
        return position; // Возврат вектора с координатами x и y
    }
};