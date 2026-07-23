#pragma once
#include <string>

// UI languages
enum class Language { EN, RU, JA, COUNT };

// Font sizes (correspond to the old fontTitle/fontHeading/... from Menu)
enum class FontSize { Title, Heading, Button, Normal, Small, COUNT };

// IDs for every translatable UI/HUD string
enum class StrId {
    START_GAME, LEVEL_SELECT, SETTINGS_BTN, CHANGE_PROFILE, EXIT,
    BLOCK_LABEL, LEVEL_LABEL, LOCKED_SUFFIX, BACK, BACK_TO_BLOCKS,
    MUSIC_SELECT_BTN, DEFAULT_THEME,
    THEME_PACK_1, THEME_PACK_2, PACK_LOCKED_FMT,
    CONTINUE, RESTART_LEVEL, MAIN_MENU_BTN, RESTART, NEXT_LEVEL,
    YES_DELETE, CANCEL, CREATE_PLUS, SELECT_BTN, DELETE_X, CREATE_BTN,
    SELECT_PROFILE_HDR, SLOT_LABEL, EMPTY_LABEL, STARS_FRACTION,
    DELETE_PROFILE_CONFIRM_MSG, PROGRESS_LOST, CREATE_PROFILE_HDR, ENTER_NICKNAME,
    PROFILE_BAR, SELECT_BLOCK_HDR, BLOCK_LEVELS_HDR, SETTINGS_HDR,
    MUSIC_VOLUME, SOUND_VOLUME, MOUSE_SENSITIVITY, MUSIC_SELECT_HDR, PAUSED_HDR,
    LEVEL_COMPLETE, YOU_DIED, TIME_LABEL, HINT_RESULT_NEXT, HINT_RESULT_NOLAST,
    NAV_HINT,
    HUD_LEVEL, HUD_DEATHS, HUD_BEST, HUD_HINT_BUTTON,
    HUD_CONTROLS, HUD_BUTTON_HINT, HUD_BLOCK_COUNT,
    LANGUAGE_BUTTON,
    COUNT
};

// Localization singleton: strings + fonts for EN/RU/JA
// Each language needs its own font — Orbitron has no Cyrillic or kanji/kana glyphs
class Localization {
public:
    static Localization& GetInstance();

    void Init();      // loads all fonts (3 languages x 5 sizes), reads the language from ProfileManager
    void Shutdown();  // releases font handles and font resources

    void SetLanguage(Language lang);  // changes the language + saves it to the profile config
    Language GetLanguage() const { return currentLanguage; }

    const wchar_t* Tr(StrId id) const;
    int GetFont(FontSize size) const;

    // The language's name in that language itself (not translated) — used for the switch button
    const wchar_t* LanguageDisplayName(Language lang) const;

private:
    Localization()                                = default;
    Localization(const Localization&)             = delete;
    Localization& operator=(const Localization&)  = delete;

    Language currentLanguage = Language::EN;
    int fontHandles[(int)Language::COUNT][(int)FontSize::COUNT] = {}; // filled with -1 in Init()
    bool initialized = false;

    void LoadFontsForLanguage(Language lang, const wchar_t* regularPath, const wchar_t* boldPath, const wchar_t* familyName);
};
