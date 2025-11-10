#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "../include/game_data.h"

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 4096
#define PORT 51171
// Estruturas para armazenar informações do tabuleiro
typedef struct {
    char nome[64];
    char emoji[32];
    int eliminado;
} Carta;

typedef struct {
    Carta cartas[MAX_CHARS];
    int num_personagens;
    int meu_personagem;
    char segredo_nome[64];
    char segredo_emoji[32];
} Jogo;

void mostrar_tabuleiro(const Jogo* jogo);
void exibir_menu(void);
void atualizar_status_personagem(Jogo* jogo);
void solicitar_escolha_personagem(Jogo* jogo, SOCKET sock, int exibir_tabuleiro);

static unsigned int utf8_avancar(const unsigned char** p) {
    unsigned int code = 0;
    unsigned char c = **p;

    if (c < 0x80) {
        code = c;
        (*p)++;
        return code;
    }

    if ((c & 0xE0) == 0xC0 && (*p)[1]) {
        code = ((unsigned int)(c & 0x1F) << 6) | (unsigned int)((*p)[1] & 0x3F);
        *p += 2;
        return code;
    }

    if ((c & 0xF0) == 0xE0 && (*p)[1] && (*p)[2]) {
        code = ((unsigned int)(c & 0x0F) << 12) |
               ((unsigned int)((*p)[1] & 0x3F) << 6) |
               (unsigned int)((*p)[2] & 0x3F);
        *p += 3;
        return code;
    }

    if ((c & 0xF8) == 0xF0 && (*p)[1] && (*p)[2] && (*p)[3]) {
        code = ((unsigned int)(c & 0x07) << 18) |
               ((unsigned int)((*p)[1] & 0x3F) << 12) |
               ((unsigned int)((*p)[2] & 0x3F) << 6) |
               (unsigned int)((*p)[3] & 0x3F);
        *p += 4;
        return code;
    }

    (*p)++;
    return c;
}

static int largura_codepoint(unsigned int code) {
    if (code >= 0x1100 &&
        (code <= 0x115F ||
         code == 0x2329 || code == 0x232A ||
         (code >= 0x2E80 && code <= 0xA4CF && code != 0x303F) ||
         (code >= 0xAC00 && code <= 0xD7A3) ||
         (code >= 0xFE10 && code <= 0xFE19) ||
         (code >= 0xFE30 && code <= 0xFE6F) ||
         (code >= 0xFF00 && code <= 0xFF60) ||
         (code >= 0xFFE0 && code <= 0xFFE6) ||
         (code >= 0x1F300 && code <= 0x1F64F) ||
         (code >= 0x1F900 && code <= 0x1F9FF))) {
        return 2;
    }
    return 1;
}

static int calcular_largura_exibicao(const char* texto) {
    int largura = 0;
    const unsigned char* p = (const unsigned char*)texto;
    while (*p) {
        unsigned int code = utf8_avancar(&p);
        largura += largura_codepoint(code);
    }
    return largura;
}

// Função para receber mensagem do servidor
int receber_mensagem(SOCKET sock, char* buffer, int tamanho) {
    int bytesReceived = recv(sock, buffer, tamanho - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
    } else if (bytesReceived == 0) {
        return 0; // Conexão fechada
    }
    return bytesReceived;
}

// Função para enviar mensagem ao servidor
int enviar_mensagem(SOCKET sock, const char* mensagem) {
    return send(sock, mensagem, strlen(mensagem), 0);
}

// Função para processar lista de personagens enviada pelo servidor
void processar_lista_personagens(Jogo* jogo, const char* mensagem) {
    printf("\n=== PERSONAGENS DISPONIVEIS ===\n");

    const char* inicio = strstr(mensagem, "PERSONAGENS:");
    if (!inicio) {
        printf("Nao foi possivel interpretar a lista de personagens.\n");
        return;
    }

    inicio += 12; // pular "PERSONAGENS:"
    char* lista = strdup(inicio);
    if (!lista) {
        printf("Falha ao alocar memoria para a lista de personagens.\n");
        return;
    }

    memset(jogo->cartas, 0, sizeof(jogo->cartas));
    jogo->num_personagens = 0;

    char* token = strtok(lista, ";");
    while (token != NULL) {
        char* separador_id = strchr(token, ':');
        if (separador_id) {
            *separador_id = '\0';
            errno = 0;
            char* fim_id = NULL;
            long id_long = strtol(token, &fim_id, 10);
            if (errno == 0 && fim_id != token && id_long >= 0 && id_long < MAX_CHARS) {
                int id = (int)id_long;
                char* dados = separador_id + 1;

                char* separador_nome_emoji = strchr(dados, '|');
                if (separador_nome_emoji) {
                    *separador_nome_emoji = '\0';
                }

                strncpy(jogo->cartas[id].nome, dados, sizeof(jogo->cartas[id].nome) - 1);
                jogo->cartas[id].nome[sizeof(jogo->cartas[id].nome) - 1] = '\0';

                const char* emoji = separador_nome_emoji ? separador_nome_emoji + 1 : "";
                strncpy(jogo->cartas[id].emoji, emoji, sizeof(jogo->cartas[id].emoji) - 1);
                jogo->cartas[id].emoji[sizeof(jogo->cartas[id].emoji) - 1] = '\0';

                jogo->cartas[id].eliminado = 0;

                if (id + 1 > jogo->num_personagens) {
                    jogo->num_personagens = id + 1;
                }
            }
        }
        token = strtok(NULL, ";");
    }

    free(lista);

    if (jogo->num_personagens == 0) {
        printf("Nenhum personagem carregado.\n");
    }

    printf("===============================\n");
    mostrar_tabuleiro(jogo);
}

// Função para processar personagem secreto
void processar_personagem_secreto(Jogo* jogo, const char* mensagem) {
    const char* inicio = strstr(mensagem, "SEU_PERSONAGEM:");
    if (!inicio) {
        return;
    }

    inicio += 15; // pular "SEU_PERSONAGEM:"
    char conteudo[BUFFER_SIZE];
    strncpy(conteudo, inicio, sizeof(conteudo) - 1);
    conteudo[sizeof(conteudo) - 1] = '\0';

    char* separador_id = strchr(conteudo, ':');
    if (!separador_id) {
        return;
    }

    *separador_id = '\0';
    errno = 0;
    char* fim_id = NULL;
    long id_long = strtol(conteudo, &fim_id, 10);
    if (errno != 0 || fim_id == conteudo || id_long < 0 || id_long >= MAX_CHARS) {
        return;
    }
    int id = (int)id_long;

    char* dados = separador_id + 1;
    char* separador_nome_emoji = strchr(dados, '|');

    if (separador_nome_emoji) {
        *separador_nome_emoji = '\0';
    }

    const char* nome = dados;
    const char* emoji = separador_nome_emoji ? separador_nome_emoji + 1 : "";

    jogo->meu_personagem = id;
    strncpy(jogo->segredo_nome, nome, sizeof(jogo->segredo_nome) - 1);
    jogo->segredo_nome[sizeof(jogo->segredo_nome) - 1] = '\0';
    strncpy(jogo->segredo_emoji, emoji, sizeof(jogo->segredo_emoji) - 1);
    jogo->segredo_emoji[sizeof(jogo->segredo_emoji) - 1] = '\0';

    if (id >= 0 && id < jogo->num_personagens) {
        // Atualiza carta correspondente com dados completos
        strncpy(jogo->cartas[id].nome, nome, sizeof(jogo->cartas[id].nome) - 1);
        jogo->cartas[id].nome[sizeof(jogo->cartas[id].nome) - 1] = '\0';

        strncpy(jogo->cartas[id].emoji, emoji, sizeof(jogo->cartas[id].emoji) - 1);
        jogo->cartas[id].emoji[sizeof(jogo->cartas[id].emoji) - 1] = '\0';
    }

    printf("\n=== SEU PERSONAGEM SECRETO ===\n");
    printf("%s %s (indice %d)\n", jogo->segredo_emoji, jogo->segredo_nome, jogo->meu_personagem);
    printf("==============================\n");
}

// Função para exibir menu de ações
void exibir_menu() {
    printf("\n=== SUA VEZ! ===\n");
    printf("[0] Fazer uma pergunta\n");
    printf("[1] Chutar o personagem do oponente\n");
    printf("[2] Marcar/Desmarcar personagem como eliminado\n");
    printf("Escolha uma opcao: ");
}

void mostrar_tabuleiro(const Jogo* jogo) {
    if (jogo->num_personagens == 0) {
        printf("\nTabuleiro ainda nao carregado.\n");
        return;
    }

    printf("\n=== TABULEIRO DE PERSONAGENS ===\n");

    char linhas[MAX_CHARS][128];
    int max_len = 0;
    int larguras_display[MAX_CHARS];

    for (int i = 0; i < jogo->num_personagens; ++i) {
        const Carta* carta = &jogo->cartas[i];
        char status = carta->eliminado ? 'X' : ' ';
        const char* marcador = (jogo->meu_personagem == i) ? " (Voce)" : "";

        snprintf(linhas[i], sizeof(linhas[i]), "%2d)[%c] %s %s%s",
                 i,
                 status,
                 carta->emoji,
                 carta->nome,
                 marcador);

        int display_len = calcular_largura_exibicao(linhas[i]);
        larguras_display[i] = display_len;
        if (display_len > max_len) {
            max_len = display_len;
        }
    }

    const int colunas = 3;
    const int largura_terminal = 120;
    int largura_coluna = largura_terminal / colunas;
    if (largura_coluna < max_len + 4) {
        largura_coluna = max_len + 4;
    }

    int linhas_impressas = (jogo->num_personagens + colunas - 1) / colunas;

#ifdef _WIN32
    fflush(stdout);
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    BOOL podeReposicionar = console != INVALID_HANDLE_VALUE &&
                            GetConsoleScreenBufferInfo(console, &info);

    if (podeReposicionar) {
        int largura_disponivel = info.dwSize.X - info.dwCursorPosition.X;
        if (largura_disponivel <= 0) {
            largura_disponivel = info.dwSize.X;
        }
        int largura_coluna_win = largura_disponivel / colunas;
        if (largura_coluna_win < max_len + 4) {
            largura_coluna_win = max_len + 4;
        }
        if (largura_coluna_win < 1) {
            largura_coluna_win = 1;
        }

        SHORT baseX = info.dwCursorPosition.X;
        SHORT baseY = info.dwCursorPosition.Y;

        for (int linha = 0; linha < linhas_impressas; ++linha) {
            for (int coluna = 0; coluna < colunas; ++coluna) {
                int idx = linha * colunas + coluna;
                if (idx >= jogo->num_personagens) {
                    break;
                }

                COORD pos = {
                    (SHORT)(baseX + coluna * largura_coluna_win),
                    (SHORT)(baseY + linha)
                };
                if (pos.X >= info.dwSize.X) {
                    pos.X = (SHORT)(info.dwSize.X - 1);
                }
                SetConsoleCursorPosition(console, pos);

                char campo[256];
                size_t bytes = strlen(linhas[idx]);
                if (bytes >= sizeof(campo) - 1) {
                    bytes = sizeof(campo) - 2;
                }
                memcpy(campo, linhas[idx], bytes);

                int padding = largura_coluna_win - larguras_display[idx];
                if (padding < 1) {
                    padding = 1;
                }
                if ((int)(bytes + padding) >= (int)sizeof(campo) - 1) {
                    padding = (int)sizeof(campo) - 1 - (int)bytes;
                    if (padding < 1) {
                        padding = 1;
                    }
                }
                memset(campo + bytes, ' ', padding);
                campo[bytes + padding] = '\0';

                DWORD escritos = 0;
                WriteConsoleA(console,
                              campo,
                              (DWORD)strlen(campo),
                              &escritos,
                              NULL);
            }
        }

        COORD fim = { baseX, (SHORT)(baseY + linhas_impressas) };
        SetConsoleCursorPosition(console, fim);
        printf("\nLegenda: [ ] ativo | [X] eliminado\n");
        fflush(stdout);
        return;
    }
#endif

    for (int inicio_linha = 0; inicio_linha < jogo->num_personagens; inicio_linha += colunas) {
        for (int coluna = 0; coluna < colunas; ++coluna) {
            int idx = inicio_linha + coluna;
            if (idx >= jogo->num_personagens) {
                break;
            }

            printf("%s", linhas[idx]);

            int padding = largura_coluna - larguras_display[idx];
            if (padding < 2) {
                padding = 2;
            }
            for (int espaco = 0; espaco < padding; ++espaco) {
                putchar(' ');
            }
        }
        printf("\n");
    }

    printf("Legenda: [ ] ativo | [X] eliminado\n");
}

void atualizar_status_personagem(Jogo* jogo) {
    if (jogo->num_personagens == 0) {
        printf("\nO tabuleiro ainda nao foi carregado.\n");
        return;
    }

    printf("\nDigite o numero do personagem para alternar o status: ");
    char entrada[32];
    if (!fgets(entrada, sizeof(entrada), stdin)) {
        return;
    }

    entrada[strcspn(entrada, "\n")] = '\0';
    if (strlen(entrada) == 0) {
        printf("Entrada vazia. Nenhuma alteracao realizada.\n");
        return;
    }

    errno = 0;
    char* fim = NULL;
    long indice = strtol(entrada, &fim, 10);
    if (errno != 0 || fim == entrada || indice < 0 || indice >= jogo->num_personagens) {
        printf("Indice invalido. Use um numero entre 0 e %d.\n", jogo->num_personagens - 1);
        return;
    }

    int idx = (int)indice;
    jogo->cartas[idx].eliminado = !jogo->cartas[idx].eliminado;

    if (idx == jogo->meu_personagem) {
        printf("Aviso: voce acabou de %s seu proprio personagem!\n",
               jogo->cartas[idx].eliminado ? "eliminar" : "reativar");
    }

    printf("%s %s agora esta marcado como %s.\n",
           jogo->cartas[idx].emoji,
           jogo->cartas[idx].nome,
           jogo->cartas[idx].eliminado ? "ELIMINADO" : "ATIVO");
}

void solicitar_escolha_personagem(Jogo* jogo, SOCKET sock, int exibir_tabuleiro) {
    if (jogo->num_personagens == 0) {
        printf("\nO tabuleiro ainda nao foi carregado. Aguarde a lista de personagens.\n");
        return;
    }

    if (exibir_tabuleiro) {
        mostrar_tabuleiro(jogo);
    }

    while (1) {
        printf("\nEscolha o indice do seu personagem secreto (0-%d): ", jogo->num_personagens - 1);
        char entrada[32];
        if (!fgets(entrada, sizeof(entrada), stdin)) {
            printf("Entrada encerrada. Nenhuma escolha enviada.\n");
            return;
        }

        entrada[strcspn(entrada, "\n")] = '\0';
        if (strlen(entrada) == 0) {
            printf("Entrada vazia. Informe um numero valido.\n");
            continue;
        }

        errno = 0;
        char* fim = NULL;
        long indice_long = strtol(entrada, &fim, 10);
        if (errno != 0 || fim == entrada || indice_long < 0 || indice_long >= jogo->num_personagens) {
            printf("Indice invalido. Use um numero entre 0 e %d.\n", jogo->num_personagens - 1);
            continue;
        }

        int indice = (int)indice_long;
        char msg[64];
        snprintf(msg, sizeof(msg), "ESCOLHA:%d", indice);

        if (enviar_mensagem(sock, msg) == SOCKET_ERROR) {
            printf("Falha ao enviar a escolha ao servidor.\n");
        } else {
            printf("Escolha enviada. Aguarde confirmacao do servidor.\n");
        }
        break;
    }
}

int main() {
    WSADATA winsocketsDados;
    Jogo jogo;
    memset(&jogo, 0, sizeof(Jogo));
    jogo.meu_personagem = -1;

    system("chcp 65001 > nul 2>&1");

    if (WSAStartup(MAKEWORD(2, 2), &winsocketsDados) != 0) {
        printf("Falha ao inicializar o Winsock\n");
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        printf("Erro ao criar o socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(PORT);

    printf("Conectando ao servidor...\n");
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Erro ao conectar ao servidor: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    printf("Conectado ao servidor!\n");

    char buffer[BUFFER_SIZE];
    int jogo_ativo = 1;

    // Loop principal do jogo
    while (jogo_ativo) {
        int bytesReceived = receber_mensagem(clientSocket, buffer, BUFFER_SIZE);
        if (bytesReceived <= 0) {
            printf("Conexao perdida ou servidor encerrou\n");
            break;
        }

        // Processar mensagens do servidor
        if (strncmp(buffer, "PERSONAGENS:", 12) == 0) {
            processar_lista_personagens(&jogo, buffer);
        }
        else if (strncmp(buffer, "SEU_PERSONAGEM:", 15) == 0) {
            processar_personagem_secreto(&jogo, buffer);
        }
        else if (strcmp(buffer, "ESCOLHA_PERSONAGEM") == 0) {
            solicitar_escolha_personagem(&jogo, clientSocket, 1);
        }
        else if (strcmp(buffer, "ESCOLHA_INVALIDA") == 0) {
            printf("\nEscolha rejeitada pelo servidor. Tente novamente.\n");
            solicitar_escolha_personagem(&jogo, clientSocket, 0);
        }
        else if (strcmp(buffer, "AGUARDE_OPONENTE_ESCOLHER") == 0) {
            printf("\nAguarde o oponente escolher o personagem.\n");
        }
        else if (strcmp(buffer, "JOGO_INICIADO") == 0) {
            printf("\n=== JOGO INICIADO! ===\n");
            printf("Aguardando seu turno...\n\n");
        }
        else if (strcmp(buffer, "SEU_TURNO") == 0) {
            int turno_concluido = 0;
            while (jogo_ativo && !turno_concluido) {
                mostrar_tabuleiro(&jogo);
                exibir_menu();

                char entrada[32];
                if (!fgets(entrada, sizeof(entrada), stdin)) {
                    printf("Entrada encerrada. Finalizando jogo.\n");
                    jogo_ativo = 0;
                    break;
                }

                entrada[strcspn(entrada, "\n")] = '\0';
                if (strlen(entrada) == 0) {
                    continue;
                }

                errno = 0;
                char* fim = NULL;
                long opcao_long = strtol(entrada, &fim, 10);
                if (errno != 0 || fim == entrada) {
                    printf("Opcao invalida. Tente novamente.\n");
                    continue;
                }

                int opcao = (int)opcao_long;
                switch (opcao) {
                    case 0: {
                        char pergunta[512];
                        printf("Digite sua pergunta: ");
                        if (!fgets(pergunta, sizeof(pergunta), stdin)) {
                            printf("Erro de leitura. Encerrando jogo.\n");
                            jogo_ativo = 0;
                            turno_concluido = 1;
                            break;
                        }
                        pergunta[strcspn(pergunta, "\n")] = '\0';
                        if (strlen(pergunta) == 0) {
                            printf("Pergunta vazia. Tente novamente.\n");
                            break;
                        }

                        char msg[BUFFER_SIZE];
                        snprintf(msg, sizeof(msg), "PERGUNTA:%s", pergunta);
                        enviar_mensagem(clientSocket, msg);

                        int resposta_bytes = receber_mensagem(clientSocket, buffer, BUFFER_SIZE);
                        if (resposta_bytes <= 0) {
                            printf("Conexao perdida ao aguardar resposta.\n");
                            jogo_ativo = 0;
                            turno_concluido = 1;
                            break;
                        }

                        if (strncmp(buffer, "RESPOSTA:", 9) == 0) {
                            int resposta = atoi(buffer + 9);
                            printf("\nResposta do oponente: %s\n", resposta ? "SIM" : "NAO");
                        } else {
                            printf("Resposta inesperada do servidor: %s\n", buffer);
                        }
                        turno_concluido = 1;
                        break;
                    }
                    case 1: {
                        if (jogo.num_personagens == 0) {
                            printf("Tabuleiro ainda nao carregado. Aguarde a lista de personagens.\n");
                            break;
                        }

                        printf("\nDigite o numero do personagem que voce quer chutar (0-%d): ", jogo.num_personagens - 1);
                        char chuteEntrada[32];
                        if (!fgets(chuteEntrada, sizeof(chuteEntrada), stdin)) {
                            printf("Erro de leitura. Encerrando jogo.\n");
                            jogo_ativo = 0;
                            turno_concluido = 1;
                            break;
                        }
                        chuteEntrada[strcspn(chuteEntrada, "\n")] = '\0';
                        if (strlen(chuteEntrada) == 0) {
                            printf("Entrada vazia. Tente novamente.\n");
                            break;
                        }

                        errno = 0;
                        char* fimChute = NULL;
                        long chute_long = strtol(chuteEntrada, &fimChute, 10);
                        if (errno != 0 || fimChute == chuteEntrada) {
                            printf("Valor invalido. Informe um numero valido.\n");
                            break;
                        }

                        int chute = (int)chute_long;
                        if (chute < 0 || chute >= jogo.num_personagens) {
                            printf("Indice fora do intervalo valido (0-%d).\n", jogo.num_personagens - 1);
                            break;
                        }

                        char msg[64];
                        snprintf(msg, sizeof(msg), "CHUTE:%d", chute);
                        enviar_mensagem(clientSocket, msg);

                        int resposta_bytes = receber_mensagem(clientSocket, buffer, BUFFER_SIZE);
                        if (resposta_bytes <= 0) {
                            printf("Conexao perdida ao aguardar o resultado do chute.\n");
                            jogo_ativo = 0;
                            turno_concluido = 1;
                            break;
                        }

                        if (strcmp(buffer, "VITORIA") == 0) {
                            printf("\n*** PARABENS! VOCE VENCEU! ***\n");
                            jogo_ativo = 0;
                            turno_concluido = 1;
                        } else if (strcmp(buffer, "CHUTE_ERRADO") == 0) {
                            printf("\nChute errado! O turno passara para o oponente.\n");
                            turno_concluido = 1;
                        } else if (strcmp(buffer, "CHUTE_INVALIDO") == 0) {
                            printf("\nChute invalido. Verifique o numero informado e tente novamente no proximo aviso de turno.\n");
                            turno_concluido = 1;
                        } else {
                            printf("Resposta inesperada do servidor: %s\n", buffer);
                            turno_concluido = 1;
                        }
                        break;
                    }
                    case 2:
                        atualizar_status_personagem(&jogo);
                        break;
                    default:
                        printf("Opcao invalida. Escolha entre 0 e 2.\n");
                        break;
                }
            }
        }
        else if (strcmp(buffer, "AGUARDE_TURNO") == 0) {
            printf("\nAguarde seu turno...\n");
        }
        else if (strncmp(buffer, "PERGUNTA_RECEBIDA:", 18) == 0) {
            char* pergunta = buffer + 18;
            printf("\n=== PERGUNTA DO OPONENTE ===\n");
            printf("%s\n", pergunta);
            mostrar_tabuleiro(&jogo);

            int resposta_valida = 0;
            int resposta_valor = 0;
            while (jogo_ativo && !resposta_valida) {
                printf("Digite sua resposta (1 para SIM, 0 para NAO): ");
                char respostaEntrada[16];
                if (!fgets(respostaEntrada, sizeof(respostaEntrada), stdin)) {
                    printf("Erro de leitura. Encerrando jogo.\n");
                    jogo_ativo = 0;
                    break;
                }

                respostaEntrada[strcspn(respostaEntrada, "\n")] = '\0';
                if (strlen(respostaEntrada) == 0) {
                    continue;
                }

                errno = 0;
                char* fimResposta = NULL;
                long valor = strtol(respostaEntrada, &fimResposta, 10);
                if (errno != 0 || fimResposta == respostaEntrada || (valor != 0 && valor != 1)) {
                    printf("Entrada invalida. Responda apenas com 1 (SIM) ou 0 (NAO).\n");
                    continue;
                }

                resposta_valor = (int)valor;
                resposta_valida = 1;
            }

            if (!jogo_ativo) {
                break;
            }

            char msg[100];
            snprintf(msg, sizeof(msg), "RESPOSTA:%d", resposta_valor);
            enviar_mensagem(clientSocket, msg);
            printf("Resposta enviada!\n");
        }
        else if (strcmp(buffer, "DERROTA") == 0) {
            printf("\n*** VOCE PERDEU! ***\n");
            printf("Seu oponente acertou seu personagem!\n");
            jogo_ativo = 0;
        }
        else if (strcmp(buffer, "CHUTE_INVALIDO") == 0) {
            printf("\nSeu ultimo chute foi considerado invalido. O servidor enviara seu turno novamente.\n");
        }
        else if (strcmp(buffer, "OPONENTE_CHUTE_INVALIDO") == 0) {
            printf("\nO oponente tentou um chute invalido. Eles receberao outra chance.\n");
        }
        else if (strcmp(buffer, "OPONENTE_ERROU_CHUTE") == 0) {
            printf("\nSeu oponente errou o chute! Continue tentando...\n");
        }
        else if (strncmp(buffer, "RESPOSTA:", 9) == 0) {
            int resposta = atoi(buffer + 9);
            printf("Resposta do oponente: %s\n", resposta ? "SIM" : "NAO");
        }
    }

    printf("\nJogo encerrado. Pressione Enter para sair...");
    getchar();

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
