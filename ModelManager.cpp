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
    // Здесь можно загрузить базовые модели
    // Пока оставим пустым - модели будут загружаться по требованию

    printfDx(TEXT("ModelManager: Initialized\n"));
    return true;
}

void ModelManager::Cleanup() {
    // Удалить все загруженные модели
    for (auto& pair : models) {
        if (pair.second.handle != -1) {
            MV1DeleteModel(pair.second.handle);
        }
    }
    models.clear();

    // Удалить все скайбоксы
    for (auto& pair : skyboxes) {
        if (pair.second.handle != -1) {
            MV1DeleteModel(pair.second.handle);
        }
    }
    skyboxes.clear();

    printfDx(TEXT("ModelManager: Cleaned up\n"));
}

bool ModelManager::LoadModel(ModelID id, const std::string& filename, float scale) {
    // Проверить, не загружена ли уже модель
    if (IsModelLoaded(id)) {
        printfDx(TEXT("ModelManager: Model %d already loaded\n"), (int)id);
        return true;
    }

    // Полный путь к файлу
    std::string fullPath = modelsPath + filename;
    std::wstring wFullPath = StringToWString(fullPath);

    // Загрузить модель
    int handle = MV1LoadModel(wFullPath.c_str());
    if (handle == -1) {
        printfDx(TEXT("ModelManager: Failed to load model %s\n"), StringToWString(fullPath).c_str());
        return false;
    }

    // Применить масштаб
    MV1SetScale(handle, VGet(scale, scale, scale));

    // Сохранить данные модели
    ModelData data;
    data.handle = handle;
    data.scale = scale;
    data.rotationOffset = VGet(0, 0, 0);
    data.hasAnimation = false;
    data.animIndex = -1;

    models[id] = data;

    printfDx(TEXT("ModelManager: Loaded model %s (ID: %d)\n"), StringToWString(filename).c_str(), (int)id);
    return true;
}

bool ModelManager::LoadSkybox(int blockId, const std::string& filename, float scale) {
    // Проверить, не загружен ли уже скайбокс
    if (IsSkyboxLoaded(blockId)) {
        printfDx(TEXT("ModelManager: Skybox for block %d already loaded\n"), blockId);
        return true;
    }

    // Полный путь к файлу
    std::string fullPath = skyboxesPath + filename;
    std::wstring wFullPath = StringToWString(fullPath);

    // Загрузить модель скайбокса
    int handle = MV1LoadModel(wFullPath.c_str());
    if (handle == -1) {
        printfDx(TEXT("ModelManager: Failed to load skybox %s\n"), StringToWString(fullPath).c_str());
        return false;
    }

    // Применить масштаб
    MV1SetScale(handle, VGet(scale, scale, scale));

    // Сохранить данные
    SkyboxData data;
    data.handle = handle;
    data.scale = scale;
    data.rotation = VGet(0, 0, 0);

    skyboxes[blockId] = data;

    printfDx(TEXT("ModelManager: Loaded skybox %s for block %d\n"), StringToWString(filename).c_str(), blockId);
    return true;
}

int ModelManager::GetModelHandle(ModelID id) const {
    auto it = models.find(id);
    if (it != models.end()) {
        return it->second.handle;
    }
    return -1;
}

const ModelData* ModelManager::GetModelData(ModelID id) const {
    auto it = models.find(id);
    if (it != models.end()) {
        return &it->second;
    }
    return nullptr;
}

int ModelManager::GetSkyboxHandle(int blockId) const {
    auto it = skyboxes.find(blockId);
    if (it != skyboxes.end()) {
        return it->second.handle;
    }
    return -1;
}

const SkyboxData* ModelManager::GetSkyboxData(int blockId) const {
    auto it = skyboxes.find(blockId);
    if (it != skyboxes.end()) {
        return &it->second;
    }
    return nullptr;
}

int ModelManager::DuplicateModel(ModelID id) {
    int originalHandle = GetModelHandle(id);
    if (originalHandle == -1) {
        return -1;
    }

    // Создать дубликат модели
    int duplicateHandle = MV1DuplicateModel(originalHandle);
    return duplicateHandle;
}

void ModelManager::DrawModel(ModelID id, VECTOR pos, VECTOR scale, VECTOR rotation) {
    const ModelData* data = GetModelData(id);
    if (!data || data->handle == -1) {
        return;
    }

    // Создать временный дубликат для отрисовки с трансформациями
    int tempHandle = MV1DuplicateModel(data->handle);
    if (tempHandle == -1) {
        return;
    }

    // Применить трансформации
    MV1SetPosition(tempHandle, pos);
    MV1SetScale(tempHandle, scale);
    MV1SetRotationXYZ(tempHandle, VAdd(rotation, data->rotationOffset));

    // Отрисовать
    MV1DrawModel(tempHandle);

    // Удалить временный дубликат
    MV1DeleteModel(tempHandle);
}

void ModelManager::DrawSkybox(int blockId, VECTOR cameraPos) {
    const SkyboxData* data = GetSkyboxData(blockId);
    if (!data || data->handle == -1) {
        return;
    }

    // Скайбокс всегда центрирован на камере
    MV1SetPosition(data->handle, cameraPos);
    MV1SetRotationXYZ(data->handle, data->rotation);

    // Отрисовать
    MV1DrawModel(data->handle);
}

bool ModelManager::IsModelLoaded(ModelID id) const {
    auto it = models.find(id);
    return (it != models.end() && it->second.handle != -1);
}

bool ModelManager::IsSkyboxLoaded(int blockId) const {
    auto it = skyboxes.find(blockId);
    return (it != skyboxes.end() && it->second.handle != -1);
}
