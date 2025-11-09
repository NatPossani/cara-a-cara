#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Função para exibir imagem em uma janela separada
void readimagefile(const char* filename, int x1, int y1, int x2, int y2) {
    printf("\n========================================\n");
    printf("[IMAGEM] Carregando: %s\n", filename);
    printf("========================================\n\n");
    
    // Converter para caminho absoluto
    char fullPath[MAX_PATH];
    if (GetFullPathNameA(filename, MAX_PATH, fullPath, NULL) == 0) {
        strcpy(fullPath, filename);
    }
    
    // Verificar se o arquivo existe
    DWORD attrs = GetFileAttributesA(fullPath);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        printf("[ERRO] Arquivo nao encontrado: %s\n", fullPath);
        printf("[INFO] Verifique se o arquivo esta na pasta images/\n");
        return;
    }
    
    // Abrir a imagem no visualizador padrão do Windows
    // Isso funciona com JPEG, PNG, BMP e outros formatos suportados
    HINSTANCE result = ShellExecuteA(
        NULL,           // hwnd
        "open",         // operação
        fullPath,       // arquivo
        NULL,           // parâmetros
        NULL,           // diretório
        SW_SHOWNORMAL   // mostrar normalmente
    );
    
    if ((INT_PTR)result > 32) {
        printf("[OK] Imagem aberta no visualizador padrao\n");
        printf("[INFO] A janela da imagem foi aberta. Feche-a quando terminar de visualizar.\n\n");
        
        // Aguardar um pouco para dar tempo da janela abrir
        Sleep(500);
    } else {
        printf("[ERRO] Nao foi possivel abrir a imagem\n");
        printf("[INFO] Codigo de erro: %ld\n", (long)result);
    }
}

// Função para inicializar gráficos (compatibilidade - não necessário)
void initgraph(int* gdriver, int* gmode, const char* path) {
    // Não necessário para esta implementação
    (void)gdriver;
    (void)gmode;
    (void)path;
}

// Função para fechar gráficos (compatibilidade - não necessário)
void closegraph(void) {
    // Não necessário para esta implementação
}

#endif // GRAPHICS_H
