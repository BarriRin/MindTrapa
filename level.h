#pragma once
#include "DxLib.h"
#include <vector>

// Типы блоков
enum class BlockType {
    PLATFORM = 0,      // Обычная платформа
    TRIGGER = 1,       // Триггер перехода на следующий уровень
    INVISIBLE_WALL = 2,// Невидимая стена (коллизия есть, но не видна)
    FAKE_PLATFORM = 3, // Фальшивая платформа (видна, но коллизии нет)
    SPIKES = 4,        // Шипы (убивают игрока)
    DISAPPEARING = 5,  // Исчезающая платформа (через N секунд)
    MOVING = 6,        // Движущаяся платформа
    BUTTON = 7,        // Кнопка (активирует что-то)
    TELEPORT = 8,      // Телепорт
    RETRACTABLE_SPIKES = 9, // Выдвижные шипы (появляются/исчезают по таймеру)
    CRUMBLING = 10,    // Рассыпающаяся платформа (падает после касания)
    FAKE_SPIKES = 11   // Фейковые шипы (выглядят опасно, но безопасны)
};

// Структура блока
struct Block {
    VECTOR pos;         // Текущая позиция
    VECTOR size;        // Размер блока
    BlockType type;     // Тип блока

    // Дополнительные параметры для механик
    VECTOR originalPos; // Начальная позиция (для движущихся платформ)
    VECTOR prevPos;     // Предыдущая позиция (для вычисления скорости)
    float timer;        // Для исчезающих блоков, таймеров
    bool isActive;      // Для кнопок, активных элементов
    VECTOR moveTarget;  // Для движущихся платформ (конечная точка)
    int linkId;         // Для связанных элементов (кнопка↔дверь)
    float moveSpeed;    // Скорость движения платформы

    // Конструктор с параметрами по умолчанию
    Block(VECTOR p, VECTOR s, BlockType t, int link = 0, bool active = true,
          VECTOR moveStart = VGet(0, 0, 0), VECTOR moveEnd = VGet(0, 0, 0),
          float speed = 1.0f, float tim = 0.0f)
        : pos(p), size(s), type(t), linkId(link), isActive(active),
          originalPos(moveStart), moveTarget(moveEnd), moveSpeed(speed), timer(tim),
          prevPos(p) {}
};

// Класс уровня
class Level {
private:
    std::vector<Block> blocks;
    VECTOR playerSpawn;
    int levelId;

    // Загрузка данных уровня из switch-case
    void LoadLevelData(int id);

public:
    Level(int id);
    ~Level();

    void Draw() const;
    void Update(float deltaTime);

    // Проверки коллизий и триггеров
    bool CheckCollision(VECTOR playerPos, VECTOR playerSize, VECTOR& newPos, VECTOR& velocity, bool& onGround, VECTOR& platformVelocity);
    bool CheckWinTrigger(VECTOR playerPos, VECTOR playerSize) const;
    bool CheckDeadlyTrigger(VECTOR playerPos, VECTOR playerSize) const;
    bool CheckTeleportTrigger(VECTOR playerPos, VECTOR playerSize, VECTOR& teleportTarget) const;

    const std::vector<Block>& GetBlocks() const { return blocks; }
    VECTOR GetPlayerSpawn() const { return playerSpawn; }
    size_t GetBlockCount() const { return blocks.size(); }

    // Получить блок по индексу (для активации кнопок)
    Block* GetBlockByIndex(int index);

    // Активация кнопки
    void ActivateButton(VECTOR playerPos, VECTOR playerSize, bool keyPressed);
};
