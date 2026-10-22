#pragma once
#include "DxLib.h"
#include "GameState.h"
#include "Localization.h"
#include "Constants.h"
#include <vector>
#include <string>

// Button actions
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
    LOAD_LEVEL,
    NEXT_LEVEL,
    // Profile actions
    CHANGE_PROFILE,
    SELECT_PROFILE_0, SELECT_PROFILE_1, SELECT_PROFILE_2, SELECT_PROFILE_3, SELECT_PROFILE_4,
    DELETE_PROFILE_0, DELETE_PROFILE_1, DELETE_PROFILE_2, DELETE_PROFILE_3, DELETE_PROFILE_4,
    DELETE_PROFILE_CONFIRM,
    DELETE_PROFILE_CANCEL,
    CREATE_PROFILE_CONFIRM,
    CYCLE_LANGUAGE,
    SELECT_THEME_PACK_0,
    SELECT_THEME_PACK_1,
    SELECT_THEME_PACK_2,
    NONE
};

// Button struct
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

    // Mouse hit test
    bool Contains(int mouseX, int mouseY) const {
        return mouseX >= x && mouseX <= x + width &&
            mouseY >= y && mouseY <= y + height;
    }

    int fontHandle = -1;  // custom font (-1 = system default)

    // Draw the button
    void Draw() const {
        unsigned int bgColor     = disabled ? GetColor(35, 35, 35)   : (isHovered ? GetColor(60, 100, 200) : GetColor(20, 30, 80));
        unsigned int borderColor = disabled ? GetColor(55, 55, 55)   : (isHovered ? GetColor(120, 180, 255) : GetColor(60, 90, 180));
        unsigned int textColor   = disabled ? GetColor(90, 90, 90)   : GetColor(255, 255, 255);

        // Semi-transparent background
        int bgAlpha = disabled ? 130 : (isHovered ? 220 : 170);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, bgAlpha);
        DrawBox(x, y, x + width, y + height, bgColor, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        // Border
        DrawBox(x, y, x + width, y + height, borderColor, FALSE);

        // Centered text
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

// Game settings (cached from the current profile)
struct Settings {
    int musicVolume      = 80;   // 0-100
    int soundVolume      = 60;   // 0-100
    int mouseSensitivity = 50;   // 0-100, 50 = 1.0x
    int selectedThemePack = 0;   // 0 = Default, 1/2 = unlockable packs
};

// Menu class
class Menu {
private:
    GameState currentState;
    int selectedButtonIndex;
    std::vector<Button> buttons;

    int totalLevels;
    int selectedBlock;       // Which block is selected (1-10)
    int unlockedBlocks;      // How many blocks are unlocked
    int unlockedLevels;      // Up to which level (inclusive) is unlocked
    Settings settings;       // Game settings

    // Navigation stack (for ESC/Back)
    std::vector<GameState> stateHistory;

    // Slider geometry for the SETTINGS screen — shared between Draw() and
    // HandleInput() so the hitbox can never drift out of sync with the drawing
    static constexpr int SLIDER_X       = SCREEN_W / 2 - 200;
    static constexpr int SLIDER_WIDTH   = 400;
    static constexpr int SLIDER_Y_MUSIC = 350;
    static constexpr int SLIDER_Y_SOUND = 470;
    static constexpr int SLIDER_Y_MOUSE = 590;

    void ApplyFontToButtons(int handle);

    // Input state (member variables, not static)
    bool mousePressed;
    bool upPressed;
    bool downPressed;
    bool enterPressed;
    bool escPressed;
    bool sliderGrabbed = false;  // slider was grabbed by a fresh click (guards against click-through)

    // Result screen
    bool  resultCompleted   = false;
    bool  resultIsLastLevel = false;
    int   resultStars       = 0;
    float resultTime        = 0.0f;

    // Profile screens
    bool profileSelectCanGoBack = false;  // true if we entered from the main menu
    int  pendingDeleteSlot      = -1;     // awaiting delete confirmation
    int  pendingCreateSlot      = -1;     // slot we're creating a profile for
    int  keyInputHandle         = -1;     // DxLib key input for nickname entry

    // Button creation
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

    // Shared "go back" logic for PROFILE_SELECT/PROFILE_CREATE — used both from
    // ESC (HandleInput) and from clicking the Back button (ActivateSelectedButton),
    // so the behavior (including canceling a pending delete) never diverges between them
    bool HandleBackNavigation(ButtonAction& outAction);

    // Sliders
    void DrawSlider(int x, int y, int width, int value, const wchar_t* label) const;
    int HandleSliderClick(int x, int y, int width, int mouseX, int mouseY);

public:
    Menu(int levels);
    ~Menu();

    // Explicitly releases DxLib resources (fonts, key input) — call before
    // DxLib_End(), otherwise the destructor would run after DxLib is already torn down
    void Shutdown();

    // Core methods
    void SetState(GameState state);          // Transition without saving history
    void PushState(GameState newState);      // Transition and push onto the stack
    void PopState();                         // Go back via the stack
    void ClearHistory();                     // Clear the stack
    void ResetInputFlags();                  // Reset input flags (prevents double activation)
    GameState GetState() const { return currentState; }
    void ShowResultScreen(bool completed, int stars, float time, bool isLastLevel = false);
    void OpenProfileSelect(bool canGoBack);
    void OpenProfileCreate(int slot);
    void SyncSettingsFromProfile();
    void UpdateUnlockState();   // Recompute unlockedBlocks/unlockedLevels from the profile

    void Draw() const;
    ButtonAction HandleInput(int& blockToLoad, int& levelToLoad);

    // Getters
    void SetSelectedBlock(int block) { selectedBlock = block; }
    Settings& GetSettings() { return settings; }

    // Keyboard navigation
    void SelectNextButton();
    void SelectPrevButton();
    ButtonAction ActivateSelectedButton(int& blockToLoad, int& levelToLoad);
};
