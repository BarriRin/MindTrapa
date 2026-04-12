// ModelConverter - утилита для конвертации 3D моделей в MV1 формат
// Использование: ModelConverter.exe input.fbx output.mv1
//            или: ModelConverter.exe input.fbx (выход будет input.mv1)

#include "DxLib.h"
#include <string>
#include <iostream>

// Получить расширение файла
std::wstring GetExtension(const std::wstring& path) {
    size_t pos = path.rfind(L'.');
    if (pos != std::wstring::npos) {
        return path.substr(pos);
    }
    return L"";
}

// Заменить расширение на .mv1
std::wstring ReplaceExtension(const std::wstring& path) {
    size_t pos = path.rfind(L'.');
    if (pos != std::wstring::npos) {
        return path.substr(0, pos) + L".mv1";
    }
    return path + L".mv1";
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Получаем аргументы командной строки
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    if (argc < 2) {
        MessageBox(NULL,
            TEXT("ModelConverter - конвертер моделей в MV1\n\n")
            TEXT("Использование:\n")
            TEXT("  ModelConverter.exe input.fbx [output.mv1]\n\n")
            TEXT("Поддерживаемые форматы: .fbx, .x, .pmd, .pmx, .mqo"),
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

    // Настройка DxLib для работы без окна
    SetOutApplicationLogValidFlag(FALSE);  // Отключить лог
    SetAlwaysRunFlag(TRUE);
    ChangeWindowMode(TRUE);
    SetGraphMode(64, 64, 32);  // Минимальное окно
    SetWindowStyleMode(2);  // Без рамки
    SetMainWindowText(TEXT("Converting..."));

    // Скрыть окно
    SetWindowVisibleFlag(FALSE);

    if (DxLib_Init() == -1) {
        MessageBox(NULL, TEXT("Ошибка инициализации DxLib"), TEXT("Error"), MB_OK | MB_ICONERROR);
        return 1;
    }

    // Загружаем модель
    int handle = MV1LoadModel(inputPath.c_str());

    if (handle == -1) {
        DxLib_End();
        std::wstring msg = L"Не удалось загрузить модель:\n" + inputPath;
        MessageBoxW(NULL, msg.c_str(), L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // Сохраняем в MV1
    int result = MV1SaveModelToMV1File(handle, outputPath.c_str(), MV1_SAVETYPE_NORMAL, -1, TRUE);

    // Удаляем модель из памяти
    MV1DeleteModel(handle);

    DxLib_End();

    if (result == 0) {
        std::wstring msg = L"Конвертация успешна!\n\nВход: " + inputPath + L"\nВыход: " + outputPath;
        MessageBoxW(NULL, msg.c_str(), L"ModelConverter", MB_OK | MB_ICONINFORMATION);
        return 0;
    } else {
        std::wstring msg = L"Ошибка сохранения модели:\n" + outputPath;
        MessageBoxW(NULL, msg.c_str(), L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }
}
