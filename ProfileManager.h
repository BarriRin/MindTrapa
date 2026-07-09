#pragma once
#include <string>
#include <array>

static const int PROFILE_SLOT_COUNT  = 5;
static const int PROFILE_LEVEL_COUNT = 50;

// Паки оформления (музыка+скайбоксы): 0 = Default (всегда открыт),
// THEME_PACK_THRESHOLDS[pack-1] звёзд суммарно нужно для pack 1/2
static const int THEME_PACK_COUNT = 3;
static const int THEME_PACK_THRESHOLDS[2] = { 75, 150 };

struct Profile {
    std::wstring nickname;
    bool  isEmpty           = true;
    int   stars[PROFILE_LEVEL_COUNT]     = {};
    float bestTimes[PROFILE_LEVEL_COUNT] = {};
    int   mouseSensitivity = 50;   // 0-100, 50 = 1.0x
    int   musicVolume      = 80;   // 0-100
    int   sfxVolume        = 60;   // 0-100
    int   themePack        = 0;    // 0 = Default, 1/2 = разблокируемый пак

    int TotalStars() const {
        int total = 0;
        for (int i = 0; i < PROFILE_LEVEL_COUNT; i++) total += stars[i];
        return total;
    }
};

class ProfileManager {
public:
    static ProfileManager& GetInstance();

    void Initialize();

    bool HasActiveProfile() const { return currentSlot >= 0; }
    int  GetCurrentSlot()   const { return currentSlot; }

    // Глобальный язык интерфейса ("EN"/"RU"/"JA"), не привязан к профилю
    const std::string& GetLanguageCode() const { return languageCode; }
    void SetLanguageCode(const std::string& code) { languageCode = code; SaveConfig(); }

    bool CreateProfile(int slot, const std::wstring& nickname);
    bool LoadProfile(int slot);
    void DeleteProfile(int slot);
    void SaveCurrentProfile();

    Profile*       GetCurrentProfile();
    const Profile* GetCurrentProfile() const;
    const Profile& GetSlot(int slot) const { return slots[(slot >= 0 && slot < PROFILE_SLOT_COUNT) ? slot : 0]; }
    bool           IsSlotEmpty(int slot)   const { return (slot >= 0 && slot < PROFILE_SLOT_COUNT) ? slots[slot].isEmpty : true; }

    // Progress helpers (operate on current profile)
    int   GetStars(int levelId)    const;
    float GetBestTime(int levelId) const;
    bool  HasBestTime(int levelId) const;
    void  SetLevelResult(int levelId, int stars, float time);

    // Паки оформления
    int  GetThemePack() const;
    bool IsThemePackUnlocked(int pack) const;
    void SetThemePack(int pack);

private:
    ProfileManager()                               = default;
    ProfileManager(const ProfileManager&)          = delete;
    ProfileManager& operator=(const ProfileManager&) = delete;

    std::array<Profile, PROFILE_SLOT_COUNT> slots;
    int currentSlot  = -1;
    int lastUsedSlot = -1;
    std::string languageCode = "EN";

    void LoadSlotFromFile(int slot);
    void SaveSlotToFile(int slot) const;
    void LoadConfig();
    void SaveConfig() const;

    static std::string GetSlotPath(int slot);
    static std::string GetConfigPath();
};
