# Guia de Instalação e Compilação

## Problema: Executáveis não encontrados

Os executáveis `.exe` precisam ser compilados a partir do código fonte. Para isso, você precisa ter o **MinGW** (compilador GCC para Windows) instalado.

## Opção 1: Instalar MinGW via MSYS2 (Recomendado)

1. **Baixar MSYS2:**
   - Acesse: https://www.msys2.org/
   - Baixe e instale o instalador

2. **Instalar MinGW:**
   - Abra o terminal MSYS2
   - Execute:
     ```bash
     pacman -S mingw-w64-x86_64-gcc
     ```

3. **Adicionar ao PATH:**
   - Adicione `C:\msys64\mingw64\bin` ao PATH do Windows
   - Ou use o terminal MSYS2 para compilar

## Opção 2: Instalar MinGW-w64 diretamente

1. **Baixar MinGW-w64:**
   - Acesse: https://www.mingw-w64.org/downloads/
   - Ou use: https://winlibs.com/ (versão pré-compilada)

2. **Extrair e adicionar ao PATH:**
   - Extraia em `C:\mingw64` (ou outro local)
   - Adicione `C:\mingw64\bin` ao PATH do Windows

3. **Verificar instalação:**
   ```bash
   gcc --version
   ```

## Opção 3: Usar Chocolatey (se instalado)

```bash
choco install mingw
```

## Compilação

Após instalar o MinGW, execute no terminal:

```bash
# Opção 1: Usar o script batch
build.bat

# Opção 2: Compilar manualmente
gcc -Wall -Wextra -std=c99 -Iinclude -o build\server.exe src\server.c -lws2_32
gcc -Wall -Wextra -std=c99 -Iinclude -o build\client.exe src\client.c -lws2_32

# Opção 3: Usar Make (se disponível)
make all
```

## Verificação

Após a compilação, você deve ter:
- `build\server.exe`
- `build\client.exe`

## Executar o jogo

1. **Terminal 1 - Servidor:**
   ```bash
   build\server.exe
   ```

2. **Terminal 2 - Cliente 1:**
   ```bash
   build\client.exe
   ```

3. **Terminal 3 - Cliente 2:**
   ```bash
   build\client.exe
   ```

## Solução Rápida: Baixar MinGW Portable

Se você não quiser instalar, pode usar uma versão portable:

1. Baixe MinGW-w64 portable de: https://winlibs.com/
2. Extraia em uma pasta
3. Use o terminal dessa pasta para compilar:
   ```bash
   gcc -Wall -Wextra -std=c99 -Iinclude -o ..\build\server.exe ..\src\server.c -lws2_32
   gcc -Wall -Wextra -std=c99 -Iinclude -o ..\build\client.exe ..\src\client.c -lws2_32
   ```

