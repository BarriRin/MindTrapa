# Интеграция 3D моделей и скайбоксов

## Обзор

MindTrapa теперь поддерживает загрузку 3D моделей для блоков и скайбоксов для создания уникальных планетарных окружений.

---

## Поддерживаемые форматы

### Рекомендуемые форматы:
- **MV1** - нативный формат DxLib (лучшая производительность)
- **MQO** - Metasequoia
- **PMD/PMX** - MikuMikuDance (с анимациями)
- **X** - DirectX .x файлы

### Конвертация в MV1
Используйте официальные утилиты DxLib для конвертации моделей в формат MV1.

---

## Структура папок

```
MindTrapa/
├── models/           # 3D модели для блоков
│   ├── platform.mv1
│   ├── trigger.mv1
│   ├── spikes.mv1
│   ├── button.mv1
│   ├── teleport.mv1
│   └── player.mv1
├── skyboxes/         # Скайбоксы для блоков уровней
│   ├── block0_mars.mv1      # Блок 1 (уровни 1-10)
│   ├── block1_moon.mv1      # Блок 2 (уровни 11-20)
│   ├── block2_jupiter.mv1   # Блок 3 (уровни 21-30)
│   └── ...
```

---

## Инициализация

### В main.cpp (при запуске игры):

```cpp
#include "ModelManager.h"

int WINAPI WinMain(...) {
    // ... инициализация DxLib ...

    // Получить ModelManager singleton
    ModelManager& modelMgr = ModelManager::GetInstance();

    // Инициализировать систему
    modelMgr.Initialize();

    // Загрузить модели для блоков
    modelMgr.LoadModel(ModelID::PLATFORM, "platform.mv1", 1.0f);
    modelMgr.LoadModel(ModelID::TRIGGER, "trigger.mv1", 1.0f);
    modelMgr.LoadModel(ModelID::SPIKES, "spikes.mv1", 1.0f);
    modelMgr.LoadModel(ModelID::BUTTON, "button.mv1", 1.0f);
    modelMgr.LoadModel(ModelID::TELEPORT, "teleport.mv1", 1.0f);
    modelMgr.LoadModel(ModelID::MOVING, "moving_platform.mv1", 1.0f);
    modelMgr.LoadModel(ModelID::PLAYER, "player.mv1", 1.0f);

    // Загрузить скайбоксы для блоков
    modelMgr.LoadSkybox(0, "block0_mars.mv1", 1000.0f);      // Блок 1
    modelMgr.LoadSkybox(1, "block1_moon.mv1", 1000.0f);      // Блок 2
    modelMgr.LoadSkybox(2, "block2_jupiter.mv1", 1000.0f);   // Блок 3
    // ... и так далее для всех блоков

    // ... остальной код игры ...

    // При выходе
    modelMgr.Cleanup();
    DxLib_End();
    return 0;
}
```

---

## Использование в игре

### Автоматическая загрузка моделей

Класс `Level` автоматически:
1. Определяет, какие модели загружены
2. Создает дубликаты моделей для каждого блока
3. Использует модели при отрисовке (если доступны)
4. Fallback на примитивы (если модель не загружена)

### Отрисовка скайбокса

В main.cpp (в игровом цикле):

```cpp
// Внутри игрового цикла
if (currentLevel) {
    // Получить позицию камеры
    VECTOR cameraPos = GetCameraPosition();

    // Отрисовать скайбокс (центрирован на камере)
    currentLevel->DrawSkybox(cameraPos);

    // Отрисовать уровень
    currentLevel->Draw();

    // Отрисовать игрока и UI
    // ...
}
```

---

## ModelManager API

### Основные функции:

```cpp
// Singleton
ModelManager& modelMgr = ModelManager::GetInstance();

// Инициализация/очистка
bool Initialize();
void Cleanup();

// Загрузка моделей
bool LoadModel(ModelID id, const string& filename, float scale = 1.0f);
bool LoadSkybox(int blockId, const string& filename, float scale = 1000.0f);

// Получение данных
int GetModelHandle(ModelID id) const;
const ModelData* GetModelData(ModelID id) const;
int GetSkyboxHandle(int blockId) const;

// Дублирование (для создания множества экземпляров)
int DuplicateModel(ModelID id);

// Отрисовка
void DrawModel(ModelID id, VECTOR pos, VECTOR scale, VECTOR rotation = VGet(0,0,0));
void DrawSkybox(int blockId, VECTOR cameraPos);

// Проверки
bool IsModelLoaded(ModelID id) const;
bool IsSkyboxLoaded(int blockId) const;
```

---

## Соответствие ModelID и BlockType

```cpp
ModelID::PLATFORM           -> BlockType::PLATFORM
ModelID::TRIGGER            -> BlockType::TRIGGER
ModelID::INVISIBLE_WALL     -> BlockType::INVISIBLE_WALL
ModelID::FAKE_PLATFORM      -> BlockType::FAKE_PLATFORM
ModelID::SPIKES             -> BlockType::SPIKES
ModelID::DISAPPEARING       -> BlockType::DISAPPEARING
ModelID::MOVING             -> BlockType::MOVING
ModelID::BUTTON             -> BlockType::BUTTON
ModelID::TELEPORT           -> BlockType::TELEPORT
ModelID::RETRACTABLE_SPIKES -> BlockType::RETRACTABLE_SPIKES
ModelID::CRUMBLING          -> BlockType::CRUMBLING
ModelID::FAKE_SPIKES        -> BlockType::FAKE_SPIKES
ModelID::PLAYER             -> Модель игрока
```

---

## Планетарные темы

### Блок 1 (Уровни 1-10): Марс
- Красно-оранжевый скайбокс
- Пустынная атмосфера
- Модели: каменные платформы, ржавые шипы

### Блок 2 (Уровни 11-20): Луна
- Чёрное космическое небо с Землёй
- Серые лунные кратеры
- Модели: футуристические платформы

### Блок 3 (Уровни 21-30): Юпитер
- Газовые облака, штормы
- Оранжево-коричневая атмосфера
- Модели: плавающие островные платформы

### Блок 4-10: TBD
- Другие планеты и небесные тела
- Уникальные визуальные стили

---

## Постепенная миграция

### Этап 1: Подготовка (текущий)
- ✅ Создана система ModelManager
- ✅ Расширена структура Block
- ✅ Добавлена поддержка скайбоксов
- ✅ Автоматический fallback на примитивы

### Этап 2: Создание моделей
- Моделирование базовых блоков
- Создание скайбоксов для каждого блока
- Конвертация в формат MV1

### Этап 3: Интеграция
- Загрузка моделей при старте
- Тестирование производительности
- Настройка масштабов и позиций

### Этап 4: Полировка
- Анимации для движущихся элементов
- Частицы для эффектов
- Освещение и тени

---

## Требования к моделям

### Блоки:
- **Размер:** модель должна быть центрирована в (0,0,0)
- **Масштаб:** 1 единица = 1 метр в игре
- **Полигоны:** до 1000 полигонов на модель (для производительности)
- **Текстуры:** 512x512 или 1024x1024 пикселей

### Скайбоксы:
- **Тип:** сферический или кубический
- **Масштаб:** достаточно большой (1000+ единиц)
- **Полигоны:** до 500 полигонов
- **Текстуры:** 2048x2048 для лучшего качества

---

## Производительность

### Оптимизация:
- Используйте **MV1** формат (оптимизирован)
- Дублируйте модели вместо повторной загрузки
- Ограничьте количество полигонов
- Используйте LOD (Level of Detail) если нужно

### Ожидаемая производительность:
- 60 FPS на средних системах
- Поддержка 100+ блоков на сцене
- Минимальное использование памяти

---

## Пример: Загрузка всех ресурсов

```cpp
void LoadAllGameResources() {
    ModelManager& modelMgr = ModelManager::GetInstance();
    modelMgr.Initialize();

    // Модели блоков
    modelMgr.LoadModel(ModelID::PLATFORM, "platform.mv1");
    modelMgr.LoadModel(ModelID::TRIGGER, "trigger.mv1");
    modelMgr.LoadModel(ModelID::SPIKES, "spikes.mv1");
    modelMgr.LoadModel(ModelID::BUTTON, "button.mv1");
    modelMgr.LoadModel(ModelID::TELEPORT, "teleport.mv1");
    modelMgr.LoadModel(ModelID::RETRACTABLE_SPIKES, "retractable_spikes.mv1");
    modelMgr.LoadModel(ModelID::CRUMBLING, "crumbling.mv1");
    modelMgr.LoadModel(ModelID::FAKE_SPIKES, "fake_spikes.mv1");
    modelMgr.LoadModel(ModelID::MOVING, "moving_platform.mv1");
    modelMgr.LoadModel(ModelID::DISAPPEARING, "disappearing.mv1");
    modelMgr.LoadModel(ModelID::PLAYER, "player.mv1");

    // Скайбоксы (10 блоков)
    modelMgr.LoadSkybox(0, "mars_skybox.mv1");      // Блок 1
    modelMgr.LoadSkybox(1, "moon_skybox.mv1");      // Блок 2
    modelMgr.LoadSkybox(2, "jupiter_skybox.mv1");   // Блок 3
    modelMgr.LoadSkybox(3, "saturn_skybox.mv1");    // Блок 4
    modelMgr.LoadSkybox(4, "venus_skybox.mv1");     // Блок 5
    modelMgr.LoadSkybox(5, "neptune_skybox.mv1");   // Блок 6
    modelMgr.LoadSkybox(6, "mercury_skybox.mv1");   // Блок 7
    modelMgr.LoadSkybox(7, "pluto_skybox.mv1");     // Блок 8
    modelMgr.LoadSkybox(8, "asteroid_skybox.mv1");  // Блок 9
    modelMgr.LoadSkybox(9, "nebula_skybox.mv1");    // Блок 10
}
```

---

## Troubleshooting

### Модель не загружается
- Проверьте путь к файлу
- Убедитесь что формат поддерживается
- Проверьте размер файла (не слишком большой?)

### Модель отображается неправильно
- Проверьте масштаб (modelScale)
- Проверьте центрирование модели в 3D редакторе
- Проверьте вращение (rotation)

### Низкая производительность
- Уменьшите количество полигонов
- Используйте формат MV1
- Оптимизируйте текстуры
- Используйте меньше дубликатов

---

## Ресурсы

### Инструменты:
- **Metasequoia** - японский 3D редактор (популярен с DxLib)
- **Blender** - бесплатный 3D редактор
- **DxLib Model Converter** - конвертация в MV1

### Где найти модели:
- [Free3D](https://free3d.com/)
- [Sketchfab](https://sketchfab.com/)
- [TurboSquid](https://www.turbosquid.com/)
- Создать свои в Blender/Metasequoia

---

## Будущие улучшения

- [ ] Система анимаций для блоков
- [ ] Динамическое освещение
- [ ] Частицы и эффекты
- [ ] Procedural generation моделей
- [ ] Shader эффекты для планет
- [ ] Система LOD (Level of Detail)

---

**Готово к использованию!** Система полностью обратно совместима - игра работает с примитивами, пока модели не загружены.
