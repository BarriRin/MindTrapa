#include "Level.h"

void Level::LoadBlock2(int id) {
    switch (id) {
    case 11: // Level 11 - RETRACTABLE_SPIKES: 4 секции, поворот в +Z
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Секция 1 — один медленный шип (туториал)
        // Платформа X:5..17, Z:-2..3, gap=2 от старта
        blocks.push_back(Block(VGet(5, 0, -2), VGet(12, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(9, 1, -2), VGet(3, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, true, VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));

        // Секция 2 — два шипа в противофазе (gap=2)
        // Когда A выдвинут — B спрятан, и наоборот
        blocks.push_back(Block(VGet(19, 0, -2), VGet(14, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(21, 1, -2), VGet(3, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, true,  VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));
        blocks.push_back(Block(VGet(26, 1, -2), VGet(3, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));

        // Поворот на +Z — два поперечных шипа блокируют путь
        // Платформа X:35..40, Z:-3..15 (gap=2 от секции 2)
        blocks.push_back(Block(VGet(35, 0, -3), VGet(5, 1, 18), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(35, 1,  1), VGet(5, 1, 3), BlockType::RETRACTABLE_SPIKES, 0, true,  VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f)); // Z:1..4
        blocks.push_back(Block(VGet(35, 1,  8), VGet(5, 1, 3), BlockType::RETRACTABLE_SPIKES, 0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f)); // Z:8..11

        // Секция 3 — три быстрых шипа (speed=2.0, волна), gap=2 в Z от поворота
        // Платформа X:33..45, Z:17..22
        blocks.push_back(Block(VGet(33, 0, 17), VGet(12, 1, 5), BlockType::PLATFORM));
        // Три шипа: A закрывает левый край (X:33..37), B — центр (X:37..41), C — правый край (X:41..45)
        // Спайки перекрывают всю ширину платформы — нет обхода по краям
        blocks.push_back(Block(VGet(33, 1, 17), VGet(4, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, true, VGet(0,0,0), VGet(0,0,0), 2.0f, 0.00f)); // 2s цикл
        blocks.push_back(Block(VGet(37, 1, 17), VGet(4, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, true, VGet(0,0,0), VGet(0,0,0), 2.0f, 0.67f)); // сдвиг 1/3 цикла
        blocks.push_back(Block(VGet(41, 1, 17), VGet(4, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, true, VGet(0,0,0), VGet(0,0,0), 2.0f, 1.33f)); // сдвиг 2/3 цикла

        // Финиш (gap=2 в Z, ширина = ширине всей секции чтобы принять игрока из любой лены)
        blocks.push_back(Block(VGet(33, 0, 24), VGet(12, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(37, 1, 28), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 12: // Level 12 - CRUMBLING: 8 платформ Г-образно, быстрое рассыпание (0.3s)
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Фаза 1: +X направление (4 платформы), speed=0.6 → 0.3s до падения
        // gap=2 от старта, каждый следующий gap=3
        blocks.push_back(Block(VGet(5,  0, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true, VGet(0,0,0), VGet(0,0,0), 0.6f));
        blocks.push_back(Block(VGet(11, 0, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true, VGet(0,0,0), VGet(0,0,0), 0.6f));
        blocks.push_back(Block(VGet(17, 0, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true, VGet(0,0,0), VGet(0,0,0), 0.6f));
        blocks.push_back(Block(VGet(23, 0, -1), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true, VGet(0,0,0), VGet(0,0,0), 0.6f));

        // Поворот на +Z (прыжок с P4 на P5, gap=3 в Z, Y+1)
        blocks.push_back(Block(VGet(23, 1,  5), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true, VGet(0,0,0), VGet(0,0,0), 0.6f)); // X:23..26 = полное перекрытие с P4
        blocks.push_back(Block(VGet(21, 1, 11), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true, VGet(0,0,0), VGet(0,0,0), 0.6f));
        blocks.push_back(Block(VGet(20, 2, 17), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true, VGet(0,0,0), VGet(0,0,0), 0.6f));
        blocks.push_back(Block(VGet(19, 2, 23), VGet(3, 1, 3), BlockType::CRUMBLING, 0, true, VGet(0,0,0), VGet(0,0,0), 0.6f));

        // Безопасная посадка (gap=3 в Z от P8)
        blocks.push_back(Block(VGet(15, 2, 29), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(17, 3, 32), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 13: // Level 13 - Два движущихся подряд + коридор из 3 шипов
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // Промежуточная площадка — посадка на первый движущийся (gap=2)
        blocks.push_back(Block(VGet(5, 0, -1), VGet(4, 1, 3), BlockType::PLATFORM));

        // Движущийся A (медленный, Y=0, gap=2 от площадки)
        // Когда A у X=24, правый край X=27 = начало посадочной → можно сойти
        blocks.push_back(Block(VGet(11, 0, 0), VGet(3, 1, 3), BlockType::MOVING,
            0, true, VGet(11, 0, 0), VGet(24, 0, 0), 1.5f));

        // Посадочная между двумя движущимися (ждать второй)
        blocks.push_back(Block(VGet(27, 0, -1), VGet(5, 1, 4), BlockType::PLATFORM));

        // Движущийся B (быстрее, Y=1, gap=2 от посадочной)
        // Когда B у X=46, правый край X=49 = начало финальной → сойти
        blocks.push_back(Block(VGet(34, 1, 0), VGet(3, 1, 3), BlockType::MOVING,
            0, true, VGet(34, 1, 0), VGet(46, 1, 0), 2.0f));

        // Финальная платформа: широкая посадочная зона + коридор с 3 шипами
        blocks.push_back(Block(VGet(49, 1, -2), VGet(15, 1, 5), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(51, 2, -2), VGet(2, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, true,  VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));
        blocks.push_back(Block(VGet(55, 2, -2), VGet(2, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, false, VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));
        blocks.push_back(Block(VGet(59, 2, -2), VGet(2, 1, 5), BlockType::RETRACTABLE_SPIKES, 0, true,  VGet(0,0,0), VGet(0,0,0), 1.0f, 1.0f));

        // Финиш (gap=2 от конца финальной платформы)
        blocks.push_back(Block(VGet(66, 1, -2), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(68, 2,  0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 14: // Level 14 - Обман ожиданий: "чистый" путь убивает, "страшный" розовый — безопасен
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // === СЕКЦИЯ 1: Вилка ===
        blocks.push_back(Block(VGet(5, 0, -7), VGet(5, 1, 16), BlockType::PLATFORM));

        // ПУТЬ A — чистый → реальные шипы у конца (тайлы 1x1x1)
        blocks.push_back(Block(VGet(10, 0, 5), VGet(12, 1, 5), BlockType::PLATFORM));
        for (int tx = 18; tx < 22; tx++)
            for (int tz = 5; tz < 10; tz++)
                blocks.push_back(Block(VGet((float)tx, 1.f, (float)tz), VGet(1, 1, 1), BlockType::SPIKES));

        // ПУТЬ B — весь в розовых шипах → безопасен (тайлы 1x1x1)
        blocks.push_back(Block(VGet(10, 0, -8), VGet(12, 1, 5), BlockType::PLATFORM));
        for (int tx = 10; tx < 22; tx++)
            for (int tz = -8; tz < -3; tz++)
                blocks.push_back(Block(VGet((float)tx, 1.f, (float)tz), VGet(1, 1, 1), BlockType::FAKE_SPIKES));

        // Посадочная
        blocks.push_back(Block(VGet(26, 0, -9), VGet(6, 1, 6), BlockType::PLATFORM));

        // === СЕКЦИЯ 2: Открытое поле ===
        blocks.push_back(Block(VGet(36, 0, -4), VGet(20, 1, 11), BlockType::PLATFORM));
        // Реальные шипы блокируют правый выход (тайлы 1x1x1)
        for (int tx = 48; tx < 56; tx++)
            for (int tz = -4; tz < 2; tz++)
                blocks.push_back(Block(VGet((float)tx, 1.f, (float)tz), VGet(1, 1, 1), BlockType::SPIKES));
        // Розовая задняя стена — единственный выход (тайлы 1x1x1)
        for (int tx = 36; tx < 56; tx++)
            for (int tz = 4; tz < 7; tz++)
                blocks.push_back(Block(VGet((float)tx, 1.f, (float)tz), VGet(1, 1, 1), BlockType::FAKE_SPIKES));

        // Выход из секции 2
        blocks.push_back(Block(VGet(56, 0, 3), VGet(6, 1, 5), BlockType::PLATFORM));

        // === СЕКЦИЯ 3: Финал — две дорожки ===
        blocks.push_back(Block(VGet(66, 0, -2), VGet(16, 1, 12), BlockType::PLATFORM));
        // Чистая дорожка → реальные шипы (тайлы 1x1x1)
        for (int tx = 71; tx < 78; tx++)
            for (int tz = -2; tz < 4; tz++)
                blocks.push_back(Block(VGet((float)tx, 1.f, (float)tz), VGet(1, 1, 1), BlockType::SPIKES));
        // Розовая дорожка → безопасна (тайлы 1x1x1)
        for (int tx = 66; tx < 82; tx++)
            for (int tz = 5; tz < 10; tz++)
                blocks.push_back(Block(VGet((float)tx, 1.f, (float)tz), VGet(1, 1, 1), BlockType::FAKE_SPIKES));
        // Посадочная за шипами + финальный флаг
        blocks.push_back(Block(VGet(82, 0, 5), VGet(4, 1, 4), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(83, 1, 6), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 15: // Level 15 - CRUMBLING + тайминг: зазоры 3 в X, поворот в +Z с зазором 4
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-3, -1, -3), VGet(6, 1, 6), BlockType::PLATFORM));

        // +X фаза: gap=2 от старта, затем gap=3 между платформами, высота +1 через одну
        blocks.push_back(Block(VGet(5,  0, -1), VGet(3, 1, 3), BlockType::CRUMBLING)); // gap=2 от старта
        blocks.push_back(Block(VGet(11, 1, -1), VGet(3, 1, 3), BlockType::CRUMBLING)); // gap=3, Y+1
        blocks.push_back(Block(VGet(17, 1, -1), VGet(3, 1, 3), BlockType::CRUMBLING)); // gap=3
        blocks.push_back(Block(VGet(23, 2, -1), VGet(3, 1, 3), BlockType::CRUMBLING)); // gap=3, Y+1

        // Поворот в +Z: gap=4 в Z от последней +X платформы (Z:-1..2 → Z:6..9)
        blocks.push_back(Block(VGet(22, 2,  6), VGet(3, 1, 3), BlockType::CRUMBLING)); // Z gap=4, X overlap ✓
        blocks.push_back(Block(VGet(21, 3, 13), VGet(3, 1, 3), BlockType::CRUMBLING)); // Z gap=4, Y+1

        // Финиш (gap=4 в Z от последней платформы)
        blocks.push_back(Block(VGet(18, 3, 20), VGet(6, 1, 6), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(20, 4, 22), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 16: // Level 16 - Невидимый коридор: невидимые стены сужают путь, шипы требуют тайминга
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // === КОРИДОР 1: направление +X ===
        // Платформа X:5..23, Z:-1..2 (ширина 3 единицы)
        blocks.push_back(Block(VGet(5, 0, -1), VGet(18, 1, 3), BlockType::PLATFORM));
        // Невидимые стены ограничивают коридор по Z
        // Начало Y=1 (поверхность пола), высота=8 → верх Y=9 > макс. прыжок ~6.3 от пола
        blocks.push_back(Block(VGet(5, 1, -3), VGet(18, 8, 2), BlockType::INVISIBLE_WALL)); // южная стена (Z:-3..-1)
        blocks.push_back(Block(VGet(5, 1,  2), VGet(18, 8, 2), BlockType::INVISIBLE_WALL)); // северная стена (Z:2..4)
        // Три шипа — волна, перекрывают всю ширину коридора (size.z=3)
        blocks.push_back(Block(VGet( 9, 1, -1), VGet(2, 1, 3), BlockType::RETRACTABLE_SPIKES, 0, true,  VGet(0,0,0), VGet(0,0,0), 1.0f, 0.0f));
        blocks.push_back(Block(VGet(14, 1, -1), VGet(2, 1, 3), BlockType::RETRACTABLE_SPIKES, 0, true,  VGet(0,0,0), VGet(0,0,0), 1.0f, 2.0f));
        blocks.push_back(Block(VGet(19, 1, -1), VGet(2, 1, 3), BlockType::RETRACTABLE_SPIKES, 0, true,  VGet(0,0,0), VGet(0,0,0), 1.0f, 1.0f));

        // === ПЛОЩАДКА ОТДЫХА: поворот на +Z ===
        // X:22..28, Z:-1..9 — соединяет конец коридора 1 с началом коридора 2
        blocks.push_back(Block(VGet(22, 0, -1), VGet(6, 1, 10), BlockType::PLATFORM));

        // === КОРИДОР 2: направление +Z ===
        // Платформа X:23..26, Z:8..23 (ширина 3 единицы)
        blocks.push_back(Block(VGet(23, 0, 8), VGet(3, 1, 15), BlockType::PLATFORM));
        // Невидимые стены по X (Y=1, высота=8 → верх Y=9 > макс. прыжок)
        blocks.push_back(Block(VGet(21, 1, 8), VGet(2, 8, 15), BlockType::INVISIBLE_WALL)); // западная (X:21..23)
        blocks.push_back(Block(VGet(26, 1, 8), VGet(2, 8, 15), BlockType::INVISIBLE_WALL)); // восточная (X:26..28)
        // Два быстрых шипа в противофазе (speed=1.5 → цикл 2.67s)
        blocks.push_back(Block(VGet(23, 1, 12), VGet(3, 1, 2), BlockType::RETRACTABLE_SPIKES, 0, true,  VGet(0,0,0), VGet(0,0,0), 1.5f, 0.00f));
        blocks.push_back(Block(VGet(23, 1, 18), VGet(3, 1, 2), BlockType::RETRACTABLE_SPIKES, 0, false, VGet(0,0,0), VGet(0,0,0), 1.5f, 1.34f));

        // === ФИНАЛ ===
        // gap=2 в Z от конца коридора 2 (Z=23), платформа X:19..29, Z:25..33
        blocks.push_back(Block(VGet(19, 0, 25), VGet(10, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(22, 1, 28), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 17: // Level 17 - "Движущийся лабиринт": 5 платформ в 3 направления, развилка с тупиком
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-3, -1, -3), VGet(6, 1, 6), BlockType::PLATFORM));

        // === Секция 1: Два движущихся по +X, разная фаза ===
        // A (фаза 0.0, Z=0): ждать когда подойдёт и прыгнуть
        blocks.push_back(Block(VGet(5, 0, 0), VGet(3, 1, 3), BlockType::MOVING,
            0, true, VGet(5, 0, 0), VGet(19, 0, 0), 1.0f, 0.0f));
        // B (фаза 0.5, Z=5): стартует с середины пути — другой тайминг
        blocks.push_back(Block(VGet(5, 0, 5), VGet(3, 1, 3), BlockType::MOVING,
            0, true, VGet(5, 0, 5), VGet(19, 0, 5), 1.0f, 0.5f));

        // Промежуточная площадка (достижима с A или B при max X)
        // A/B max: X=19, right edge=22 → площадка X:22..27
        blocks.push_back(Block(VGet(22, 0, -1), VGet(5, 1, 10), BlockType::PLATFORM));

        // === Секция 2: Развилка ===
        // C (ТУПИК, +Z): Z:12→26, X=23 — нет выхода с острова
        blocks.push_back(Block(VGet(23, 0, 12), VGet(3, 1, 3), BlockType::MOVING,
            0, true, VGet(23, 0, 12), VGet(23, 0, 26), 1.0f, 0.3f));
        blocks.push_back(Block(VGet(20, 0, 29), VGet(8, 1, 8), BlockType::PLATFORM)); // тупиковый остров

        // D (ПРАВИЛЬНЫЙ, +X): X:28→44, Z=4 — ведёт вперёд
        // Gap от площадки (right edge X=27) до D start (X=28) = 1 ✓
        blocks.push_back(Block(VGet(28, 0, 4), VGet(4, 1, 4), BlockType::MOVING,
            0, true, VGet(28, 0, 4), VGet(44, 0, 4), 1.0f, 0.0f));

        // Площадка 2 (after D): D max X=44, right edge=48 → площадка X:48..56
        blocks.push_back(Block(VGet(48, 0, 2), VGet(8, 1, 6), BlockType::PLATFORM));

        // === Секция 3: Вертикальная платформа ===
        // E (Y: 0→11): X=58, Z=4 — поднимает на верхний ярус
        // Gap от площадки 2 (right X=56) до E start (X=58) = 2 ✓
        // E — лифт сбоку от верхней площадки: поднимается до Y=12, игрок прыгает вправо
        blocks.push_back(Block(VGet(58, 0, 4), VGet(4, 1, 4), BlockType::MOVING,
            0, true, VGet(58, 0, 4), VGet(58, 12, 4), 1.0f, 0.7f));

        // Верхняя площадка Y=12: X=63 (gap=1 от правого края лифта X=62) — игрок прыгает с лифта
        blocks.push_back(Block(VGet(63, 12, 2), VGet(8, 1, 8), BlockType::PLATFORM));

        // === Секция 4: Две встречные платформы ===
        // F (→, Z=2): X:73→89
        blocks.push_back(Block(VGet(73, 12, 2), VGet(3, 1, 3), BlockType::MOVING,
            0, true, VGet(73, 12, 2), VGet(89, 12, 2), 1.0f, 0.0f));
        // G (←, Z=6): X:89→73 — встречная к F
        blocks.push_back(Block(VGet(89, 12, 6), VGet(3, 1, 3), BlockType::MOVING,
            0, true, VGet(89, 12, 6), VGet(73, 12, 6), 1.0f, 0.0f));

        // Финальная платформа (F/G max X=89, right edge=92 → финал X:92..99)
        blocks.push_back(Block(VGet(92, 12, 3), VGet(7, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(94, 13, 5), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 18: // Level 18 - "Кнопки-обманки": настоящие кнопки открывают мосты, обманки блокируют невидимыми стенами
        playerSpawn = VGet(0, 1, 0);

        // Старт
        blocks.push_back(Block(VGet(-5, -1, -5), VGet(8, 1, 8), BlockType::PLATFORM));

        // === Зона 1: Одна кнопка — обучение механике ===
        blocks.push_back(Block(VGet(5, 0, -2), VGet(9, 1, 6), BlockType::PLATFORM)); // X:5..14, Z:-2..4
        // Настоящая кнопка (linkId=1) — открывает мост через пропасть
        blocks.push_back(Block(VGet(11, 1, 1), VGet(1, 1, 1), BlockType::BUTTON, 1));
        // Мост через пропасть (linkId=1, начинает inactive)
        blocks.push_back(Block(VGet(16, 0, -1), VGet(9, 1, 4), BlockType::DISAPPEARING, 1, false));
        // Платформа за мостом
        blocks.push_back(Block(VGet(27, 0, -2), VGet(8, 1, 7), BlockType::PLATFORM)); // X:27..35, Z:-2..5

        // === Зона 2: Две кнопки — одна настоящая, одна обманка ===
        blocks.push_back(Block(VGet(37, 0, -3), VGet(12, 1, 9), BlockType::PLATFORM)); // X:37..49, Z:-3..6
        // Кнопка A (ОБМАНКА, linkId=2) — создаёт невидимую стену прямо в проходе к мосту
        blocks.push_back(Block(VGet(40, 1, -1), VGet(1, 1, 1), BlockType::BUTTON, 2));
        blocks.push_back(Block(VGet(49, 0, -3), VGet(1, 6, 9), BlockType::INVISIBLE_WALL, 2, false));
        // Кнопка B (НАСТОЯЩАЯ, linkId=3) — открывает мост
        blocks.push_back(Block(VGet(44, 1, 3), VGet(1, 1, 1), BlockType::BUTTON, 3));
        // Мост (linkId=3, начинает inactive)
        blocks.push_back(Block(VGet(51, 0, -1), VGet(9, 1, 4), BlockType::DISAPPEARING, 3, false));
        // Платформа за вторым мостом
        blocks.push_back(Block(VGet(62, 0, -3), VGet(14, 1, 9), BlockType::PLATFORM)); // X:62..76, Z:-3..6

        // === Зона 3: Три кнопки — две обманки, одна настоящая ===
        // Кнопка L (ОБМАНКА, linkId=4) — стена перекрывает выход с платформы (X=76)
        blocks.push_back(Block(VGet(65, 1, -1), VGet(1, 1, 1), BlockType::BUTTON, 4));
        blocks.push_back(Block(VGet(76, 0, -3), VGet(1, 6, 9), BlockType::INVISIBLE_WALL, 4, false));
        // Кнопка M (НАСТОЯЩАЯ, linkId=5) — открывает мост (X:77→86)
        blocks.push_back(Block(VGet(69, 1, 1), VGet(1, 1, 1), BlockType::BUTTON, 5));
        blocks.push_back(Block(VGet(77, 0, -1), VGet(9, 1, 4), BlockType::DISAPPEARING, 5, false));
        // Кнопка R (ОБМАНКА, linkId=6) — стена в середине моста (X=81): можно зайти, но не пройти
        blocks.push_back(Block(VGet(73, 1, 4), VGet(1, 1, 1), BlockType::BUTTON, 6));
        blocks.push_back(Block(VGet(81, 0, -3), VGet(1, 6, 9), BlockType::INVISIBLE_WALL, 6, false));

        // Финальная платформа + триггер
        blocks.push_back(Block(VGet(88, 0, -2), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(90, 1, 0), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 19: // Level 19 - "Обман на обмане": 5 секций нарастающего троллинга
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // === Секция 1: FAKE мост прямо + настоящий путь через FAKE_SPIKES сбоку ===
        // Четыре FAKE_PLATFORM подряд — игрок проваливается на первом шаге
        blocks.push_back(Block(VGet(5,  0, -1), VGet(4, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(11, 0, -1), VGet(4, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(17, 0, -1), VGet(4, 1, 3), BlockType::FAKE_PLATFORM));
        blocks.push_back(Block(VGet(23, 0, -1), VGet(4, 1, 3), BlockType::FAKE_PLATFORM));
        // Настоящий путь сбоку (Z:5..9) с FAKE_SPIKES — выглядит страшно, но безопасно
        // Gap от старта (Z:-4..3) до бокового пути (Z=5): прыжок вбок = 2 единицы ✓
        blocks.push_back(Block(VGet(5, 0, 5), VGet(24, 1, 5), BlockType::PLATFORM)); // X:5..29
        blocks.push_back(Block(VGet(9,  1, 6), VGet(3, 1, 3), BlockType::FAKE_SPIKES));
        blocks.push_back(Block(VGet(18, 1, 6), VGet(3, 1, 3), BlockType::FAKE_SPIKES));
        // Стыковочная площадка (возврат к центру)
        blocks.push_back(Block(VGet(29, 0, 0), VGet(7, 1, 10), BlockType::PLATFORM));

        // === Секция 2: FAKE_SPIKES × 2 → настоящие SPIKES-ловушка ===
        // Игрок привыкает что розовые = безопасно, а потом попадает на красные
        blocks.push_back(Block(VGet(36, 0, 0), VGet(22, 1, 8), BlockType::PLATFORM)); // X:36..58
        blocks.push_back(Block(VGet(38, 1, 1), VGet(3, 1, 5), BlockType::FAKE_SPIKES)); // розовые (ок)
        blocks.push_back(Block(VGet(44, 1, 1), VGet(3, 1, 5), BlockType::FAKE_SPIKES)); // розовые (ок)
        blocks.push_back(Block(VGet(50, 1, 1), VGet(3, 1, 4), BlockType::SPIKES));       // КРАСНЫЕ — смерть! (Z:1..5)
        // Обход у дальнего края (Z:5..8) — 3 единицы свободного прохода
        blocks.push_back(Block(VGet(55, 1, 1), VGet(3, 1, 5), BlockType::FAKE_SPIKES)); // снова безопасные

        // === Секция 3: CRUMBLING (жёлтые) + FAKE_PLATFORM (серые) — прыжки через серые ===
        // Серые проваливаются, жёлтые рассыпаются → надо прыгать с жёлтого на жёлтое
        // Размер C=4, F=1, gap=1: прыжок C→C = 1gap + 1F + 1gap = 3 единицы ✓
        blocks.push_back(Block(VGet(60, 0, 2), VGet(4, 1, 4), BlockType::CRUMBLING));    // жёлтый (X:60..64)
        blocks.push_back(Block(VGet(65, 0, 2), VGet(1, 1, 4), BlockType::FAKE_PLATFORM));// серый gap=1 (X:65..66)
        blocks.push_back(Block(VGet(67, 0, 2), VGet(4, 1, 4), BlockType::CRUMBLING));    // жёлтый gap=1 (X:67..71)
        blocks.push_back(Block(VGet(72, 0, 2), VGet(1, 1, 4), BlockType::FAKE_PLATFORM));// серый (X:72..73)
        blocks.push_back(Block(VGet(74, 0, 2), VGet(4, 1, 4), BlockType::CRUMBLING));    // жёлтый (X:74..78)
        blocks.push_back(Block(VGet(81, 0, 1), VGet(7, 1, 6), BlockType::PLATFORM));     // посадочная (gap=3)

        // === Секция 4: Очевидный переход — FAKE, реальный путь — движущаяся платформа ===
        // FAKE_PLATFORM прямо (Z:1..6): игрок считает "вот мост" → падает
        blocks.push_back(Block(VGet(91, 0, 1), VGet(9, 1, 5), BlockType::FAKE_PLATFORM));
        // MOVING реальная (Z:7..10): от посадочной (Z:1..7) достать без прыжка gap=0 ✓
        blocks.push_back(Block(VGet(91, 0, 7), VGet(3, 1, 4), BlockType::MOVING,
            0, true, VGet(91, 0, 7), VGet(105, 0, 7), 1.0f, 0.0f));
        // Посадочная от движущейся (X:107..115, Z:5..12)
        blocks.push_back(Block(VGet(107, 0, 5), VGet(8, 1, 8), BlockType::PLATFORM));

        // === Секция 5: Финал — три RETRACTABLE_SPIKES волной ===
        // Спайки перекрывают всю ширину (Z:3..11) → только тайминг
        blocks.push_back(Block(VGet(117, 0, 3), VGet(16, 1, 9), BlockType::PLATFORM)); // X:117..133
        // Фазы: cycleTime=2.67s → сдвиг на 0.89s между спайками (настоящая волна)
        blocks.push_back(Block(VGet(119, 1, 3), VGet(2, 1, 9), BlockType::RETRACTABLE_SPIKES,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.5f, 0.0f));
        blocks.push_back(Block(VGet(124, 1, 3), VGet(2, 1, 9), BlockType::RETRACTABLE_SPIKES,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.5f, 0.89f));
        blocks.push_back(Block(VGet(129, 1, 3), VGet(2, 1, 9), BlockType::RETRACTABLE_SPIKES,
            0, true, VGet(0,0,0), VGet(0,0,0), 1.5f, 1.78f));
        // Финиш
        blocks.push_back(Block(VGet(135, 0, 3), VGet(7, 1, 7), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(137, 1, 5), VGet(2, 1, 2), BlockType::TRIGGER));
        break;

    case 20: // Level 20 - "Финал Block 2": все механики в одном гранд-финале
        playerSpawn = VGet(0, 1, 0);
        blocks.push_back(Block(VGet(-4, -1, -4), VGet(7, 1, 7), BlockType::PLATFORM));

        // === Секция 1: CRUMBLING спринт — 5 платформ, надо бежать не останавливаясь ===
        // gap=2 между каждой, рассыпаются через 0.5s
        blocks.push_back(Block(VGet(5,  0, -1), VGet(4, 1, 4), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(11, 0, -1), VGet(4, 1, 4), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(17, 0, -1), VGet(4, 1, 4), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(23, 0, -1), VGet(4, 1, 4), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(29, 0, -1), VGet(4, 1, 4), BlockType::CRUMBLING));
        // Отдых 1
        blocks.push_back(Block(VGet(35, 0, -2), VGet(7, 1, 6), BlockType::PLATFORM));

        // === Секция 2: RETRACTABLE_SPIKES коридор — 4 спайка волной (speed=1.5) ===
        // cycleTime=2.67s: фазы равномерно сдвинуты на 0.67s
        blocks.push_back(Block(VGet(44, 0, -2), VGet(24, 1, 5), BlockType::PLATFORM)); // X:44..68
        blocks.push_back(Block(VGet(47, 1, -2), VGet(2, 1, 5), BlockType::RETRACTABLE_SPIKES,
            0, true,  VGet(0,0,0), VGet(0,0,0), 1.5f, 0.0f));
        blocks.push_back(Block(VGet(52, 1, -2), VGet(2, 1, 5), BlockType::RETRACTABLE_SPIKES,
            0, true,  VGet(0,0,0), VGet(0,0,0), 1.5f, 0.67f));
        blocks.push_back(Block(VGet(57, 1, -2), VGet(2, 1, 5), BlockType::RETRACTABLE_SPIKES,
            0, true,  VGet(0,0,0), VGet(0,0,0), 1.5f, 1.34f)); // 1.34 > halfCycle(1.333) → чётко начинает спрятанным
        blocks.push_back(Block(VGet(62, 1, -2), VGet(2, 1, 5), BlockType::RETRACTABLE_SPIKES,
            0, true,  VGet(0,0,0), VGet(0,0,0), 1.5f, 2.0f));
        // Отдых 2
        blocks.push_back(Block(VGet(70, 0, -2), VGet(7, 1, 6), BlockType::PLATFORM));

        // === Секция 3: MOVING × 2 — два движущихся с промежуточной площадкой ===
        // A (медленный, tim=0.0): X:79→94, сразу доступен
        blocks.push_back(Block(VGet(79, 0, 0), VGet(4, 1, 4), BlockType::MOVING,
            0, true, VGet(79, 0, 0), VGet(94, 0, 0), 1.0f, 0.0f));
        // Промежуточная площадка (A max=X:94..98 → площадка X:97..103, gap=1 ✓)
        blocks.push_back(Block(VGet(97, 0, -1), VGet(6, 1, 6), BlockType::PLATFORM));
        // B (быстрее, tim=0.4): X:105→122, стартует ближе к середине → нужно чуть подождать
        blocks.push_back(Block(VGet(105, 0, 1), VGet(4, 1, 4), BlockType::MOVING,
            0, true, VGet(105, 0, 1), VGet(122, 0, 1), 1.0f, 0.4f));
        // Посадочная (B max=X:122..126 → площадка X:124..132, gap=1 ✓)
        blocks.push_back(Block(VGet(124, 0, -1), VGet(8, 1, 7), BlockType::PLATFORM));

        // === Секция 4: FAKE_SPIKES × 2 → SPIKES-ловушка + FAKE_PLATFORM над пропастью ===
        blocks.push_back(Block(VGet(134, 0, -1), VGet(20, 1, 8), BlockType::PLATFORM)); // X:134..154
        blocks.push_back(Block(VGet(137, 1,  0), VGet(3, 1, 6), BlockType::FAKE_SPIKES));  // розовые (безопасны)
        blocks.push_back(Block(VGet(143, 1,  0), VGet(3, 1, 6), BlockType::FAKE_SPIKES));  // розовые (безопасны)
        blocks.push_back(Block(VGet(149, 1,  0), VGet(3, 1, 5), BlockType::SPIKES));        // КРАСНЫЕ — смерть! (Z:0..5)
        // Обход у дальнего края (Z:5..7) — 2 единицы свободного прохода
        // FAKE_PLATFORM за пропастью (прямо, Z:-1..5): провалится
        blocks.push_back(Block(VGet(156, 0, -1), VGet(8, 1, 6), BlockType::FAKE_PLATFORM));
        // Настоящая посадочная (Z:6..12): нужно прыгнуть вбок от края секции 4
        blocks.push_back(Block(VGet(156, 0, 6), VGet(8, 1, 7), BlockType::PLATFORM));

        // === Секция 5: BUTTON — обманка (→ невидимая стена) + настоящая (→ мост) ===
        blocks.push_back(Block(VGet(166, 0, 6), VGet(12, 1, 8), BlockType::PLATFORM)); // X:166..178
        // Кнопка-обманка (linkId=7): стена перекрывает выход с платформы (X=178)
        blocks.push_back(Block(VGet(168, 1, 8),  VGet(1, 1, 1), BlockType::BUTTON, 7));
        blocks.push_back(Block(VGet(178, 0, 6),  VGet(1, 6, 8), BlockType::INVISIBLE_WALL, 7, false));
        // Настоящая кнопка (linkId=8): открывает мост через пропасть
        blocks.push_back(Block(VGet(174, 1, 10), VGet(1, 1, 1), BlockType::BUTTON, 8));
        blocks.push_back(Block(VGet(180, 0, 8),  VGet(10, 1, 4), BlockType::DISAPPEARING, 8, false));
        // Посадочная за мостом
        blocks.push_back(Block(VGet(192, 0, 7), VGet(8, 1, 7), BlockType::PLATFORM));

        // === Секция 6: MOVING + CRUMBLING комбо — финальный рывок ===
        // Движущаяся переносит к цепочке CRUMBLING
        blocks.push_back(Block(VGet(202, 0, 9), VGet(4, 1, 4), BlockType::MOVING,
            0, true, VGet(202, 0, 9), VGet(218, 0, 9), 1.0f, 0.0f));
        // Три CRUMBLING — надо бежать без остановки (gap=2)
        // MOVING max: X:218..222 → первый CRUMBLING X:221, gap=1 ✓
        blocks.push_back(Block(VGet(221, 0, 8), VGet(4, 1, 4), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(227, 0, 8), VGet(4, 1, 4), BlockType::CRUMBLING));
        blocks.push_back(Block(VGet(233, 0, 8), VGet(4, 1, 4), BlockType::CRUMBLING));
        // Финальная платформа + триггер
        blocks.push_back(Block(VGet(239, 0, 7), VGet(8, 1, 8), BlockType::PLATFORM));
        blocks.push_back(Block(VGet(241, 1, 9), VGet(2, 1, 2), BlockType::TRIGGER));
        break;
    }
}
