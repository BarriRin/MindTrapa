#include "Menu.h"
#include "ProfileManager.h"
#include <cmath>
#include <windows.h>

void Menu::LoadFonts() {
    AddFontResourceEx(L"fonts/Orbitron/static/Orbitron-Bold.ttf",    FR_PRIVATE, NULL);
    AddFontResourceEx(L"fonts/Orbitron/static/Orbitron-Regular.ttf", FR_PRIVATE, NULL);

    fontTitle   = CreateFontToHandle(L"Orbitron", 60, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4, -1, 3);
    fontHeading = CreateFontToHandle(L"Orbitron", 32, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4, -1, 2);
    fontButton  = CreateFontToHandle(L"Orbitron", 22, -1, DX_FONTTYPE_ANTIALIASING_4X4);
    fontNormal  = CreateFontToHandle(L"Orbitron", 18, -1, DX_FONTTYPE_ANTIALIASING_4X4);
    fontSmall   = CreateFontToHandle(L"Orbitron", 13, -1, DX_FONTTYPE_ANTIALIASING_4X4);
}

void Menu::ApplyFontToButtons(int handle) {
    for (auto& b : buttons) b.fontHandle = handle;
}

Menu::Menu(int levels)
    : currentState(GameState::MAIN_MENU), selectedButtonIndex(0), totalLevels(levels),
      selectedBlock(0), unlockedBlocks(1), unlockedLevels(1),
      mousePressed(false), upPressed(false), downPressed(false),
      enterPressed(false), escPressed(false) {
    stateHistory.clear();
    LoadFonts();
    CreateMainMenuButtons();
    ApplyFontToButtons(fontButton);
}

Menu::~Menu() {
    if (fontTitle   != -1) DeleteFontToHandle(fontTitle);
    if (fontHeading != -1) DeleteFontToHandle(fontHeading);
    if (fontButton  != -1) DeleteFontToHandle(fontButton);
    if (fontNormal  != -1) DeleteFontToHandle(fontNormal);
    if (fontSmall   != -1) DeleteFontToHandle(fontSmall);
    RemoveFontResourceEx(L"fonts/Orbitron/static/Orbitron-Bold.ttf",    FR_PRIVATE, NULL);
    RemoveFontResourceEx(L"fonts/Orbitron/static/Orbitron-Regular.ttf", FR_PRIVATE, NULL);
    buttons.clear();
}

void Menu::SetState(GameState state) {
    currentState = state;
    selectedButtonIndex = 0;
    buttons.clear();

    switch (state) {
    case GameState::MAIN_MENU:
        CreateMainMenuButtons();
        ApplyFontToButtons(fontButton);
        break;
    case GameState::LEVEL_SELECT_BLOCKS:
        // НЕ обнуляем selectedBlock - он уже установлен если пришли из выбора блока
        CreateBlockSelectButtons();
        ApplyFontToButtons(fontButton);
        break;
    case GameState::LEVEL_SELECT_LEVELS:
        CreateLevelSelectButtons();
        ApplyFontToButtons(fontSmall);
        break;
    case GameState::SETTINGS:
        CreateSettingsButtons();
        ApplyFontToButtons(fontButton);
        break;
    case GameState::MUSIC_SELECT:
        CreateMusicSelectButtons();
        ApplyFontToButtons(fontButton);
        break;
    case GameState::PAUSED:
        CreatePauseMenuButtons();
        ApplyFontToButtons(fontButton);
        break;
    case GameState::PROFILE_SELECT:
        CreateProfileSelectButtons();
        ApplyFontToButtons(fontButton);
        break;
    case GameState::PROFILE_CREATE:
        CreateProfileCreateButtons();
        ApplyFontToButtons(fontButton);
        break;
    case GameState::PLAYING:
        buttons.clear();
        break;
    case GameState::DYING:
        buttons.clear();
        break;
    case GameState::LEVEL_CELEBRATING:
        buttons.clear();
        break;
    case GameState::LEVEL_RESULT:
        CreateResultScreenButtons();
        ApplyFontToButtons(fontButton);
        break;
    }
}

void Menu::ShowResultScreen(bool completed, int stars, float time, bool isLastLevel) {
    resultCompleted   = completed;
    resultIsLastLevel = isLastLevel;
    resultStars       = stars;
    resultTime        = time;
    SetState(GameState::LEVEL_RESULT);
    ResetInputFlags();
}

void Menu::PushState(GameState newState) {
    // Сохраняем текущее состояние в стек
    stateHistory.push_back(currentState);
    // Переходим в новое
    SetState(newState);
}

void Menu::PopState() {
    // Если стек пуст, ничего не делаем
    if (stateHistory.empty()) {
        return;
    }

    // Достаём предыдущее состояние
    GameState previousState = stateHistory.back();
    stateHistory.pop_back();

    // Переходим в него
    SetState(previousState);
}

void Menu::ClearHistory() {
    stateHistory.clear();
}

void Menu::ResetInputFlags() {
    mousePressed = true;   // Устанавливаем в true чтобы не обработать текущее нажатие
    upPressed = true;
    downPressed = true;
    enterPressed = true;
    escPressed = true;
}

void Menu::CreateMainMenuButtons() {
    int centerX = 1920 / 2;
    int startY = 400;
    int buttonWidth = 300;
    int buttonHeight = 60;
    int spacing = 80;

    buttons.push_back(Button(L"Start Game",     centerX - buttonWidth / 2, startY,             buttonWidth, buttonHeight, ButtonAction::START_GAME));
    buttons.push_back(Button(L"Level Select",   centerX - buttonWidth / 2, startY + spacing,   buttonWidth, buttonHeight, ButtonAction::OPEN_LEVEL_SELECT));
    buttons.push_back(Button(L"Settings",       centerX - buttonWidth / 2, startY + spacing*2, buttonWidth, buttonHeight, ButtonAction::OPEN_SETTINGS));
    buttons.push_back(Button(L"Change Profile", centerX - buttonWidth / 2, startY + spacing*3, buttonWidth, buttonHeight, ButtonAction::CHANGE_PROFILE));
    buttons.push_back(Button(L"Exit",           centerX - buttonWidth / 2, startY + spacing*4, buttonWidth, buttonHeight, ButtonAction::EXIT_GAME));
}

void Menu::CreateBlockSelectButtons() {
    int startX = 500;
    int startY = 250;
    int buttonWidth = 220;
    int buttonHeight = 120;
    int spacingX = 260;
    int spacingY = 150;
    int buttonsPerRow = 3;

    // Создаём 5 кнопок блоков (блоки 6-10 удалены, игра 50 уровней)
    for (int i = 0; i < 5; i++) {
        int row = i / buttonsPerRow;
        int col = i % buttonsPerRow;
        int x = startX + col * spacingX;
        int y = startY + row * spacingY;

        std::wstring blockText = L"Block " + std::to_wstring(i + 1);
        if (i >= unlockedBlocks) {
            blockText += L"\n(LOCKED)";
        }

        ButtonAction action = (ButtonAction)((int)ButtonAction::SELECT_BLOCK_1 + i);
        buttons.push_back(Button(blockText, x, y, buttonWidth, buttonHeight, action));
    }

    // Кнопка назад
    buttons.push_back(Button(L"< Back", 100, 950, 200, 60, ButtonAction::BACK_TO_MENU));
}

void Menu::CreateLevelSelectButtons() {
    int startX = 400;
    int startY = 300;
    int buttonWidth = 120;
    int buttonHeight = 80;
    int spacingX = 150;
    int spacingY = 100;
    int buttonsPerRow = 5;

    // 10 levels per block
    for (int i = 0; i < 10; i++) {
        int row = i / buttonsPerRow;
        int col = i % buttonsPerRow;
        int x = startX + col * spacingX;
        int y = startY + row * spacingY;

        std::wstring levelText = L"Level " + std::to_wstring(i + 1);

        // Check if block is unlocked and level exists
        int calculatedLevel = (selectedBlock - 1) * 10 + i + 1;
        if (selectedBlock > unlockedBlocks || calculatedLevel > totalLevels || calculatedLevel > unlockedLevels) {
            levelText += L"\n(LOCKED)";
        }

        buttons.push_back(Button(levelText, x, y, buttonWidth, buttonHeight, ButtonAction::LOAD_LEVEL));
    }

    // Back button - returns to block selection
    buttons.push_back(Button(L"< Back to Blocks", 100, 900, 250, 60, ButtonAction::BACK_TO_MENU));
}

void Menu::CreateSettingsButtons() {
    int centerX = 1920 / 2;
    int startY = 350;
    int buttonWidth = 300;
    int buttonHeight = 60;
    int spacing = 120;

    // Sliders drawn in Draw(); buttons only for navigation
    buttons.push_back(Button(L"Music Select", centerX - buttonWidth / 2, startY + spacing * 3, buttonWidth, buttonHeight, ButtonAction::OPEN_MUSIC_SELECT));
    buttons.push_back(Button(L"< Back",       100, 950, 200, 60,                                ButtonAction::BACK_TO_MENU));
}

void Menu::CreateMusicSelectButtons() {
    int centerX = 1920 / 2;
    int startY = 300;
    int buttonWidth = 400;
    int buttonHeight = 70;
    int spacing = 90;

    // 5 music tracks: Default + 4 custom (unlocked by achievements)
    const wchar_t* trackNames[] = {
        L"Default Theme",
        L"Track 1 (LOCKED)",
        L"Track 2 (LOCKED)",
        L"Track 3 (LOCKED)",
        L"Track 4 (LOCKED)"
    };

    for (int i = 0; i < 5; i++) {
        buttons.push_back(Button(trackNames[i], centerX - buttonWidth / 2, startY + i * spacing, buttonWidth, buttonHeight, ButtonAction::NONE));
    }

    buttons.push_back(Button(L"< Back", 100, 950, 200, 60, ButtonAction::BACK_TO_SETTINGS));
}

void Menu::CreatePauseMenuButtons() {
    int centerX = 1920 / 2;
    int startY = 400;
    int buttonWidth = 300;
    int buttonHeight = 60;
    int spacing = 80;

    // 4 кнопки: Continue, Restart, Settings, Main Menu
    buttons.push_back(Button(L"Continue", centerX - buttonWidth / 2, startY, buttonWidth, buttonHeight, ButtonAction::CONTINUE));
    buttons.push_back(Button(L"Restart Level", centerX - buttonWidth / 2, startY + spacing, buttonWidth, buttonHeight, ButtonAction::RESTART));
    buttons.push_back(Button(L"Settings", centerX - buttonWidth / 2, startY + spacing * 2, buttonWidth, buttonHeight, ButtonAction::OPEN_SETTINGS));
    buttons.push_back(Button(L"Main Menu", centerX - buttonWidth / 2, startY + spacing * 3, buttonWidth, buttonHeight, ButtonAction::BACK_TO_MENU));
}

void Menu::CreateResultScreenButtons() {
    int buttonWidth  = 240;
    int buttonHeight = 60;
    int gap          = 30;
    int totalWidth   = 3 * buttonWidth + 2 * gap;
    int startX       = 1920 / 2 - totalWidth / 2;
    int y            = 750;

    bool nextDisabled = !resultCompleted || resultIsLastLevel;
    buttons.push_back(Button(L"Restart",    startX,                           y, buttonWidth, buttonHeight, ButtonAction::RESTART));
    buttons.push_back(Button(L"Main Menu",  startX + (buttonWidth + gap),     y, buttonWidth, buttonHeight, ButtonAction::BACK_TO_MENU));
    buttons.push_back(Button(L"Next Level", startX + (buttonWidth + gap) * 2, y, buttonWidth, buttonHeight, ButtonAction::NEXT_LEVEL, nextDisabled));
}

// ── Profile screens ───────────────────────────────────────────────────────────

void Menu::OpenProfileSelect(bool canGoBack) {
    profileSelectCanGoBack = canGoBack;
    pendingDeleteSlot      = -1;
    SetState(GameState::PROFILE_SELECT);
    ResetInputFlags();
}

void Menu::OpenProfileCreate(int slot) {
    pendingCreateSlot = slot;
    if (keyInputHandle != -1) { DeleteKeyInput(keyInputHandle); keyInputHandle = -1; }
    keyInputHandle = MakeKeyInput(16, FALSE, FALSE, FALSE);
    SetActiveKeyInput(keyInputHandle);
    SetState(GameState::PROFILE_CREATE);
    ResetInputFlags();
}

void Menu::SyncSettingsFromProfile() {
    const Profile* p = ProfileManager::GetInstance().GetCurrentProfile();
    if (!p) return;
    settings.musicVolume      = p->musicVolume;
    settings.soundVolume      = p->sfxVolume;
    settings.mouseSensitivity = p->mouseSensitivity;
    UpdateUnlockState();
}

void Menu::UpdateUnlockState() {
    const Profile* p = ProfileManager::GetInstance().GetCurrentProfile();
    if (!p) {
        unlockedLevels = 1;
        unlockedBlocks = 1;
        return;
    }
    // Уровень N открыт, если N-1 пройден (stars > 0); уровень 1 всегда открыт
    unlockedLevels = 1;
    for (int i = 0; i < PROFILE_LEVEL_COUNT - 1; i++) {
        if (p->stars[i] > 0)
            unlockedLevels = i + 2; // следующий за пройденным
        else
            break; // цепочка прерывается
    }
    // Блок N открыт если в нём есть хотя бы один доступный уровень
    unlockedBlocks = (unlockedLevels - 1) / 10 + 1;
    if (unlockedBlocks > 5) unlockedBlocks = 5;
}

void Menu::CreateProfileSelectButtons() {
    ProfileManager& pm = ProfileManager::GetInstance();

    if (pendingDeleteSlot >= 0) {
        const int bw = 240, bh = 60, gap = 30;
        int startX = 1920 / 2 - (bw * 2 + gap) / 2;
        buttons.push_back(Button(L"Yes, Delete", startX,           540, bw, bh, ButtonAction::DELETE_PROFILE_CONFIRM));
        buttons.push_back(Button(L"Cancel",      startX + bw + gap, 540, bw, bh, ButtonAction::DELETE_PROFILE_CANCEL));
        return;
    }

    const int cardW    = 250;
    const int cardH    = 300;
    const int spacing  = 40;
    const int total    = PROFILE_SLOT_COUNT * cardW + (PROFILE_SLOT_COUNT - 1) * spacing;
    const int startX   = (1920 - total) / 2;
    const int cardY    = 310;
    const int btnW     = 180;
    const int btnH     = 50;

    for (int i = 0; i < PROFILE_SLOT_COUNT; i++) {
        int cx = startX + i * (cardW + spacing);
        ButtonAction selAction = (ButtonAction)((int)ButtonAction::SELECT_PROFILE_0 + i);

        if (pm.IsSlotEmpty(i)) {
            buttons.push_back(Button(L"+ Create", cx + (cardW - btnW) / 2, cardY + cardH - 65, btnW, btnH, selAction));
        } else {
            buttons.push_back(Button(L"Select",   cx + (cardW - btnW) / 2, cardY + cardH - 65, btnW, btnH, selAction));
            ButtonAction delAction = (ButtonAction)((int)ButtonAction::DELETE_PROFILE_0 + i);
            buttons.push_back(Button(L"X", cx + cardW - 38, cardY + 5, 33, 33, delAction));
        }
    }

    if (profileSelectCanGoBack) {
        buttons.push_back(Button(L"< Back", 100, 950, 200, 60, ButtonAction::BACK_TO_MENU));
    }
}

void Menu::CreateProfileCreateButtons() {
    const int bw = 240, bh = 60, gap = 30;
    int startX = 1920 / 2 - (bw * 2 + gap) / 2;
    buttons.push_back(Button(L"Create", startX,           620, bw, bh, ButtonAction::CREATE_PROFILE_CONFIRM));
    buttons.push_back(Button(L"Cancel", startX + bw + gap, 620, bw, bh, ButtonAction::BACK_TO_MENU));
}

void Menu::UpdateHover(int mouseX, int mouseY) {
    for (size_t i = 0; i < buttons.size(); i++) {
        buttons[i].isHovered = buttons[i].Contains(mouseX, mouseY);
        if (buttons[i].isHovered) {
            selectedButtonIndex = i; // Синхронизация с клавиатурной навигацией
        }
    }
}

void Menu::Draw() const {
    // Затемнённый фон
    if (currentState == GameState::PAUSED || currentState == GameState::LEVEL_RESULT) {
        int alpha = (currentState == GameState::LEVEL_RESULT) ? 180 : 128;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    // Заголовок
    unsigned int titleColor = GetColor(255, 255, 255);
    int titleSize = 48;

    switch (currentState) {
    case GameState::PROFILE_SELECT:
        {
            ProfileManager& pm = ProfileManager::GetInstance();
            {
                const wchar_t* hdr = L"SELECT PROFILE";
                int hw = GetDrawStringWidthToHandle(hdr, (int)wcslen(hdr), fontHeading);
                DrawStringToHandle(1920/2 - hw/2, 170, hdr, titleColor, fontHeading);
            }

            const int cardW   = 250, cardH = 300;
            const int spacing = 40;
            const int total   = PROFILE_SLOT_COUNT * cardW + (PROFILE_SLOT_COUNT-1) * spacing;
            const int startX  = (1920 - total) / 2;
            const int cardY   = 310;

            for (int i = 0; i < PROFILE_SLOT_COUNT; i++) {
                int cx = startX + i * (cardW + spacing);
                bool active = (pm.GetCurrentSlot() == i);
                unsigned int bg     = active ? GetColor(30,60,110) : GetColor(25,25,45);
                unsigned int border = active ? GetColor(100,180,255) : GetColor(70,70,100);

                DrawBox(cx, cardY, cx+cardW, cardY+cardH, bg, TRUE);
                DrawBox(cx, cardY, cx+cardW, cardY+cardH, border, FALSE);

                wchar_t slotLabel[16];
                swprintf_s(slotLabel, L"SLOT %d", i+1);
                DrawStringToHandle(cx+10, cardY+10, slotLabel, GetColor(130,130,160), fontSmall);

                if (pm.IsSlotEmpty(i)) {
                    DrawStringToHandle(cx+55, cardY+110, L"EMPTY", GetColor(80,80,100), fontNormal);
                } else {
                    const Profile& p = pm.GetSlot(i);
                    int nameW = GetDrawStringWidthToHandle(p.nickname.c_str(), (int)p.nickname.size(), fontNormal);
                    DrawStringToHandle(cx + (cardW-nameW)/2, cardY+90, p.nickname.c_str(), GetColor(220,220,255), fontNormal);
                    wchar_t starsBuf[32];
                    swprintf_s(starsBuf, L"Stars: %d / %d", p.TotalStars(), PROFILE_LEVEL_COUNT * 3);
                    DrawStringToHandle(cx+20, cardY+140, starsBuf, GetColor(255,210,0), fontSmall);
                }
            }

            // Delete confirmation overlay
            if (pendingDeleteSlot >= 0) {
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
                DrawBox(0, 0, 1920, 1080, GetColor(0,0,0), TRUE);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                const Profile& p = pm.GetSlot(pendingDeleteSlot);
                wchar_t confirmMsg[64];
                swprintf_s(confirmMsg, L"Delete profile \"%ls\"?", p.nickname.c_str());
                int mw = GetDrawStringWidthToHandle(confirmMsg, (int)wcslen(confirmMsg), fontNormal);
                DrawStringToHandle(1920/2 - mw/2, 460, confirmMsg, GetColor(255,80,80), fontNormal);
                DrawStringToHandle(1920/2 - 220, 510, L"All progress will be lost.", GetColor(180,180,180), fontSmall);
            }
        }
        break;

    case GameState::PROFILE_CREATE:
        {
            const wchar_t* hdr = L"CREATE PROFILE";
            int hw = GetDrawStringWidthToHandle(hdr, (int)wcslen(hdr), fontHeading);
            DrawStringToHandle(1920/2 - hw/2, 280, hdr, titleColor, fontHeading);
            DrawStringToHandle(1920/2 - 160, 450, L"Enter nickname:", GetColor(200,200,200), fontNormal);
        }
        if (keyInputHandle != -1) {
            DrawBox(1920/2 - 160, 490, 1920/2 + 160, 540, GetColor(30,30,60), TRUE);
            DrawBox(1920/2 - 160, 490, 1920/2 + 160, 540, GetColor(80,80,120), FALSE);
            DrawKeyInputString(1920/2 - 150, 500, keyInputHandle, TRUE);
        }
        break;

    case GameState::MAIN_MENU:
        {
            const wchar_t* gameTitle = L"MINDTRAPA";
            int tw = GetDrawStringWidthToHandle(gameTitle, (int)wcslen(gameTitle), fontTitle);
            DrawStringToHandle(1920/2 - tw/2, 190, gameTitle, titleColor, fontTitle);

            const Profile* p = ProfileManager::GetInstance().GetCurrentProfile();
            if (p) {
                wchar_t profileBuf[64];
                swprintf_s(profileBuf, L"%ls  |  Stars: %d", p->nickname.c_str(), p->TotalStars());
                DrawStringToHandle(20, 20, profileBuf, GetColor(120, 180, 255), fontNormal);
            }
        }
        break;
    case GameState::LEVEL_SELECT_BLOCKS:
        {
            const wchar_t* hdr = L"SELECT BLOCK";
            int hw = GetDrawStringWidthToHandle(hdr, (int)wcslen(hdr), fontHeading);
            DrawStringToHandle(1920/2 - hw/2, 150, hdr, titleColor, fontHeading);
        }
        break;
    case GameState::LEVEL_SELECT_LEVELS:
        {
            wchar_t blockHeader[64];
            swprintf_s(blockHeader, L"BLOCK %d — LEVELS 1-10", selectedBlock);
            int hw = GetDrawStringWidthToHandle(blockHeader, (int)wcslen(blockHeader), fontHeading);
            DrawStringToHandle(1920/2 - hw/2, 150, blockHeader, titleColor, fontHeading);

            // Звёзды под кнопками уровней
            const Profile* p = ProfileManager::GetInstance().GetCurrentProfile();
            const float PI = 3.14159265f;
            auto drawSmallStar = [&](int cx, int cy, int earned) {
                float outerR = 7.0f, innerR = 3.0f;
                unsigned int col = earned ? GetColor(255, 200, 0) : GetColor(45, 45, 65);
                for (int t = 0; t < 5; t++) {
                    float a0 = PI / 2 + t * 2 * PI / 5;
                    float a1 = a0 + PI / 5;
                    float a2 = a0 + 2 * PI / 5;
                    int x0 = cx + (int)(outerR * cosf(a0)), y0 = cy - (int)(outerR * sinf(a0));
                    int x1 = cx + (int)(innerR * cosf(a1)), y1 = cy - (int)(innerR * sinf(a1));
                    int x2 = cx + (int)(outerR * cosf(a2)), y2 = cy - (int)(outerR * sinf(a2));
                    DrawTriangle(cx, cy, x0, y0, x1, y1, col, TRUE);
                    DrawTriangle(cx, cy, x1, y1, x2, y2, col, TRUE);
                }
            };

            int startX = 400, startY = 300;
            int buttonWidth = 120, buttonHeight = 80;
            int spacingX = 150, spacingY = 100;
            for (int i = 0; i < 10; i++) {
                int row = i / 5, col = i % 5;
                int bx = startX + col * spacingX;
                int by = startY + row * spacingY;
                int levelId = (selectedBlock - 1) * 10 + i + 1;
                int earned = (p && levelId <= PROFILE_LEVEL_COUNT) ? p->stars[levelId - 1] : 0;

                int starBaseX = bx + buttonWidth / 2 - 16;
                int starY = by + buttonHeight + 12;
                for (int s = 0; s < 3; s++)
                    drawSmallStar(starBaseX + s * 16, starY, s < earned);
            }
        }
        break;
    case GameState::SETTINGS:
        {
            const wchar_t* hdr = L"SETTINGS";
            int hw = GetDrawStringWidthToHandle(hdr, (int)wcslen(hdr), fontHeading);
            DrawStringToHandle(1920/2 - hw/2, 200, hdr, titleColor, fontHeading);
        }
        DrawSlider(1920/2 - 200, 350, 400, settings.musicVolume,      L"Music Volume");
        DrawSlider(1920/2 - 200, 470, 400, settings.soundVolume,      L"Sound Volume");
        DrawSlider(1920/2 - 200, 590, 400, settings.mouseSensitivity, L"Mouse Sensitivity");
        break;
    case GameState::MUSIC_SELECT:
        {
            const wchar_t* hdr = L"MUSIC SELECT";
            int hw = GetDrawStringWidthToHandle(hdr, (int)wcslen(hdr), fontHeading);
            DrawStringToHandle(1920/2 - hw/2, 200, hdr, titleColor, fontHeading);
        }
        break;
    case GameState::PAUSED:
        {
            const wchar_t* hdr = L"PAUSED";
            int hw = GetDrawStringWidthToHandle(hdr, (int)wcslen(hdr), fontHeading);
            DrawStringToHandle(1920/2 - hw/2, 250, hdr, titleColor, fontHeading);
        }
        break;
    case GameState::LEVEL_RESULT:
        {
            // Лямбда для рисования 5-конечной звезды через треугольники
            auto drawStar = [](int cx, int cy, int outerR, int innerR, unsigned int color) {
                const float PI        = 3.14159265f;
                const float step      = 2.0f * PI / 5.0f;
                const float halfStep  = step / 2.0f;
                const float startAngle = -PI / 2.0f;
                for (int i = 0; i < 5; i++) {
                    float a1 = startAngle + i * step;
                    float a2 = a1 + halfStep;
                    float a3 = a1 + step;
                    int ox1 = cx + (int)(outerR * cosf(a1));
                    int oy1 = cy + (int)(outerR * sinf(a1));
                    int ix  = cx + (int)(innerR * cosf(a2));
                    int iy  = cy + (int)(innerR * sinf(a2));
                    int ox2 = cx + (int)(outerR * cosf(a3));
                    int oy2 = cy + (int)(outerR * sinf(a3));
                    DrawTriangle(cx, cy, ox1, oy1, ix, iy, color, TRUE);
                    DrawTriangle(cx, cy, ix, iy, ox2, oy2, color, TRUE);
                }
            };

            // Заголовок
            const wchar_t* headline = resultCompleted ? L"LEVEL COMPLETE!" : L"YOU DIED";
            unsigned int headlineColor = resultCompleted ? GetColor(100, 255, 150) : GetColor(255, 80, 80);
            int headlineW = GetDrawStringWidthToHandle(headline, (int)wcslen(headline), fontHeading);
            DrawStringToHandle(1920 / 2 - headlineW / 2, 230, headline, headlineColor, fontHeading);

            // Время
            int minutes = (int)(resultTime / 60.0f);
            float secs  = resultTime - minutes * 60.0f;
            wchar_t timeBuf[64];
            swprintf_s(timeBuf, L"Time: %d:%05.2f", minutes, secs);
            int timeW = GetDrawStringWidthToHandle(timeBuf, (int)wcslen(timeBuf), fontNormal);
            DrawStringToHandle(1920 / 2 - timeW / 2, 320, timeBuf, GetColor(220, 220, 220), fontNormal);

            // Звёзды — 3 геометрические пятиконечные звезды
            {
                unsigned int goldColor = GetColor(255, 210, 0);
                unsigned int goldBorder = GetColor(200, 160, 0);
                unsigned int grayColor = GetColor(55, 55, 55);
                unsigned int grayBorder = GetColor(80, 80, 80);
                const int outerR  = 48;
                const int innerR  = 20;
                const int spacing = 130;
                int starY = 460;
                int starStartX = 1920 / 2 - spacing;
                for (int i = 0; i < 3; i++) {
                    int cx = starStartX + i * spacing;
                    bool earned = (i < resultStars);
                    // Обводка рисуется первой (больше), заливка поверх
                    drawStar(cx, starY, outerR + 4, innerR + 4, earned ? goldBorder : grayBorder);
                    drawStar(cx, starY, outerR, innerR, earned ? goldColor : grayColor);
                }
            }

            // Подсказки клавиш
            {
                const wchar_t* hint = (resultCompleted && !resultIsLastLevel)
                    ? L"[R] Restart   [Space] Next Level   [ESC] Main Menu"
                    : L"[R] Restart   [ESC] Main Menu";
                int hw = GetDrawStringWidthToHandle(hint, (int)wcslen(hint), fontSmall);
                DrawStringToHandle(1920/2 - hw/2, 860, hint, GetColor(120, 120, 120), fontSmall);
            }
        }
        break;
    }

    // Отрисовка кнопок
    for (size_t i = 0; i < buttons.size(); i++) {
        // Подсветка выбранной кнопки (клавиатурная навигация)
        if (i == selectedButtonIndex && !buttons[i].isHovered) {
            // Рамка вокруг выбранной кнопки
            DrawBox(buttons[i].x - 5, buttons[i].y - 5,
                buttons[i].x + buttons[i].width + 5,
                buttons[i].y + buttons[i].height + 5,
                GetColor(255, 255, 100), FALSE);
        }
        buttons[i].Draw();
    }

    // Подсказки управления
    if (currentState != GameState::PLAYING) {
        const wchar_t* hint = L"Mouse: Click | Arrow Keys + Enter | ESC: Back";
        DrawStringToHandle(10, 1040, hint, GetColor(150, 150, 150), fontSmall);
    }
}

void Menu::DrawSlider(int x, int y, int width, int value, const wchar_t* label) const {
    unsigned int labelColor   = GetColor(220, 220, 220);
    unsigned int barBgColor   = GetColor(60, 60, 80);
    unsigned int barFillColor = GetColor(100, 150, 255);
    unsigned int borderColor  = GetColor(120, 120, 150);

    // Label
    DrawStringToHandle(x, y - 28, label, labelColor, fontNormal);

    // Background bar
    DrawBox(x, y, x + width, y + 30, barBgColor, TRUE);
    DrawBox(x, y, x + width, y + 30, borderColor, FALSE);

    // Fill bar (0-100%)
    int fillWidth = (width * value) / 100;
    DrawBox(x, y, x + fillWidth, y + 30, barFillColor, TRUE);

    // Value text
    wchar_t valueText[16];
    swprintf_s(valueText, L"%d%%", value);
    DrawStringToHandle(x + width + 20, y + 5, valueText, labelColor, fontSmall);
}

int Menu::HandleSliderClick(int x, int y, int width, int mouseX, int mouseY) {
    if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + 30) {
        int newValue = ((mouseX - x) * 100) / width;
        if (newValue < 0) newValue = 0;
        if (newValue > 100) newValue = 100;
        return newValue;
    }
    return -1;
}

ButtonAction Menu::HandleInput(int& blockToLoad, int& levelToLoad) {
    blockToLoad = -1;
    levelToLoad = -1;

    if (buttons.empty()) return ButtonAction::NONE;

    // Обновляем наведение мыши
    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);
    UpdateHover(mouseX, mouseY);

    // Обновление слайдеров (с защитой от сквозных кликов)
    if (currentState == GameState::SETTINGS) {
        bool mouseDown = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
        if (mouseDown) {
            bool shouldUpdate = false;
            if (!mousePressed) {
                // Свежий клик — захватываем слайдер только если попали в него
                sliderGrabbed = false;
                int v;
                v = HandleSliderClick(1920/2 - 200, 350, 400, mouseX, mouseY);
                if (v >= 0) { settings.musicVolume = v;      sliderGrabbed = true; }
                v = HandleSliderClick(1920/2 - 200, 470, 400, mouseX, mouseY);
                if (v >= 0) { settings.soundVolume = v;      sliderGrabbed = true; }
                v = HandleSliderClick(1920/2 - 200, 590, 400, mouseX, mouseY);
                if (v >= 0) { settings.mouseSensitivity = v; sliderGrabbed = true; }
                shouldUpdate = sliderGrabbed;
            } else if (sliderGrabbed) {
                // Продолжаем drag уже захваченного слайдера
                int v;
                v = HandleSliderClick(1920/2 - 200, 350, 400, mouseX, mouseY);
                if (v >= 0) settings.musicVolume = v;
                v = HandleSliderClick(1920/2 - 200, 470, 400, mouseX, mouseY);
                if (v >= 0) settings.soundVolume = v;
                v = HandleSliderClick(1920/2 - 200, 590, 400, mouseX, mouseY);
                if (v >= 0) settings.mouseSensitivity = v;
                shouldUpdate = true;
            }
            if (shouldUpdate) {
                Profile* p = ProfileManager::GetInstance().GetCurrentProfile();
                if (p) {
                    p->musicVolume      = settings.musicVolume;
                    p->sfxVolume        = settings.soundVolume;
                    p->mouseSensitivity = settings.mouseSensitivity;
                }
            }
        } else {
            sliderGrabbed = false;
        }
    }

    // Ввод никнейма обрабатывается автоматически через ProcessMessage()

    // Клик мыши
    if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
        if (!mousePressed) {
            for (const auto& button : buttons) {
                if (button.isHovered) {
                    mousePressed = true;
                    return ActivateSelectedButton(blockToLoad, levelToLoad);
                }
            }
        }
        mousePressed = true;
    }
    else {
        mousePressed = false;
    }

    // Клавиатурная навигация
    if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W)) {
        if (!upPressed) {
            SelectPrevButton();
        }
        upPressed = true;
    }
    else {
        upPressed = false;
    }

    if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S)) {
        if (!downPressed) {
            SelectNextButton();
        }
        downPressed = true;
    }
    else {
        downPressed = false;
    }

    if (CheckHitKey(KEY_INPUT_RETURN)) {
        if (!enterPressed) {
            enterPressed = true;
            return ActivateSelectedButton(blockToLoad, levelToLoad);
        }
    }
    else {
        enterPressed = false;
    }

    // ESC - универсальный возврат назад
    if (CheckHitKey(KEY_INPUT_ESCAPE)) {
        if (!escPressed) {
            escPressed = true;

            // Специальный случай: пауза возвращает в игру
            if (currentState == GameState::PAUSED) {
                return ButtonAction::CONTINUE;
            }

            // Экран результата: ESC → главное меню
            if (currentState == GameState::LEVEL_RESULT) {
                return ButtonAction::BACK_TO_MENU;
            }

            // Profile create: ESC → обратно на выбор
            if (currentState == GameState::PROFILE_CREATE) {
                if (keyInputHandle != -1) { DeleteKeyInput(keyInputHandle); keyInputHandle = -1; }
                SetState(GameState::PROFILE_SELECT);
                return ButtonAction::NONE;
            }

            // Profile select: ESC только если можно вернуться
            if (currentState == GameState::PROFILE_SELECT) {
                if (pendingDeleteSlot >= 0) {
                    pendingDeleteSlot = -1;
                    SetState(GameState::PROFILE_SELECT);
                } else if (profileSelectCanGoBack) {
                    SetState(GameState::MAIN_MENU);
                }
                return ButtonAction::NONE;
            }

            // Все остальные состояния: возврат по стеку
            PopState();
        }
    }
    else {
        escPressed = false;
    }

    return ButtonAction::NONE;
}

void Menu::SelectNextButton() {
    if (buttons.empty()) return;
    selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
}

void Menu::SelectPrevButton() {
    if (buttons.empty()) return;
    selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
}

ButtonAction Menu::ActivateSelectedButton(int& blockToLoad, int& levelToLoad) {
    if (selectedButtonIndex < 0 || selectedButtonIndex >= (int)buttons.size()) {
        return ButtonAction::NONE;
    }

    if (buttons[selectedButtonIndex].disabled) {
        return ButtonAction::NONE;
    }

    ButtonAction action = buttons[selectedButtonIndex].action;

    // Block selection - переход к выбору уровней
    if (action >= ButtonAction::SELECT_BLOCK_1 && action <= ButtonAction::SELECT_BLOCK_10) {
        int blockIndex = static_cast<int>(action) - static_cast<int>(ButtonAction::SELECT_BLOCK_1);
        if (blockIndex < unlockedBlocks) {
            selectedBlock = blockIndex + 1;
            blockToLoad = selectedBlock;
            PushState(GameState::LEVEL_SELECT_LEVELS);  // Переход с сохранением в стек
        }
        return ButtonAction::NONE;
    }

    // Level selection - загружаем уровень
    if (action == ButtonAction::LOAD_LEVEL) {
        if (selectedBlock <= unlockedBlocks) {
            int calculatedLevel = (selectedBlock - 1) * 10 + selectedButtonIndex + 1;
            if (calculatedLevel <= totalLevels && calculatedLevel <= unlockedLevels) {
                levelToLoad = calculatedLevel;
            }
        }
        return action;
    }

    // Back button = PopState (как ESC)
    // НО: BACK_TO_MENU из паузы означает "вернуться в главное меню" (не PopState)
    if (action == ButtonAction::BACK_TO_MENU) {
        if (currentState == GameState::PAUSED || currentState == GameState::LEVEL_RESULT) {
            return action;
        }
        if (currentState == GameState::PROFILE_SELECT) {
            if (profileSelectCanGoBack)
                SetState(GameState::MAIN_MENU);
            return ButtonAction::NONE;
        }
        if (currentState == GameState::PROFILE_CREATE) {
            if (keyInputHandle != -1) { DeleteKeyInput(keyInputHandle); keyInputHandle = -1; }
            SetState(GameState::PROFILE_SELECT);
            return ButtonAction::NONE;
        }
        PopState();
        return ButtonAction::NONE;
    }

    if (action == ButtonAction::BACK_TO_SETTINGS) {
        PopState();
        return ButtonAction::NONE;
    }

    // ── Profile actions ───────────────────────────────────────────────────────

    // SELECT_PROFILE_0..4: выбрать или создать
    if (action >= ButtonAction::SELECT_PROFILE_0 && action <= ButtonAction::SELECT_PROFILE_4) {
        int slot = (int)action - (int)ButtonAction::SELECT_PROFILE_0;
        if (ProfileManager::GetInstance().IsSlotEmpty(slot)) {
            OpenProfileCreate(slot);
        } else {
            ProfileManager::GetInstance().LoadProfile(slot);
            SyncSettingsFromProfile();
            if (profileSelectCanGoBack)
                SetState(GameState::MAIN_MENU);
            else
                SetState(GameState::MAIN_MENU);
        }
        return ButtonAction::NONE;
    }

    // DELETE_PROFILE_0..4: запрос подтверждения
    if (action >= ButtonAction::DELETE_PROFILE_0 && action <= ButtonAction::DELETE_PROFILE_4) {
        pendingDeleteSlot = (int)action - (int)ButtonAction::DELETE_PROFILE_0;
        SetState(GameState::PROFILE_SELECT); // перестраивает кнопки (overlay mode)
        return ButtonAction::NONE;
    }

    if (action == ButtonAction::DELETE_PROFILE_CONFIRM) {
        if (pendingDeleteSlot >= 0) {
            ProfileManager::GetInstance().DeleteProfile(pendingDeleteSlot);
            pendingDeleteSlot = -1;
        }
        // Если удалили активный профиль — нужно выбрать новый
        if (!ProfileManager::GetInstance().HasActiveProfile()) {
            profileSelectCanGoBack = false;
            SetState(GameState::PROFILE_SELECT);
        } else {
            SetState(GameState::PROFILE_SELECT);
        }
        return ButtonAction::NONE;
    }

    if (action == ButtonAction::DELETE_PROFILE_CANCEL) {
        pendingDeleteSlot = -1;
        SetState(GameState::PROFILE_SELECT);
        return ButtonAction::NONE;
    }

    if (action == ButtonAction::CREATE_PROFILE_CONFIRM) {
        if (keyInputHandle == -1) {
            // Что-то пошло не так — возвращаемся к выбору профиля
            SetState(GameState::PROFILE_SELECT);
            return ButtonAction::NONE;
        }
        wchar_t buf[17] = {};
        GetKeyInputString(buf, keyInputHandle);
        std::wstring name = buf;
        if (name.empty()) name = L"Player";
        DeleteKeyInput(keyInputHandle);
        keyInputHandle = -1;
        ProfileManager::GetInstance().CreateProfile(pendingCreateSlot, name);
        SyncSettingsFromProfile();
        SetState(GameState::MAIN_MENU);
        return ButtonAction::NONE;
    }

    // CHANGE_PROFILE: открыть выбор из главного меню
    if (action == ButtonAction::CHANGE_PROFILE) {
        OpenProfileSelect(true);
        return ButtonAction::NONE;
    }

    // Эти кнопки просто возвращают action (main.cpp обработает)
    return action;
}