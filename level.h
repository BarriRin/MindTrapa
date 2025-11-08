#pragma once
#include "DxLib.h"
#include <vector>

// �S�y���� �q�|���{���r
enum class BlockType {
    PLATFORM = 0,      // �O�q�����~�p�� ���|�p���������}�p
    TRIGGER = 1,       // �S���y�s�s�u�� ���u���u�����t�p �~�p ���|�u�t�������y�z �������r�u�~��
    INVISIBLE_WALL = 2,// �N�u�r�y�t�y�}�p�� �����u�~�p (�{���|�|�y�x�y�� �u������, �~�� �~�u �r�y�t�~�p)
    FAKE_PLATFORM = 3, // �U�p�|�����y�r�p�� ���|�p���������}�p (�r�y�t�~�p, �~�� �{���|�|�y�x�y�y �~�u��)
    SPIKES = 4,        // �Y�y���� (���q�y�r�p���� �y�s�����{�p)
    DISAPPEARING = 5,  // �I�����u�x�p�����p�� ���|�p���������}�p (���u���u�x N ���u�{���~�t)
    MOVING = 6,        // �D�r�y�w�����p������ ���|�p���������}�p
    BUTTON = 7,        // �K�~�����{�p (�p�{���y�r�y�����u�� ������-����)
    TELEPORT = 8       // �S�u�|�u��������
};

// �R�������{�������p �q�|���{�p
struct Block {
    VECTOR pos;         // �S�u�{�����p�� �����x�y���y��
    VECTOR size;        // �Q�p�x�}�u�� �q�|���{�p
    BlockType type;     // �S�y�� �q�|���{�p

    // �D�������|�~�y���u�|���~���u ���p���p�}�u������ �t�|�� �}�u���p�~�y�{
    VECTOR originalPos; // �N�p���p�|���~�p�� �����x�y���y�� (�t�|�� �t�r�y�w�����y������ ���|�p���������})
    VECTOR prevPos;     // �P���u�t���t�����p�� �����x�y���y�� (�t�|�� �r�����y���|�u�~�y�� ���{�����������y)
    float timer;        // �D�|�� �y�����u�x�p�����y�� �q�|���{���r, ���p�z�}�u�����r
    bool isActive;      // �D�|�� �{�~�������{, �p�{���y�r�~���� ���|�u�}�u�~�����r
    VECTOR moveTarget;  // �D�|�� �t�r�y�w�����y������ ���|�p���������} (�{���~�u���~�p�� �������{�p)
    int linkId;         // �D�|�� ���r���x�p�~�~���� ���|�u�}�u�~�����r (�{�~�����{�p���t�r�u����)

    Block(VECTOR position, VECTOR blockSize, BlockType blockType,
        float time = 0.0f, bool active = true, VECTOR target = VGet(0, 0, 0), int link = 0)
        : pos(position), size(blockSize), type(blockType),
        originalPos(position), // �H�p�����}�y�~�p�u�} �~�p���p�|���~���� �����x�y���y��!
        prevPos(position),     // �I�x�~�p���p�|���~�� prevPos = pos
        timer(time), isActive(active), moveTarget(target), linkId(link) {
    }
};

// �K�|�p���� �������r�~��
class Level {
private:
    std::vector<Block> blocks;
    VECTOR playerSpawn;
    int levelId;

public:
    Level(int id);
    ~Level();

    // �O���~���r�~���u �}�u�����t��
    void LoadLevelData(int id);
    void Update(float deltaTime); // �O�q�~���r�|�u�~�y�u �}�u���p�~�y�{
    void Draw() const;

    bool CheckCollision(VECTOR playerPos, VECTOR playerSize, VECTOR& newPos, VECTOR& velocity, bool& onGround, VECTOR& platformVelocity);
    bool CheckWinTrigger(VECTOR playerPos, VECTOR playerSize) const;
    bool CheckDeadlyTrigger(VECTOR playerPos, VECTOR playerSize) const;
    bool CheckTeleportTrigger(VECTOR playerPos, VECTOR playerSize, VECTOR& teleportTarget) const;
    void ActivateButton(VECTOR playerPos, VECTOR playerSize, bool keyPressed);

    // �C�u�����u����
    VECTOR GetPlayerSpawn() const { return playerSpawn; }
    int GetId() const { return levelId; }
    size_t GetBlockCount() const { return blocks.size(); }
    const std::vector<Block>& GetBlocks() const { return blocks; }

    // �D�|�� �����|�p�t�{�y
    void PrintDebugInfo() const;
};