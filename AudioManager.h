#pragma once
#include "DxLib.h"
#include <string>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    // Explicitly releases sound handles — call before DxLib_End(), otherwise
    // the destructor would run after DxLib is already torn down on exit
    void Shutdown();

    // packFolder is the pack's folder name under media/ (e.g. L"Defolt_Pack", L"Pack1");
    // calling this again unloads the previous handles and reloads from scratch
    void LoadBgm(const std::wstring& packFolder);
    void LoadSfx();

    // BGM
    void PlayBgm(int blockIdx, int volumePct);
    void StopBgm();
    void SetBgmVolume(int volumePct);

    // SFX
    void PlaySfx(int handle, int volumePct);
    void PlayJump(int volumePct)   { PlaySfx(sfxJump,   volumePct); }
    void PlayLand(int volumePct)   { PlaySfx(sfxLand,   volumePct); }
    void PlayDeath(int volumePct)  { PlaySfx(sfxDeath,  volumePct); }
    void PlayWin(int volumePct)    { PlaySfx(sfxWin,    volumePct); }
    void PlayBounce(int volumePct)   { PlaySfx(sfxBounce,   volumePct); }
    void PlayTeleport(int volumePct) { PlaySfx(sfxTeleport, volumePct); }

private:
    static constexpr int BGM_COUNT = 5;

    int bgmHandles[BGM_COUNT];
    int currentBgmIdx;

    int sfxJump;
    int sfxLand;
    int sfxDeath;
    int sfxWin;
    int sfxBounce;
    int sfxTeleport;
};
