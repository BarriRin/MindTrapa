#include "ModelManager.h"
#include "DxLib.h"
#include <locale>
#include <codecvt>

// Вспомогательная функция для конвертации std::string в std::wstring
static std::wstring StringToWString(const std::string& str) {
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

ModelManager::ModelManager()
    : modelsPath("models/"), skyboxesPath("skyboxes/") {
}

ModelManager& ModelManager::GetInstance() {
    static ModelManager instance;
    return instance;
}

ModelManager::~ModelManager() {
    Cleanup();
}

bool ModelManager::Initialize() {
    return true;
}

void ModelManager::Cleanup() {
    for (auto& pair : models) {
        if (pair.second.handle != -1) {
            MV1DeleteModel(pair.second.handle);
        }
    }
    models.clear();

    for (auto& pair : skyboxes) {
        if (pair.second.handle != -1) {
            MV1DeleteModel(pair.second.handle);
        }
    }
    skyboxes.clear();
}

bool ModelManager::LoadModel(ModelID id, const std::string& filename, float scale) {
    if (IsModelLoaded(id)) return true;

    std::string fullPath = modelsPath + filename;
    std::wstring wFullPath = StringToWString(fullPath);

    int handle = MV1LoadModel(wFullPath.c_str());
    if (handle == -1) return false;

    MV1SetScale(handle, VGet(scale, scale, scale));

    ModelData data;
    data.handle = handle;
    data.scale = scale;
    data.rotationOffset = VGet(0, 0, 0);
    data.hasAnimation = false;
    data.animIndex = -1;

    models[id] = data;
    return true;
}

bool ModelManager::LoadSkybox(int blockId, const std::string& filename, float scale) {
    if (IsSkyboxLoaded(blockId)) return true;

    std::string fullPath = skyboxesPath + filename;
    std::wstring wFullPath = StringToWString(fullPath);

    int handle = MV1LoadModel(wFullPath.c_str());
    if (handle == -1) return false;

    MV1SetScale(handle, VGet(scale, scale, scale));

    SkyboxData data;
    data.handle = handle;
    data.scale = scale;
    data.rotation = VGet(0, 0, 0);

    skyboxes[blockId] = data;
    return true;
}

int ModelManager::GetModelHandle(ModelID id) const {
    auto it = models.find(id);
    return (it != models.end()) ? it->second.handle : -1;
}

const ModelData* ModelManager::GetModelData(ModelID id) const {
    auto it = models.find(id);
    return (it != models.end()) ? &it->second : nullptr;
}

int ModelManager::GetSkyboxHandle(int blockId) const {
    auto it = skyboxes.find(blockId);
    return (it != skyboxes.end()) ? it->second.handle : -1;
}

const SkyboxData* ModelManager::GetSkyboxData(int blockId) const {
    auto it = skyboxes.find(blockId);
    return (it != skyboxes.end()) ? &it->second : nullptr;
}

int ModelManager::DuplicateModel(ModelID id) {
    int handle = GetModelHandle(id);
    if (handle == -1) return -1;
    return MV1DuplicateModel(handle);
}

void ModelManager::DrawModel(ModelID id, VECTOR pos, VECTOR scale, VECTOR rotation) {
    int handle = GetModelHandle(id);
    if (handle == -1) return;
    MV1SetPosition(handle, pos);
    MV1SetScale(handle, scale);
    MV1SetRotationXYZ(handle, rotation);
    MV1DrawModel(handle);
}

void ModelManager::DrawSkybox(int blockId, VECTOR cameraPos) {
    auto it = skyboxes.find(blockId);
    if (it == skyboxes.end()) return;
    MV1SetPosition(it->second.handle, cameraPos);
    MV1SetScale(it->second.handle, VGet(it->second.scale, it->second.scale, it->second.scale));
    MV1SetRotationXYZ(it->second.handle, it->second.rotation);
    MV1DrawModel(it->second.handle);
}

bool ModelManager::IsModelLoaded(ModelID id) const {
    return models.find(id) != models.end();
}

bool ModelManager::IsSkyboxLoaded(int blockId) const {
    return skyboxes.find(blockId) != skyboxes.end();
}
