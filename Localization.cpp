#include "Localization.h"
#include "ProfileManager.h"
#include "DxLib.h"
#include <windows.h>

namespace {

struct FontFiles {
    const wchar_t* regular;
    const wchar_t* bold;
    const wchar_t* family;
};

// Индекс совпадает с Language enum (EN, RU, JA)
const FontFiles LANG_FONTS[3] = {
    { L"fonts/Orbitron/static/Orbitron-Regular.ttf",   L"fonts/Orbitron/static/Orbitron-Bold.ttf",   L"Orbitron" },
    { L"fonts/Exo2/Exo2-Regular.ttf",                  L"fonts/Exo2/Exo2-Bold.ttf",                  L"Exo 2" },
    { L"fonts/NotoSansJP/NotoSansJP-Regular.ttf",      L"fonts/NotoSansJP/NotoSansJP-Bold.ttf",       L"Noto Sans JP" },
};

const wchar_t* LANG_NAMES[3] = { L"English", L"Русский", L"日本語" };

// STRINGS[язык][StrId] — таблица переводов
const wchar_t* const STRINGS[3][(int)StrId::COUNT] = {
    // ── EN ──────────────────────────────────────────────────────────────
    {
        /* START_GAME */                 L"Start Game",
        /* LEVEL_SELECT */                L"Level Select",
        /* SETTINGS_BTN */                L"Settings",
        /* CHANGE_PROFILE */              L"Change Profile",
        /* EXIT */                        L"Exit",
        /* BLOCK_LABEL */                 L"Block %d",
        /* LEVEL_LABEL */                 L"Level %d",
        /* LOCKED_SUFFIX */               L"\n(LOCKED)",
        /* BACK */                        L"< Back",
        /* BACK_TO_BLOCKS */              L"< Back to Blocks",
        /* MUSIC_SELECT_BTN */            L"Theme Packs",
        /* DEFAULT_THEME */               L"Default Theme",
        /* THEME_PACK_1 */                L"Pack 1",
        /* THEME_PACK_2 */                L"Pack 2",
        /* PACK_LOCKED_FMT */             L"Locked (%d★ needed)",
        /* CONTINUE */                    L"Continue",
        /* RESTART_LEVEL */               L"Restart Level",
        /* MAIN_MENU_BTN */               L"Main Menu",
        /* RESTART */                     L"Restart",
        /* NEXT_LEVEL */                  L"Next Level",
        /* YES_DELETE */                  L"Yes, Delete",
        /* CANCEL */                      L"Cancel",
        /* CREATE_PLUS */                 L"+ Create",
        /* SELECT_BTN */                  L"Select",
        /* DELETE_X */                    L"X",
        /* CREATE_BTN */                  L"Create",
        /* SELECT_PROFILE_HDR */          L"SELECT PROFILE",
        /* SLOT_LABEL */                  L"SLOT %d",
        /* EMPTY_LABEL */                 L"EMPTY",
        /* STARS_FRACTION */              L"Stars: %d / %d",
        /* DELETE_PROFILE_CONFIRM_MSG */  L"Delete profile \"%ls\"?",
        /* PROGRESS_LOST */               L"All progress will be lost.",
        /* CREATE_PROFILE_HDR */          L"CREATE PROFILE",
        /* ENTER_NICKNAME */              L"Enter nickname:",
        /* PROFILE_BAR */                 L"%ls  |  Stars: %d",
        /* SELECT_BLOCK_HDR */            L"SELECT BLOCK",
        /* BLOCK_LEVELS_HDR */            L"BLOCK %d — LEVELS 1-10",
        /* SETTINGS_HDR */                L"SETTINGS",
        /* MUSIC_VOLUME */                L"Music Volume",
        /* SOUND_VOLUME */                L"Sound Volume",
        /* MOUSE_SENSITIVITY */           L"Mouse Sensitivity",
        /* MUSIC_SELECT_HDR */            L"THEME PACKS",
        /* PAUSED_HDR */                  L"PAUSED",
        /* LEVEL_COMPLETE */              L"LEVEL COMPLETE!",
        /* YOU_DIED */                    L"YOU DIED",
        /* TIME_LABEL */                  L"Time: %d:%05.2f",
        /* HINT_RESULT_NEXT */            L"[R] Restart   [Space] Next Level   [ESC] Main Menu",
        /* HINT_RESULT_NOLAST */          L"[R] Restart   [ESC] Main Menu",
        /* NAV_HINT */                    L"Mouse: Click | Arrow Keys + Enter | ESC: Back",
        /* HUD_LEVEL */                   L"Level: %d/%d",
        /* HUD_DEATHS */                  L"Deaths: %d",
        /* HUD_BEST */                    L"Best: %d:%05.2f",
        /* HUD_HINT_BUTTON */             L"Get close to YELLOW button and press E!",
        /* HUD_HINT_CRUMBLE */            L"WARNING: Platforms disappear 1.5 sec after touch!",
        /* HUD_HINT_SPIKES */             L"Watch the spikes! They retract every 2 seconds!",
        /* HUD_CONTROLS */                L"WASD - move, Space - jump, R - restart",
        /* HUD_BUTTON_HINT */             L"E - activate button/switch",
        /* HUD_BLOCK_COUNT */             L"Blocks in level: %d",
        /* LANGUAGE_BUTTON */             L"Language: %ls",
    },
    // ── RU ──────────────────────────────────────────────────────────────
    {
        /* START_GAME */                 L"Начать игру",
        /* LEVEL_SELECT */                L"Выбор уровня",
        /* SETTINGS_BTN */                L"Настройки",
        /* CHANGE_PROFILE */              L"Сменить профиль",
        /* EXIT */                        L"Выход",
        /* BLOCK_LABEL */                 L"Блок %d",
        /* LEVEL_LABEL */                 L"Уровень %d",
        /* LOCKED_SUFFIX */               L"\n(ЗАКРЫТО)",
        /* BACK */                        L"< Назад",
        /* BACK_TO_BLOCKS */              L"< К блокам",
        /* MUSIC_SELECT_BTN */            L"Паки оформления",
        /* DEFAULT_THEME */               L"Тема по умолчанию",
        /* THEME_PACK_1 */                L"Пак 1",
        /* THEME_PACK_2 */                L"Пак 2",
        /* PACK_LOCKED_FMT */             L"Заблокировано (нужно %d★)",
        /* CONTINUE */                    L"Продолжить",
        /* RESTART_LEVEL */               L"Перезапустить уровень",
        /* MAIN_MENU_BTN */               L"Главное меню",
        /* RESTART */                     L"Заново",
        /* NEXT_LEVEL */                  L"Следующий уровень",
        /* YES_DELETE */                  L"Да, удалить",
        /* CANCEL */                      L"Отмена",
        /* CREATE_PLUS */                 L"+ Создать",
        /* SELECT_BTN */                  L"Выбрать",
        /* DELETE_X */                    L"X",
        /* CREATE_BTN */                  L"Создать",
        /* SELECT_PROFILE_HDR */          L"ВЫБОР ПРОФИЛЯ",
        /* SLOT_LABEL */                  L"СЛОТ %d",
        /* EMPTY_LABEL */                 L"ПУСТО",
        /* STARS_FRACTION */              L"Звёзды: %d / %d",
        /* DELETE_PROFILE_CONFIRM_MSG */  L"Удалить профиль «%ls»?",
        /* PROGRESS_LOST */               L"Весь прогресс будет потерян.",
        /* CREATE_PROFILE_HDR */          L"СОЗДАНИЕ ПРОФИЛЯ",
        /* ENTER_NICKNAME */              L"Введите никнейм:",
        /* PROFILE_BAR */                 L"%ls  |  Звёзды: %d",
        /* SELECT_BLOCK_HDR */            L"ВЫБОР БЛОКА",
        /* BLOCK_LEVELS_HDR */            L"БЛОК %d — УРОВНИ 1-10",
        /* SETTINGS_HDR */                L"НАСТРОЙКИ",
        /* MUSIC_VOLUME */                L"Громкость музыки",
        /* SOUND_VOLUME */                L"Громкость звука",
        /* MOUSE_SENSITIVITY */           L"Чувствительность мыши",
        /* MUSIC_SELECT_HDR */            L"ПАКИ ОФОРМЛЕНИЯ",
        /* PAUSED_HDR */                  L"ПАУЗА",
        /* LEVEL_COMPLETE */              L"УРОВЕНЬ ПРОЙДЕН!",
        /* YOU_DIED */                    L"ВЫ ПОГИБЛИ",
        /* TIME_LABEL */                  L"Время: %d:%05.2f",
        /* HINT_RESULT_NEXT */            L"[R] Заново   [Space] Следующий уровень   [ESC] Меню",
        /* HINT_RESULT_NOLAST */          L"[R] Заново   [ESC] Меню",
        /* NAV_HINT */                    L"Мышь: клик | Стрелки + Enter | ESC: назад",
        /* HUD_LEVEL */                   L"Уровень: %d/%d",
        /* HUD_DEATHS */                  L"Смертей: %d",
        /* HUD_BEST */                    L"Рекорд: %d:%05.2f",
        /* HUD_HINT_BUTTON */             L"Подойдите к ЖЁЛТОЙ кнопке и нажмите E!",
        /* HUD_HINT_CRUMBLE */            L"ВНИМАНИЕ: платформы исчезают через 1.5 сек после касания!",
        /* HUD_HINT_SPIKES */             L"Осторожно, шипы! Они убираются каждые 2 секунды!",
        /* HUD_CONTROLS */                L"WASD — движение, Space — прыжок, R — рестарт",
        /* HUD_BUTTON_HINT */             L"E — активировать кнопку/рычаг",
        /* HUD_BLOCK_COUNT */             L"Блоков в уровне: %d",
        /* LANGUAGE_BUTTON */             L"Язык: %ls",
    },
    // ── JA ──────────────────────────────────────────────────────────────
    {
        /* START_GAME */                 L"スタート",
        /* LEVEL_SELECT */                L"レベル選択",
        /* SETTINGS_BTN */                L"設定",
        /* CHANGE_PROFILE */              L"プロフィール変更",
        /* EXIT */                        L"終了",
        /* BLOCK_LABEL */                 L"ブロック %d",
        /* LEVEL_LABEL */                 L"レベル %d",
        /* LOCKED_SUFFIX */               L"\n(ロック中)",
        /* BACK */                        L"< 戻る",
        /* BACK_TO_BLOCKS */              L"< ブロック一覧へ",
        /* MUSIC_SELECT_BTN */            L"テーマパック",
        /* DEFAULT_THEME */               L"デフォルトテーマ",
        /* THEME_PACK_1 */                L"パック 1",
        /* THEME_PACK_2 */                L"パック 2",
        /* PACK_LOCKED_FMT */             L"ロック中 (%d★必要)",
        /* CONTINUE */                    L"再開",
        /* RESTART_LEVEL */               L"レベルを再開",
        /* MAIN_MENU_BTN */               L"メインメニュー",
        /* RESTART */                     L"リトライ",
        /* NEXT_LEVEL */                  L"次のレベル",
        /* YES_DELETE */                  L"はい、削除",
        /* CANCEL */                      L"キャンセル",
        /* CREATE_PLUS */                 L"+ 作成",
        /* SELECT_BTN */                  L"選択",
        /* DELETE_X */                    L"X",
        /* CREATE_BTN */                  L"作成",
        /* SELECT_PROFILE_HDR */          L"プロフィール選択",
        /* SLOT_LABEL */                  L"スロット %d",
        /* EMPTY_LABEL */                 L"空き",
        /* STARS_FRACTION */              L"スター: %d / %d",
        /* DELETE_PROFILE_CONFIRM_MSG */  L"プロフィール「%ls」を削除しますか?",
        /* PROGRESS_LOST */               L"進行状況はすべて失われます。",
        /* CREATE_PROFILE_HDR */          L"プロフィール作成",
        /* ENTER_NICKNAME */              L"ニックネームを入力:",
        /* PROFILE_BAR */                 L"%ls  |  スター: %d",
        /* SELECT_BLOCK_HDR */            L"ブロック選択",
        /* BLOCK_LEVELS_HDR */            L"ブロック %d — レベル 1-10",
        /* SETTINGS_HDR */                L"設定",
        /* MUSIC_VOLUME */                L"音楽音量",
        /* SOUND_VOLUME */                L"効果音音量",
        /* MOUSE_SENSITIVITY */           L"マウス感度",
        /* MUSIC_SELECT_HDR */            L"テーマパック",
        /* PAUSED_HDR */                  L"一時停止",
        /* LEVEL_COMPLETE */              L"レベルクリア!",
        /* YOU_DIED */                    L"死亡しました",
        /* TIME_LABEL */                  L"タイム: %d:%05.2f",
        /* HINT_RESULT_NEXT */            L"[R] リトライ   [Space] 次のレベル   [ESC] メニュー",
        /* HINT_RESULT_NOLAST */          L"[R] リトライ   [ESC] メニュー",
        /* NAV_HINT */                    L"マウス: クリック | 矢印キー+Enter | ESC: 戻る",
        /* HUD_LEVEL */                   L"レベル: %d/%d",
        /* HUD_DEATHS */                  L"死亡回数: %d",
        /* HUD_BEST */                    L"ベスト: %d:%05.2f",
        /* HUD_HINT_BUTTON */             L"黄色いボタンに近づいてEキーを押してください!",
        /* HUD_HINT_CRUMBLE */            L"警告: 触れてから1.5秒でプラットフォームが消えます!",
        /* HUD_HINT_SPIKES */             L"トゲに注意! 2秒ごとに引っ込みます!",
        /* HUD_CONTROLS */                L"WASD - 移動, スペース - ジャンプ, R - リスタート",
        /* HUD_BUTTON_HINT */             L"E - ボタン/スイッチを起動",
        /* HUD_BLOCK_COUNT */             L"レベル内のブロック数: %d",
        /* LANGUAGE_BUTTON */             L"言語: %ls",
    },
};

Language LanguageFromCode(const std::string& code) {
    if (code == "RU") return Language::RU;
    if (code == "JA") return Language::JA;
    return Language::EN;
}

const char* CodeFromLanguage(Language lang) {
    switch (lang) {
        case Language::RU: return "RU";
        case Language::JA: return "JA";
        default:           return "EN";
    }
}

} // namespace

Localization& Localization::GetInstance() {
    static Localization instance;
    return instance;
}

void Localization::LoadFontsForLanguage(Language lang, const wchar_t* regularPath, const wchar_t* boldPath, const wchar_t* familyName) {
    AddFontResourceEx(regularPath, FR_PRIVATE, NULL);
    AddFontResourceEx(boldPath,    FR_PRIVATE, NULL);

    int idx = (int)lang;
    fontHandles[idx][(int)FontSize::Title]   = CreateFontToHandle(familyName, 60, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4, -1, 3);
    fontHandles[idx][(int)FontSize::Heading] = CreateFontToHandle(familyName, 32, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4, -1, 2);
    fontHandles[idx][(int)FontSize::Button]  = CreateFontToHandle(familyName, 22, -1, DX_FONTTYPE_ANTIALIASING_4X4);
    fontHandles[idx][(int)FontSize::Normal]  = CreateFontToHandle(familyName, 18, -1, DX_FONTTYPE_ANTIALIASING_4X4);
    fontHandles[idx][(int)FontSize::Small]   = CreateFontToHandle(familyName, 13, -1, DX_FONTTYPE_ANTIALIASING_4X4);
}

void Localization::Init() {
    if (initialized) return;

    for (int i = 0; i < 3; i++) {
        const FontFiles& f = LANG_FONTS[i];
        LoadFontsForLanguage((Language)i, f.regular, f.bold, f.family);
    }

    currentLanguage = LanguageFromCode(ProfileManager::GetInstance().GetLanguageCode());
    initialized = true;
}

void Localization::Shutdown() {
    if (!initialized) return;

    for (int i = 0; i < 3; i++) {
        for (int s = 0; s < (int)FontSize::COUNT; s++) {
            if (fontHandles[i][s] != -1) DeleteFontToHandle(fontHandles[i][s]);
        }
        const FontFiles& f = LANG_FONTS[i];
        RemoveFontResourceEx(f.regular, FR_PRIVATE, NULL);
        RemoveFontResourceEx(f.bold,    FR_PRIVATE, NULL);
    }
    initialized = false;
}

void Localization::SetLanguage(Language lang) {
    currentLanguage = lang;
    ProfileManager::GetInstance().SetLanguageCode(CodeFromLanguage(lang));
}

const wchar_t* Localization::Tr(StrId id) const {
    return STRINGS[(int)currentLanguage][(int)id];
}

int Localization::GetFont(FontSize size) const {
    return fontHandles[(int)currentLanguage][(int)size];
}

const wchar_t* Localization::LanguageDisplayName(Language lang) const {
    return LANG_NAMES[(int)lang];
}
