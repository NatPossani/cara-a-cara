@echo off
echo Compilando servidor e cliente...

if not exist build mkdir build

echo.
echo Compilando server.exe...
gcc -Wall -Wextra -std=c99 -Iinclude -o build\server.exe src\server.c -lws2_32
if %errorlevel% neq 0 (
    echo Erro ao compilar server.exe
    pause
    exit /b 1
)

echo.
echo Compilando client.exe...
gcc -Wall -Wextra -std=c99 -Iinclude -o build\client.exe src\client.c -lws2_32
if %errorlevel% neq 0 (
    echo Erro ao compilar client.exe
    pause
    exit /b 1
)

echo.
echo Compilacao concluida com sucesso!
echo Executaveis gerados em: build\
echo.
pause

