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

    // ========== BLOCK 4: DARKNESS (Levels 31-40) ==========

    case 31: // Level 31 - Туториал темноты: зигзаг с разной высотой
        playerSpawn = VGet(0, 1, 0);

        // Стартовая платформа
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Путь: зигзаг по Z, с увеличенными расстояниями и разной высотой
        blocks.push_back(Block(VGet(8,  0,  2), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(15, 1, -4), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 0,  3), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(29, 2, -3), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(36, 1,  1), VGet(3, 1, 3), BlockType::PLATFORM));

        // Финиш
        blocks.push_back(Block(VGet(43, 1, -2), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(45, 2,  0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 32: // Level 32 - Строгий зигзаг: большой разброс по Z и высоте
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Зигзаг: чётные — левая сторона (Z=-5), нечётные — правая (Z=4), высота нарастает
        blocks.push_back(Block(VGet(8,  0, -5), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(15, 2,  4), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 1, -5), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(29, 4,  4), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(36, 2, -4), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(43, 5,  3), VGet(3, 1, 3), BlockType::PLATFORM));

        // Финиш
        blocks.push_back(Block(VGet(50, 5, -1), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(52, 6,  1), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 33: // Level 33 - Шахматный порядок: реальные/фейковые вперемешку, выбор на каждом шаге
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // 5 колонок × 3 ряда (Z=-5, 0, +5), шахматный порядок реальных/фейков
        // Колонка 1 (X=8, Y=0): L=REAL, C=FAKE, R=REAL
        blocks.push_back(Block(VGet(8,  0, -5), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(8,  0,  0), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(8,  0,  4), VGet(3, 1, 3), BlockType::PLATFORM));

        // Колонка 2 (X=15, Y=1): L=FAKE, C=REAL, R=FAKE
        blocks.push_back(Block(VGet(15, 1, -5), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(15, 1,  0), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(15, 1,  4), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));

        // Колонка 3 (X=22, Y=2): L=REAL, C=FAKE, R=REAL
        blocks.push_back(Block(VGet(22, 2, -5), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 2,  0), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(22, 2,  5), VGet(3, 1, 3), BlockType::PLATFORM));

        // Колонка 4 (X=29, Y=1): L=FAKE, C=REAL, R=FAKE
        blocks.push_back(Block(VGet(29, 1, -5), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(29, 1,  0), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(29, 1,  5), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));

        // Колонка 5 (X=36, Y=3): L=REAL, C=FAKE, R=REAL
        blocks.push_back(Block(VGet(36, 3, -4), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(36, 3,  1), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(36, 3,  5), VGet(3, 1, 3), BlockType::PLATFORM));

        // Финиш
        blocks.push_back(Block(VGet(43, 3, -2), VGet(5, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(45, 4,  0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 34: // Level 34 — Дуга: право (+X) → вверх (+Y) → назад (-X), 2 обманки
        // Секция 1: Z≈0, Секция 3: Z≈8 — нет XZ-перекрытий.
        // Секция 3 и финиш на top=7: от старта (Y=0) max прыжок = 5.3 — недостижимы напрямую.
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-4, -1, -4), VGet(8, 1, 8), BlockType::PLATFORM));

        // Зона темноты Block 4
        blocks.push_back(Block(VGet(5, -2, -8), VGet(52, 11, 24), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 5.0f));

        // === СЕКЦИЯ 1: ВПРАВО (+X), Z≈0, Y медленно растёт ===
        blocks.push_back(Block(VGet(12,  0,  0), VGet(4, 1, 4), BlockType::PLATFORM));     // P1, top=1
        blocks.push_back(Block(VGet(22,  1,  0), VGet(4, 1, 4), BlockType::PLATFORM));     // P2, top=2
        blocks.push_back(Block(VGet(22,  1, -6), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));// обманка #1: −Z
        blocks.push_back(Block(VGet(32,  2,  0), VGet(4, 1, 4), BlockType::PLATFORM));     // P3, top=3

        // === ПИК: большой прыжок вверх+вправо, Z расширяется к 8 ===
        blocks.push_back(Block(VGet(44,  6,  1), VGet(5, 1, 8), BlockType::PLATFORM));     // P4, top=7, Z:1..9
        blocks.push_back(Block(VGet(51,  6,  1), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));// обманка #2: перелёт

        // === СЕКЦИЯ 3: НАЗАД (-X), Z≈8, top=7 (недостижимо со старта) ===
        blocks.push_back(Block(VGet(34,  6,  7), VGet(4, 1, 4), BlockType::PLATFORM));     // P5, top=7
        blocks.push_back(Block(VGet(24,  6,  7), VGet(4, 1, 4), BlockType::PLATFORM));     // P6, top=7
        blocks.push_back(Block(VGet(14,  6,  7), VGet(4, 1, 4), BlockType::PLATFORM));     // P7, top=7

        // Финиш: top=7, Z:7..13 — нет прямого прыжка со старта
        blocks.push_back(Block(VGet( 4,  6,  7), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet( 6,  7, 10), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 35: // Level 35 — 7 островов по спирали, паттерн Л-Л-П-П-Л-П-Л
        // Острова разнесены на 20+ units (C-C) — нельзя допрыгнуть, только по телепортам.
        // Остров 1 в 5 units от старта — доходим пешком/прыжком.
        // Arrival-телепорты зарыты (pos.y = platform.pos.y): игрок не триггерит стоя.
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-4, -1, -4), VGet(8, 1, 8), BlockType::PLATFORM));

        // Зона темноты Block 4
        blocks.push_back(Block(VGet(3, -3, -25), VGet(62, 13, 60), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 6.0f));

        // === ОСТРОВ 1: center(12,0,0), surface=0, ЛЕВЫЙ=верный (linkId=21) ===
        // От старта: X gap = 9-4 = 5 units — допрыгнуть можно
        blocks.push_back(Block(VGet( 9, -1, -3), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(10,  0, -2), VGet(2, 1, 2), BlockType::TELEPORT, 21)); // ЛЕВЫЙ = верный
        blocks.push_back(Block(VGet(13,  0,  1), VGet(2, 1, 2), BlockType::TELEPORT,  0)); // правый = спавн

        // === ОСТРОВ 2: center(36,1,-20), surface=1, ЛЕВЫЙ=верный (linkId=22) ===
        // C-C от о.1: ~28 units — нельзя допрыгнуть
        blocks.push_back(Block(VGet(33,  0, -23), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(36,  0, -20), VGet(1, 1, 1), BlockType::TELEPORT, 21)); // arrival от о.1
        blocks.push_back(Block(VGet(34,  1, -22), VGet(2, 1, 2), BlockType::TELEPORT, 22)); // ЛЕВЫЙ = верный
        blocks.push_back(Block(VGet(37,  1, -19), VGet(2, 1, 2), BlockType::TELEPORT,  0)); // правый = спавн

        // === ОСТРОВ 3: center(56,2,-10), surface=2, ПРАВЫЙ=верный (linkId=23) ===
        blocks.push_back(Block(VGet(53,  1, -13), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(56,  1, -10), VGet(1, 1, 1), BlockType::TELEPORT, 22)); // arrival от о.2
        blocks.push_back(Block(VGet(54,  2, -12), VGet(2, 1, 2), BlockType::TELEPORT,  0)); // левый = спавн
        blocks.push_back(Block(VGet(57,  2,  -9), VGet(2, 1, 2), BlockType::TELEPORT, 23)); // ПРАВЫЙ = верный

        // === ОСТРОВ 4: center(58,3,14), surface=3, ПРАВЫЙ=верный (linkId=24) ===
        blocks.push_back(Block(VGet(55,  2, 11), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(58,  2, 14), VGet(1, 1, 1), BlockType::TELEPORT, 23)); // arrival от о.3
        blocks.push_back(Block(VGet(56,  3, 11), VGet(2, 1, 2), BlockType::TELEPORT,  0)); // левый = спавн
        blocks.push_back(Block(VGet(59,  3, 15), VGet(2, 1, 2), BlockType::TELEPORT, 24)); // ПРАВЫЙ = верный

        // === ОСТРОВ 5: center(38,4,30), surface=4, ЛЕВЫЙ=верный (linkId=25) ===
        blocks.push_back(Block(VGet(35,  3, 27), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(38,  3, 30), VGet(1, 1, 1), BlockType::TELEPORT, 24)); // arrival от о.4
        blocks.push_back(Block(VGet(36,  4, 27), VGet(2, 1, 2), BlockType::TELEPORT, 25)); // ЛЕВЫЙ = верный
        blocks.push_back(Block(VGet(39,  4, 31), VGet(2, 1, 2), BlockType::TELEPORT,  0)); // правый = спавн

        // === ОСТРОВ 6: center(16,5,22), surface=5, ПРАВЫЙ=верный (linkId=26) ===
        blocks.push_back(Block(VGet(13,  4, 19), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(16,  4, 22), VGet(1, 1, 1), BlockType::TELEPORT, 25)); // arrival от о.5
        blocks.push_back(Block(VGet(14,  5, 19), VGet(2, 1, 2), BlockType::TELEPORT,  0)); // левый = спавн
        blocks.push_back(Block(VGet(17,  5, 23), VGet(2, 1, 2), BlockType::TELEPORT, 26)); // ПРАВЫЙ = верный

        // === ОСТРОВ 7: center(12,6,-10), surface=6, ЛЕВЫЙ=верный (linkId=27) ===
        // top=6 > max_jump(5.3) от старта — нельзя прыгнуть со старта несмотря на схожий X
        blocks.push_back(Block(VGet( 9,  5, -13), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(12,  5, -10), VGet(1, 1, 1), BlockType::TELEPORT, 26)); // arrival от о.6
        blocks.push_back(Block(VGet(10,  6, -13), VGet(2, 1, 2), BlockType::TELEPORT, 27)); // ЛЕВЫЙ = верный
        blocks.push_back(Block(VGet(13,  6,  -9), VGet(2, 1, 2), BlockType::TELEPORT,  0)); // правый = спавн

        // === ФИНИШ: center(36,8,2) ===
        blocks.push_back(Block(VGet(33,  7, -1), VGet(8, 1, 7), BlockType::PLATFORM)); // top=8
        blocks.push_back(Block(VGet(36,  7,  2), VGet(1, 1, 1), BlockType::TELEPORT, 27)); // arrival от о.7
        blocks.push_back(Block(VGet(35,  8,  0), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 36: // Level 36 - LIGHT_PULSE + RETRACTABLE_SPIKES
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Зона мерцания: период 4 сек (быстро!)
        blocks.push_back(Block(VGet(5, -1, -3), VGet(32, 10, 6), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 4.0f));

        // Коридор с выдвижными шипами — нужно угадать фазу в темноте
        blocks.push_back(Block(VGet(6, 0, -2), VGet(28, 1, 4), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(9, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));
        blocks.push_back(Block(VGet(13, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f)); // сдвиг фазы
        blocks.push_back(Block(VGet(17, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 1.0f));
        blocks.push_back(Block(VGet(21, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 3.0f));
        blocks.push_back(Block(VGet(25, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.5f));

        blocks.push_back(Block(VGet(37, 1, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(39, 2, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 37: // Level 37 - Два сегмента: +X с прыжками, затем +Z с перпендикулярными топорами
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-4, -1, -4), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(5, -2, -4), VGet(50, 18, 12), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 6.0f));

        // === Сегмент A: движение +X, коридор Z:-2.5..0.5, высота растёт ===
        blocks.push_back(Block(VGet(7, 0, -1), VGet(5, 1, 3), BlockType::PLATFORM));   // PlatA1, top=1

        blocks.push_back(Block(VGet(13, 2, -1), VGet(1.5f, 1.5f, 2.0f), BlockType::PENDULUM_BLADE)); // Blade1 — покрывает весь Z коридора
        blocks.back().pivotPoint = VGet(13, 9, -1);
        blocks.back().swingSpeed = 1.5f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        blocks.push_back(Block(VGet(16, 3, -1), VGet(4, 1, 3), BlockType::PLATFORM));  // PlatA2, top=4 (+3Y прыжок)

        blocks.push_back(Block(VGet(22, 5, -1), VGet(1.5f, 1.5f, 2.0f), BlockType::PENDULUM_BLADE)); // Blade2
        blocks.back().pivotPoint = VGet(22, 11, -1);
        blocks.back().swingSpeed = 2.0f;
        blocks.back().swingRange = DX_PI_F / 2.5f;
        blocks.back().timer = 1.5f;

        blocks.push_back(Block(VGet(25, 6, -1), VGet(4, 1, 3), BlockType::PLATFORM));  // PlatA3, top=7 (+3Y прыжок)

        blocks.push_back(Block(VGet(31, 7, -1), VGet(1.5f, 1.5f, 2.0f), BlockType::PENDULUM_BLADE)); // Blade3
        blocks.back().pivotPoint = VGet(31, 13, -1);
        blocks.back().swingSpeed = 1.8f;
        blocks.back().swingRange = DX_PI_F / 2.8f;
        blocks.back().timer = 2.5f;

        blocks.push_back(Block(VGet(33, 6, -1), VGet(4, 1, 9), BlockType::PLATFORM));  // Corner, Z:-1..8, top=7

        // === Сегмент B: движение +Z, коридор X:30..33, топоры перпендикулярны движению ===
        blocks.push_back(Block(VGet(30, 7, 10), VGet(3, 1, 4), BlockType::PLATFORM));  // PlatB1, top=8

        blocks.push_back(Block(VGet(30.0f, 8, 16), VGet(3.0f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE)); // Blade4 — X-покрытие коридора X:30..33
        blocks.back().pivotPoint = VGet(31.5f, 14, 16);
        blocks.back().swingSpeed = 2.0f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        blocks.push_back(Block(VGet(30, 8, 19), VGet(3, 1, 4), BlockType::PLATFORM));  // PlatB2, top=9

        blocks.push_back(Block(VGet(30.0f, 9, 25), VGet(3.0f, 1.5f, 1.5f), BlockType::PENDULUM_BLADE)); // Blade5 — X:30..33
        blocks.back().pivotPoint = VGet(31.5f, 15, 25);
        blocks.back().swingSpeed = 1.7f;
        blocks.back().swingRange = DX_PI_F / 3.0f;
        blocks.back().timer = 1.5f;

        blocks.push_back(Block(VGet(28, 8, 28), VGet(5, 1, 5), BlockType::PLATFORM));  // Finish, top=9
        blocks.push_back(Block(VGet(30, 9, 30), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 38: // Level 38 - Зигзаг при 0.3g: нелинейный путь, нельзя пройти одним прыжком
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-4, -1, -4), VGet(8, 1, 8), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(-4, 0, -6), VGet(69, 25, 20), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.3f;
        blocks.push_back(Block(VGet(-4, 0, -6), VGet(69, 25, 20), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 5.0f));

        // Зигзаг: P1(+X) → P2(+Z) → P3(+X) → P4(-Z) → P5(+X) → Finish
        blocks.push_back(Block(VGet(8,  1,  -2), VGet(5, 1, 5), BlockType::PLATFORM));  // P1, top=2
        blocks.push_back(Block(VGet(8,  4,  10), VGet(5, 1, 5), BlockType::PLATFORM));  // P2, top=5
        blocks.push_back(Block(VGet(22, 8,   8), VGet(5, 1, 5), BlockType::PLATFORM));  // P3, top=9
        blocks.push_back(Block(VGet(22, 11, -4), VGet(5, 1, 5), BlockType::PLATFORM));  // P4, top=12
        blocks.push_back(Block(VGet(36, 14, -2), VGet(5, 1, 5), BlockType::PLATFORM));  // P5, top=15

        blocks.push_back(Block(VGet(44, 14, -3), VGet(6, 1, 6), BlockType::PLATFORM));  // Finish, top=15
        blocks.push_back(Block(VGet(46, 15,  0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 39: // Level 39 - Большая карта: свет открывает секции по очереди
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Секция A: медленно (10 сек) — можно изучить маршрут
        blocks.push_back(Block(VGet(7, -1, -5), VGet(24, 14, 10), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 10.0f));
        blocks.push_back(Block(VGet(8,  0, -3), VGet(4, 1, 6), BlockType::PLATFORM));  // gap=5 от старта
        blocks.push_back(Block(VGet(17, 2, -3), VGet(4, 1, 6), BlockType::PLATFORM));  // gap=5, ΔY=+2
        blocks.push_back(Block(VGet(26, 4, -2), VGet(3, 1, 4), BlockType::CRUMBLING)); // gap=5, ΔY=+2

        // Секция B: быстро (3 сек) — надо действовать быстро
        blocks.push_back(Block(VGet(31, -1, -5), VGet(24, 14, 10), BlockType::LIGHT_PULSE_ZONE,
            0, false, VGet(0,0,0), VGet(0,0,0), 3.0f));
        blocks.push_back(Block(VGet(33, 4, -3), VGet(4, 1, 6), BlockType::PLATFORM));  // gap=4 от crumbling
        blocks.push_back(Block(VGet(41, 4, -3), VGet(3, 1, 4), BlockType::MOVING,
            0, true, VGet(41, 4, -3), VGet(41, 8, -3), 1.5f));                         // gap=4
        blocks.push_back(Block(VGet(47, 4, -3), VGet(4, 1, 6), BlockType::PLATFORM));  // gap=3 от moving end

        // Секция C: средний (6 сек), финальный рывок
        blocks.push_back(Block(VGet(55, -1, -5), VGet(28, 14, 10), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 6.0f));
        blocks.push_back(Block(VGet(57, 4, -3), VGet(3, 1, 4), BlockType::PLATFORM));  // gap=6 от PlatB2
        blocks.push_back(Block(VGet(65, 6, -2), VGet(3, 1, 4), BlockType::CRUMBLING)); // gap=5, ΔY=+2
        blocks.push_back(Block(VGet(73, 7, -3), VGet(4, 1, 4), BlockType::PLATFORM));  // gap=5, ΔY=+1

        blocks.push_back(Block(VGet(82, 4, -3), VGet(5, 1, 6), BlockType::PLATFORM));  // gap=5, ΔY=-3
        blocks.push_back(Block(VGet(84, 5, 0),  VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 40: // Level 40 - ФИНАЛ Block 4: всё вместе!
        playerSpawn = VGet(0, 1, 0);

        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // === СЕКЦИЯ 1: Мерцание + движущаяся платформа ===
        blocks.push_back(Block(VGet(7, -1, -4), VGet(22, 12, 8), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 5.0f));
        blocks.push_back(Block(VGet(9,  0, -2), VGet(3, 1, 4), BlockType::PLATFORM));     // gap=6 от старта
        blocks.push_back(Block(VGet(15, 1, -2), VGet(4, 1, 3), BlockType::MOVING,
            0, true, VGet(15, 1, -2), VGet(23, 1, -2), 2.0f));                            // gap=3 от Plat1

        // === СЕКЦИЯ 2: Мерцание + выдвижные шипы ===
        blocks.push_back(Block(VGet(29, -1, -4), VGet(18, 10, 8), BlockType::LIGHT_PULSE_ZONE,
            0, false, VGet(0,0,0), VGet(0,0,0), 4.0f));
        blocks.push_back(Block(VGet(30, 0, -2), VGet(14, 1, 4), BlockType::PLATFORM));    // gap=3 от moving end
        blocks.push_back(Block(VGet(32, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));
        blocks.push_back(Block(VGet(36, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));
        blocks.push_back(Block(VGet(40, 1, -2), VGet(2, 1, 4), BlockType::RETRACTABLE_SPIKES,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 1.0f));

        // === СЕКЦИЯ 3: Гравитация 0.35x + мерцание + МАЯТНИК ПЕРПЕНДИКУЛЯРНО ДВИЖЕНИЮ ===
        // Игрок поворачивает на +Z — маятник качается в XY и перекрывает ширину коридора по X
        blocks.push_back(Block(VGet(46, -1, -5), VGet(12, 18, 48), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.35f;
        blocks.push_back(Block(VGet(46, -1, -5), VGet(12, 18, 48), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 6.0f));
        blocks.push_back(Block(VGet(48, 0, -2), VGet(5, 1, 8), BlockType::PLATFORM));     // Entry, gap=4, Z:-2..6
        blocks.push_back(Block(VGet(48, 2, 10), VGet(5, 1, 5), BlockType::PLATFORM));     // PlatS3a, ΔZ=4, ΔY=+2

        // Маятник — X-размах покрывает коридор X:48..53, игрок идёт в +Z → топор поперёк пути
        blocks.push_back(Block(VGet(50.5f, 3, 17), VGet(6, 2, 1.5f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(50.5f, 14, 17);
        blocks.back().swingSpeed = 1.8f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        blocks.push_back(Block(VGet(48, 3, 20), VGet(5, 1, 5), BlockType::PLATFORM));     // PlatS3b, за маятником
        blocks.push_back(Block(VGet(48, 5, 27), VGet(5, 1, 5), BlockType::CRUMBLING));    // ΔY=+2, gap=2

        // === ФИНИШ ===
        blocks.push_back(Block(VGet(46, 5, 35), VGet(7, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(49, 6, 40), VGet(2, 1, 2), BlockType::TRIGGER));
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

        case BlockType::LIGHT_PULSE_ZONE:
            // Зона мигающего света — очень прозрачная, чтобы игрок видел границы
            SetWriteZBuffer3D(FALSE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 25);
            DrawCube3D(block.pos, VAdd(block.pos, block.size),
                GetColor(80, 60, 120), GetColor(80, 60, 120), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            SetWriteZBuffer3D(TRUE);
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

        // LIGHT_PULSE_ZONE - мигающий свет [Block 4]
        // moveSpeed = период цикла (сек), isActive = true когда светло
        if (block.type == BlockType::LIGHT_PULSE_ZONE) {
            block.timer += deltaTime;
            if (block.timer >= block.moveSpeed) {
                block.timer -= block.moveSpeed;
            }
            block.isActive = (block.timer < block.moveSpeed * 0.5f); // первая половина = светло
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

                // linkId=0: неправильный выбор — возврат на спавн уровня
                if (block.linkId == 0) {
                    teleportTarget = VGet(playerSpawn.x + 0.5f, playerSpawn.y + 2.0f, playerSpawn.z + 0.5f);
                    return true;
                }

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

bool Level::IsAnyLightPulseZoneActive() const {
    for (const auto& block : blocks) {
        if (block.type == BlockType::LIGHT_PULSE_ZONE && block.isActive)
            return true;
    }
    return false;
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