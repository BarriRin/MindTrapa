#pragma once
#include <string>
#include <array>

static const int PROFILE_SLOT_COUNT  = 5;
static const int PROFILE_LEVEL_COUNT = 50;

struct Profile {
    std::wstring nickname;
    bool  isEmpty           = true;
    int   stars[PROFILE_LEVEL_COUNT]     = {};
    float bestTimes[PROFILE_LEVEL_COUNT] = {};
    int   mouseSensitivity = 50;   // 0-100, 50 = 1.0x
    int   musicVolume      = 80;   // 0-100
    int   sfxVolume        = 60;   // 0-100

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
    const Profile& GetSlot(int slot) const { return slots[slot]; }
    bool           IsSlotEmpty(int slot)   const { return slots[slot].isEmpty; }

    // Progress helpers (operate on current profile)
    int   GetStars(int levelId)    const;
    float GetBestTime(int levelId) const;
    bool  HasBestTime(int levelId) const;
    void  SetLevelResult(int levelId, int stars, float time);

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
