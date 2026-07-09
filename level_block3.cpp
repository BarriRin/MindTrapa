#include "Level.h"

void Level::LoadBlock3(int id) {
    switch (id) {
    case 21: // Level 21 - Вводим LOW GRAVITY (низкая гравитация)
        playerSpawn = VGet(0, 1, 0);

        // Стартовая платформа
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Обычная платформа перед зоной
        blocks.push_back(Block(VGet(5, 0, -2), VGet(4, 1, 4), BlockType::PLATFORM));

        // Зона с низкой гравитацией (floaty jumps)
        blocks.push_back(Block(VGet(11, 0, -3), VGet(15, 15, 6), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.3f;

        // Платформы в зоне низкой гравитации
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
        blocks.back().gravityMultiplier = 2.5f;

        // Платформы в зоне - прыжки низкие
        blocks.push_back(Block(VGet(6, 0, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(10, 0, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(14, 0, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(18, 0, -2), VGet(3, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 1, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        // Финиш
        blocks.push_back(Block(VGet(27, 2, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(29, 3, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 23: // Level 23 - Вводим PENDULUM BLADE (перпендикулярно к пути)
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // === Коридор 1: один медленный маятник ===
        // Маятник перпендикулярен движению (+X): size.z=4 покрывает всю ширину Z:-2..2
        // Игрок не может обойти сбоку — только тайминг
        blocks.push_back(Block(VGet(5, 0, -2), VGet(17, 1, 4), BlockType::PLATFORM)); // X:5..22
        blocks.push_back(Block(VGet(11, 1, -2), VGet(1.5f, 1.5f, 4.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(11, 8, -2); // pivot.z = pos.z (Z не изменяется при качании)
        blocks.back().swingSpeed = 1.2f;
        blocks.back().swingRange = DX_PI_F / 4.0f;

        // Площадка отдыха
        blocks.push_back(Block(VGet(22, 1, -3), VGet(6, 1, 6), BlockType::PLATFORM));

        // === Коридор 2: более быстрый маятник ===
        blocks.push_back(Block(VGet(30, 1, -2), VGet(18, 1, 4), BlockType::PLATFORM)); // X:30..48
        blocks.push_back(Block(VGet(38, 2, -2), VGet(1.5f, 1.5f, 4.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(38, 9, -2);
        blocks.back().swingSpeed = 2.0f;
        blocks.back().swingRange = DX_PI_F / 3.5f;

        // Финиш
        blocks.push_back(Block(VGet(50, 2, -3), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(52, 3, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 24: // Level 24 - КОМБО: Low Gravity + Pendulum (перпендикулярно к пути)
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Расширенная зона низкой гравитации (X:5..43, Z:-3..3)
        blocks.push_back(Block(VGet(5, 0, -3), VGet(38, 22, 6), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.4f;

        // P1
        blocks.push_back(Block(VGet(6, 1, -2), VGet(4, 1, 4), BlockType::PLATFORM));
        // P2
        blocks.push_back(Block(VGet(12, 5, -2), VGet(4, 1, 4), BlockType::PLATFORM)); // поверхность Y=6

        // === Маятник 1 — перпендикулярен пути, size.z=6 покрывает всю ширину Z:-3..3 ===
        // Качается в плоскости XY, игрок не может обойти сбоку — только тайминг
        blocks.push_back(Block(VGet(17, 3, -3), VGet(1.5f, 1.5f, 6.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(17, 12, -3); // pivot.z = pos.z (Z не изменяется при качании)
        blocks.back().swingSpeed = 1.5f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        // P3
        blocks.push_back(Block(VGet(20, 8, -2), VGet(4, 1, 4), BlockType::PLATFORM)); // поверхность Y=9
        // P4
        blocks.push_back(Block(VGet(26, 11, -2), VGet(4, 1, 4), BlockType::PLATFORM)); // поверхность Y=12

        // === Маятник 2 — быстрее, сдвинут по фазе ===
        blocks.push_back(Block(VGet(31, 9, -3), VGet(1.5f, 1.5f, 6.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(31, 17, -3);
        blocks.back().swingSpeed = 1.8f;
        blocks.back().swingRange = DX_PI_F / 3.5f;
        blocks.back().timer = 1.5f; // смещение фазы

        // P5
        blocks.push_back(Block(VGet(34, 13, -2), VGet(4, 1, 4), BlockType::PLATFORM)); // поверхность Y=14
        // P6
        blocks.push_back(Block(VGet(40, 15, -2), VGet(4, 1, 4), BlockType::PLATFORM)); // поверхность Y=16

        // Финиш (за зоной гравитации)
        blocks.push_back(Block(VGet(45, 16, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(47, 17, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 25: // Level 25 - REVERSE GRAVITY (обратная гравитация!)
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Платформа перед зоной
        blocks.push_back(Block(VGet(5, 0, -2), VGet(4, 1, 4), BlockType::PLATFORM));

        // Зона с ОБРАТНОЙ гравитацией
        blocks.push_back(Block(VGet(11, 0, -3), VGet(18, 18, 6), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = -1.2f;

        // "Потолок" теперь пол - платформы вверху
        blocks.push_back(Block(VGet(12, 12, -2), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(17, 12, -2), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(23, 12, -2), VGet(4, 1, 4), BlockType::PLATFORM));

        // Выход из зоны — посадочная платформа
        blocks.push_back(Block(VGet(30, 0, -3), VGet(4, 1, 6), BlockType::PLATFORM));

        // Прыжок через разрыв к финишу (нормальная гравитация уже работает)
        blocks.push_back(Block(VGet(37, 0, -3), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(40, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 26: // Level 26 - СЛОЖНЫЕ ПАТТЕРНЫ МАЯТНИКОВ (4 маятника, перпендикулярно пути)
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Длинный коридор с четырьмя маятниками (Z:-2..2, ширина 4)
        blocks.push_back(Block(VGet(5, 0, -2), VGet(45, 1, 4), BlockType::PLATFORM));

        // Маятник 1 — медленный
        // size.z=4 покрывает всю ширину коридора Z:-2..2, обойти нельзя
        blocks.push_back(Block(VGet(10, 1, -2), VGet(1.5f, 1.5f, 4.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(10, 9, -2); // pivot.z = pos.z
        blocks.back().swingSpeed = 1.0f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        // Маятник 2 — быстрый, смещение фазы
        blocks.push_back(Block(VGet(19, 1, -2), VGet(1.5f, 1.5f, 4.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(19, 10, -2);
        blocks.back().swingSpeed = 2.2f;
        blocks.back().swingRange = DX_PI_F / 2.5f;
        blocks.back().timer = 1.5f;

        // Маятник 3 — средний, широкий размах
        blocks.push_back(Block(VGet(29, 1, -2), VGet(2.0f, 2.0f, 4.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(29, 11, -2);
        blocks.back().swingSpeed = 1.6f;
        blocks.back().swingRange = DX_PI_F / 2.2f;
        blocks.back().timer = 3.0f;

        // Маятник 4 — самый быстрый, хаотичная фаза
        blocks.push_back(Block(VGet(39, 1, -2), VGet(1.5f, 1.5f, 4.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(39, 9, -2);
        blocks.back().swingSpeed = 2.5f;
        blocks.back().swingRange = DX_PI_F / 2.8f;
        blocks.back().timer = 0.8f;

        // Финиш
        blocks.push_back(Block(VGet(52, 1, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(54, 2, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 27: // Level 27 - МУЛЬТИ-ЗОНЫ ГРАВИТАЦИИ + МАЯТНИКИ
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Секция 1: Низкая гравитация + маятник
        blocks.push_back(Block(VGet(5, 0, -2), VGet(12, 12, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.35f;

        blocks.push_back(Block(VGet(6, 1, -1), VGet(3, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(10, 1, -1), VGet(1.5f, 1.5f, 2.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(10, 10, -1);
        blocks.back().swingSpeed = 1.4f;
        blocks.back().swingRange = DX_PI_F / 3.5f;

        blocks.push_back(Block(VGet(13, 6, -1), VGet(3, 1, 2), BlockType::PLATFORM));

        // Секция 2: Высокая гравитация + быстрый маятник
        blocks.push_back(Block(VGet(18, -1, -2), VGet(12, 8, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 2.8f;

        blocks.push_back(Block(VGet(19, 0, -1), VGet(3, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(23, 1, -1), VGet(1.5f, 1.5f, 2.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(23, 6, -1);
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

        // Платформа 1
        blocks.push_back(Block(VGet(5, 0, -1), VGet(4, 1, 2), BlockType::PLATFORM));

        // ТРОЛЛИНГ: Неожиданная высокая гравитация
        blocks.push_back(Block(VGet(11, 0, -2), VGet(8, 6, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 3.5f;

        // Настоящий путь - сбоку с низкой платформой
        blocks.push_back(Block(VGet(12, -1, -1), VGet(3, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(16, -1, -1), VGet(3, 1, 2), BlockType::PLATFORM));

        // Платформа 2
        blocks.push_back(Block(VGet(21, 0, -2), VGet(4, 1, 4), BlockType::PLATFORM));

        // ТРОЛЛИНГ 2: Кажется, что низкая гравитация, но это ОБРАТНАЯ!
        blocks.push_back(Block(VGet(27, 0, -2), VGet(10, 12, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = -1.5f;

        // Платформы вверху
        blocks.push_back(Block(VGet(29, 10, -1), VGet(3, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(33, 10, -1), VGet(3, 1, 2), BlockType::PLATFORM));

        // Посадочная платформа после зоны 2
        blocks.push_back(Block(VGet(39, 0, -3), VGet(6, 1, 6), BlockType::PLATFORM)); // X:39..45

        // ТРОЛЛИНГ 3: Внезапно высокая гравитация — прыжки стали очень короткими
        blocks.push_back(Block(VGet(45, 0, -2), VGet(12, 8, 4), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 3.0f;

        blocks.push_back(Block(VGet(46, 0, -1), VGet(2, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(50, 0, -1), VGet(2, 1, 2), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(54, 0, -1), VGet(2, 1, 2), BlockType::PLATFORM));

        // Финиш
        blocks.push_back(Block(VGet(59, 0, -3), VGet(5, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(61, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 29: // Level 29 - ИСПЫТАНИЕ ЛЕЗВИЙ + ГРАВИТАЦИЯ (сложно!)
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // Зона низкой гравитации на весь уровень
        blocks.push_back(Block(VGet(5, 0, -3), VGet(45, 20, 6), BlockType::GRAVITY_ZONE));
        blocks.back().gravityMultiplier = 0.4f;

        // Серия платформ с маятниками
        blocks.push_back(Block(VGet(6, 1, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        // Маятники перпендикулярны пути: size.z=6 покрывает всю ширину зоны Z:-3..3
        blocks.push_back(Block(VGet(11, 2, -3), VGet(1.5f, 1.5f, 6.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(11, 12, -3); // pivot.z = pos.z
        blocks.back().swingSpeed = 1.8f;
        blocks.back().swingRange = DX_PI_F / 3.0f;

        blocks.push_back(Block(VGet(14, 4, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(19, 5, -3), VGet(1.5f, 1.5f, 6.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(19, 14, -3);
        blocks.back().swingSpeed = 2.0f;
        blocks.back().swingRange = DX_PI_F / 2.5f;
        blocks.back().timer = 1.2f;

        blocks.push_back(Block(VGet(22, 7, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(27, 8, -3), VGet(2.0f, 2.0f, 6.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(27, 16, -3);
        blocks.back().swingSpeed = 1.3f;
        blocks.back().swingRange = DX_PI_F / 2.0f;
        blocks.back().timer = 2.5f;

        blocks.push_back(Block(VGet(30, 10, -2), VGet(3, 1, 4), BlockType::PLATFORM));

        blocks.push_back(Block(VGet(35, 11, -3), VGet(1.5f, 1.5f, 6.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(35, 18, -3);
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
        blocks.push_back(Block(VGet(11, 3, -1), VGet(1.5f, 1.5f, 2.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(11, 12, -1);
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
        blocks.push_back(Block(VGet(43, 8, -1), VGet(1.5f, 1.5f, 2.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(43, 2, -1);
        blocks.back().swingSpeed = 1.7f;
        blocks.back().swingRange = DX_PI_F / 3.5f;

        blocks.push_back(Block(VGet(49, 7, -1), VGet(1.5f, 1.5f, 2.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(49, 1, -1);
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
        blocks.push_back(Block(VGet(77, 5, -1), VGet(1.5f, 1.5f, 3.0f), BlockType::PENDULUM_BLADE));
        blocks.back().pivotPoint = VGet(77, 14, -1);
        blocks.back().swingSpeed = 2.0f;
        blocks.back().swingRange = DX_PI_F / 2.5f;

        // ФИНИШ
        blocks.push_back(Block(VGet(80, 9, -3), VGet(7, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(83, 10, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;
    }
}
