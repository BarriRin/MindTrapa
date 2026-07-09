#include "AudioManager.h"
#include <string>

AudioManager::AudioManager()
    : currentBgmIdx(-1)
    , sfxJump(-1), sfxLand(-1), sfxDeath(-1), sfxWin(-1)
{
    for (int i = 0; i < BGM_COUNT; i++) bgmHandles[i] = -1;
}

AudioManager::~AudioManager() {
    StopBgm();
    for (int i = 0; i < BGM_COUNT; i++)
        if (bgmHandles[i] != -1) DeleteSoundMem(bgmHandles[i]);
    if (sfxJump  != -1) DeleteSoundMem(sfxJump);
    if (sfxLand  != -1) DeleteSoundMem(sfxLand);
    if (sfxDeath != -1) DeleteSoundMem(sfxDeath);
    if (sfxWin   != -1) DeleteSoundMem(sfxWin);
}

void AudioManager::LoadBgm(const std::wstring& packFolder) {
    StopBgm();
    for (int i = 0; i < BGM_COUNT; i++) {
        if (bgmHandles[i] != -1) { DeleteSoundMem(bgmHandles[i]); bgmHandles[i] = -1; }
    }

    SetCreateSoundDataType(DX_SOUNDDATATYPE_FILE);

    const wchar_t* names[BGM_COUNT] = { L"block1", L"block2", L"block3", L"block4", L"block5" };
    const wchar_t* exts[]           = { L".ogg", L".mp3", L".wav" };

    for (int i = 0; i < BGM_COUNT; i++) {
        for (auto ext : exts) {
            std::wstring path = L"media/" + packFolder + L"/music/" + names[i] + ext;
            int h = LoadSoundMem(path.c_str());
            if (h != -1) { bgmHandles[i] = h; break; }
        }
    }

    SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
    // currentBgmIdx уже сброшен в -1 через StopBgm() — вызывающая сторона
    // сама решает, нужно ли тут же перезапустить PlayBgm(blockIdx, ...)
}

void AudioManager::LoadSfx() {
    sfxJump  = LoadSoundMem(L"media/sfx/jump.wav");
    sfxLand  = LoadSoundMem(L"media/sfx/land.wav");
    sfxDeath = LoadSoundMem(L"media/sfx/fall.wav");
    sfxWin   = LoadSoundMem(L"media/sfx/win.wav");
}

void AudioManager::PlayBgm(int blockIdx, int volumePct) {
    if (blockIdx < 0 || blockIdx >= BGM_COUNT) return;
    if (bgmHandles[blockIdx] == -1) return;

    if (currentBgmIdx == blockIdx) {
        SetBgmVolume(volumePct);
        return;
    }
    StopBgm();
    ChangeVolumeSoundMem(volumePct * 255 / 100, bgmHandles[blockIdx]);
    PlaySoundMem(bgmHandles[blockIdx], DX_PLAYTYPE_LOOP, TRUE);
    currentBgmIdx = blockIdx;
}

void AudioManager::StopBgm() {
    if (currentBgmIdx >= 0 && bgmHandles[currentBgmIdx] != -1)
        StopSoundMem(bgmHandles[currentBgmIdx]);
    currentBgmIdx = -1;
}

void AudioManager::SetBgmVolume(int volumePct) {
    if (currentBgmIdx >= 0 && bgmHandles[currentBgmIdx] != -1)
        ChangeVolumeSoundMem(volumePct * 255 / 100, bgmHandles[currentBgmIdx]);
}

void AudioManager::PlaySfx(int handle, int volumePct) {
    if (handle == -1) return;
    StopSoundMem(handle);
    ChangeVolumeSoundMem(volumePct * 255 / 100, handle);
    PlaySoundMem(handle, DX_PLAYTYPE_BACK, TRUE);
}
