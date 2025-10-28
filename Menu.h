#pragma once
#include "DxLib.h"
#include <vector>
#include <string>

// „R„€„ƒ„„„€„‘„~„y„‘ „y„s„‚„
enum class GameState {
    MAIN_MENU,
    LEVEL_SELECT,
    PLAYING,
    PAUSED
};

// „D„u„z„ƒ„„„r„y„‘ „{„~„€„„€„{
enum class ButtonAction {
    START_GAME,
    LEVEL_SELECT,
    EXIT_GAME,
    CONTINUE,
    RESTART,
    BACK_TO_MENU,
    LOAD_LEVEL_1,
    LOAD_LEVEL_2,
    LOAD_LEVEL_3,
    LOAD_LEVEL_4,
    LOAD_LEVEL_5,
    LOAD_LEVEL_6,
    LOAD_LEVEL_7,
    LOAD_LEVEL_8,
    LOAD_LEVEL_9,
    LOAD_LEVEL_10,
    NONE
};

// „R„„„‚„…„{„„„…„‚„p „{„~„€„„{„y
struct Button {
    std::wstring text;
    int x, y;
    int width, height;
    ButtonAction action;
    bool isHovered;

    Button(std::wstring txt, int posX, int posY, int w, int h, ButtonAction act)
        : text(txt), x(posX), y(posY), width(w), height(h), action(act), isHovered(false) {
    }

    // „P„‚„€„r„u„‚„{„p „„€„„p„t„p„~„y„‘ „}„„Š„y
    bool Contains(int mouseX, int mouseY) const {
        return mouseX >= x && mouseX <= x + width &&
            mouseY >= y && mouseY <= y + height;
    }

    // „O„„„‚„y„ƒ„€„r„{„p „{„~„€„„{„y
    void Draw() const {
        unsigned int bgColor = isHovered ? GetColor(100, 150, 255) : GetColor(50, 50, 100);
        unsigned int borderColor = isHovered ? GetColor(150, 200, 255) : GetColor(100, 100, 150);
        unsigned int textColor = GetColor(255, 255, 255);

        // „U„€„~ „{„~„€„„{„y
        DrawBox(x, y, x + width, y + height, bgColor, TRUE);
        // „Q„p„}„{„p
        DrawBox(x, y, x + width, y + height, borderColor, FALSE);

        // „S„u„{„ƒ„„ „„€ „ˆ„u„~„„„‚„… („y„ƒ„„€„|„„x„…„u„} „ƒ„„„p„~„t„p„‚„„„~„…„ „†„…„~„{„ˆ„y„)
        int textWidth = GetDrawStringWidth(text.c_str(), wcslen(text.c_str()));
        int textX = x + (width - textWidth) / 2;
        int textY = y + (height - 20) / 2;
        DrawString(textX, textY, text.c_str(), textColor);
    }
};

// „K„|„p„ƒ„ƒ „}„u„~„
class Menu {
private:
    GameState currentState;
    int selectedButtonIndex; // „D„|„‘ „~„p„r„y„s„p„ˆ„y„y „{„|„p„r„y„p„„„…„‚„€„z
    std::vector<Button> buttons;

    int totalLevels; // „R„{„€„|„„{„€ „…„‚„€„r„~„u„z „t„€„ƒ„„„…„„~„€

    // „B„ƒ„„€„}„€„s„p„„„u„|„„~„„u „}„u„„„€„t„
    void CreateMainMenuButtons();
    void CreateLevelSelectButtons();
    void CreatePauseMenuButtons();
    void UpdateHover(int mouseX, int mouseY);

public:
    Menu(int levels);
    ~Menu();

    // „O„ƒ„~„€„r„~„„u „}„u„„„€„t„
    void SetState(GameState state);
    GameState GetState() const { return currentState; }

    void Draw() const;
    ButtonAction HandleInput(int& levelToLoad); // „B„€„x„r„‚„p„‹„p„u„„ „t„u„z„ƒ„„„r„y„u „y „~„€„}„u„‚ „…„‚„€„r„~„‘ „u„ƒ„|„y „~„…„w„u„~

    // „N„p„r„y„s„p„ˆ„y„‘ „{„|„p„r„y„p„„„…„‚„€„z
    void SelectNextButton();
    void SelectPrevButton();
    ButtonAction ActivateSelectedButton(int& levelToLoad);
};