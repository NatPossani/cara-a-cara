#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <winsock2.h>
#include <windows.h>

#include "../include/game_data.h"

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 4096
#define PORT 51171

// Função para enviar mensagem ao cliente
int enviar_mensagem(SOCKET sock, const char* mensagem) {
    int result = send(sock, mensagem, strlen(mensagem), 0);
    if (result == SOCKET_ERROR) {
        printf("Erro ao enviar mensagem: %d\n", WSAGetLastError());
    }
    return result;
}

// Função para receber mensagem do cliente
int receber_mensagem(SOCKET sock, char* buffer, int tamanho) {
    int bytesReceived = recv(sock, buffer, tamanho - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
    }
    return bytesReceived;
}

// Função para enviar lista de personagens
void enviar_lista_personagens(SOCKET sock) {
    char mensagem[BUFFER_SIZE];
    strcpy(mensagem, "PERSONAGENS:");

    for (int i = 0; i < MAX_CHARS; i++) {
        char temp[512];
        int escrito = snprintf(
            temp,
            sizeof(temp),
            "%d:%s|%s|%s;",
            i,
            PERSONAGENS_DADOS[i].nome,
            PERSONAGENS_DADOS[i].emoji,
            PERSONAGENS_DADOS[i].resumo
        );

        if (escrito < 0) {
            continue;
        }

        size_t usado = strlen(mensagem);
        size_t restante = BUFFER_SIZE - usado;
        if ((size_t)escrito >= restante) {
            break;
        }

        strncat(mensagem, temp, restante - 1);
    }

    enviar_mensagem(sock, mensagem);
}

static int aguardar_escolha_personagem(SOCKET jogador) {
    char buffer[BUFFER_SIZE];
    while (1) {
        int bytesReceived = receber_mensagem(jogador, buffer, BUFFER_SIZE);
        if (bytesReceived <= 0) {
            return -1;
        }

        if (strncmp(buffer, "ESCOLHA:", 8) == 0) {
            errno = 0;
            char* fim = NULL;
            long indice = strtol(buffer + 8, &fim, 10);
            if (errno == 0 && fim != buffer + 8 && indice >= 0 && indice < MAX_CHARS) {
                return (int)indice;
            }
        }

        enviar_mensagem(jogador, "ESCOLHA_INVALIDA");
    }
}

static void enviar_personagem_secreto(SOCKET sock, int personagem) {
    if (personagem < 0 || personagem >= MAX_CHARS) {
        return;
    }

    char msg[512];
    snprintf(
        msg,
        sizeof(msg),
        "SEU_PERSONAGEM:%d:%s|%s|%s",
        personagem,
        PERSONAGENS_DADOS[personagem].nome,
        PERSONAGENS_DADOS[personagem].emoji,
        PERSONAGENS_DADOS[personagem].resumo
    );
    enviar_mensagem(sock, msg);
    Sleep(50);
}

int main() {
    WSADATA winsocketsDados;

    if (WSAStartup(MAKEWORD(2, 2), &winsocketsDados) != 0) {
        printf("WSAStartup falhou\n");
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        printf("Erro ao criar o socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Permitir reuso de endereço
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Erro ao associar o socket: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        printf("Erro ao colocar o socket em estado de escuta: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("========================================\n");
    printf("Servidor Cara a Cara iniciado!\n");
    printf("Aguardando conexoes na porta %d...\n", PORT);
    printf("========================================\n\n");

    // Aceitar primeiro cliente
    printf("Aguardando Jogador 1...\n");
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    SOCKET clientSocket1 = accept(sock, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket1 == INVALID_SOCKET) {
        printf("Erro ao aceitar a primeira conexao: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    printf("Jogador 1 conectado!\n\n");

    // Aceitar segundo cliente
    printf("Aguardando Jogador 2...\n");
    SOCKET clientSocket2 = accept(sock, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket2 == INVALID_SOCKET) {
        printf("Erro ao aceitar a segunda conexao: %d\n", WSAGetLastError());
        closesocket(clientSocket1);
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    printf("Jogador 2 conectado!\n");
    printf("Iniciando jogo...\n\n");

    int personagem1 = -1;
    int personagem2 = -1;

    // Enviar lista de personagens para ambos
    printf("Enviando lista de personagens...\n");
    enviar_lista_personagens(clientSocket1);
    Sleep(100); // Pequeno delay para garantir que a mensagem foi enviada
    enviar_lista_personagens(clientSocket2);
    Sleep(100);

    // Permitir que os jogadores escolham os personagens
    enviar_mensagem(clientSocket1, "ESCOLHA_PERSONAGEM");
    enviar_mensagem(clientSocket2, "AGUARDE_OPONENTE_ESCOLHER");
    personagem1 = aguardar_escolha_personagem(clientSocket1);
    if (personagem1 < 0) {
        printf("Conexao perdida durante a escolha do Jogador 1.\n");
        goto cleanup;
    }
    printf(
        "Jogador 1 escolheu: %s %s (indice %d)\n",
        PERSONAGENS_DADOS[personagem1].emoji,
        PERSONAGENS_DADOS[personagem1].nome,
        personagem1
    );
    enviar_personagem_secreto(clientSocket1, personagem1);
    Sleep(50);
    enviar_mensagem(clientSocket1, "AGUARDE_OPONENTE_ESCOLHER");

    enviar_mensagem(clientSocket2, "ESCOLHA_PERSONAGEM");
    personagem2 = aguardar_escolha_personagem(clientSocket2);
    if (personagem2 < 0) {
        printf("Conexao perdida durante a escolha do Jogador 2.\n");
        goto cleanup;
    }
    printf(
        "Jogador 2 escolheu: %s %s (indice %d)\n",
        PERSONAGENS_DADOS[personagem2].emoji,
        PERSONAGENS_DADOS[personagem2].nome,
        personagem2
    );
    enviar_personagem_secreto(clientSocket2, personagem2);
    Sleep(50);
    enviar_mensagem(clientSocket2, "AGUARDE_OPONENTE_ESCOLHER");

    // Enviar sinal de início do jogo
    printf("Iniciando jogo...\n");
    enviar_mensagem(clientSocket1, "JOGO_INICIADO");
    enviar_mensagem(clientSocket2, "JOGO_INICIADO");
    Sleep(200);

    // Variáveis do jogo
    int turno = 1; // 1 = jogador 1, 2 = jogador 2
    int jogo_ativo = 1;
    char buffer[BUFFER_SIZE];

    printf("\n=== JOGO INICIADO ===\n");
    printf("Turno do Jogador 1\n\n");

    // Loop principal do jogo
    while (jogo_ativo) {
        SOCKET jogador_atual = (turno == 1) ? clientSocket1 : clientSocket2;
        SOCKET jogador_oponente = (turno == 1) ? clientSocket2 : clientSocket1;
        int personagem_oponente = (turno == 1) ? personagem2 : personagem1;

        // Enviar mensagem de turno
        printf("\n--- Turno do Jogador %d ---\n", turno);
        char msg_turno[100];
        sprintf(msg_turno, "SEU_TURNO");
        if (enviar_mensagem(jogador_atual, msg_turno) == SOCKET_ERROR) {
            printf("Erro ao enviar mensagem para jogador %d\n", turno);
            jogo_ativo = 0;
            break;
        }
        
        char msg_espera[100];
        sprintf(msg_espera, "AGUARDE_TURNO");
        enviar_mensagem(jogador_oponente, msg_espera);

        // Receber ação do jogador
        int bytesReceived = receber_mensagem(jogador_atual, buffer, BUFFER_SIZE);
        if (bytesReceived <= 0) {
            printf("Erro ao receber dados do jogador %d ou conexao perdida\n", turno);
            jogo_ativo = 0;
            break;
        }
        
        printf("Jogador %d enviou: %s\n", turno, buffer);

        // Processar ação
        if (strncmp(buffer, "PERGUNTA:", 9) == 0) {
            // Jogador fez uma pergunta
            char pergunta[BUFFER_SIZE];
            strcpy(pergunta, buffer + 9);
            
            printf("Jogador %d perguntou: %s\n", turno, pergunta);
            
            // Enviar pergunta para o oponente
            char msg_pergunta[BUFFER_SIZE];
            sprintf(msg_pergunta, "PERGUNTA_RECEBIDA:%s", pergunta);
            enviar_mensagem(jogador_oponente, msg_pergunta);
            
            // Aguardar resposta do oponente
            bytesReceived = receber_mensagem(jogador_oponente, buffer, BUFFER_SIZE);
            if (bytesReceived > 0 && strncmp(buffer, "RESPOSTA:", 9) == 0) {
                int resposta = atoi(buffer + 9);
                printf("Oponente respondeu: %s\n", resposta ? "SIM" : "NAO");
                
                // Enviar resposta para o jogador que perguntou
                char msg_resposta[100];
                sprintf(msg_resposta, "RESPOSTA:%d", resposta);
                enviar_mensagem(jogador_atual, msg_resposta);
            } else if (bytesReceived <= 0) {
                printf("Erro ao receber resposta do oponente ou conexao perdida\n");
                jogo_ativo = 0;
            }
            
            // Alternar turno
            if (jogo_ativo) {
                turno = (turno == 1) ? 2 : 1;
            }
            
        } else if (strncmp(buffer, "CHUTE:", 6) == 0) {
            // Jogador fez um chute
            int chute = atoi(buffer + 6);
            if (chute < 0 || chute >= MAX_CHARS) {
                enviar_mensagem(jogador_atual, "CHUTE_INVALIDO");
                enviar_mensagem(jogador_oponente, "OPONENTE_CHUTE_INVALIDO");
                printf("Jogador %d enviou um chute invalido: %d\n", turno, chute);
                continue;
            }
            printf(
                "Jogador %d chutou: %d (personagem: %s %s)\n",
                turno,
                chute,
                PERSONAGENS_DADOS[chute].emoji,
                PERSONAGENS_DADOS[chute].nome
            );

            if (chute == personagem_oponente) {
                // Acertou!
                enviar_mensagem(jogador_atual, "VITORIA");
                enviar_mensagem(jogador_oponente, "DERROTA");
                printf("Jogador %d venceu!\n", turno);
                jogo_ativo = 0;
            } else {
                // Errou
                enviar_mensagem(jogador_atual, "CHUTE_ERRADO");
                enviar_mensagem(jogador_oponente, "OPONENTE_ERROU_CHUTE");
                printf("Jogador %d errou o chute\n", turno);
                // Alternar turno após chute errado
                turno = (turno == 1) ? 2 : 1;
            }
        }
    }

cleanup:
    Sleep(2000);

    if (clientSocket1 != INVALID_SOCKET) {
        closesocket(clientSocket1);
    }
    if (clientSocket2 != INVALID_SOCKET) {
        closesocket(clientSocket2);
    }
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
    }
    WSACleanup();
    
    printf("Jogo encerrado.\n");
    return 0;
}
