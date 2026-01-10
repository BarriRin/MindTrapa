#include "Menu.h"

Menu::Menu(int levels)
    : currentState(GameState::MAIN_MENU), selectedButtonIndex(0), totalLevels(levels),
      selectedBlock(0), unlockedBlocks(3),
      mousePressed(false), upPressed(false), downPressed(false),
      enterPressed(false), escPressed(false) {
    stateHistory.clear(); // Начинаем с пустого стека
    CreateMainMenuButtons();
}

Menu::~Menu() {
    buttons.clear();
}

void Menu::SetState(GameState state) {
    currentState = state;
    selectedButtonIndex = 0;
    buttons.clear();

    switch (state) {
    case GameState::MAIN_MENU:
        CreateMainMenuButtons();
        break;
    case GameState::LEVEL_SELECT_BLOCKS:
        // НЕ обнуляем selectedBlock - он уже установлен если пришли из выбора блока
        CreateBlockSelectButtons();
        break;
    case GameState::LEVEL_SELECT_LEVELS:
        CreateLevelSelectButtons();
        break;
    case GameState::SETTINGS:
        CreateSettingsButtons();
        break;
    case GameState::MUSIC_SELECT:
        CreateMusicSelectButtons();
        break;
    case GameState::PAUSED:
        CreatePauseMenuButtons();
        break;
    case GameState::PLAYING:
        buttons.clear();
        break;
    }
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

    buttons.push_back(Button(L"Start Game", centerX - buttonWidth / 2, startY, buttonWidth, buttonHeight, ButtonAction::START_GAME));
    buttons.push_back(Button(L"Level Select", centerX - buttonWidth / 2, startY + spacing, buttonWidth, buttonHeight, ButtonAction::OPEN_LEVEL_SELECT));
    buttons.push_back(Button(L"Settings", centerX - buttonWidth / 2, startY + spacing * 2, buttonWidth, buttonHeight, ButtonAction::OPEN_SETTINGS));
    buttons.push_back(Button(L"Exit", centerX - buttonWidth / 2, startY + spacing * 3, buttonWidth, buttonHeight, ButtonAction::EXIT_GAME));
}

void Menu::CreateBlockSelectButtons() {
    int startX = 500;
    int startY = 250;
    int buttonWidth = 220;
    int buttonHeight = 120;
    int spacingX = 260;
    int spacingY = 150;
    int buttonsPerRow = 3;

    // Создаём 10 кнопок блоков (планеты)
    for (int i = 0; i < 10; i++) {
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
        if (selectedBlock > unlockedBlocks || calculatedLevel > totalLevels) {
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

    // Sliders will be drawn in Draw(), buttons just for navigation
    buttons.push_back(Button(L"Music Select", centerX - buttonWidth / 2, startY + spacing * 2, buttonWidth, buttonHeight, ButtonAction::OPEN_MUSIC_SELECT));
    buttons.push_back(Button(L"< Back", 100, 950, 200, 60, ButtonAction::BACK_TO_MENU));
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
    if (currentState == GameState::PAUSED) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
        DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    // Заголовок
    unsigned int titleColor = GetColor(255, 255, 255);
    int titleSize = 48;

    switch (currentState) {
    case GameState::MAIN_MENU:
        DrawFormatString(1920 / 2 - 150, 200, titleColor, L"MINDTRAPA");
        DrawFormatString(1920 / 2 - 120, 260, GetColor(200, 200, 200), L"3D Trolling Platformer");
        break;
    case GameState::LEVEL_SELECT_BLOCKS:
        DrawFormatString(1920 / 2 - 120, 150, titleColor, L"SELECT BLOCK");
        break;
    case GameState::LEVEL_SELECT_LEVELS:
        {
            wchar_t blockHeader[64];
            swprintf_s(blockHeader, L"BLOCK %d: LEVELS 1-10", selectedBlock);
            DrawFormatString(1920 / 2 - 150, 150, titleColor, blockHeader);
        }
        break;
    case GameState::SETTINGS:
        DrawFormatString(1920 / 2 - 100, 200, titleColor, L"SETTINGS");
        // Draw sliders
        DrawSlider(1920 / 2 - 200, 350, 400, settings.musicVolume, L"Music Volume");
        DrawSlider(1920 / 2 - 200, 470, 400, settings.soundVolume, L"Sound Volume");
        break;
    case GameState::MUSIC_SELECT:
        DrawFormatString(1920 / 2 - 120, 200, titleColor, L"MUSIC SELECT");
        break;
    case GameState::PAUSED:
        DrawFormatString(1920 / 2 - 80, 250, titleColor, L"PAUSED");
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
        DrawFormatString(10, 1040, GetColor(150, 150, 150), L"Mouse: Click | Keyboard: Arrow Keys + Enter | ESC: Back");
    }
}

void Menu::DrawSlider(int x, int y, int width, int value, const wchar_t* label) const {
    unsigned int labelColor = GetColor(220, 220, 220);
    unsigned int barBgColor = GetColor(60, 60, 80);
    unsigned int barFillColor = GetColor(100, 150, 255);
    unsigned int borderColor = GetColor(120, 120, 150);

    // Label
    DrawFormatString(x, y - 30, labelColor, label);

    // Background bar
    DrawBox(x, y, x + width, y + 30, barBgColor, TRUE);
    DrawBox(x, y, x + width, y + 30, borderColor, FALSE);

    // Fill bar (0-100%)
    int fillWidth = (width * value) / 100;
    DrawBox(x, y, x + fillWidth, y + 30, barFillColor, TRUE);

    // Value text
    wchar_t valueText[16];
    swprintf_s(valueText, L"%d%%", value);
    DrawFormatString(x + width + 20, y + 5, labelColor, valueText);
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

    // Обновление слайдеров
    if (currentState == GameState::SETTINGS) {
        static bool mouseHeld = false;
        if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
            int newMusicVol = HandleSliderClick(1920 / 2 - 200, 350, 400, mouseX, mouseY);
            if (newMusicVol >= 0) {
                settings.musicVolume = newMusicVol;
            }
            int newSoundVol = HandleSliderClick(1920 / 2 - 200, 470, 400, mouseX, mouseY);
            if (newSoundVol >= 0) {
                settings.soundVolume = newSoundVol;
            }
            mouseHeld = true;
        }
        else {
            mouseHeld = false;
        }
    }

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
    if (selectedButtonIndex < 0 || selectedButtonIndex >= buttons.size()) {
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
        // Проверяем что блок разблокирован
        if (selectedBlock <= unlockedBlocks) {
            // Вычисляем номер уровня: Block 1 = уровни 1-10, Block 2 = 11-20, и т.д.
            int calculatedLevel = (selectedBlock - 1) * 10 + selectedButtonIndex + 1;
            if (calculatedLevel <= totalLevels) {
                levelToLoad = calculatedLevel;
            }
        }
        return action;
    }

    // Back button = PopState (как ESC)
    // НО: BACK_TO_MENU из паузы означает "вернуться в главное меню" (не PopState)
    if (action == ButtonAction::BACK_TO_MENU) {
        if (currentState == GameState::PAUSED) {
            // Из паузы "Main Menu" - возвращаем action для обработки в main.cpp
            return action;
        }
        else {
            // Из других меню - обычная навигация назад
            PopState();
            return ButtonAction::NONE;
        }
    }

    if (action == ButtonAction::BACK_TO_SETTINGS) {
        PopState();
        return ButtonAction::NONE;
    }

    // Эти кнопки просто возвращают action (main.cpp обработает)
    return action;
}