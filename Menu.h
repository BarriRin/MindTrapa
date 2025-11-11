#pragma once
#include "DxLib.h"
#include <vector>
#include <string>

// Состояния игры
enum class GameState {
    MAIN_MENU,
    LEVEL_SELECT_BLOCKS,   // Выбор блока (10 блоков)
    LEVEL_SELECT_LEVELS,   // Выбор уровня внутри блока (10 уровней)
    SETTINGS,              // Настройки (музыка, звук, язык)
    MUSIC_SELECT,          // Выбор музыкального трека
    PLAYING,
    PAUSED
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
    NONE
};

// Структура кнопки
struct Button {
    std::wstring text;
    int x, y;
    int width, height;
    ButtonAction action;
    bool isHovered;

    Button(std::wstring txt, int posX, int posY, int w, int h, ButtonAction act)
        : text(txt), x(posX), y(posY), width(w), height(h), action(act), isHovered(false) {
    }

    // Проверка попадания мыши
    bool Contains(int mouseX, int mouseY) const {
        return mouseX >= x && mouseX <= x + width &&
            mouseY >= y && mouseY <= y + height;
    }

    // Отрисовка кнопки
    void Draw() const {
        unsigned int bgColor = isHovered ? GetColor(100, 150, 255) : GetColor(50, 50, 100);
        unsigned int borderColor = isHovered ? GetColor(150, 200, 255) : GetColor(100, 100, 150);
        unsigned int textColor = GetColor(255, 255, 255);

        // Фон кнопки
        DrawBox(x, y, x + width, y + height, bgColor, TRUE);
        // Рамка
        DrawBox(x, y, x + width, y + height, borderColor, FALSE);

        // Текст по центру
        int textWidth = GetDrawStringWidth(text.c_str(), wcslen(text.c_str()));
        int textX = x + (width - textWidth) / 2;
        int textY = y + (height - 20) / 2;
        DrawString(textX, textY, text.c_str(), textColor);
    }
};

// Настройки игры
struct Settings {
    int musicVolume = 80;      // 0-100
    int soundVolume = 60;      // 0-100
    std::string language = "EN"; // Заглушка пока
    int selectedMusicTrack = 0;  // 0=Default, 1-4=Custom (пока всё локд)
};

// Класс меню
class Menu {
private:
    GameState currentState;
    int selectedButtonIndex;
    std::vector<Button> buttons;

    int totalLevels;
    int selectedBlock;       // Какой блок выбран (1-10)
    int unlockedBlocks;      // Сколько блоков разблокировано (пока только 1)
    Settings settings;       // Настройки игры

    // Стек навигации (для ESC/Back)
    std::vector<GameState> stateHistory;

    // Состояния ввода (member variables, не static)
    bool mousePressed;
    bool upPressed;
    bool downPressed;
    bool enterPressed;
    bool escPressed;

    // Создание кнопок
    void CreateMainMenuButtons();
    void CreateBlockSelectButtons();
    void CreateLevelSelectButtons();
    void CreateSettingsButtons();
    void CreateMusicSelectButtons();
    void CreatePauseMenuButtons();
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
