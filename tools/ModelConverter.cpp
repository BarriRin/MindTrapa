// ModelConverter - utility for converting 3D models to the MV1 format
// Usage: ModelConverter.exe input.fbx output.mv1
//    or: ModelConverter.exe input.fbx (output will be input.mv1)

#include "DxLib.h"
#include <string>
#include <iostream>

// Get a file's extension
std::wstring GetExtension(const std::wstring& path) {
    size_t pos = path.rfind(L'.');
    if (pos != std::wstring::npos) {
        return path.substr(pos);
    }
    return L"";
}

// Replace the extension with .mv1
std::wstring ReplaceExtension(const std::wstring& path) {
    size_t pos = path.rfind(L'.');
    if (pos != std::wstring::npos) {
        return path.substr(0, pos) + L".mv1";
    }
    return path + L".mv1";
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Get the command line arguments
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    if (argc < 2) {
        MessageBox(NULL,
            TEXT("ModelConverter - model to MV1 converter\n\n")
            TEXT("Usage:\n")
            TEXT("  ModelConverter.exe input.fbx [output.mv1]\n\n")
            TEXT("Supported formats: .fbx, .x, .pmd, .pmx, .mqo"),
            TEXT("ModelConverter"), MB_OK | MB_ICONINFORMATION);
        LocalFree(argv);
        return 1;
    }

    std::wstring inputPath = argv[1];
    std::wstring outputPath;

    if (argc >= 3) {
        outputPath = argv[2];
    } else {
        outputPath = ReplaceExtension(inputPath);
    }

    LocalFree(argv);

    // Configure DxLib to run without a window
    SetOutApplicationLogValidFlag(FALSE);  // Disable logging
    SetAlwaysRunFlag(TRUE);
    ChangeWindowMode(TRUE);
    SetGraphMode(64, 64, 32);  // Minimal window
    SetWindowStyleMode(2);  // No border
    SetMainWindowText(TEXT("Converting..."));

    // Hide the window
    SetWindowVisibleFlag(FALSE);

    if (DxLib_Init() == -1) {
        MessageBox(NULL, TEXT("DxLib initialization failed"), TEXT("Error"), MB_OK | MB_ICONERROR);
        return 1;
    }

    // Load the model
    int handle = MV1LoadModel(inputPath.c_str());

    if (handle == -1) {
        DxLib_End();
        std::wstring msg = L"Failed to load model:\n" + inputPath;
        MessageBoxW(NULL, msg.c_str(), L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Save as MV1
    int result = MV1SaveModelToMV1File(handle, outputPath.c_str(), MV1_SAVETYPE_NORMAL, -1, TRUE);

    // Free the model from memory
    MV1DeleteModel(handle);

    DxLib_End();

    if (result == 0) {
        std::wstring msg = L"Conversion successful!\n\nInput: " + inputPath + L"\nOutput: " + outputPath;
        MessageBoxW(NULL, msg.c_str(), L"ModelConverter", MB_OK | MB_ICONINFORMATION);
        return 0;
    } else {
        std::wstring msg = L"Failed to save model:\n" + outputPath;
        MessageBoxW(NULL, msg.c_str(), L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }
}
