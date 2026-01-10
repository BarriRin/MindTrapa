#include "Level.h"

Level::Level(int id) : levelId(id) {
    // Определить ID блока по уровню (10 уровней = 1 блок)
    blockId = (id - 1) / 10;

    LoadLevelData(id);
    InitializeModels();
}

Level::~Level() {
    // Удалить все дублированные модели блоков
    for (auto& block : blocks) {
        if (block.modelHandle != -1) {
            MV1DeleteModel(block.modelHandle);
        }
    }
    blocks.clear();
}

void Level::LoadLevelData(int id) {
    blocks.clear();

    switch (id) {
    case 1: // Уровень 1 - простой туториал
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(10, 1, 10), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, 0), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(15, 3, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 2: // Уровень 2 - прыжки + первые шипы!
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(5, 1, -2), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(12, 3, 0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 2, 0), VGet(1, 1, 1), BlockType::SPIKES));
        blocks.push_back(Block(VGet(15, 4, 2), VGet(1, 1, 1), BlockType::SPIKES));
        blocks.push_back(Block(VGet(20, 3, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 3: // Уровень 3 - невидимые стены
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, 0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(5, 0, -2), VGet(1, 4, 6), BlockType::INVISIBLE_WALL));
        blocks.push_back(Block(VGet(3, 1, 5), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, 7), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(15, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 4: // Уровень 4 - телепорты
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, 0), VGet(2, 1, 2), BlockType::TELEPORT, 1));
        blocks.push_back(Block(VGet(20, 3, 0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(21, 4, 1), VGet(2, 1, 2), BlockType::TELEPORT, 1));
        blocks.push_back(Block(VGet(30, 5, 0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(32, 6, 1), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 5: // Уровень 5 - ложные телепорты
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, -3), VGet(2, 1, 2), BlockType::TELEPORT, 1));
        blocks.push_back(Block(VGet(8, 1, 0), VGet(2, 1, 2), BlockType::TELEPORT, 2));
        blocks.push_back(Block(VGet(8, 1, 3), VGet(2, 1, 2), BlockType::TELEPORT, 3));
        blocks.push_back(Block(VGet(20, 1, -3), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 2, -3), VGet(1, 1, 1), BlockType::SPIKES));
        blocks.push_back(Block(VGet(20, 1, 3), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(25, 1, 3), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 6: // Уровень 6 - кнопки и двери
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, 0), VGet(2, 1, 2), BlockType::BUTTON, 10, false));
        blocks.push_back(Block(VGet(15, 1, 0), VGet(4, 1, 4), BlockType::DISAPPEARING, 10, false));
        blocks.push_back(Block(VGet(22, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 7: // Уровень 7 - движущиеся платформы
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(5, 1, 0), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(10, 2, 0), VGet(4, 1, 4), BlockType::MOVING, 0, true, VGet(10, 2, 0), VGet(20, 2, 0)));
        blocks.push_back(Block(VGet(25, 3, 0), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(30, 4, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 8: // Уровень 8 - исчезающие платформы
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(5, 2, 0), VGet(3, 1, 3), BlockType::DISAPPEARING, 0, true));
        blocks.push_back(Block(VGet(10, 3, 0), VGet(3, 1, 3), BlockType::DISAPPEARING, 0, true));
        blocks.push_back(Block(VGet(15, 4, 0), VGet(2, 1, 2), BlockType::DISAPPEARING, 0, true));
        blocks.push_back(Block(VGet(20, 5, 0), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(25, 5, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 9: // Уровень 9 - выдвигающиеся шипы
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Первая секция - узкий коридор
        blocks.push_back(Block(VGet(5, 0, -1), VGet(12, 1, 3), BlockType::PLATFORM));
        // Шипы на всю ширину платформы (3 единицы)
        blocks.push_back(Block(VGet(7, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 1));
        blocks.push_back(Block(VGet(10, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));
        blocks.push_back(Block(VGet(13, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 1));
        blocks.push_back(Block(VGet(16, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));

        // Вторая секция - немного шире
        blocks.push_back(Block(VGet(20, 2, -2), VGet(8, 1, 5), BlockType::PLATFORM));
        // Шипы на всю ширину (5 единиц)
        blocks.push_back(Block(VGet(21, 3, -2), VGet(1, 1, 5), BlockType::SPIKES, 1));
        blocks.push_back(Block(VGet(23, 3, -2), VGet(1, 1, 5), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 1.0f));
        blocks.push_back(Block(VGet(25, 3, -2), VGet(1, 1, 5), BlockType::SPIKES, 1));
        blocks.push_back(Block(VGet(27, 3, -2), VGet(1, 1, 5), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 1.0f));

        // Третья секция - финальный рывок
        blocks.push_back(Block(VGet(31, 3, -1), VGet(10, 1, 3), BlockType::PLATFORM));
        // Много шипов подряд, нужно выбрать момент
        blocks.push_back(Block(VGet(32, 4, -1), VGet(1, 1, 3), BlockType::SPIKES, 1));
        blocks.push_back(Block(VGet(34, 4, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.5f));
        blocks.push_back(Block(VGet(36, 4, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 1.0f));
        blocks.push_back(Block(VGet(38, 4, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 1.5f));
        blocks.push_back(Block(VGet(40, 4, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));

        // Финиш
        blocks.push_back(Block(VGet(44, 4, -2), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(46, 5, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 10: // Уровень 10 - ФИНАЛ БЛОКА 1! Все механики обязательны
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // === СЕКЦИЯ 1: Выдвигающиеся шипы (обязательно пройти) ===
        blocks.push_back(Block(VGet(5, 0, -1), VGet(10, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(7, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 1));
        blocks.push_back(Block(VGet(10, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 0)); // Статичные!
        blocks.push_back(Block(VGet(13, 1, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));

        // === СЕКЦИЯ 2: Движущаяся платформа - ОБЯЗАТЕЛЬНА (большая пропасть) ===
        // Слишком далеко чтобы прыгнуть напрямую!
        blocks.push_back(Block(VGet(18, 2, -1), VGet(3, 1, 3), BlockType::PLATFORM)); // Маленькая площадка
        // Движущаяся платформа - единственный путь
        blocks.push_back(Block(VGet(22, 3, -1), VGet(4, 1, 3), BlockType::MOVING, 0, true, VGet(22, 3, -1), VGet(35, 3, -1)));
        // Целевая платформа далеко
        blocks.push_back(Block(VGet(38, 4, -2), VGet(5, 1, 5), BlockType::PLATFORM));

        // === СЕКЦИЯ 3: Телепорт - ОБЯЗАТЕЛЕН (невидимые стены блокируют обход) ===
        blocks.push_back(Block(VGet(30, 10, -2), VGet(4, 1, 5), BlockType::PLATFORM)); // Высоко!
        blocks.push_back(Block(VGet(31, 11, 0), VGet(2, 1, 2), BlockType::TELEPORT, 10));
        // Невидимые стены со всех сторон - нельзя обойти!
        blocks.push_back(Block(VGet(34, 10, -2), VGet(1, 4, 5), BlockType::INVISIBLE_WALL)); // Справа
        blocks.push_back(Block(VGet(29, 10, -2), VGet(1, 4, 5), BlockType::INVISIBLE_WALL)); // Слева

        // Куда телепортирует (далеко и высоко)
        blocks.push_back(Block(VGet(50, 6, -3), VGet(6, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(52, 7, 0), VGet(2, 1, 2), BlockType::TELEPORT, 10));

        // === СЕКЦИЯ 4: Кнопка + исчезающие платформы - ВСЁ ОБЯЗАТЕЛЬНО ===
        blocks.push_back(Block(VGet(60, 7, -2), VGet(4, 1, 5), BlockType::PLATFORM));
        // Кнопка
        blocks.push_back(Block(VGet(61, 8, 0), VGet(2, 1, 2), BlockType::BUTTON, 20, false));

        // Исчезающие платформы к двери (расстояния большие!)
        blocks.push_back(Block(VGet(67, 8, 0), VGet(2, 1, 2), BlockType::DISAPPEARING, 0, true));
        blocks.push_back(Block(VGet(72, 9, 0), VGet(2, 1, 2), BlockType::DISAPPEARING, 0, true));
        blocks.push_back(Block(VGet(77, 10, 0), VGet(2, 1, 2), BlockType::DISAPPEARING, 0, true));

        // Дверь (открывается только кнопкой) - блокирует путь!
        blocks.push_back(Block(VGet(81, 10, -1), VGet(1, 4, 3), BlockType::DISAPPEARING, 20, false));
        // Невидимая стена сверху чтобы не перепрыгнуть дверь
        blocks.push_back(Block(VGet(81, 14, -1), VGet(1, 5, 3), BlockType::INVISIBLE_WALL));

        // === ФИНАЛ: Последний рывок через выдвигающиеся шипы ===
        blocks.push_back(Block(VGet(84, 11, -1), VGet(8, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(85, 12, -1), VGet(1, 1, 3), BlockType::SPIKES, 1));
        blocks.push_back(Block(VGet(87, 12, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 1.0f));
        blocks.push_back(Block(VGet(89, 12, -1), VGet(1, 1, 3), BlockType::SPIKES, 1, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));

        // Финальная платформа и триггер
        blocks.push_back(Block(VGet(94, 12, -3), VGet(6, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(97, 13, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    // ========== BLOCK 2: TIMING & PRECISION (Levels 11-20) ==========

    case 11: // Level 11 - Вводим RETRACTABLE_SPIKES (выдвижные шипы)
        playerSpawn = VGet(0, 1, 0);

        // Стартовая платформа
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Первая секция - один выдвижной шип (простой)
        blocks.push_back(Block(VGet(5, 0, -2), VGet(8, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES, 0, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));

        // Вторая секция - два выдвижных шипа со сдвигом фаз
        blocks.push_back(Block(VGet(15, 1, -2), VGet(10, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(17, 2, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES, 0, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));
        blocks.push_back(Block(VGet(21, 2, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES, 0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f)); // Сдвиг фазы

        // Финал
        blocks.push_back(Block(VGet(27, 2, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(29, 3, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 12: // Level 12 - Вводим CRUMBLING platforms (рассыпающиеся)
        playerSpawn = VGet(0, 1, 0);

        // Стартовая платформа
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Прыжки по рассыпающимся платформам - нужно быстро бежать!
        blocks.push_back(Block(VGet(5, 1, 0), VGet(3, 1, 3), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(10, 2, 0), VGet(3, 1, 3), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(15, 3, 0), VGet(3, 1, 3), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(20, 4, 0), VGet(3, 1, 3), BlockType::CRUMBLING));

        // Безопасная платформа и триггер
        blocks.push_back(Block(VGet(25, 5, -2), VGet(5, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(27, 6, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 13: // Level 13 - Комбо: MOVING платформы + RETRACTABLE_SPIKES
        playerSpawn = VGet(0, 1, 0);

        // Стартовая платформа
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Движущаяся платформа над пропастью
        blocks.push_back(Block(VGet(5, 2, 0), VGet(4, 1, 3), BlockType::MOVING, 0, true, VGet(5, 2, 0), VGet(18, 2, 0)));

        // Целевая платформа с выдвижными шипами - нужно спрыгнуть в правильный момент!
        blocks.push_back(Block(VGet(20, 2, -2), VGet(10, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 3, -2), VGet(2, 1, 6), BlockType::RETRACTABLE_SPIKES, 0, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));
        blocks.push_back(Block(VGet(26, 3, -2), VGet(2, 1, 6), BlockType::RETRACTABLE_SPIKES, 0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));

        // Финал
        blocks.push_back(Block(VGet(32, 3, -1), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(34, 4, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 14: // Level 14 - ТРОЛЛИНГ! FAKE_SPIKES (розовые безопасные шипы)
        playerSpawn = VGet(0, 1, 0);

        // Стартовая платформа
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Путь выглядит опасным (розовые шипы), но это ФЕЙК!
        blocks.push_back(Block(VGet(5, 0, -2), VGet(12, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(7, 1, -2), VGet(2, 1, 4), BlockType::FAKE_SPIKES)); // ФЕЙК - безопасно!
        blocks.push_back(Block(VGet(10, 1, -2), VGet(2, 1, 4), BlockType::FAKE_SPIKES)); // ФЕЙК
        blocks.push_back(Block(VGet(13, 1, -2), VGet(2, 1, 4), BlockType::FAKE_SPIKES)); // ФЕЙК

        // А вот "безопасный" обходной путь - на самом деле там НАСТОЯЩИЕ шипы!
        blocks.push_back(Block(VGet(5, 0, 4), VGet(12, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8, 1, 4), VGet(2, 1, 4), BlockType::SPIKES)); // НАСТОЯЩИЕ шипы!

        // Платформа после "опасного" пути (который на самом деле безопасный)
        blocks.push_back(Block(VGet(19, 1, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(21, 2, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 15: // Level 15 - CRUMBLING платформы + точный тайминг (паркур)
        playerSpawn = VGet(0, 1, 0);

        // Стартовая платформа
        blocks.push_back(Block(VGet(-3, -1, -3), VGet(6, 1, 6), BlockType::PLATFORM));

        // Серия CRUMBLING платформ - нужно быстро пробежать
        blocks.push_back(Block(VGet(5, 1, -1), VGet(3, 1, 3), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(9, 1, -1), VGet(3, 1, 3), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(13, 1, -1), VGet(3, 1, 3), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(17, 1, -1), VGet(3, 1, 3), BlockType::CRUMBLING));

        // Поворот - CRUMBLING платформы в сторону
        blocks.push_back(Block(VGet(20, 2, 3), VGet(3, 1, 3), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(20, 3, 7), VGet(3, 1, 3), BlockType::CRUMBLING));

        // Финал
        blocks.push_back(Block(VGet(17, 4, 10), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(19, 5, 12), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 16: // Level 16 - INVISIBLE_WALL лабиринт + RETRACTABLE_SPIKES
        playerSpawn = VGet(0, 1, 0);

        // Стартовая платформа
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(10, 1, 20), BlockType::PLATFORM));

        // Кажется, что можно идти прямо, но невидимые стены блокируют
        blocks.push_back(Block(VGet(0, 0, 0), VGet(1, 5, 5), BlockType::INVISIBLE_WALL)); // Блокирует прямой путь

        // Нужно обойти слева
        blocks.push_back(Block(VGet(-4, 0, 7), VGet(8, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(-2, 1, 8), VGet(2, 1, 2), BlockType::RETRACTABLE_SPIKES, 0, true));

        // Ещё одна невидимая стена справа
        blocks.push_back(Block(VGet(2, 0, 10), VGet(1, 5, 4), BlockType::INVISIBLE_WALL));

        // Правильный путь через середину
        blocks.push_back(Block(VGet(-2, 0, 13), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(0, 1, 15), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 17: // Level 17 - MOVING платформы над пропастью + CRUMBLING финал
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-3, -1, -3), VGet(6, 1, 6), BlockType::PLATFORM));

        // Первая движущаяся платформа (горизонтально)
        blocks.push_back(Block(VGet(5, 2, -1), VGet(4, 1, 4), BlockType::MOVING, 0, true,
            VGet(5, 2, -1), VGet(12, 2, -1), 2.0f));

        // Вторая движущаяся платформа (вертикально)
        blocks.push_back(Block(VGet(16, 3, -1), VGet(4, 1, 4), BlockType::MOVING, 0, true,
            VGet(16, 3, -1), VGet(16, 7, -1), 1.5f));

        // Платформа с CRUMBLING финалом
        blocks.push_back(Block(VGet(22, 7, -2), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(29, 7, -1), VGet(3, 1, 3), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(33, 7, -1), VGet(3, 1, 3), BlockType::CRUMBLING));

        // Финал
        blocks.push_back(Block(VGet(37, 7, -2), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(39, 8, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 18: // Level 18 - BUTTON + DISAPPEARING комбо с RETRACTABLE_SPIKES
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-3, -1, -3), VGet(6, 1, 6), BlockType::PLATFORM));

        // Кнопка на платформе
        blocks.push_back(Block(VGet(5, 0, -2), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(7, 1, 0), VGet(1, 1, 1), BlockType::BUTTON, 1));

        // DISAPPEARING платформа активируется кнопкой
        blocks.push_back(Block(VGet(12, 2, -1), VGet(5, 1, 4), BlockType::DISAPPEARING, 1, false));

        // Путь через исчезающую платформу с RETRACTABLE_SPIKES
        blocks.push_back(Block(VGet(13, 3, 0), VGet(2, 1, 2), BlockType::RETRACTABLE_SPIKES, 0, true));

        // Финал
        blocks.push_back(Block(VGet(19, 2, -2), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(21, 3, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 19: // Level 19 - Комплексный троллинг: FAKE_PLATFORM + FAKE_SPIKES
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Выглядит как безопасная платформа, но это FAKE_PLATFORM!
        blocks.push_back(Block(VGet(5, 0, -2), VGet(8, 1, 5), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(7, 1, 0), VGet(2, 1, 2), BlockType::SPIKES)); // Шипы под фейк-платформой

        // Настоящий путь - через "опасные" розовые шипы (FAKE_SPIKES)
        blocks.push_back(Block(VGet(5, -1, 5), VGet(8, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(7, 0, 6), VGet(3, 1, 3), BlockType::FAKE_SPIKES)); // Безопасно!

        // Вторая ловушка
        blocks.push_back(Block(VGet(15, 0, 5), VGet(6, 1, 5), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(15, 0, -2), VGet(6, 1, 5), BlockType::PLATFORM)); // Настоящая платформа

        // Финал с CRUMBLING
        blocks.push_back(Block(VGet(23, 1, -1), VGet(3, 1, 3), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(27, 1, -2), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(29, 2, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 20: // Level 20 - ФИНАЛ Block 2: Все механики вместе!
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Секция 1: CRUMBLING быстрый старт
        blocks.push_back(Block(VGet(5, 1, -2), VGet(3, 1, 4), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(9, 1, -2), VGet(3, 1, 4), BlockType::CRUMBLING));

        // Секция 2: RETRACTABLE_SPIKES разнофазные
        blocks.push_back(Block(VGet(14, 1, -3), VGet(8, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(16, 2, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES, 0, true));
        blocks.push_back(Block(VGet(19, 2, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES, 0, false));

        // Секция 3: MOVING платформа над пропастью
        blocks.push_back(Block(VGet(24, 3, -1), VGet(4, 1, 4), BlockType::MOVING, 0, true,
            VGet(24, 3, -1), VGet(32, 3, -1), 2.5f));

        // Секция 4: FAKE_SPIKES троллинг
        blocks.push_back(Block(VGet(34, 3, -3), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(36, 4, -2), VGet(2, 1, 4), BlockType::FAKE_SPIKES)); // Безопасно!

        // Секция 5: BUTTON + DISAPPEARING
        blocks.push_back(Block(VGet(42, 3, -2), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(44, 4, 0), VGet(1, 1, 1), BlockType::BUTTON, 1));
        blocks.push_back(Block(VGet(49, 5, -1), VGet(5, 1, 4), BlockType::DISAPPEARING, 1, false));

        // Финал: CRUMBLING + прыжок к триггеру
        blocks.push_back(Block(VGet(56, 5, -1), VGet(3, 1, 3), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(60, 5, -2), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(62, 6, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    // ========== BLOCK 3: GRAVITY & BLADES (Levels 21-30) ==========

    case 21: // Level 21 - Вводим LOW GRAVITY (низкая гравитация)
        playerSpawn = VGet(0, 1, 0);

        // Стартовая платформа
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Обычная платформа перед зоной
        blocks.push_back(Block(VGet(5, 0, -2), VGet(4, 1, 4), BlockType::PLATFORM));

        // Зона с низкой гравитацией (floaty jumps)
        blocks.push_back(Block(VGet(11, 0, -3), VGet(15, 15, 6), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.3f; // Очень низкая гравитация

        // Платформы в зоне низкой гравитации - нужно контролировать прыжки
        blocks.push_back(Block(VGet(12, 1, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(17, 4, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 7, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        // Выход из зоны и финиш
        blocks.push_back(Block(VGet(28, 8, -2), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(30, 9, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 22: // Level 22 - Вводим HIGH GRAVITY (высокая гравитация)
        playerSpawn = VGet(0, 1, 0);

        // Стартовая платформа
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Зона с высокой гравитацией (heavy, fast fall)
        blocks.push_back(Block(VGet(5, -1, -4), VGet(20, 12, 8), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 2.5f; // Высокая гравитация

        // Платформы в зоне - прыжки низкие, нужно точно рассчитывать
        blocks.push_back(Block(VGet(6, 0, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(10, 0, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(14, 0, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(18, 0, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 1, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        // Финиш
        blocks.push_back(Block(VGet(27, 2, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(29, 3, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 23: // Level 23 - Вводим PENDULUM BLADE (простой маятник)
        playerSpawn = VGet(0, 1, 0);

        // Стартовая платформа
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Коридор с одним простым маятником
        blocks.push_back(Block(VGet(5, 0, -2), VGet(15, 1, 4), BlockType::PLATFORM));

        // Простой маятник посередине - медленный, предсказуемый
        blocks.push_back(Block(VGet(12, 1, 0), VGet(1.5f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(12, 8, 0);
        blocks.back().swingSpeed = 1.2f;  // Медленная скорость
        blocks.back().swingRange = DX_PI_F / 4.0f; // ±45 градусов

        // Финиш
        blocks.push_back(Block(VGet(22, 1, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(24, 2, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 24: // Level 24 - КОМБО: Low Gravity + Pendulum
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Зона низкой гравитации
        blocks.push_back(Block(VGet(5, 0, -3), VGet(25, 18, 6), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.4f;

        // Платформы в зоне
        blocks.push_back(Block(VGet(6, 1, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(12, 5, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        // Маятник в зоне низкой гравитации - сложнее контролировать движение
        blocks.push_back(Block(VGet(15, 3, 0), VGet(1.5f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(15, 12, 0);
        blocks.back().swingSpeed = 1.5f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        blocks.push_back(Block(VGet(18, 8, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(24, 10, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        // Финиш
        blocks.push_back(Block(VGet(29, 11, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(31, 12, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 25: // Level 25 - REVERSE GRAVITY (обратная гравитация!)
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Платформа перед зоной
        blocks.push_back(Block(VGet(5, 0, -2), VGet(4, 1, 4), BlockType::PLATFORM));

        // Зона с ОБРАТНОЙ гравитацией (walk on ceiling!)
        blocks.push_back(Block(VGet(11, 0, -3), VGet(18, 18, 6), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = -1.2f; // Обратная гравитация

        // "Потолок" теперь пол - платформы вверху
        blocks.push_back(Block(VGet(12, 12, -2), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(17, 12, -2), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(23, 12, -2), VGet(4, 1, 4), BlockType::PLATFORM));

        // Выход из зоны - вернуться к нормальной гравитации
        blocks.push_back(Block(VGet(30, 0, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(32, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 26: // Level 26 - СЛОЖНЫЕ ПАТТЕРНЫ МАЯТНИКОВ (3 маятника разной скорости)
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Длинный коридор с тремя маятниками
        blocks.push_back(Block(VGet(5, 0, -2), VGet(30, 1, 4), BlockType::PLATFORM));

        // Первый маятник - медленный
        blocks.push_back(Block(VGet(10, 1, 0), VGet(1.5f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(10, 9, 0);
        blocks.back().swingSpeed = 1.0f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        // Второй маятник - быстрый, другая фаза
        blocks.push_back(Block(VGet(18, 1, 0), VGet(1.5f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(18, 10, 0);
        blocks.back().swingSpeed = 2.2f;
        blocks.back().swingRange = DX_PI_F / 2.5f;
        blocks.back().timer = 1.5f; // Сдвиг фазы

        // Третий маятник - средняя скорость, широкий размах
        blocks.push_back(Block(VGet(27, 1, 0), VGet(2.0f, 2.0f, 2.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(27, 11, 0);
        blocks.back().swingSpeed = 1.6f;
        blocks.back().swingRange = DX_PI_F / 2.2f;
        blocks.back().timer = 3.0f; // Другая фаза

        // Финиш
        blocks.push_back(Block(VGet(37, 1, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(39, 2, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 27: // Level 27 - МУЛЬТИ-ЗОНЫ ГРАВИТАЦИИ + МАЯТНИКИ
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Секция 1: Низкая гравитация + маятник
        blocks.push_back(Block(VGet(5, 0, -2), VGet(12, 12, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.35f;

        blocks.push_back(Block(VGet(6, 1, -1), VGet(3, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(10, 1, 0), VGet(1.5f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(10, 10, 0);
        blocks.back().swingSpeed = 1.4f;
        blocks.back().swingRange = DX_PI_F / 3.5f;

        blocks.push_back(Block(VGet(13, 6, -1), VGet(3, 1, 2), BlockType::PLATFORM));

        // Секция 2: Высокая гравитация + быстрый маятник
        blocks.push_back(Block(VGet(18, -1, -2), VGet(12, 8, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 2.8f;

        blocks.push_back(Block(VGet(19, 0, -1), VGet(3, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(23, 1, 0), VGet(1.5f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(23, 6, 0);
        blocks.back().swingSpeed = 2.5f;
        blocks.back().swingRange = DX_PI_F / 4.0f;
        blocks.back().timer = 1.0f;

        blocks.push_back(Block(VGet(27, 1, -1), VGet(3, 1, 2), BlockType::PLATFORM));

        // Финиш
        blocks.push_back(Block(VGet(32, 2, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(34, 3, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 28: // Level 28 - ТРОЛЛИНГ С ГРАВИТАЦИЕЙ (неожиданные переключения)
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Платформа 1 - кажется нормальной
        blocks.push_back(Block(VGet(5, 0, -1), VGet(4, 1, 2), BlockType::PLATFORM));

        // ТРОЛЛИНГ: Неожиданная высокая гравитация (прыжок не достанет!)
        blocks.push_back(Block(VGet(11, 0, -2), VGet(8, 6, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 3.5f; // Очень высокая!

        // Настоящий путь - сбоку с низкой платформой
        blocks.push_back(Block(VGet(12, -1, -1), VGet(3, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(16, -1, -1), VGet(3, 1, 2), BlockType::PLATFORM));

        // Платформа 2
        blocks.push_back(Block(VGet(21, 0, -2), VGet(4, 1, 4), BlockType::PLATFORM));

        // ТРОЛЛИНГ 2: Кажется, что низкая гравитация, но это ОБРАТНАЯ!
        blocks.push_back(Block(VGet(27, 0, -2), VGet(10, 12, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = -1.5f; // Обратная!

        // Платформы вверху
        blocks.push_back(Block(VGet(29, 10, -1), VGet(3, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(33, 10, -1), VGet(3, 1, 2), BlockType::PLATFORM));

        // Финиш (нормальная зона)
        blocks.push_back(Block(VGet(39, 0, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(41, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 29: // Level 29 - ИСПЫТАНИЕ ЛЕЗВИЙ + ГРАВИТАЦИЯ (сложно!)
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Зона низкой гравитации на весь уровень
        blocks.push_back(Block(VGet(5, 0, -3), VGet(45, 20, 6), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.4f;

        // Серия платформ с маятниками между ними
        blocks.push_back(Block(VGet(6, 1, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        // Маятник 1
        blocks.push_back(Block(VGet(11, 2, 0), VGet(1.5f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(11, 12, 0);
        blocks.back().swingSpeed = 1.8f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        blocks.push_back(Block(VGet(14, 4, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        // Маятник 2
        blocks.push_back(Block(VGet(19, 5, 0), VGet(1.5f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(19, 14, 0);
        blocks.back().swingSpeed = 2.0f;
        blocks.back().swingRange = DX_PI_F / 2.5f;
        blocks.back().timer = 1.2f;

        blocks.push_back(Block(VGet(22, 7, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        // Маятник 3 - крупный и медленный
        blocks.push_back(Block(VGet(27, 8, 0), VGet(2.0f, 2.0f, 2.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(27, 16, 0);
        blocks.back().swingSpeed = 1.3f;
        blocks.back().swingRange = DX_PI_F / 2.0f;
        blocks.back().timer = 2.5f;

        blocks.push_back(Block(VGet(30, 10, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        // Маятник 4 - быстрый финальный
        blocks.push_back(Block(VGet(35, 11, 0), VGet(1.5f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(35, 18, 0);
        blocks.back().swingSpeed = 2.4f;
        blocks.back().swingRange = DX_PI_F / 3.5f;

        // Финиш
        blocks.push_back(Block(VGet(38, 12, -2), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(40, 13, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 30: // Level 30 - ФИНАЛ BLOCK 3: ВСЁ ВМЕСТЕ!
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // === СЕКЦИЯ 1: Низкая гравитация + маятник ===
        blocks.push_back(Block(VGet(5, 0, -2), VGet(15, 15, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.35f;

        blocks.push_back(Block(VGet(6, 1, -1), VGet(3, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(11, 3, 0), VGet(1.5f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(11, 12, 0);
        blocks.back().swingSpeed = 1.6f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        blocks.push_back(Block(VGet(15, 7, -1), VGet(3, 1, 2), BlockType::PLATFORM));

        // === СЕКЦИЯ 2: Высокая гравитация + CRUMBLING ===
        blocks.push_back(Block(VGet(20, 0, -2), VGet(18, 10, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 2.6f;

        blocks.push_back(Block(VGet(21, 1, -1), VGet(3, 1, 2), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(25, 1, -1), VGet(3, 1, 2), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(29, 1, -1), VGet(3, 1, 2), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(33, 2, -1), VGet(4, 1, 2), BlockType::PLATFORM));

        // === СЕКЦИЯ 3: Обратная гравитация + маятники + RETRACTABLE_SPIKES ===
        blocks.push_back(Block(VGet(39, 0, -2), VGet(22, 16, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = -1.3f;

        // Потолочные платформы
        blocks.push_back(Block(VGet(40, 14, -1), VGet(4, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(46, 14, -1), VGet(4, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(52, 14, -1), VGet(4, 1, 2), BlockType::PLATFORM));

        // Маятники на "полу" (который теперь потолок)
        blocks.push_back(Block(VGet(43, 8, 0), VGet(1.5f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(43, 2, 0);
        blocks.back().swingSpeed = 1.7f;
        blocks.back().swingRange = DX_PI_F / 3.5f;

        blocks.push_back(Block(VGet(49, 7, 0), VGet(1.5f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(49, 1, 0);
        blocks.back().swingSpeed = 2.1f;
        blocks.back().swingRange = DX_PI_F / 2.8f;
        blocks.back().timer = 1.5f;

        // Выдвижные шипы на потолочных платформах
        blocks.push_back(Block(VGet(47, 15, -1), VGet(2, 1, 2), BlockType::RETRACTABLE_SPIKES));

        // === СЕКЦИЯ 4: Финальный рывок - нормальная гравитация + всё подряд ===
        blocks.push_back(Block(VGet(63, 0, -3), VGet(6, 1, 6), BlockType::PLATFORM));

        // Движущаяся платформа
        blocks.push_back(Block(VGet(71, 2, -1), VGet(4, 1, 3), BlockType::MOVING, 0, true,
            VGet(71, 2, -1), VGet(71, 8, -1), 1.8f));

        // Последний маятник
        blocks.push_back(Block(VGet(77, 5, 0), VGet(1.5f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(77, 14, 0);
        blocks.back().swingSpeed = 2.0f;
        blocks.back().swingRange = DX_PI_F / 2.5f;

        // ФИНИШ - ты дошёл!
        blocks.push_back(Block(VGet(80, 9, -3), VGet(7, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(83, 10, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    default:
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-3, -1, -3), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(5, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;
    }
}

void Level::Draw() const {
    for (const auto& block : blocks) {
        // Если у блока есть модель - отрисовать модель
        if (block.useModel && block.modelHandle != -1) {
            // Установить позицию модели
            MV1SetPosition(block.modelHandle, VAdd(block.pos, VScale(block.size, 0.5f)));

            // Установить масштаб
            MV1SetScale(block.modelHandle, block.modelScale);

            // Установить вращение
            MV1SetRotationXYZ(block.modelHandle, block.rotation);

            // Отрисовать модель
            MV1DrawModel(block.modelHandle);
            continue;
        }

        // Иначе использовать примитивы (текущая реализация)
        switch (block.type) {
        case BlockType::PLATFORM:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(200, 200, 200), GetColor(150, 150, 150), TRUE);
            break;

        case BlockType::TRIGGER:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(100, 200, 255), GetColor(50, 150, 200), TRUE);
            break;

        case BlockType::INVISIBLE_WALL:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
            break;

        case BlockType::FAKE_PLATFORM:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(180, 180, 180), GetColor(130, 130, 130), TRUE);
            break;

        case BlockType::SPIKES:
            if (block.linkId == 1) {
                if (block.isActive) {
                    DrawCube3D(block.pos, VAdd(block.pos, block.size),
                        GetColor(255, 50, 50), GetColor(200, 0, 0), TRUE);
                }
                else {
                    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
                    DrawCube3D(block.pos, VAdd(block.pos, block.size),
                        GetColor(150, 150, 150), GetColor(100, 100, 100), TRUE);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                }
            }
            else {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(255, 50, 50), GetColor(200, 0, 0), TRUE);
            }
            break;

        case BlockType::DISAPPEARING:
            if (block.isActive) {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(255, 255, 100), GetColor(200, 200, 50), TRUE);
            }
            break;

        case BlockType::MOVING:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);
            break;

        case BlockType::BUTTON:
            if (block.isActive) {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(100, 255, 100), GetColor(50, 200, 50), TRUE);
            }
            else {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(255, 255, 100), GetColor(200, 200, 50), TRUE);
            }
            break;

        case BlockType::TELEPORT:
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(255, 100, 255), GetColor(200, 50, 200), TRUE);
            break;

        case BlockType::RETRACTABLE_SPIKES:
            // Выдвигающиеся шипы - красные когда активны, прозрачные когда спрятаны
            if (block.isActive) {
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(255, 50, 50), GetColor(200, 0, 0), TRUE);
            }
            else {
                // Полупрозрачные когда спрятаны (показываем что они будут)
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(150, 150, 150), GetColor(100, 100, 100), TRUE);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            }
            break;

        case BlockType::CRUMBLING:
            // Рассыпающиеся платформы - жёлто-коричневые
            if (block.isActive) {
                // Нормальное состояние
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(200, 150, 50), GetColor(150, 100, 30), TRUE);
            }
            else {
                // Падают/исчезают (полупрозрачные)
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
                DrawCube3D(block.pos, VAdd(block.pos, block.size),
                    GetColor(200, 150, 50), GetColor(150, 100, 30), TRUE);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            }
            break;

        case BlockType::FAKE_SPIKES:
            // Фейковые шипы - РОЗОВЫЕ (выглядят опасно, но безопасны)
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(255, 100, 150), GetColor(200, 50, 100), TRUE);
            break;

        case BlockType::GRAVITY_ZONE:
            // Гравитационные зоны - очень прозрачные с цветом зависящим от типа гравитации
            unsigned int gravityColor;
            if (block.gravityMultiplier < 0.0f) {
                // Обратная гравитация - фиолетовый
                gravityColor = GetColor(200, 100, 255);
            } else if (block.gravityMultiplier < 1.0f) {
                // Низкая гравитация - голубой
                gravityColor = GetColor(100, 200, 255);
            } else if (block.gravityMultiplier > 1.0f) {
                // Высокая гравитация - оранжевый
                gravityColor = GetColor(255, 150, 50);
            } else {
                // Нормальная (не должно быть, но на всякий случай)
                gravityColor = GetColor(150, 150, 150);
            }
            // Отключаем запись в Z-буфер чтобы зона не перекрывала объекты внутри
            SetWriteZBuffer3D(FALSE);
            // Очень прозрачные - альфа 50 из 255 (~20% непрозрачности)
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 50);
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                gravityColor, gravityColor, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            // Включаем запись в Z-буфер обратно
            SetWriteZBuffer3D(TRUE);
            break;

        case BlockType::PENDULUM_BLADE:
            // Маятник-топор - рисуем цепь (или трос) и лезвие
            // Цепь от точки подвеса до лезвия
            DrawLine3D(block.pivotPoint, block.pos, GetColor(100, 100, 100));

            // Лезвие - красный куб (временно, потом будет модель)
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(200, 50, 50), GetColor(150, 30, 30), TRUE);

            // Точка подвеса - маленькая сфера (упрощённо - куб)
            VECTOR pivotSize = VGet(0.3f, 0.3f, 0.3f);
            DrawCube3D(VGet(block.pivotPoint.x - 0.15f, block.pivotPoint.y - 0.15f, block.pivotPoint.z - 0.15f),
                VGet(block.pivotPoint.x + 0.15f, block.pivotPoint.y + 0.15f, block.pivotPoint.z + 0.15f),
                GetColor(80, 80, 80), GetColor(60, 60, 60), TRUE);
            break;
        }
    }
}

bool Level::CheckCollision(VECTOR playerPos, VECTOR playerSize, VECTOR& newPos, VECTOR& velocity, bool& onGround, VECTOR& platformVelocity) {
    onGround = false;
    platformVelocity = VGet(0, 0, 0);

    for (auto& block : blocks) {
        if (block.type == BlockType::PLATFORM ||
            block.type == BlockType::INVISIBLE_WALL ||
            block.type == BlockType::MOVING ||
            block.type == BlockType::BUTTON ||
            (block.type == BlockType::DISAPPEARING && block.isActive) ||
            (block.type == BlockType::CRUMBLING && block.isActive)) {

            if (newPos.x < block.pos.x + block.size.x && newPos.x + playerSize.x > block.pos.x &&
                newPos.y < block.pos.y + block.size.y && newPos.y + playerSize.y > block.pos.y &&
                newPos.z < block.pos.z + block.size.z && newPos.z + playerSize.z > block.pos.z) {

                float overlapX = 0, overlapY = 0, overlapZ = 0;

                if (newPos.x < block.pos.x) {
                    overlapX = (newPos.x + playerSize.x) - block.pos.x;
                }
                else {
                    overlapX = (block.pos.x + block.size.x) - newPos.x;
                }

                if (newPos.y < block.pos.y) {
                    overlapY = (newPos.y + playerSize.y) - block.pos.y;
                }
                else {
                    overlapY = (block.pos.y + block.size.y) - newPos.y;
                }

                if (newPos.z < block.pos.z) {
                    overlapZ = (newPos.z + playerSize.z) - block.pos.z;
                }
                else {
                    overlapZ = (block.pos.z + block.size.z) - newPos.z;
                }

                if (overlapY <= overlapX && overlapY <= overlapZ) {
                    if (velocity.y <= 0 && playerPos.y > block.pos.y + block.size.y - 1.5f) {
                        newPos.y = block.pos.y + block.size.y;
                        velocity.y = 0;
                        onGround = true;

                        if (block.type == BlockType::MOVING) {
                            platformVelocity = VGet(
                                block.pos.x - block.prevPos.x,
                                block.pos.y - block.prevPos.y,
                                block.pos.z - block.prevPos.z
                            );
                        }

                        if (block.type == BlockType::DISAPPEARING && block.linkId == 0) {
                            if (block.timer == 0.0f) {
                                block.timer = 1.5f;
                            }
                        }

                        // CRUMBLING - платформа начинает рассыпаться при касании
                        if (block.type == BlockType::CRUMBLING) {
                            if (block.timer == 0.0f) {
                                block.timer = 0.5f; // 0.5 сек до падения
                            }
                        }
                    }
                    else if (velocity.y > 0) {
                        newPos.y = block.pos.y - playerSize.y;
                        velocity.y = 0;
                    }
                }
                else if (overlapX <= overlapZ) {
                    if (newPos.x < block.pos.x) {
                        newPos.x = block.pos.x - playerSize.x;
                    }
                    else {
                        newPos.x = block.pos.x + block.size.x;
                    }
                }
                else {
                    if (newPos.z < block.pos.z) {
                        newPos.z = block.pos.z - playerSize.z;
                    }
                    else {
                        newPos.z = block.pos.z + block.size.z;
                    }
                }

                return true;
            }
        }
    }
    return false;
}

bool Level::CheckWinTrigger(VECTOR playerPos, VECTOR playerSize) const {
    for (const auto& block : blocks) {
        if (block.type == BlockType::TRIGGER) {
            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return true;
            }
        }
    }
    return false;
}

bool Level::CheckDeadlyTrigger(VECTOR playerPos, VECTOR playerSize) const {
    for (const auto& block : blocks) {
        // Обычные шипы
        if (block.type == BlockType::SPIKES) {
            if (block.linkId == 1 && !block.isActive) {
                continue;
            }

            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return true;
            }
        }

        // Выдвижные шипы - убивают только когда активны
        if (block.type == BlockType::RETRACTABLE_SPIKES && block.isActive) {
            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return true;
            }
        }

        // Маятники-топоры (Block 3) - всегда смертельны
        if (block.type == BlockType::PENDULUM_BLADE) {
            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return true;
            }
        }

        // FAKE_SPIKES НЕ убивают - это троллинг!
    }
    return false;
}

void Level::Update(float deltaTime) {
    for (auto& block : blocks) {
        if (block.type == BlockType::MOVING) {
            block.prevPos = block.pos;

            block.timer += deltaTime * 0.3f;

            if (block.timer > 1.0f) {
                block.timer = 1.0f;
                block.isActive = false;
            }
            if (block.timer < 0.0f) {
                block.timer = 0.0f;
                block.isActive = true;
            }

            if (!block.isActive) {
                block.timer -= deltaTime * 0.6f;
            }

            float t = block.timer;
            block.pos.x = block.originalPos.x + (block.moveTarget.x - block.originalPos.x) * t;
            block.pos.y = block.originalPos.y + (block.moveTarget.y - block.originalPos.y) * t;
            block.pos.z = block.originalPos.z + (block.moveTarget.z - block.originalPos.z) * t;
        }

        if (block.type == BlockType::DISAPPEARING) {
            if (block.linkId == 0 && block.timer > 0.0f) {
                block.timer -= deltaTime;

                if (block.timer <= 0.0f) {
                    block.isActive = false;
                    block.timer = -3.0f;
                }
            }

            if (block.linkId == 0 && block.timer < 0.0f) {
                block.timer += deltaTime;

                if (block.timer >= 0.0f) {
                    block.isActive = true;
                    block.timer = 0.0f;
                }
            }
        }

        if (block.type == BlockType::SPIKES && block.linkId == 1) {
            block.timer += deltaTime;

            float cycleTime = 4.0f;
            float phase = fmod(block.timer, cycleTime);

            if (phase < 2.0f) {
                block.isActive = true;
            }
            else {
                block.isActive = false;
            }
        }

        // RETRACTABLE_SPIKES - выдвигаются/прячутся по таймеру
        if (block.type == BlockType::RETRACTABLE_SPIKES) {
            block.timer += deltaTime;

            // Цикл: 2 сек активны (опасны), 2 сек спрятаны (безопасны)
            float cycleTime = 4.0f;
            float phase = fmod(block.timer, cycleTime);

            if (phase < 2.0f) {
                block.isActive = true;  // Шипы выдвинуты - опасно!
            }
            else {
                block.isActive = false; // Шипы спрятаны - можно пройти
            }
        }

        // CRUMBLING - платформы рассыпаются после касания игрока
        // (логика касания будет в CheckCollision)
        if (block.type == BlockType::CRUMBLING) {
            // Если платформа начала рассыпаться (timer > 0)
            if (block.timer > 0.0f) {
                block.timer -= deltaTime;

                // Через 0.5 сек платформа исчезает
                if (block.timer <= 0.0f) {
                    block.isActive = false; // Платформа упала
                    block.timer = -2.0f;    // Время до возрождения
                }
            }

            // Платформа исчезла, ждём возрождения
            if (block.timer < 0.0f) {
                block.timer += deltaTime;

                // Через 2 сек платформа возвращается
                if (block.timer >= 0.0f) {
                    block.isActive = true;
                    block.timer = 0.0f;
                }
            }
        }

        // PENDULUM_BLADE - качающиеся маятники (Block 3)
        if (block.type == BlockType::PENDULUM_BLADE) {
            // Обновляем угол качания используя синусоиду
            block.swingAngle = sin(block.timer * block.swingSpeed) * block.swingRange;

            // Обновляем таймер
            block.timer += deltaTime;

            // Вычисляем длину маятника как расстояние от точки подвеса до начальной позиции
            float length = VSize(VSub(block.pivotPoint, block.originalPos));

            // Вычисляем позицию лезвия на основе угла качания
            block.pos.x = block.pivotPoint.x + sin(block.swingAngle) * length;
            block.pos.y = block.pivotPoint.y - cos(block.swingAngle) * length;
            block.pos.z = block.pivotPoint.z;
        }
    }
}

bool Level::CheckTeleportTrigger(VECTOR playerPos, VECTOR playerSize, VECTOR& teleportTarget) const {
    for (const auto& block : blocks) {
        if (block.type == BlockType::TELEPORT) {
            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {

                for (const auto& otherBlock : blocks) {
                    if (otherBlock.type == BlockType::TELEPORT &&
                        otherBlock.linkId == block.linkId &&
                        &otherBlock != &block) {

                        teleportTarget = VGet(otherBlock.pos.x + otherBlock.size.x / 2,
                            otherBlock.pos.y + otherBlock.size.y + 1,
                            otherBlock.pos.z + otherBlock.size.z / 2);
                        return true;
                    }
                }

                if (block.linkId == 1) {
                    teleportTarget = VGet(25, 7, 0);
                    return true;
                }
                else if (block.linkId == 2) {
                    teleportTarget = VGet(5, 15, 0);
                    return true;
                }
                else if (block.linkId == 3) {
                    teleportTarget = VGet(20, 3, 3);
                    return true;
                }
            }
        }
    }
    return false;
}

void Level::ActivateButton(VECTOR playerPos, VECTOR playerSize, bool keyPressed) {
    for (auto& block : blocks) {
        if (block.type == BlockType::BUTTON) {
            float distance = sqrtf(
                (playerPos.x - (block.pos.x + block.size.x / 2)) * (playerPos.x - (block.pos.x + block.size.x / 2)) +
                (playerPos.z - (block.pos.z + block.size.z / 2)) * (playerPos.z - (block.pos.z + block.size.z / 2))
            );

            if (distance < 3.0f && keyPressed) {
                block.isActive = !block.isActive;

                for (auto& otherBlock : blocks) {
                    if (otherBlock.linkId == block.linkId && otherBlock.type == BlockType::DISAPPEARING) {
                        otherBlock.isActive = block.isActive;
                    }
                }
            }
        }
    }
}

void Level::InitializeModels() {
    ModelManager& modelMgr = ModelManager::GetInstance();

    for (auto& block : blocks) {
        // Определить ModelID по BlockType
        ModelID modelId = static_cast<ModelID>(static_cast<int>(block.type));

        // Если модель загружена для данного типа блока
        if (modelMgr.IsModelLoaded(modelId)) {
            // Создать дубликат модели для этого блока
            block.modelHandle = modelMgr.DuplicateModel(modelId);
            block.useModel = true;

            // Можно задать индивидуальный масштаб модели
            block.modelScale = VGet(
                block.size.x,
                block.size.y,
                block.size.z
            );
        }
        else {
            // Модель не загружена - будут использоваться примитивы
            block.modelHandle = -1;
            block.useModel = false;
        }
    }
}

void Level::DrawSkybox(VECTOR cameraPos) const {
    ModelManager& modelMgr = ModelManager::GetInstance();

    // Отрисовать скайбокс для текущего блока уровней
    if (modelMgr.IsSkyboxLoaded(blockId)) {
        modelMgr.DrawSkybox(blockId, cameraPos);
    }
}

// Block 3: Проверка гравитационных зон
float Level::CheckGravityZone(VECTOR playerPos, VECTOR playerSize) const {
    for (const auto& block : blocks) {
        if (block.type == BlockType::GRAVITY_ZONE) {
            // Проверка нахождения игрока в зоне
            if (playerPos.x < block.pos.x + block.size.x && playerPos.x + playerSize.x > block.pos.x &&
                playerPos.y < block.pos.y + block.size.y && playerPos.y + playerSize.y > block.pos.y &&
                playerPos.z < block.pos.z + block.size.z && playerPos.z + playerSize.z > block.pos.z) {
                return block.gravityMultiplier; // Возвращаем множитель гравитации
            }
        }
    }
    return 1.0f; // Нормальная гравитация по умолчанию
}