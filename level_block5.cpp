#include "level.h"

void Level::LoadBlock5(int id) {
    switch (id) {

    case 41: // Level 41 - ICE: Z-образный зигзаг — тормози перед поворотами
        // Три полосы льда с двумя поворотами на 90°.
        // На каждом повороте: если несёшься — вылетишь в пропасть.
        // Лёд скользче обычного (0.992). Единственный выход — BOUNCE_PAD в конце.
        playerSpawn = VGet(0, 1, 0);

        // Стартовая платформа
        blocks.push_back(Block(VGet(-4, -1, -2), VGet(9, 1, 6), BlockType::PLATFORM));

        // Полоса 1 (+X направление): X:5..23, Z:0..4, ширина 4
        { Block b(VGet(5, -1, 0), VGet(18, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.992f; blocks.push_back(b); }
        // Поворот 1 (X:21..29, Z:-2..7): тормози по X, набирай Z
        { Block b(VGet(21, -1, -2), VGet(8, 1, 9), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.992f; blocks.push_back(b); }
        // Полоса 2 (+Z направление): X:22..26, Z:4..22, ширина 4
        { Block b(VGet(22, -1, 4), VGet(4, 1, 18), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.992f; blocks.push_back(b); }
        // Поворот 2 (X:19..29, Z:20..28): тормози по Z, набирай X
        { Block b(VGet(19, -1, 20), VGet(10, 1, 8), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.992f; blocks.push_back(b); }
        // Полоса 3 (+X направление): X:27..52, Z:22..26, ширина 4
        { Block b(VGet(27, -1, 22), VGet(25, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.992f; blocks.push_back(b); }

        // BOUNCE_PAD в конце полосы 3 — единственный выход наверх
        blocks.push_back(Block(VGet(52, -1, 23), VGet(4, 1, 3), BlockType::BOUNCE_PAD));

        // Финальная платформа (top=7): достижима батутом + инерция льда
        blocks.push_back(Block(VGet(58, 6, 21), VGet(9, 1, 7), BlockType::PLATFORM));
        // Триггер прямо на поверхности платформы — без подставки
        blocks.push_back(Block(VGet(60, 7, 23), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 42: // Level 42 - ICE + ворота: Z-зигзаг со сменой позиции на каждых воротах
        // Весь уровень — один сплошной лёд, без перешейков.
        // 7 ворот из шипов с чередующимися проходами — нужно постоянно дрейфовать по льду.
        // frictionMultiplier=0.9936 (скользче 41-го на 20%), выход только через BOUNCE_PAD.
        playerSpawn = VGet(0, 1, 2); // Z=2, по центру первой полосы

        // Стартовая платформа (Z:-1..6)
        blocks.push_back(Block(VGet(-4, -1, -1), VGet(8, 1, 7), BlockType::PLATFORM));

        // Полоса 1 (+X, X:4..22, Z:0..6, ширина 6)
        { Block b(VGet(4, -1, 0), VGet(18, 1, 6), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.9936f; blocks.push_back(b); }
        // Ворота 1 (X=9): проход Z=3..6, шипы Z=0..3
        blocks.push_back(Block(VGet(9, 0, 0), VGet(2, 1, 3), BlockType::SPIKES));
        // Ворота 2 (X=16): проход Z=0..3, шипы Z=3..6 (смещение)
        blocks.push_back(Block(VGet(16, 0, 3), VGet(2, 1, 3), BlockType::SPIKES));

        // Поворот 1 (X:20..28, Z:-2..9) — широкий угол для манёвра
        { Block b(VGet(20, -1, -2), VGet(8, 1, 11), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.9936f; blocks.push_back(b); }

        // Полоса 2 (+Z, X:21..27, Z:6..24, ширина 6)
        { Block b(VGet(21, -1, 6), VGet(6, 1, 18), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.9936f; blocks.push_back(b); }
        // Ворота 3 (Z=11): проход X=24..27, шипы X=21..24
        blocks.push_back(Block(VGet(21, 0, 11), VGet(3, 1, 2), BlockType::SPIKES));
        // Ворота 4 (Z=17): проход X=21..24, шипы X=24..27 (смещение)
        blocks.push_back(Block(VGet(24, 0, 17), VGet(3, 1, 2), BlockType::SPIKES));

        // Поворот 2 (X:18..29, Z:22..30) — соединяет с третьей полосой
        { Block b(VGet(18, -1, 22), VGet(11, 1, 8), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.9936f; blocks.push_back(b); }

        // Полоса 3 (+X, X:26..52, Z:23..29, ширина 6)
        { Block b(VGet(26, -1, 23), VGet(26, 1, 6), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.9936f; blocks.push_back(b); }
        // Ворота 5 (X=32): проход Z=26..29, шипы Z=23..26
        blocks.push_back(Block(VGet(32, 0, 23), VGet(2, 1, 3), BlockType::SPIKES));
        // Ворота 6 (X=40): проход Z=23..26, шипы Z=26..29 (смещение)
        blocks.push_back(Block(VGet(40, 0, 26), VGet(2, 1, 3), BlockType::SPIKES));
        // Ворота 7 (X=47): тесные — проход Z=24..27, шипы с обоих краёв
        blocks.push_back(Block(VGet(47, 0, 23), VGet(2, 1, 1), BlockType::SPIKES)); // Z:23..24
        blocks.push_back(Block(VGet(47, 0, 27), VGet(2, 1, 2), BlockType::SPIKES)); // Z:27..29

        // BOUNCE_PAD в конце полосы 3
        blocks.push_back(Block(VGet(52, -1, 25), VGet(4, 1, 3), BlockType::BOUNCE_PAD));

        // Финальная платформа (top=7)
        blocks.push_back(Block(VGet(58, 6, 23), VGet(9, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(60, 7, 25), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 43: // Level 43 - BOUNCE: развилки в воздухе + ложные платформы
        // Вилка 1: батут-приманка ТОЛЬКО на фейке (Z+), с реальной (Z-) идёшь на вилку 2 прыжком
        // Вилка 2: земля, обе на одной высоте, фейк прямо по курсу, реальная в Z+
        // Вилка 3: top=6, чистая=фейк, розовые шипы=реальная
        // Пады: стартовый — на 1 юнит ниже платформы (сходишь с края, падаешь на пад)
        //        остальные — pos.y = platform_top (поверх платформы, нет перекрытия)
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-4, -1, -3), VGet(8, 1, 7), BlockType::PLATFORM));
        // Батут 1: на 1 юнит ниже старта (top=-1) — сходишь с края и падаешь на него
        blocks.push_back(Block(VGet(4, -2, 0), VGet(3, 1, 3), BlockType::BOUNCE_PAD));

        // === ВИЛКА 1: X:18..25 ===
        // Fake (Z+): с батутом-приманкой поверх (top=1) — падаешь сквозь фейк, пад выбрасывает
        blocks.push_back(Block(VGet(18, -1,  3), VGet(7, 1, 4), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(21,  0,  4), VGet(3, 1, 3), BlockType::BOUNCE_PAD)); // pos.y=platform_top=0
        // Real (Z-): без батута — прыгаешь отсюда на вилку 2
        blocks.push_back(Block(VGet(18, -1, -6), VGet(7, 1, 4), BlockType::PLATFORM));

        // === ВИЛКА 2: X:30..37, земля, top=0 — прыгаем с fork1 real ===
        // Fake (прямо по курсу, Z:-5..-1)
        blocks.push_back(Block(VGet(30, -1, -5), VGet(7, 1, 4), BlockType::FAKE_PLATFORM));
        // Real (Z+, Z:2..6) — надо прыгнуть вбок
        blocks.push_back(Block(VGet(30, -1,  2), VGet(7, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(33,  0,  3), VGet(3, 1, 3), BlockType::BOUNCE_PAD)); // pos.y=platform_top=0

        // === ВИЛКА 3: X:44..51, top=6 ===
        // Fake (чистая, по курсу, Z:2..6)
        blocks.push_back(Block(VGet(44, 5,  2), VGet(7, 1, 4), BlockType::FAKE_PLATFORM));
        // Real (с розовыми шипами, Z:9..13)
        blocks.push_back(Block(VGet(44, 5,  9), VGet(7, 1, 4), BlockType::PLATFORM));
        for (int tx = 44; tx < 51; tx++)
            for (int tz = 9; tz < 13; tz++)
                blocks.push_back(Block(VGet((float)tx, 6.f, (float)tz), VGet(1, 1, 1), BlockType::FAKE_SPIKES));
        blocks.push_back(Block(VGet(47,  6, 10), VGet(3, 1, 3), BlockType::BOUNCE_PAD)); // pos.y=platform_top=6

        // Финальная платформа (top=12)
        blocks.push_back(Block(VGet(58, 11,  8), VGet(9, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(60, 12,  9), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 44: // Level 44 — ICE + BOUNCE: три полосы в трёх направлениях, мини-лабиринты из шипов
        // +X (top=0) → батут → +Z (top=7) → батут → +X (top=14) → триггер
        // На каждой полосе — угловые шипы и зигзаг-препятствия прямо на льду
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-3, -1, -2), VGet(7, 1, 5), BlockType::PLATFORM));

        // === ПОЛОСА 1: +X, top=0, ширина Z:-2..3 ===
        { Block b(VGet(4, -1, -2), VGet(18, 1, 5), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        // Угловые шипы у входа (декоративные, срезают углы)
        blocks.push_back(Block(VGet(4, 0, -2), VGet(2, 1, 1), BlockType::SPIKES)); // лев. угол входа
        blocks.push_back(Block(VGet(4, 0,  2), VGet(2, 1, 1), BlockType::SPIKES)); // прав. угол входа
        // Зигзаг на льду: идём вправо → влево → вправо
        blocks.push_back(Block(VGet( 9, 0,  1), VGet(3, 1, 2), BlockType::SPIKES)); // Z:1..3, проход слева
        blocks.push_back(Block(VGet(15, 0, -2), VGet(3, 1, 3), BlockType::SPIKES)); // Z:-2..1, проход справа
        blocks.push_back(Block(VGet(20, 0,  1), VGet(2, 1, 2), BlockType::SPIKES)); // Z:1..3, у выхода
        // Батут (скатываемся с льда)
        blocks.push_back(Block(VGet(22, -1, -1), VGet(3, 1, 4), BlockType::BOUNCE_PAD));

        // === ПОЛОСА 2: +Z, top=7, ширина X:33..42 ===
        { Block b(VGet(33, 6, 2), VGet(9, 1, 16), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        // Угловые шипы у посадки (X-углы при Z=2)
        blocks.push_back(Block(VGet(33, 7,  2), VGet(2, 1, 3), BlockType::SPIKES)); // лев. угол посадки
        blocks.push_back(Block(VGet(40, 7,  2), VGet(2, 1, 3), BlockType::SPIKES)); // прав. угол посадки
        // Зигзаг на льду: идём вправо (X+) → влево (X-)
        blocks.push_back(Block(VGet(33, 7,  8), VGet(4, 1, 3), BlockType::SPIKES)); // X:33..37, проход справа
        blocks.push_back(Block(VGet(39, 7, 13), VGet(3, 1, 3), BlockType::SPIKES)); // X:39..42, проход слева
        // Батут (скатываемся по Z)
        blocks.push_back(Block(VGet(34, 6, 18), VGet(7, 1, 3), BlockType::BOUNCE_PAD));

        // === ПОЛОСА 3: +X, top=14, ширина Z:27..39 ===
        { Block b(VGet(33, 13, 27), VGet(20, 1, 12), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        // Угловые шипы у посадки (Z-углы при X=33)
        blocks.push_back(Block(VGet(33, 14, 27), VGet(3, 1, 2), BlockType::SPIKES)); // нижн. угол
        blocks.push_back(Block(VGet(33, 14, 37), VGet(3, 1, 2), BlockType::SPIKES)); // верхн. угол
        // Зигзаг на льду: идём по Z+ → по Z-
        blocks.push_back(Block(VGet(39, 14, 34), VGet(4, 1, 5), BlockType::SPIKES)); // Z:34..39, проход снизу
        blocks.push_back(Block(VGet(45, 14, 27), VGet(4, 1, 5), BlockType::SPIKES)); // Z:27..32, проход сверху
        // Триггер
        blocks.push_back(Block(VGet(50, 14, 30), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 45: // Level 45 — ICE + MOVING: S-образный маршрут с 3 разрывами на движущихся платформах
        // +X → поворот → +Z → поворот → -X → финал
        // На каждой полосе разрыв 6 юнитов — без платформы не перейти.
        // Ключевая механика: льды тормозят плохо, надо рассчитывать скорость входа на платформу.
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-4, -1, -3), VGet(8, 1, 7), BlockType::PLATFORM));

        // === ПОЛОСА 1: +X, Z:-2..2 (ширина 4) ===
        // Лёд A: X:4..14
        { Block b(VGet(4, -1, -2), VGet(10, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        // РАЗРЫВ 1 (X:14..20): платформа ходит от льда A до льда B
        { Block m(VGet(14, -1, -2), VGet(4, 1, 4), BlockType::MOVING,
            0, true, VGet(14, -1, -2), VGet(20, -1, -2), 1.2f, 0.0f);
          blocks.push_back(m); }
        // Лёд B: X:20..30, шипы-коридор
        { Block b(VGet(20, -1, -2), VGet(10, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(23, 0,  1), VGet(3, 1, 1), BlockType::SPIKES)); // правая стена
        blocks.push_back(Block(VGet(27, 0, -2), VGet(2, 1, 1), BlockType::SPIKES)); // левая стена

        // Поворот 1 (ледяной угол X:28..36, Z:-2..8)
        { Block b(VGet(28, -1, -2), VGet(8, 1, 10), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }

        // === ПОЛОСА 2: +Z, X:32..36 (ширина 4) ===
        // Лёд A: Z:6..16
        { Block b(VGet(32, -1, 6), VGet(4, 1, 10), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(32, 0, 10), VGet(1, 1, 3), BlockType::SPIKES)); // левая стена
        // РАЗРЫВ 2 (Z:16..22): платформа ходит вдоль Z
        { Block m(VGet(32, -1, 16), VGet(4, 1, 4), BlockType::MOVING,
            0, true, VGet(32, -1, 16), VGet(32, -1, 22), 1.0f, 0.0f);
          blocks.push_back(m); }
        // Лёд B: Z:22..34, шипы-коридор
        { Block b(VGet(32, -1, 22), VGet(4, 1, 12), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(35, 0, 26), VGet(1, 1, 3), BlockType::SPIKES)); // правая стена
        blocks.push_back(Block(VGet(32, 0, 32), VGet(2, 1, 2), BlockType::SPIKES)); // угол у поворота

        // Поворот 2 (ледяной угол X:24..36, Z:32..40)
        { Block b(VGet(24, -1, 32), VGet(12, 1, 8), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }

        // === ПОЛОСА 3: -X, Z:36..40 (ширина 4) ===
        // Лёд A: X:12..26 (вход со стороны поворота 2)
        { Block b(VGet(12, -1, 36), VGet(14, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(17, 0, 39), VGet(4, 1, 1), BlockType::SPIKES)); // верхняя стена
        // РАЗРЫВ 3 (X:6..12): платформа стартует у льда B, ходит к льду A
        { Block m(VGet(6, -1, 36), VGet(4, 1, 4), BlockType::MOVING,
            0, true, VGet(6, -1, 36), VGet(8, -1, 36), 1.1f, 0.0f);
          blocks.push_back(m); }
        // Лёд B: X:0..6
        { Block b(VGet(0, -1, 36), VGet(6, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(1, 0, 36), VGet(2, 1, 1), BlockType::SPIKES)); // нижний угол

        // Финальная платформа
        blocks.push_back(Block(VGet(-8, -1, 35), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(-6, 0, 37), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 46: // Level 46 — ICE + CRUMBLING: разгон по льду — влетаешь в цепочку рассыпашек
        // Цепочка 1: 5 рассыпашек, зигзаг Z±1, щадящий — учимся ритму
        // Цепочка 2: 3 рассыпашки, зигзаг Z±2, уже — сложнее прицелиться после льда
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-4, -1, -3), VGet(8, 1, 7), BlockType::PLATFORM));

        // Лёд 1 (разгон): X:4..18, Z:-2..2 — шипы загоняют в центральный коридор
        { Block b(VGet(4, -1, -2), VGet(14, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(8,  0,  1), VGet(5, 1, 1), BlockType::SPIKES)); // верхняя стена
        blocks.push_back(Block(VGet(13, 0, -2), VGet(3, 1, 1), BlockType::SPIKES)); // нижняя стена

        // === ЦЕПОЧКА 1: 5 рассыпашек, gap 3, зигзаг Z±1 ===
        // Влетаешь со льда прямо на C1, между платформами надо прыгать
        blocks.push_back(Block(VGet(18, -1, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true)); // Z:-1..2
        blocks.push_back(Block(VGet(24, -1,  0), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true)); // Z:0..3
        blocks.push_back(Block(VGet(30, -1, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true)); // Z:-1..2
        blocks.push_back(Block(VGet(36, -1,  0), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true)); // Z:0..3
        blocks.push_back(Block(VGet(42, -1, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true)); // Z:-1..2

        // Промежуточный ЛЁД вместо платформы — нет отдыха, сразу несёшься на цепочку 2
        { Block b(VGet(45, -1, -2), VGet(11, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(50, 0,  1), VGet(3, 1, 1), BlockType::SPIKES)); // верхняя стена
        blocks.push_back(Block(VGet(53, 0, -2), VGet(2, 1, 1), BlockType::SPIKES)); // нижняя стена

        // === ЦЕПОЧКА 2: 3 рассыпашки, gap 3, зигзаг Z±2 — перекрытие 1 юнит ===
        // Влетаешь со льда на C6 без остановки, времени на прицел меньше
        blocks.push_back(Block(VGet(56, -1, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true)); // Z:-1..2
        blocks.push_back(Block(VGet(62, -1,  1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true)); // Z:1..4, сдвиг+2
        blocks.push_back(Block(VGet(68, -1, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true)); // Z:-1..2, сдвиг-2

        // Финал
        blocks.push_back(Block(VGet(71, -1, -2), VGet(8, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(73, 0, -1), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 47: // Level 47 — три зоны гравитации: low-g → normal → high-g
        // Low-g (0.65x): прыжки ~8 units — плавучесть без безумия, 4 маленькие парящие платформы
        // Normal: чувствуется тяжелее при переходе, короткий мост
        // High-g (2.5x): прыжки ~2 units — только батут спасает, и то едва
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-4, -1, -3), VGet(8, 1, 7), BlockType::PLATFORM));

        // === ЗОНА 1: LOW-G (0.65x) — X:4..36 ===
        // Прыжок ~8 units — плавучий, но не улетаешь. Без батута — нужна точность.
        { Block g(VGet(4, -3, -6), VGet(32, 30, 14), BlockType::GRAVITY_ZONE);
          g.gravityMultiplier = 0.65f; blocks.push_back(g); }
        // Цепочка 3 парящих платформ (4×1×6), зигзаг Z±4, +5 по Y каждая
        // Моргающие шипы повёрнуты вдоль Z — блокируют X-край входа/выхода, фазы смещены
        // A (top=5): Z:-1..5 — шип на правом X-крае (X:12..13), надо не залететь в конец
        blocks.push_back(Block(VGet(9, 4, -1), VGet(4, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(12, 5, -1), VGet(1, 1, 6), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.2f, 0.0f));  // X:12..13, фаза 0
        // B (top=10): сдвиг Z-4 → Z:-5..1 — шип на левом X-крае (X:17..18), надо перепрыгнуть
        blocks.push_back(Block(VGet(17, 9, -5), VGet(4, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(17, 10, -5), VGet(1, 1, 6), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.2f, 0.4f)); // X:17..18, фаза 0.4
        // C (top=15): сдвиг Z+4 → Z:-1..5 — шип на правом X-крае (X:27..28)
        blocks.push_back(Block(VGet(24, 14, -1), VGet(4, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(27, 15, -1), VGet(1, 1, 6), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.2f, 0.8f)); // X:27..28, фаза 0.8

        // === МОСТ: нормальная гравитация — укорочен до X:30..36 ===
        // Разрыв X:36..41 — прыжок прямо на границе зоны, гравитация меняется в полёте
        blocks.push_back(Block(VGet(30, 17, -2), VGet(6, 1, 5), BlockType::PLATFORM)); // X:30..36

        // === ЗОНА 3: HIGH-G (2.5x) — X:42..80 ===
        // Прыжок ~2 units, батут ~5 units — каждый подъём впритык
        { Block g(VGet(42, -3, -4), VGet(38, 38, 11), BlockType::GRAVITY_ZONE);
          g.gravityMultiplier = 2.5f; blocks.push_back(g); }
        // Platform D (top=18): начинается X:41 — принимает прыжок через разрыв
        blocks.push_back(Block(VGet(41, 17, -2), VGet(9, 1, 5), BlockType::PLATFORM));
        // Батут 1 на D (top=19): запускает на 19+4.8=23.8 — достанем E (top=23)
        blocks.push_back(Block(VGet(44, 18, -1), VGet(3, 1, 3), BlockType::BOUNCE_PAD));
        // Platform E (top=23)
        blocks.push_back(Block(VGet(50, 22, -2), VGet(7, 1, 5), BlockType::PLATFORM));
        // Батут 2 на E (top=24): запускает на 24+4.8=28.8 — достанем F (top=28)
        blocks.push_back(Block(VGet(52, 23, -1), VGet(3, 1, 3), BlockType::BOUNCE_PAD));
        // Финальная платформа F (top=28)
        blocks.push_back(Block(VGet(58, 27, -2), VGet(8, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(60, 28, -1), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 48: // Level 48 — ICE + темнота: 5 поворотов, вспышка только у T3
        // Тьма стакается мгновенно (2f/s). Одна вспышка на полпути — успей запомнить маршрут.
        // Финал: 3 реальных ступени → батут вверх на финиш. Впереди — только фейки.
        // Corridor E: фейковые шипы вместо реальных — паника вхолостую.
        playerSpawn = VGet(0, 1, 2);

        // Старт
        blocks.push_back(Block(VGet(-4, -1, -2), VGet(8, 1, 7), BlockType::PLATFORM));

        // Зон осветления нет — уровень тёмный всегда (DARKEN_SPEED=2f/s)

        // --- A: +X, Z:0..4 ---
        { Block b(VGet(4, -1, 0), VGet(12, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(7,  0, 3), VGet(4, 1, 1), BlockType::SPIKES)); // дальняя стена
        blocks.push_back(Block(VGet(12, 0, 0), VGet(3, 1, 1), BlockType::SPIKES)); // ближняя стена

        // --- T1: +X → +Z ---
        { Block b(VGet(14, -1, -2), VGet(8, 1, 10), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(14, 0, -2), VGet(2, 1, 3), BlockType::SPIKES)); // inner corner

        // --- B: +Z, X:20..24 ---
        { Block b(VGet(20, -1, 6), VGet(4, 1, 14), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(20, 0, 10), VGet(1, 1, 4), BlockType::SPIKES)); // дальняя стена
        blocks.push_back(Block(VGet(20, 0, 17), VGet(1, 1, 2), BlockType::SPIKES)); // конец B

        // --- T2: +Z → +X ---
        { Block b(VGet(16, -1, 18), VGet(10, 1, 10), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(16, 0, 18), VGet(3, 1, 2), BlockType::SPIKES)); // inner corner

        // --- C: +X, Z:24..28 ---
        { Block b(VGet(24, -1, 24), VGet(14, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(27, 0, 27), VGet(4, 1, 1), BlockType::SPIKES)); // дальняя стена
        blocks.push_back(Block(VGet(33, 0, 24), VGet(3, 1, 1), BlockType::SPIKES)); // ближняя стена

        // --- T3: +X → +Z (зона вспышки вокруг этого угла) ---
        { Block b(VGet(36, -1, 22), VGet(10, 1, 12), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(36, 0, 22), VGet(3, 1, 2), BlockType::SPIKES)); // inner corner

        // --- D: +Z, X:42..46 ---
        { Block b(VGet(42, -1, 32), VGet(4, 1, 14), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(45, 0, 36), VGet(1, 1, 5), BlockType::SPIKES)); // дальняя стена
        blocks.push_back(Block(VGet(42, 0, 43), VGet(1, 1, 2), BlockType::SPIKES)); // ближняя стена

        // --- T4: +Z → +X ---
        { Block b(VGet(38, -1, 44), VGet(12, 1, 10), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(38, 0, 44), VGet(3, 1, 2), BlockType::SPIKES)); // inner corner

        // --- E: +X, Z:52..56 (FAKE_SPIKES — выглядят страшно, но безвредны) ---
        { Block b(VGet(48, -1, 52), VGet(14, 1, 4), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(51, 0, 55), VGet(6, 1, 1), BlockType::FAKE_SPIKES)); // ФЕЙК — паника
        blocks.push_back(Block(VGet(52, 0, 52), VGet(3, 1, 1), BlockType::SPIKES));      // реальная угроза

        // --- T5: +X → +Z ---
        { Block b(VGet(60, -1, 50), VGet(10, 1, 14), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(60, 0, 50), VGet(2, 1, 3), BlockType::SPIKES)); // inner corner

        // --- F: +Z, X:66..70 ---
        { Block b(VGet(66, -1, 62), VGet(4, 1, 14), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(69, 0, 66), VGet(1, 1, 5), BlockType::SPIKES)); // дальняя стена
        blocks.push_back(Block(VGet(69, 0, 73), VGet(1, 1, 2), BlockType::SPIKES)); // конец F

        // BOUNCE_PAD — выход со льда вверх
        blocks.push_back(Block(VGet(66, -1, 75), VGet(4, 1, 4), BlockType::BOUNCE_PAD));

        // === ФИНАЛЬНАЯ СЕКЦИЯ: развилка в центре ===
        // Посадочная площадка (top=7) после прыжка с батута
        blocks.push_back(Block(VGet(64, 6, 79), VGet(10, 1, 8), BlockType::PLATFORM));

        // Ступень A — реальная (4×4)
        blocks.push_back(Block(VGet(65, 6, 89), VGet(4, 1, 4), BlockType::PLATFORM));

        // Ступень B — реальная (3×3) с фейковыми шипами — паника вхолостую
        blocks.push_back(Block(VGet(66, 6, 96), VGet(3, 1, 3), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(66, 7, 96), VGet(3, 1, 2), BlockType::FAKE_SPIKES));

        // *** РАЗВИЛКА (top=7, 8×6): наступаешь — видишь два пути ***
        blocks.push_back(Block(VGet(64, 6, 102), VGet(8, 1, 6), BlockType::PLATFORM));

        // Фейковый путь (+Z) — три ступени вперёд, выглядят как продолжение
        blocks.push_back(Block(VGet(65, 6, 111), VGet(4, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(65, 6, 117), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(65, 6, 123), VGet(3, 1, 3), BlockType::FAKE_PLATFORM));

        // Реальный путь (+X от развилки) — одна маленькая реальная платформа с триггером
        blocks.push_back(Block(VGet(72, 6, 103), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(72, 7, 103), VGet(4, 1, 4), BlockType::TRIGGER));
        break;

    case 49: // Level 49 — ICE + RETRACTABLE_SPIKES + CRUMBLING
        // Три ледяных коридора со слаломом из мигающих ворот (5-юнитовые коридоры).
        // Между секциями — рассыпающиеся мосты с 5-6 юнитовыми разрывами: разогнался — торопись.
        playerSpawn = VGet(0, 1, 2);

        // Старт
        blocks.push_back(Block(VGet(-3, -1, -1), VGet(7, 1, 7), BlockType::PLATFORM));

        // === СЕКЦИЯ A: +X, коридор Z:0..5, 4 слаломных ворот ===
        { Block b(VGet(4, -1, 0), VGet(16, 1, 5), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(7,  0, 0), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.4f, 0.0f));   // G1: лево Z:0..2
        blocks.push_back(Block(VGet(11, 0, 3), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.4f, 0.35f));  // G2: право Z:3..5
        blocks.push_back(Block(VGet(14, 0, 0), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.2f, 0.0f));   // G3: лево
        blocks.push_back(Block(VGet(17, 0, 3), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.2f, 0.6f));   // G4: право

        // === РАССЫПАЮЩИЙСЯ МОСТ 1 (X:20..44) ===
        blocks.push_back(Block(VGet(20, -1, 0), VGet(4, 1, 5), BlockType::CRUMBLING, 0, true)); // C1 полная ширина
        // Разрыв X:24..30 (6 юнитов)
        blocks.push_back(Block(VGet(30, -1, 1), VGet(4, 1, 3), BlockType::CRUMBLING, 0, true)); // C2 узкий Z:1..4
        // Разрыв X:34..40 (6 юнитов)
        blocks.push_back(Block(VGet(40, -1, 0), VGet(4, 1, 5), BlockType::CRUMBLING, 0, true)); // C3 полная ширина

        // Угловая площадка (переход к секции B)
        blocks.push_back(Block(VGet(44, -1, -2), VGet(9, 1, 9), BlockType::PLATFORM));

        // === СЕКЦИЯ B: +Z, коридор X:47..52, 4 ворот ===
        { Block b(VGet(47, -1, 5), VGet(5, 1, 17), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(47, 0, 9),  VGet(2, 1, 1), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.3f, 0.0f));   // G5: X-лево (X:47..49)
        blocks.push_back(Block(VGet(50, 0, 14), VGet(2, 1, 1), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.3f, 0.43f));  // G6: X-право (X:50..52)
        blocks.push_back(Block(VGet(47, 0, 18), VGet(2, 1, 1), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.1f, 0.0f));   // G7: X-лево
        blocks.push_back(Block(VGet(50, 0, 19), VGet(2, 1, 1), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.1f, 0.55f));  // G8: X-право

        // === РАССЫПАЮЩИЙСЯ МОСТ 2 (Z:22..44) ===
        blocks.push_back(Block(VGet(47, -1, 22), VGet(5, 1, 4), BlockType::CRUMBLING, 0, true)); // D1 полная ширина
        // Разрыв Z:26..31 (5 юнитов)
        blocks.push_back(Block(VGet(48, -1, 31), VGet(3, 1, 4), BlockType::CRUMBLING, 0, true)); // D2 узкий X:48..51
        // Разрыв Z:35..40 (5 юнитов)
        blocks.push_back(Block(VGet(47, -1, 40), VGet(5, 1, 4), BlockType::CRUMBLING, 0, true)); // D3 полная ширина

        // Угловая площадка (переход к секции C)
        blocks.push_back(Block(VGet(44, -1, 44), VGet(9, 1, 8), BlockType::PLATFORM));

        // === СЕКЦИЯ C: +X, коридор Z:46..51, 5 ворот (нормальная гравитация) ===
        // Ритм шипов ускоряется: 1.0s → 0.9s → 0.8s.
        { Block b(VGet(53, -1, 46), VGet(21, 1, 5), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        blocks.push_back(Block(VGet(57, 0, 46), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));   // G9:  лево
        blocks.push_back(Block(VGet(61, 0, 49), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.25f));  // G10: право
        blocks.push_back(Block(VGet(65, 0, 46), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 0.9f, 0.0f));   // G11: лево
        blocks.push_back(Block(VGet(69, 0, 49), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 0.9f, 0.45f));  // G12: право
        blocks.push_back(Block(VGet(73, 0, 46), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 0.8f, 0.0f));   // G13: лево (самый быстрый)

        // BOUNCE_PAD — выход со льда (нормальная гравитация)
        blocks.push_back(Block(VGet(74, -1, 47), VGet(3, 1, 3), BlockType::BOUNCE_PAD));

        // Финальная платформа (top=7) + триггер
        blocks.push_back(Block(VGet(79, 6, 45), VGet(9, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(81, 7, 47), VGet(3, 1, 3), BlockType::TRIGGER));
        break;

    case 50: // Level 50 — ФИНАЛ: все механики
        // С1: форк (фейки + движущаяся) → С2: рассыпашки под мигающими шипами →
        // С3: low-g + маятники → С4: темнота + исчезающие → С5: лёд + батут
        playerSpawn = VGet(0, 1, 3);

        // Старт
        blocks.push_back(Block(VGet(-4, -1, -2), VGet(8, 1, 10), BlockType::PLATFORM));

        // ================================================================
        // СЕКЦИЯ 1: ФОРК — три пути, два фейковых, один реальный
        // ================================================================
        // Движущаяся платформа-мост через разрыв X:4..10
        { Block m(VGet(4, -1, 0), VGet(6, 1, 6), BlockType::MOVING,
              0, true, VGet(4,-1,0), VGet(6,-1,0), 1.0f, 0.0f);
          blocks.push_back(m); }

        // Узел форка: три коридора по Z
        blocks.push_back(Block(VGet(10, -1, -4), VGet(8, 1, 14), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(17,  0, -4), VGet(1, 1, 2), BlockType::FAKE_SPIKES)); // угол у пути A
        blocks.push_back(Block(VGet(17,  0,  8), VGet(1, 1, 2), BlockType::FAKE_SPIKES)); // угол у пути C

        // Путь A (Z:-4..0): FAKE — чистый, широкий, "безопасный" вид
        blocks.push_back(Block(VGet(18, -1, -4), VGet(10, 1, 4), BlockType::FAKE_PLATFORM));

        // Путь B (Z:1..5): REAL — мигающие шипы, надо дождаться окна
        blocks.push_back(Block(VGet(18, -1,  1), VGet(10, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(21,  0,  1), VGet(2, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.5f, 0.0f));
        blocks.push_back(Block(VGet(25,  0,  3), VGet(2, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.5f, 0.5f));

        // Путь C (Z:6..10): FAKE — с фейковыми шипами (выглядит опасно = "значит реальный")
        blocks.push_back(Block(VGet(18, -1,  6), VGet(10, 1, 4), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(20,  0,  7), VGet(4, 1, 2), BlockType::FAKE_SPIKES));

        // Площадка после форка (только путь B Z:1..5 сюда доходит)
        blocks.push_back(Block(VGet(28, -1,  0), VGet(7, 1, 6), BlockType::PLATFORM));

        // ================================================================
        // СЕКЦИЯ 2: РАССЫПАШКИ + МИГАЮЩИЕ ШИПЫ НА КАЖДОЙ
        // ================================================================
        blocks.push_back(Block(VGet(35, -1,  0), VGet(4, 1, 4), BlockType::CRUMBLING, 0, true)); // R1
        blocks.push_back(Block(VGet(36,  0,  1), VGet(2, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.2f, 0.0f));
        // Разрыв X:39..44
        blocks.push_back(Block(VGet(44, -1,  1), VGet(4, 1, 3), BlockType::CRUMBLING, 0, true)); // R2
        blocks.push_back(Block(VGet(45,  0,  1), VGet(2, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.1f, 0.3f));
        // Разрыв X:48..53
        blocks.push_back(Block(VGet(53, -1,  0), VGet(4, 1, 4), BlockType::CRUMBLING, 0, true)); // R3
        blocks.push_back(Block(VGet(54,  0,  1), VGet(2, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));
        // Разрыв X:57..62
        blocks.push_back(Block(VGet(62, -1,  0), VGet(4, 1, 4), BlockType::CRUMBLING, 0, true)); // R4
        blocks.push_back(Block(VGet(63,  0,  1), VGet(2, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 0.9f, 0.45f));

        blocks.push_back(Block(VGet(66, -1, -1), VGet(7, 1, 7), BlockType::PLATFORM));

        // ================================================================
        // СЕКЦИЯ 3: LOW-G (0.65x) + МАЯТНИКИ НАД РАЗРЫВАМИ
        // ================================================================
        // Все платформы на уровне земли. Прыжки в 1.5× длиннее — но маятники блокируют разрывы.
        { Block gz(VGet(73, -3, -5), VGet(38, 20, 12), BlockType::GRAVITY_ZONE);
          gz.gravityMultiplier = 0.65f; blocks.push_back(gz); }

        blocks.push_back(Block(VGet(73, -1, -2), VGet(6, 1, 5), BlockType::PLATFORM)); // GA
        // Разрыв X:79..84, маятник в центре разрыва
        { Block p(VGet(79, 2,  0), VGet(2, 1, 2), BlockType::PENDULUM_BLADE,
              0, true, VGet(0,0,0), VGet(0,0,0), 1.4f, 0.0f);
          p.pivotPoint = VGet(80, 8, 1);
          p.swingSpeed = 1.3f;
          p.swingRange = DX_PI_F / 2.8f;
          blocks.push_back(p); }
        blocks.push_back(Block(VGet(84, -1,  1), VGet(6, 1, 5), BlockType::PLATFORM)); // GB (Z-сдвиг)
        // Разрыв X:90..96, маятник быстрее
        { Block p(VGet(90, 2, -1), VGet(2, 1, 2), BlockType::PENDULUM_BLADE,
              0, true, VGet(0,0,0), VGet(0,0,0), 1.2f, 0.5f);
          p.pivotPoint = VGet(91, 8, 0);
          p.swingSpeed = 1.4f;
          p.swingRange = DX_PI_F / 2.5f;
          blocks.push_back(p); }
        blocks.push_back(Block(VGet(96, -1, -2), VGet(6, 1, 5), BlockType::PLATFORM)); // GC
        // Разрыв X:102..107 — без маятника, выход из зоны (зона X:73..111)
        blocks.push_back(Block(VGet(107, -1, -2), VGet(7, 1, 6), BlockType::PLATFORM));

        // ================================================================
        // СЕКЦИЯ 4: ТЕМНОТА (LIGHT_PULSE_ZONE) + ИСЧЕЗАЮЩИЕ ПЛАТФОРМЫ
        // ================================================================
        // Темнота 2f/s, вспышка каждые 2с. Платформы в противофазе — одна есть, другой нет.
        blocks.push_back(Block(VGet(114, -3, -4), VGet(32, 20, 11), BlockType::LIGHT_PULSE_ZONE,
            0, true, VGet(0,0,0), VGet(0,0,0), 2.0f, 0.0f));

        blocks.push_back(Block(VGet(114, -1, -1), VGet(6, 1, 5), BlockType::DISAPPEARING,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.5f, 0.0f));  // E1
        // Разрыв X:120..125
        blocks.push_back(Block(VGet(125, -1, -1), VGet(6, 1, 5), BlockType::DISAPPEARING,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.5f, 0.5f));  // E2 противофаза
        // Разрыв X:131..136
        blocks.push_back(Block(VGet(136, -1, -1), VGet(6, 1, 5), BlockType::DISAPPEARING,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.5f, 0.0f));  // E3

        blocks.push_back(Block(VGet(142, -1, -2), VGet(7, 1, 7), BlockType::PLATFORM));

        // ================================================================
        // СЕКЦИЯ 5: ЛЁД + РЕТРАКТ ВОРОТА + БАТУТ НА ФИНИШ
        // ================================================================
        { Block b(VGet(149, -1, -1), VGet(22, 1, 5), BlockType::ICE_PLATFORM);
          b.frictionMultiplier = 0.990f; blocks.push_back(b); }
        // 5 ворот: лево/право по Z, ритм 1.0s→0.8s
        blocks.push_back(Block(VGet(152, 0, -1), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));   // лево
        blocks.push_back(Block(VGet(156, 0,  2), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.25f));  // право
        blocks.push_back(Block(VGet(160, 0, -1), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 0.9f, 0.0f));   // лево
        blocks.push_back(Block(VGet(164, 0,  2), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 0.9f, 0.45f));  // право
        blocks.push_back(Block(VGet(168, 0, -1), VGet(1, 1, 2), BlockType::RETRACTABLE_SPIKES,
            0, false, VGet(0,0,0), VGet(0,0,0), 0.8f, 0.0f));   // лево (самый быстрый)

        // BOUNCE_PAD — выход со льда на финиш
        blocks.push_back(Block(VGet(171, -1,  0), VGet(3, 1, 3), BlockType::BOUNCE_PAD));

        // Финальная платформа (top=10) — достижима батутом (max height ~12u)
        blocks.push_back(Block(VGet(177,  9, -1), VGet(10, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(179, 10,  1), VGet(4, 1, 4), BlockType::TRIGGER));
        break;
    }
}
