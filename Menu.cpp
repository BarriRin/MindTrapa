#include "Menu.h"

Menu::Menu(int levels)
    : currentState(GameState::MAIN_MENU), selectedButtonIndex(0), totalLevels(levels) {
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
    case GameState::LEVEL_SELECT:
        CreateLevelSelectButtons();
        break;
    case GameState::PAUSED:
        CreatePauseMenuButtons();
        break;
    case GameState::PLAYING:
        buttons.clear(); // ÑB ÑyÑsÑÇÑu Ñ{Ñ~ÑÄÑÅÑÄÑ{ Ñ~ÑuÑÑ
        break;
    }
}

void Menu::CreateMainMenuButtons() {
    int centerX = 1920 / 2;
    int startY = 400;
    int buttonWidth = 300;
    int buttonHeight = 60;
    int spacing = 80;

    buttons.push_back(Button(L"Start Game", centerX - buttonWidth / 2, startY, buttonWidth, buttonHeight, ButtonAction::START_GAME));
    buttons.push_back(Button(L"Level Select", centerX - buttonWidth / 2, startY + spacing, buttonWidth, buttonHeight, ButtonAction::LEVEL_SELECT));
    buttons.push_back(Button(L"Exit", centerX - buttonWidth / 2, startY + spacing * 2, buttonWidth, buttonHeight, ButtonAction::EXIT_GAME));
}

void Menu::CreateLevelSelectButtons() {
    int startX = 400;
    int startY = 300;
    int buttonWidth = 120;
    int buttonHeight = 80;
    int spacingX = 150;
    int spacingY = 100;
    int buttonsPerRow = 5;

    // ÑOÑSÑLÑ@ÑDÑKÑ@: ÑrÑçÑrÑÄÑtÑyÑ} Ñ{ÑÄÑ|ÑyÑâÑuÑÉÑÑÑrÑÄ ÑÖÑÇÑÄÑrÑ~ÑuÑz
    char debug[100];
    sprintf_s(debug, "Creating buttons for %d levels", totalLevels);
    OutputDebugStringA(debug);

    // ÑKÑ~ÑÄÑÅÑ{Ñy ÑÖÑÇÑÄÑrÑ~ÑuÑz - ÑëÑrÑ~ÑÄ ÑÉÑÄÑxÑtÑpÑvÑ} 10 Ñ{Ñ~ÑÄÑÅÑÄÑ{
    int maxButtons = (totalLevels < 10) ? totalLevels : 10;

    for (int i = 0; i < maxButtons; i++) {
        int row = i / buttonsPerRow;
        int col = i % buttonsPerRow;
        int x = startX + col * spacingX;
        int y = startY + row * spacingY;

        std::wstring levelText = L"Level " + std::to_wstring(i + 1);

        ButtonAction action = ButtonAction::NONE;
        if (i == 0) action = ButtonAction::LOAD_LEVEL_1;
        else if (i == 1) action = ButtonAction::LOAD_LEVEL_2;
        else if (i == 2) action = ButtonAction::LOAD_LEVEL_3;
        else if (i == 3) action = ButtonAction::LOAD_LEVEL_4;
        else if (i == 4) action = ButtonAction::LOAD_LEVEL_5;
        else if (i == 5) action = ButtonAction::LOAD_LEVEL_6;
        else if (i == 6) action = ButtonAction::LOAD_LEVEL_7;
        else if (i == 7) action = ButtonAction::LOAD_LEVEL_8;
        else if (i == 8) action = ButtonAction::LOAD_LEVEL_9;
        else if (i == 9) action = ButtonAction::LOAD_LEVEL_10;

        buttons.push_back(Button(levelText, x, y, buttonWidth, buttonHeight, action));

        sprintf_s(debug, "Created button %d: %ls", i + 1, levelText.c_str());
        OutputDebugStringA(debug);
    }

    // ÑKÑ~ÑÄÑÅÑ{Ñp Back
    buttons.push_back(Button(L"Back", 100, 900, 200, 60, ButtonAction::BACK_TO_MENU));
}

void Menu::CreatePauseMenuButtons() {
    int centerX = 1920 / 2;
    int startY = 400;
    int buttonWidth = 300;
    int buttonHeight = 60;
    int spacing = 80;

    buttons.push_back(Button(L"Continue", centerX - buttonWidth / 2, startY, buttonWidth, buttonHeight, ButtonAction::CONTINUE));
    buttons.push_back(Button(L"Restart Level", centerX - buttonWidth / 2, startY + spacing, buttonWidth, buttonHeight, ButtonAction::RESTART));
    buttons.push_back(Button(L"Level Select", centerX - buttonWidth / 2, startY + spacing * 2, buttonWidth, buttonHeight, ButtonAction::LEVEL_SELECT));
    buttons.push_back(Button(L"Main Menu", centerX - buttonWidth / 2, startY + spacing * 3, buttonWidth, buttonHeight, ButtonAction::BACK_TO_MENU));
}

void Menu::UpdateHover(int mouseX, int mouseY) {
    for (size_t i = 0; i < buttons.size(); i++) {
        buttons[i].isHovered = buttons[i].Contains(mouseX, mouseY);
        if (buttons[i].isHovered) {
            selectedButtonIndex = i; // ÑRÑyÑ~ÑáÑÇÑÄÑ~ÑyÑxÑpÑàÑyÑë ÑÉ Ñ{Ñ|ÑpÑrÑyÑpÑÑÑÖÑÇÑ~ÑÄÑz Ñ~ÑpÑrÑyÑsÑpÑàÑyÑuÑz
        }
    }
}

void Menu::Draw() const {
    // ÑHÑpÑÑÑuÑ}Ñ~ÑvÑ~Ñ~ÑçÑz ÑÜÑÄÑ~
    if (currentState == GameState::PAUSED) {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
        DrawBox(0, 0, 1920, 1080, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    // ÑHÑpÑsÑÄÑ|ÑÄÑrÑÄÑ{
    unsigned int titleColor = GetColor(255, 255, 255);
    int titleSize = 48;

    switch (currentState) {
    case GameState::MAIN_MENU:
        DrawFormatString(1920 / 2 - 150, 200, titleColor, L"MINDTRAPA");
        DrawFormatString(1920 / 2 - 120, 260, GetColor(200, 200, 200), L"3D Trolling Platformer");
        break;
    case GameState::LEVEL_SELECT:
        DrawFormatString(1920 / 2 - 100, 150, titleColor, L"SELECT LEVEL");
        break;
    case GameState::PAUSED:
        DrawFormatString(1920 / 2 - 80, 250, titleColor, L"PAUSED");
        break;
    }

    // ÑOÑÑÑÇÑyÑÉÑÄÑrÑ{Ñp Ñ{Ñ~ÑÄÑÅÑÄÑ{
    for (size_t i = 0; i < buttons.size(); i++) {
        // ÑPÑÄÑtÑÉÑrÑuÑÑÑ{Ñp ÑrÑçÑqÑÇÑpÑ~Ñ~ÑÄÑz Ñ{Ñ~ÑÄÑÅÑ{Ñy (Ñ{Ñ|ÑpÑrÑyÑpÑÑÑÖÑÇÑ~ÑpÑë Ñ~ÑpÑrÑyÑsÑpÑàÑyÑë)
        if (i == selectedButtonIndex && !buttons[i].isHovered) {
            // ÑQÑpÑ}Ñ{Ñp ÑrÑÄÑ{ÑÇÑÖÑs ÑrÑçÑqÑÇÑpÑ~Ñ~ÑÄÑz Ñ{Ñ~ÑÄÑÅÑ{Ñy
            DrawBox(buttons[i].x - 5, buttons[i].y - 5,
                buttons[i].x + buttons[i].width + 5,
                buttons[i].y + buttons[i].height + 5,
                GetColor(255, 255, 100), FALSE);
        }
        buttons[i].Draw();
    }

    // ÑPÑÄÑtÑÉÑ{ÑpÑxÑ{Ñy ÑÖÑÅÑÇÑpÑrÑ|ÑuÑ~ÑyÑë
    if (currentState != GameState::PLAYING) {
        DrawFormatString(10, 1040, GetColor(150, 150, 150), L"Mouse: Click | Keyboard: Arrow Keys + Enter | ESC: Back");
    }
}

ButtonAction Menu::HandleInput(int& levelToLoad) {
    levelToLoad = -1; // ÑPÑÄ ÑÖÑ}ÑÄÑ|ÑâÑpÑ~ÑyÑê Ñ~Ñu ÑxÑpÑsÑÇÑÖÑwÑpÑuÑ} ÑÖÑÇÑÄÑrÑuÑ~Ñé

    if (buttons.empty()) return ButtonAction::NONE;

    // ÑOÑqÑ~ÑÄÑrÑ|ÑëÑuÑ} Ñ~ÑpÑrÑuÑtÑuÑ~ÑyÑu Ñ}ÑçÑäÑy
    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);
    UpdateHover(mouseX, mouseY);

    // ÑKÑ|ÑyÑ{ Ñ}ÑçÑäÑy
    static bool mousePressed = false;
    if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
        if (!mousePressed) {
            for (const auto& button : buttons) {
                if (button.isHovered) {
                    return ActivateSelectedButton(levelToLoad);
                }
            }
        }
        mousePressed = true;
    }
    else {
        mousePressed = false;
    }

    // ÑKÑ|ÑpÑrÑyÑpÑÑÑÖÑÇÑ~ÑpÑë Ñ~ÑpÑrÑyÑsÑpÑàÑyÑë
    static bool upPressed = false;
    static bool downPressed = false;
    static bool enterPressed = false;

    if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_W)) {
        if (!upPressed) SelectPrevButton();
        upPressed = true;
    }
    else {
        upPressed = false;
    }

    if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_S)) {
        if (!downPressed) SelectNextButton();
        downPressed = true;
    }
    else {
        downPressed = false;
    }

    if (CheckHitKey(KEY_INPUT_RETURN)) {
        if (!enterPressed) {
            return ActivateSelectedButton(levelToLoad);
        }
        enterPressed = true;
    }
    else {
        enterPressed = false;
    }

    // ESC ÑtÑ|Ñë ÑrÑÄÑxÑrÑÇÑpÑÑÑp Ñ~ÑpÑxÑpÑt (ÑÑÑÄÑ|ÑéÑ{ÑÄ ÑtÑ|Ñë Level Select, Ñ~Ñu ÑtÑ|Ñë ÑÅÑpÑÖÑxÑç!)
    static bool escPressed = false;
    if (CheckHitKey(KEY_INPUT_ESCAPE)) {
        if (!escPressed) {
            if (currentState == GameState::LEVEL_SELECT) {
                SetState(GameState::MAIN_MENU);
            }
            // ÑDÑ|Ñë ÑÅÑpÑÖÑxÑç ESC ÑÄÑqÑÇÑpÑqÑpÑÑÑçÑrÑpÑuÑÑÑÉÑë Ñr main.cpp
        }
        escPressed = true;
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

ButtonAction Menu::ActivateSelectedButton(int& levelToLoad) {
    if (selectedButtonIndex < 0 || selectedButtonIndex >= buttons.size()) {
        return ButtonAction::NONE;
    }

    ButtonAction action = buttons[selectedButtonIndex].action;

    // ÑEÑÉÑ|Ñy ÑèÑÑÑÄ ÑxÑpÑsÑÇÑÖÑxÑ{Ñp ÑÖÑÇÑÄÑrÑ~Ñë - ÑrÑçÑâÑyÑÉÑ|ÑëÑuÑ} Ñ~ÑÄÑ}ÑuÑÇ
    if (action >= ButtonAction::LOAD_LEVEL_1 && action <= ButtonAction::LOAD_LEVEL_10) {
        levelToLoad = static_cast<int>(action) - static_cast<int>(ButtonAction::LOAD_LEVEL_1) + 1;
    }

    return action;
}