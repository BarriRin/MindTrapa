#pragma once
#include "DxLib.h"
#include <vector>
#include "ModelManager.h"

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
    RETRACTABLE_SPIKES = 9, // Выдвижные шипы (появляются/исчезают по таймеру) [Block 2]
    CRUMBLING = 10,    // Рассыпающаяся платформа (падает после касания) [Block 2]
    FAKE_SPIKES = 11,  // Фейковые шипы (выглядят опасно, но безопасны) [Block 2]
    GRAVITY_ZONE = 12,     // Зона с изменённой гравитацией [Block 3]
    PENDULUM_BLADE = 13,   // Качающийся топор-маятник (смертелен) [Block 3]
    LIGHT_PULSE_ZONE = 14  // Зона мигающего света (темнота/свет по таймеру) [Block 4]
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

    // Параметры для 3D моделей
    int modelHandle;    // Handle дублированной модели (-1 если используются примитивы)
    VECTOR rotation;    // Вращение модели
    VECTOR modelScale;  // Масштаб модели (отдельно от size для коллизий)
    bool useModel;      // Использовать модель или примитивы

    // Параметры для Block 3 механик
    float gravityMultiplier; // Множитель гравитации (для GRAVITY_ZONE): 0.5=low, 2.0=high, -1.0=reverse
    float swingAngle;        // Текущий угол качания маятника (для PENDULUM_BLADE)
    float swingSpeed;        // Скорость качания маятника
    float swingRange;        // Диапазон качания в радианах (например, PI/4 = ±45°)
    VECTOR pivotPoint;       // Точка подвеса маятника (для PENDULUM_BLADE)

    // Конструктор с параметрами по умолчанию
    Block(VECTOR p, VECTOR s, BlockType t, int link = 0, bool active = true,
          VECTOR moveStart = VGet(0, 0, 0), VECTOR moveEnd = VGet(0, 0, 0),
          float speed = 1.0f, float tim = 0.0f)
        : pos(p), size(s), type(t), linkId(link), isActive(active),
          originalPos((t == BlockType::PENDULUM_BLADE) ? p : moveStart),
          moveTarget(moveEnd), moveSpeed(speed),
          timer(t == BlockType::LIGHT_PULSE_ZONE && !active ? speed * 0.5f : tim),
          prevPos(p), modelHandle(-1), rotation(VGet(0, 0, 0)),
          modelScale(VGet(1, 1, 1)), useModel(false),
          gravityMultiplier(1.0f), swingAngle(0.0f), swingSpeed(1.0f),
          swingRange(DX_PI_F / 3.0f), pivotPoint(VGet(0, 0, 0)) {}
};

// Класс уровня
class Level {
private:
    std::vector<Block> blocks;
    VECTOR playerSpawn;
    int levelId;
    int blockId;        // ID блока уровней (для скайбокса)

    // Загрузка данных уровня
    void LoadLevelData(int id);
    void LoadBlock1(int id);
    void LoadBlock2(int id);
    void LoadBlock3(int id);
    void LoadBlock4(int id);

    // Инициализация моделей для блоков
    void InitializeModels();

public:
    Level(int id);
    ~Level();

    void Draw(bool debugMode) const;
    void DrawSkybox(VECTOR cameraPos) const;
    void Update(float deltaTime);

    // Проверки коллизий и триггеров
    bool CheckCollision(VECTOR playerPos, VECTOR playerSize, VECTOR& newPos, VECTOR& velocity, bool& onGround, VECTOR& platformVelocity);
    bool CheckWinTrigger(VECTOR playerPos, VECTOR playerSize) const;
    bool CheckDeadlyTrigger(VECTOR playerPos, VECTOR playerSize) const;
    bool CheckTeleportTrigger(VECTOR playerPos, VECTOR playerSize, VECTOR& teleportTarget) const;

    // Block 3 механики
    float CheckGravityZone(VECTOR playerPos, VECTOR playerSize) const; // Возвращает множитель гравитации

    // Block 4 механики
    bool IsAnyLightPulseZoneActive() const; // true = светлая фаза (хотя бы одна зона активна)

    const std::vector<Block>& GetBlocks() const { return blocks; }
    VECTOR GetPlayerSpawn() const { return playerSpawn; }
    size_t GetBlockCount() const { return blocks.size(); }

    // Получить блок по индексу (для активации кнопок)
    Block* GetBlockByIndex(int index);

    // Активация кнопки
    void ActivateButton(VECTOR playerPos, VECTOR playerSize, bool keyPressed);
};
