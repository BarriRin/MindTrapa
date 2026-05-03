#pragma once
#include "DxLib.h"
#include <vector>
#include <string>

// Состояния игры
enum class GameState {
    PROFILE_SELECT,        // Выбор/создание профиля (стартовый экран или смена)
    PROFILE_CREATE,        // Ввод никнейма при создании профиля
    MAIN_MENU,
    LEVEL_SELECT_BLOCKS,   // Выбор блока (10 блоков)
    LEVEL_SELECT_LEVELS,   // Выбор уровня внутри блока (10 уровней)
    SETTINGS,              // Настройки (мышь, музыка, звук)
    MUSIC_SELECT,          // Выбор музыкального трека
    PLAYING,
    PAUSED,
    DYING,
    LEVEL_CELEBRATING,
    LEVEL_RESULT
};

// Действия кнопок
enum class ButtonAction {
    START_GAME,
    OPEN_SETTINGS,
    OPEN_MUSIC_SELECT,
    OPEN_LEVEL_SELECT,
    EXIT_GAME,
    CONTINUE,
    RESTART,
    LEVEL_SELECT,
    BACK_TO_MENU,
    BACK_TO_SETTINGS,
    SELECT_BLOCK_1,
    SELECT_BLOCK_2,
    SELECT_BLOCK_3,
    SELECT_BLOCK_4,
    SELECT_BLOCK_5,
    SELECT_BLOCK_6,
    SELECT_BLOCK_7,
    SELECT_BLOCK_8,
    SELECT_BLOCK_9,
    SELECT_BLOCK_10,
    LOAD_LEVEL,
    NEXT_LEVEL,
    // Profile actions
    CHANGE_PROFILE,
    SELECT_PROFILE_0, SELECT_PROFILE_1, SELECT_PROFILE_2, SELECT_PROFILE_3, SELECT_PROFILE_4,
    DELETE_PROFILE_0, DELETE_PROFILE_1, DELETE_PROFILE_2, DELETE_PROFILE_3, DELETE_PROFILE_4,
    DELETE_PROFILE_CONFIRM,
    DELETE_PROFILE_CANCEL,
    CREATE_PROFILE_CONFIRM,
    NONE
};

// Структура кнопки
struct Button {
    std::wstring text;
    int x, y;
    int width, height;
    ButtonAction action;
    bool isHovered;
    bool disabled;

    Button(std::wstring txt, int posX, int posY, int w, int h, ButtonAction act, bool dis = false)
        : text(txt), x(posX), y(posY), width(w), height(h), action(act), isHovered(false), disabled(dis) {
    }

    // Проверка попадания мыши
    bool Contains(int mouseX, int mouseY) const {
        return mouseX >= x && mouseX <= x + width &&
            mouseY >= y && mouseY <= y + height;
    }

    int fontHandle = -1;  // кастомный шрифт (-1 = системный)

    // Отрисовка кнопки
    void Draw() const {
        unsigned int bgColor     = disabled ? GetColor(35, 35, 35)   : (isHovered ? GetColor(60, 100, 200) : GetColor(20, 30, 80));
        unsigned int borderColor = disabled ? GetColor(55, 55, 55)   : (isHovered ? GetColor(120, 180, 255) : GetColor(60, 90, 180));
        unsigned int textColor   = disabled ? GetColor(90, 90, 90)   : GetColor(255, 255, 255);

        // Полупрозрачный фон
        int bgAlpha = disabled ? 130 : (isHovered ? 220 : 170);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, bgAlpha);
        DrawBox(x, y, x + width, y + height, bgColor, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        // Рамка
        DrawBox(x, y, x + width, y + height, borderColor, FALSE);

        // Текст по центру
        int textWidth, textY;
        if (fontHandle != -1) {
            textWidth = GetDrawStringWidthToHandle(text.c_str(), (int)text.size(), fontHandle);
            textY = y + (height - 22) / 2;
            DrawStringToHandle(x + (width - textWidth) / 2, textY, text.c_str(), textColor, fontHandle);
        } else {
            textWidth = GetDrawStringWidth(text.c_str(), (int)wcslen(text.c_str()));
            textY = y + (height - 20) / 2;
            DrawString(x + (width - textWidth) / 2, textY, text.c_str(), textColor);
        }
    }
};

// Настройки игры (кэш из текущего профиля)
struct Settings {
    int musicVolume      = 80;   // 0-100
    int soundVolume      = 60;   // 0-100
    int mouseSensitivity = 50;   // 0-100, 50 = 1.0x
    std::string language = "EN";
    int selectedMusicTrack = 0;
};

// Класс меню
class Menu {
private:
    GameState currentState;
    int selectedButtonIndex;
    std::vector<Button> buttons;

    int totalLevels;
    int selectedBlock;       // Какой блок выбран (1-10)
    int unlockedBlocks;      // Сколько блоков разблокировано
    int unlockedLevels;      // До какого уровня включительно открыто
    Settings settings;       // Настройки игры

    // Стек навигации (для ESC/Back)
    std::vector<GameState> stateHistory;

    // Шрифты
    int fontTitle   = -1;   // Orbitron Bold 60px, edge
    int fontHeading = -1;   // Orbitron Bold 32px, edge
    int fontButton  = -1;   // Orbitron Bold 22px
    int fontNormal  = -1;   // Orbitron Regular 18px
    int fontSmall   = -1;   // Orbitron Regular 13px

    void LoadFonts();
    void ApplyFontToButtons(int handle);

    // Состояния ввода (member variables, не static)
    bool mousePressed;
    bool upPressed;
    bool downPressed;
    bool enterPressed;
    bool escPressed;
    bool sliderGrabbed = false;  // слайдер захвачен свежим кликом (защита от сквозных кликов)

    // Экран результата
    bool  resultCompleted   = false;
    bool  resultIsLastLevel = false;
    int   resultStars       = 0;
    float resultTime        = 0.0f;

    // Profile screens
    bool profileSelectCanGoBack = false;  // true если вошли из главного меню
    int  pendingDeleteSlot      = -1;     // ожидает подтверждения удаления
    int  pendingCreateSlot      = -1;     // слот для которого создаём профиль
    int  keyInputHandle         = -1;     // DxLib key input для ввода ника

    // Создание кнопок
    void CreateMainMenuButtons();
    void CreateBlockSelectButtons();
    void CreateLevelSelectButtons();
    void CreateSettingsButtons();
    void CreateMusicSelectButtons();
    void CreatePauseMenuButtons();
    void CreateResultScreenButtons();
    void CreateProfileSelectButtons();
    void CreateProfileCreateButtons();
    void UpdateHover(int mouseX, int mouseY);

    // Ползунки
    void DrawSlider(int x, int y, int width, int value, const wchar_t* label) const;
    int HandleSliderClick(int x, int y, int width, int mouseX, int mouseY);

public:
    Menu(int levels);
    ~Menu();

    // Основные методы
    void SetState(GameState state);          // Переход без сохранения истории
    void PushState(GameState newState);      // Переход с сохранением в стек
    void PopState();                         // Возврат назад по стеку
    void ClearHistory();                     // Очистить стек
    void ResetInputFlags();                  // Сбросить флаги ввода (предотвратить двойное нажатие)
    GameState GetState() const { return currentState; }
    void ShowResultScreen(bool completed, int stars, float time, bool isLastLevel = false);
    void OpenProfileSelect(bool canGoBack);
    void OpenProfileCreate(int slot);
    void SyncSettingsFromProfile();
    void UpdateUnlockState();   // Пересчитать unlockedBlocks/unlockedLevels из профиля

    void Draw() const;
    ButtonAction HandleInput(int& blockToLoad, int& levelToLoad);

    // Геттеры
    int GetSelectedBlock() const { return selectedBlock; }
    void SetSelectedBlock(int block) { selectedBlock = block; }
    Settings& GetSettings() { return settings; }

    // Навигация клавиатурой
    void SelectNextButton();
    void SelectPrevButton();
    ButtonAction ActivateSelectedButton(int& blockToLoad, int& levelToLoad);
};
