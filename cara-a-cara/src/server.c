#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_CHARS 10
#define BUFFER_SIZE 1024
#define PORT 51171

// Lista de personagens (baseada nas imagens disponíveis)
const char* personagens[MAX_CHARS] = {
    "2a11dc0774d583dc400f17158a244a36",
    "48cffb00285969f09d8b7037a16f6069",
    "5ce6a7fde6d11060e833d3cbcaa9448f",
    "64d0962733f113dc16bb9429715fbe65",
    "acb51114bc454d50fa726a2c2582c13d",
    "bd809f77937fd06963775c22dd0cb8d7",
    "d30ed3c904a5f1628b269ce6965e2a33",
    "eadffbeafb9357aa015d25e397dcdb08",
    "fa2d378b01e96a946bad90b448ddc807",
    "gerard_way"
};

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
        char temp[100];
        sprintf(temp, "%d:%s;", i, personagens[i]);
        if (strlen(mensagem) + strlen(temp) < BUFFER_SIZE - 1) {
            strcat(mensagem, temp);
        }
    }
    enviar_mensagem(sock, mensagem);
}

int main() {
    WSADATA winsocketsDados;
    srand((unsigned int)time(NULL));

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

    // Sortear personagens secretos
    int personagem1 = rand() % MAX_CHARS;
    int personagem2 = rand() % MAX_CHARS;
    
    // Garantir que sejam diferentes
    while (personagem2 == personagem1) {
        personagem2 = rand() % MAX_CHARS;
    }

    printf("Personagem do Jogador 1: %s (indice %d)\n", personagens[personagem1], personagem1);
    printf("Personagem do Jogador 2: %s (indice %d)\n", personagens[personagem2], personagem2);

    // Enviar lista de personagens para ambos
    printf("Enviando lista de personagens...\n");
    enviar_lista_personagens(clientSocket1);
    Sleep(100); // Pequeno delay para garantir que a mensagem foi enviada
    enviar_lista_personagens(clientSocket2);
    Sleep(100);

    // Enviar personagem secreto para cada jogador
    printf("Enviando personagens secretos...\n");
    char msg_secreto1[100];
    sprintf(msg_secreto1, "SEU_PERSONAGEM:%d:%s", personagem1, personagens[personagem1]);
    enviar_mensagem(clientSocket1, msg_secreto1);
    Sleep(100);

    char msg_secreto2[100];
    sprintf(msg_secreto2, "SEU_PERSONAGEM:%d:%s", personagem2, personagens[personagem2]);
    enviar_mensagem(clientSocket2, msg_secreto2);
    Sleep(100);

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
        int personagem_atual = (turno == 1) ? personagem1 : personagem2;
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
        if (strncmp(buffer, "PERGUNTA:", 10) == 0) {
            // Jogador fez uma pergunta
            char pergunta[BUFFER_SIZE];
            strcpy(pergunta, buffer + 10);
            
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
            printf("Jogador %d chutou: %d (personagem: %s)\n", turno, chute, personagens[chute]);
            
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

    // Aguardar um pouco antes de fechar
    Sleep(2000);

    // Fechar conexões
    closesocket(clientSocket1);
    closesocket(clientSocket2);
    closesocket(sock);
    WSACleanup();
    
    printf("Jogo encerrado.\n");
    return 0;
}
