#pragma once
#include "DxLib.h"
#include <string>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    // Явное освобождение звуковых хендлов — вызывать до DxLib_End(), иначе
    // деструктор сделает это уже после выгрузки DxLib при выходе из программы
    void Shutdown();

    // packFolder — имя папки пака внутри media/ (напр. L"Defolt_Pack", L"Pack1");
    // при повторном вызове выгружает предыдущие хендлы и грузит заново
    void LoadBgm(const std::wstring& packFolder);
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
