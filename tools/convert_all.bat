@echo off
echo ========================================
echo   Model Converter - Batch Processing
echo ========================================
echo.

set CONVERTER="%~dp0bin\ModelConverter.exe"
set INPUT_DIR="%~dp0input"
set OUTPUT_DIR="%~dp0..\models"

if not exist %CONVERTER% (
    echo ERROR: ModelConverter.exe not found!
    echo Please build the converter first.
    pause
    exit /b 1
)

if not exist %INPUT_DIR% (
    mkdir %INPUT_DIR%
    echo Created input folder: %INPUT_DIR%
    echo Put your .fbx, .x, .pmd, .pmx files there and run again.
    pause
    exit /b 0
)

if not exist %OUTPUT_DIR% (
    mkdir %OUTPUT_DIR%
)

echo Converting models from: %INPUT_DIR%
echo Output to: %OUTPUT_DIR%
echo.

set COUNT=0

for %%f in (%INPUT_DIR%\*.fbx %INPUT_DIR%\*.x %INPUT_DIR%\*.pmd %INPUT_DIR%\*.pmx %INPUT_DIR%\*.mqo) do (
    echo Converting: %%~nxf
    %CONVERTER% "%%f" "%OUTPUT_DIR%\%%~nf.mv1"
    set /a COUNT+=1
)

echo.
echo ========================================
echo   Done! Converted %COUNT% model(s)
echo ========================================
pause
