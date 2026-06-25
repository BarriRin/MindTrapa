#pragma once
#include "DxLib.h"

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    void LoadBgm();
    void LoadSfx();

    // BGM
    void PlayBgm(int blockIdx, int volumePct);
    void StopBgm();
    void SetBgmVolume(int volumePct);

    // SFX
    void PlaySfx(int handle, int volumePct);
    void PlayJump(int volumePct)  { PlaySfx(sfxJump,  volumePct); }
    void PlayLand(int volumePct)  { PlaySfx(sfxLand,  volumePct); }
    void PlayDeath(int volumePct) { PlaySfx(sfxDeath, volumePct); }
    void PlayWin(int volumePct)   { PlaySfx(sfxWin,   volumePct); }

private:
    static constexpr int BGM_COUNT = 5;

    int bgmHandles[BGM_COUNT];
    int currentBgmIdx;

    int sfxJump;
    int sfxLand;
    int sfxDeath;
    int sfxWin;
};
