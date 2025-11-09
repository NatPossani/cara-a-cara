@echo off
echo ========================================
echo Compilador do Jogo Cara a Cara
echo ========================================
echo.

REM Tentar encontrar GCC
set GCC_PATH=
set GCC_FOUND=0

REM Caminhos comuns do MinGW
set PATHS[0]=C:\mingw64\bin\gcc.exe
set PATHS[1]=C:\mingw\bin\gcc.exe
set PATHS[2]=C:\msys64\mingw64\bin\gcc.exe
set PATHS[3]=C:\TDM-GCC-64\bin\gcc.exe
set PATHS[4]=gcc.exe

echo Procurando compilador GCC...
echo.

REM Verificar se gcc esta no PATH
where gcc >nul 2>&1
if %errorlevel% == 0 (
    echo [OK] GCC encontrado no PATH
    set GCC_FOUND=1
    set GCC_CMD=gcc
) else (
    echo [X] GCC nao encontrado no PATH
    echo.
    echo Tentando caminhos comuns...
    
    if exist "C:\mingw64\bin\gcc.exe" (
        echo [OK] GCC encontrado em: C:\mingw64\bin\gcc.exe
        set GCC_CMD=C:\mingw64\bin\gcc.exe
        set GCC_FOUND=1
    ) else if exist "C:\mingw\bin\gcc.exe" (
        echo [OK] GCC encontrado em: C:\mingw\bin\gcc.exe
        set GCC_CMD=C:\mingw\bin\gcc.exe
        set GCC_FOUND=1
    ) else if exist "C:\msys64\mingw64\bin\gcc.exe" (
        echo [OK] GCC encontrado em: C:\msys64\mingw64\bin\gcc.exe
        set GCC_CMD=C:\msys64\mingw64\bin\gcc.exe
        set GCC_FOUND=1
    ) else if exist "C:\TDM-GCC-64\bin\gcc.exe" (
        echo [OK] GCC encontrado em: C:\TDM-GCC-64\bin\gcc.exe
        set GCC_CMD=C:\TDM-GCC-64\bin\gcc.exe
        set GCC_FOUND=1
    )
)

echo.

if %GCC_FOUND% == 0 (
    echo ========================================
    echo ERRO: Compilador GCC nao encontrado!
    echo ========================================
    echo.
    echo Por favor, instale o MinGW:
    echo 1. Acesse: https://www.msys2.org/
    echo 2. Ou baixe de: https://winlibs.com/
    echo 3. Adicione o binario ao PATH do Windows
    echo.
    echo Veja o arquivo INSTALACAO.md para mais detalhes.
    echo.
    pause
    exit /b 1
)

echo ========================================
echo Compilando executaveis...
echo ========================================
echo.

if not exist build mkdir build

echo Compilando server.exe...
%GCC_CMD% -Wall -Wextra -std=c99 -Iinclude -o build\server.exe src\server.c -lws2_32
if %errorlevel% neq 0 (
    echo.
    echo ERRO ao compilar server.exe!
    pause
    exit /b 1
)
echo [OK] server.exe compilado com sucesso!

echo.
echo Compilando client.exe...
%GCC_CMD% -Wall -Wextra -std=c99 -Iinclude -o build\client.exe src\client.c -lws2_32
if %errorlevel% neq 0 (
    echo.
    echo ERRO ao compilar client.exe!
    pause
    exit /b 1
)
echo [OK] client.exe compilado com sucesso!

echo.
echo ========================================
echo Compilacao concluida com sucesso!
echo ========================================
echo.
echo Executaveis gerados em: build\
echo   - build\server.exe
echo   - build\client.exe
echo.
echo Para executar o jogo:
echo   1. Execute: build\server.exe
echo   2. Execute: build\client.exe (em outra janela)
echo   3. Execute: build\client.exe (em outra janela)
echo.
pause

