#pragma once
#include "DxLib.h"
#include <string>
#include <map>

// Model types for blocks
enum class ModelID {
    PLATFORM = 0,
    TRIGGER = 1,
    INVISIBLE_WALL = 2,
    FAKE_PLATFORM = 3,
    SPIKES = 4,
    DISAPPEARING = 5,
    MOVING = 6,
    BUTTON = 7,
    TELEPORT = 8,
    RETRACTABLE_SPIKES = 9,
    CRUMBLING = 10,
    FAKE_SPIKES = 11,
    GRAVITY_ZONE = 12,     // Block 3
    PENDULUM_BLADE = 13,   // Block 3
    PLAYER = 14,
    ICE_PLATFORM = 15,   // Block 5
    BOUNCE_PAD = 16,     // Block 5
    COUNT // Number of model types
};

// Holds a model and its parameters
struct ModelData {
    int handle;              // DxLib model handle
    float scale;             // Model scale
    VECTOR rotationOffset;   // Rotation offset (if needed)
    bool hasAnimation;       // Whether it has an animation
    int animIndex;           // Index of the attached animation

    ModelData() : handle(-1), scale(1.0f), rotationOffset(VGet(0, 0, 0)),
                  hasAnimation(false), animIndex(-1) {}
};

// Holds a skybox
struct SkyboxData {
    int handle;              // Skybox model handle
    float scale;             // Scale
    VECTOR rotation;         // Skybox rotation

    SkyboxData() : handle(-1), scale(1000.0f), rotation(VGet(0, 0, 0)) {}
};

// Model manager
class ModelManager {
private:
    // Model map: ModelID -> ModelData
    std::map<ModelID, ModelData> models;

    // Skybox map: level block ID -> SkyboxData
    std::map<int, SkyboxData> skyboxes;

    // Resource folder paths
    std::string modelsPath;
    std::string skyboxesPath;

    // Private constructor for the singleton
    ModelManager();

    // Disallow copying
    ModelManager(const ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;

public:
    // Get the singleton instance
    static ModelManager& GetInstance();

    // Destructor
    ~ModelManager();

    // Initialize - loads all models
    bool Initialize();

    // Release all resources
    void Cleanup();

    // Load a specific model
    bool LoadModel(ModelID id, const std::string& filename, float scale = 1.0f);

    // Load the skybox for a level block
    bool LoadSkybox(int blockId, const std::string& filename, float scale = 1000.0f);

    // Get a model's handle
    int GetModelHandle(ModelID id) const;

    // Get a model's data
    const ModelData* GetModelData(ModelID id) const;

    // Get a block's skybox handle
    int GetSkyboxHandle(int blockId) const;

    // Get a block's skybox data
    const SkyboxData* GetSkyboxData(int blockId) const;

    // Duplicate a model (to create multiple instances)
    int DuplicateModel(ModelID id);

    // Draw a model at the given position
    void DrawModel(ModelID id, VECTOR pos, VECTOR scale, VECTOR rotation = VGet(0, 0, 0));

    // Draw the skybox
    void DrawSkybox(int blockId, VECTOR cameraPos);

    // Check whether a model is loaded
    bool IsModelLoaded(ModelID id) const;

    // Check whether a skybox is loaded
    bool IsSkyboxLoaded(int blockId) const;

    // Unload a model (to load a different one in its place)
    void UnloadModel(ModelID id);

    // Set the models folder path
    void SetModelsPath(const std::string& path) { modelsPath = path; }
    void SetSkyboxesPath(const std::string& path) { skyboxesPath = path; }
};
