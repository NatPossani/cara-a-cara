@echo off
echo ========================================
echo Testar se o GCC esta funcionando
echo ========================================
echo.

REM Tentar encontrar GCC
where gcc >nul 2>&1
if %errorlevel% == 0 (
    echo [OK] GCC encontrado no PATH!
    echo.
    echo Testando versao do GCC:
    gcc --version
    echo.
    echo ========================================
    echo ✅ GCC esta funcionando corretamente!
    echo ========================================
    echo.
    echo Agora voce pode executar: compilar.bat
    echo.
) else (
    echo [X] GCC nao encontrado no PATH
    echo.
    echo Verificando caminhos comuns...
    echo.
    
    if exist "C:\msys64\mingw64\bin\gcc.exe" (
        echo [OK] GCC encontrado em: C:\msys64\mingw64\bin\gcc.exe
        echo.
        echo Mas nao esta no PATH. Execute: adicionar-ao-path.ps1
        echo.
        C:\msys64\mingw64\bin\gcc.exe --version
    ) else if exist "C:\mingw64\bin\gcc.exe" (
        echo [OK] GCC encontrado em: C:\mingw64\bin\gcc.exe
        echo.
        echo Mas nao esta no PATH. Adicione ao PATH manualmente.
        echo.
        C:\mingw64\bin\gcc.exe --version
    ) else (
        echo [X] GCC nao encontrado em nenhum local comum
        echo.
        echo Por favor:
        echo 1. Instale o MinGW (veja INSTALACAO.md)
        echo 2. Adicione ao PATH (veja ADICIONAR_AO_PATH.md)
        echo.
    )
)

echo.
pause

