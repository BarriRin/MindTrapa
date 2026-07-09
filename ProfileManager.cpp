#include "ProfileManager.h"
#include <fstream>
#include <sstream>
#include <windows.h>
#include <direct.h>

static const char* SAVES_DIR = "saves";

// ── UTF conversion ────────────────────────────────────────────────────────────

static std::string WStringToUTF8(const std::wstring& ws) {
    if (ws.empty()) return {};
    int sz = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(sz - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, &result[0], sz, nullptr, nullptr);
    return result;
}

static std::wstring UTF8ToWString(const std::string& s) {
    if (s.empty()) return {};
    int sz = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    std::wstring result(sz - 1, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &result[0], sz);
    return result;
}

// ── Paths ─────────────────────────────────────────────────────────────────────

std::string ProfileManager::GetSlotPath(int slot) {
    return std::string(SAVES_DIR) + "/slot_" + std::to_string(slot) + ".dat";
}

std::string ProfileManager::GetConfigPath() {
    return std::string(SAVES_DIR) + "/config.dat";
}

// ── Singleton ─────────────────────────────────────────────────────────────────

ProfileManager& ProfileManager::GetInstance() {
    static ProfileManager instance;
    return instance;
}

// ── Public API ────────────────────────────────────────────────────────────────

void ProfileManager::Initialize() {
    _mkdir(SAVES_DIR);
    for (int i = 0; i < PROFILE_SLOT_COUNT; i++) {
        LoadSlotFromFile(i);
    }
    LoadConfig();
    if (lastUsedSlot >= 0 && lastUsedSlot < PROFILE_SLOT_COUNT && !slots[lastUsedSlot].isEmpty) {
        currentSlot = lastUsedSlot;
    }
}

bool ProfileManager::CreateProfile(int slot, const std::wstring& nickname) {
    if (slot < 0 || slot >= PROFILE_SLOT_COUNT) return false;
    slots[slot]          = Profile{};
    slots[slot].nickname = nickname.empty() ? L"Player" : nickname;
    slots[slot].isEmpty  = false;
    currentSlot          = slot;
    lastUsedSlot         = slot;
    SaveSlotToFile(slot);
    SaveConfig();
    return true;
}

bool ProfileManager::LoadProfile(int slot) {
    if (slot < 0 || slot >= PROFILE_SLOT_COUNT) return false;
    if (slots[slot].isEmpty) return false;
    currentSlot  = slot;
    lastUsedSlot = slot;
    SaveConfig();
    return true;
}

void ProfileManager::DeleteProfile(int slot) {
    if (slot < 0 || slot >= PROFILE_SLOT_COUNT) return;
    slots[slot] = Profile{};
    std::remove(GetSlotPath(slot).c_str());
    bool dirty = false;
    if (currentSlot  == slot) { currentSlot  = -1; dirty = true; }
    if (lastUsedSlot == slot) { lastUsedSlot = -1; dirty = true; }
    if (dirty) SaveConfig();
}

void ProfileManager::SaveCurrentProfile() {
    if (currentSlot >= 0) SaveSlotToFile(currentSlot);
}

Profile* ProfileManager::GetCurrentProfile() {
    return (currentSlot >= 0) ? &slots[currentSlot] : nullptr;
}

const Profile* ProfileManager::GetCurrentProfile() const {
    return (currentSlot >= 0) ? &slots[currentSlot] : nullptr;
}

// ── Progress helpers ──────────────────────────────────────────────────────────

int ProfileManager::GetStars(int levelId) const {
    const Profile* p = GetCurrentProfile();
    if (!p || levelId < 1 || levelId > PROFILE_LEVEL_COUNT) return 0;
    return p->stars[levelId - 1];
}

float ProfileManager::GetBestTime(int levelId) const {
    const Profile* p = GetCurrentProfile();
    if (!p || levelId < 1 || levelId > PROFILE_LEVEL_COUNT) return 0.0f;
    return p->bestTimes[levelId - 1];
}

bool ProfileManager::HasBestTime(int levelId) const {
    const Profile* p = GetCurrentProfile();
    if (!p || levelId < 1 || levelId > PROFILE_LEVEL_COUNT) return false;
    return p->bestTimes[levelId - 1] > 0.0f;
}

void ProfileManager::SetLevelResult(int levelId, int newStars, float time) {
    Profile* p = GetCurrentProfile();
    if (!p || levelId < 1 || levelId > PROFILE_LEVEL_COUNT) return;
    int idx = levelId - 1;
    if (newStars > p->stars[idx]) p->stars[idx] = newStars;
    if (time > 0.0f && (p->bestTimes[idx] == 0.0f || time < p->bestTimes[idx]))
        p->bestTimes[idx] = time;
    SaveCurrentProfile();
}

// ── Паки оформления ───────────────────────────────────────────────────────────

int ProfileManager::GetThemePack() const {
    const Profile* p = GetCurrentProfile();
    return p ? p->themePack : 0;
}

bool ProfileManager::IsThemePackUnlocked(int pack) const {
    if (pack <= 0) return true;
    if (pack > THEME_PACK_COUNT - 1) return false;
    const Profile* p = GetCurrentProfile();
    if (!p) return false;
    return p->TotalStars() >= THEME_PACK_THRESHOLDS[pack - 1];
}

void ProfileManager::SetThemePack(int pack) {
    Profile* p = GetCurrentProfile();
    if (!p || !IsThemePackUnlocked(pack)) return;
    p->themePack = pack;
    SaveCurrentProfile();
}

// ── File I/O ──────────────────────────────────────────────────────────────────

void ProfileManager::LoadSlotFromFile(int slot) {
    slots[slot] = Profile{};
    std::ifstream f(GetSlotPath(slot));
    if (!f.is_open()) return;

    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = line.substr(0, eq);
        std::string val = line.substr(eq + 1);

        auto safeStoi = [](const std::string& s, int def = 0) -> int {
            try { return std::stoi(s); } catch (...) { return def; }
        };
        auto safeStof = [](const std::string& s, float def = 0.0f) -> float {
            try { return std::stof(s); } catch (...) { return def; }
        };

        if (key == "NAME") {
            slots[slot].nickname = UTF8ToWString(val);
            slots[slot].isEmpty  = false;
        } else if (key == "MOUSE_SENS") {
            slots[slot].mouseSensitivity = safeStoi(val, 50);
        } else if (key == "MUSIC_VOL") {
            slots[slot].musicVolume = safeStoi(val, 80);
        } else if (key == "SFX_VOL") {
            slots[slot].sfxVolume = safeStoi(val, 60);
        } else if (key == "THEME_PACK") {
            slots[slot].themePack = safeStoi(val, 0);
        } else if (key == "STARS") {
            std::stringstream ss(val);
            std::string item;
            int i = 0;
            while (std::getline(ss, item, ',') && i < PROFILE_LEVEL_COUNT)
                slots[slot].stars[i++] = safeStoi(item);
        } else if (key == "BEST_TIMES") {
            std::stringstream ss(val);
            std::string item;
            int i = 0;
            while (std::getline(ss, item, ',') && i < PROFILE_LEVEL_COUNT)
                slots[slot].bestTimes[i++] = safeStof(item);
        }
    }
}

void ProfileManager::SaveSlotToFile(int slot) const {
    if (slots[slot].isEmpty) return;
    std::ofstream f(GetSlotPath(slot));
    if (!f.is_open()) return;

    f << "NAME=" << WStringToUTF8(slots[slot].nickname) << "\n";
    f << "MOUSE_SENS=" << slots[slot].mouseSensitivity << "\n";
    f << "MUSIC_VOL=" << slots[slot].musicVolume << "\n";
    f << "SFX_VOL=" << slots[slot].sfxVolume << "\n";
    f << "THEME_PACK=" << slots[slot].themePack << "\n";

    f << "STARS=";
    for (int i = 0; i < PROFILE_LEVEL_COUNT; i++) {
        f << slots[slot].stars[i];
        if (i < PROFILE_LEVEL_COUNT - 1) f << ",";
    }
    f << "\n";

    f << "BEST_TIMES=";
    for (int i = 0; i < PROFILE_LEVEL_COUNT; i++) {
        f << slots[slot].bestTimes[i];
        if (i < PROFILE_LEVEL_COUNT - 1) f << ",";
    }
    f << "\n";
}

void ProfileManager::LoadConfig() {
    std::ifstream f(GetConfigPath());
    if (!f.is_open()) return;
    std::string line;
    while (std::getline(f, line)) {
        auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = line.substr(0, eq);
        std::string value = line.substr(eq + 1);
        if (key == "LAST_SLOT") {
            try { lastUsedSlot = std::stoi(value); } catch (...) {}
        } else if (key == "LANGUAGE") {
            if (value == "RU" || value == "JA" || value == "EN") languageCode = value;
        }
    }
}

void ProfileManager::SaveConfig() const {
    std::ofstream f(GetConfigPath());
    if (f.is_open()) {
        f << "LAST_SLOT=" << lastUsedSlot << "\n";
        f << "LANGUAGE=" << languageCode << "\n";
    }
}
