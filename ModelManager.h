#pragma once
#include "DxLib.h"
#include <string>
#include <map>

// Типы моделей для блоков
enum class ModelID {
    PLATFORM = 0,
    TRIGGER = 1,
    INVISIBLE_WALL = 2,
    FAKE_PLATFORM = 3,
    SPIKES = 4,
    DISAPPEARING = 5,
    MOVING = 6,
    BUTTON = 7,
    TELEPORT = 8,
    RETRACTABLE_SPIKES = 9,
    CRUMBLING = 10,
    FAKE_SPIKES = 11,
    GRAVITY_ZONE = 12,     // Block 3
    PENDULUM_BLADE = 13,   // Block 3
    PLAYER = 14,
    COUNT // Количество типов моделей
};

// Структура для хранения модели и её параметров
struct ModelData {
    int handle;              // DxLib handle модели
    float scale;             // Масштаб модели
    VECTOR rotationOffset;   // Смещение ротации (если нужно)
    bool hasAnimation;       // Есть ли анимация
    int animIndex;           // Индекс прикрепленной анимации

    ModelData() : handle(-1), scale(1.0f), rotationOffset(VGet(0, 0, 0)),
                  hasAnimation(false), animIndex(-1) {}
};

// Структура для skybox
struct SkyboxData {
    int handle;              // Handle модели skybox
    float scale;             // Масштаб
    VECTOR rotation;         // Вращение skybox

    SkyboxData() : handle(-1), scale(1000.0f), rotation(VGet(0, 0, 0)) {}
};

// Менеджер моделей
class ModelManager {
private:
    // Карта моделей: ModelID -> ModelData
    std::map<ModelID, ModelData> models;

    // Карта скайбоксов: level block ID -> SkyboxData
    std::map<int, SkyboxData> skyboxes;

    // Пути к папкам с ресурсами
    std::string modelsPath;
    std::string skyboxesPath;

    // Приватный конструктор для singleton
    ModelManager();

    // Запретить копирование
    ModelManager(const ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;

public:
    // Получить instance singleton
    static ModelManager& GetInstance();

    // Деструктор
    ~ModelManager();

    // Инициализация - загрузка всех моделей
    bool Initialize();

    // Очистка всех ресурсов
    void Cleanup();

    // Загрузка конкретной модели
    bool LoadModel(ModelID id, const std::string& filename, float scale = 1.0f);

    // Загрузка скайбокса для блока уровней
    bool LoadSkybox(int blockId, const std::string& filename, float scale = 1000.0f);

    // Получить handle модели
    int GetModelHandle(ModelID id) const;

    // Получить данные модели
    const ModelData* GetModelData(ModelID id) const;

    // Получить handle скайбокса для блока
    int GetSkyboxHandle(int blockId) const;

    // Получить данные скайбокса
    const SkyboxData* GetSkyboxData(int blockId) const;

    // Дублировать модель (для создания нескольких экземпляров)
    int DuplicateModel(ModelID id);

    // Отрисовка модели в заданной позиции
    void DrawModel(ModelID id, VECTOR pos, VECTOR scale, VECTOR rotation = VGet(0, 0, 0));

    // Отрисовка скайбокса
    void DrawSkybox(int blockId, VECTOR cameraPos);

    // Проверка загружена ли модель
    bool IsModelLoaded(ModelID id) const;

    // Проверка загружен ли скайбокс
    bool IsSkyboxLoaded(int blockId) const;

    // Установить путь к папке с моделями
    void SetModelsPath(const std::string& path) { modelsPath = path; }
    void SetSkyboxesPath(const std::string& path) { skyboxesPath = path; }
};
