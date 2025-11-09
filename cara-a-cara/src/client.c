#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/graphics.h"

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024
#define PORT 51171
#define MAX_CHARS 10

// Estrutura para armazenar informações do jogo
typedef struct {
    char personagens[MAX_CHARS][100];
    int meu_personagem;
    char meu_personagem_nome[100];
    int num_personagens;
} Jogo;

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

// Função para processar lista de personagens
void processar_lista_personagens(Jogo* jogo, const char* mensagem) {
    printf("\n=== PERSONAGENS DISPONIVEIS ===\n");
    
    // Extrair lista de personagens da mensagem
    // Formato: PERSONAGENS:0:nome1;1:nome2;...
    const char* inicio = strstr(mensagem, "PERSONAGENS:");
    if (inicio) {
        inicio += 12; // Pular "PERSONAGENS:"
        char* lista = strdup(inicio);
        char* token = strtok(lista, ";");
        int indice = 0;
        
        while (token != NULL && indice < MAX_CHARS) {
            char* dois_pontos = strchr(token, ':');
            if (dois_pontos) {
                *dois_pontos = '\0';
                int id = atoi(token);
                strcpy(jogo->personagens[id], dois_pontos + 1);
                printf("[%d] %s\n", id, jogo->personagens[id]);
                indice++;
            }
            token = strtok(NULL, ";");
        }
        jogo->num_personagens = indice;
        free(lista);
    }
    printf("===============================\n\n");
}

// Função para processar personagem secreto
void processar_personagem_secreto(Jogo* jogo, const char* mensagem) {
    // Formato: SEU_PERSONAGEM:id:nome
    const char* inicio = strstr(mensagem, "SEU_PERSONAGEM:");
    if (inicio) {
        inicio += 15; // Pular "SEU_PERSONAGEM:"
        char* dois_pontos = strchr(inicio, ':');
        if (dois_pontos) {
            *dois_pontos = '\0';
            jogo->meu_personagem = atoi(inicio);
            strcpy(jogo->meu_personagem_nome, dois_pontos + 1);
            
            printf("\n=== SEU PERSONAGEM SECRETO ===\n");
            printf("Personagem: %s (Indice: %d)\n", jogo->meu_personagem_nome, jogo->meu_personagem);
            
            // Construir caminho da imagem
            char caminho_imagem[256];
            sprintf(caminho_imagem, "images/%s.jpg", jogo->meu_personagem_nome);
            
            // Exibir imagem
            readimagefile(caminho_imagem, 100, 100, 400, 400);
            printf("==============================\n\n");
        }
    }
}

// Função para exibir menu de ações
void exibir_menu() {
    printf("\n=== SUA VEZ! ===\n");
    printf("[0] Fazer uma pergunta\n");
    printf("[1] Chutar o personagem do oponente\n");
    printf("Escolha uma opcao: ");
}

int main() {
    WSADATA winsocketsDados;
    Jogo jogo;
    memset(&jogo, 0, sizeof(Jogo));
    
    // Inicializar gráficos
    int gdriver = 0, gmode = 0;
    initgraph(&gdriver, &gmode, "");
    
    if (WSAStartup(MAKEWORD(2, 2), &winsocketsDados) != 0) {
        printf("Falha ao inicializar o Winsock\n");
        closegraph();
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
        else if (strcmp(buffer, "JOGO_INICIADO") == 0) {
            printf("\n=== JOGO INICIADO! ===\n");
            printf("Aguardando seu turno...\n\n");
        }
        else if (strcmp(buffer, "SEU_TURNO") == 0) {
            // É minha vez de jogar
            exibir_menu();
            
            int opcao;
            scanf("%d", &opcao);
            getchar(); // Limpar buffer
            
            if (opcao == 0) {
                // Fazer pergunta
                printf("Digite sua pergunta: ");
                char pergunta[512];
                fgets(pergunta, sizeof(pergunta), stdin);
                pergunta[strcspn(pergunta, "\n")] = 0;
                
                char msg[BUFFER_SIZE];
                sprintf(msg, "PERGUNTA:%s", pergunta);
                enviar_mensagem(clientSocket, msg);
                
                // Aguardar resposta do oponente
                bytesReceived = receber_mensagem(clientSocket, buffer, BUFFER_SIZE);
                if (bytesReceived > 0) {
                    if (strncmp(buffer, "RESPOSTA:", 9) == 0) {
                        int resposta = atoi(buffer + 9);
                        printf("\nResposta do oponente: %s\n", resposta ? "SIM" : "NAO");
                    }
                }
            }
            else if (opcao == 1) {
                // Chutar personagem
                printf("\nDigite o numero do personagem que voce quer chutar (0-%d): ", jogo.num_personagens - 1);
                int chute;
                scanf("%d", &chute);
                getchar(); // Limpar buffer
                
                if (chute >= 0 && chute < jogo.num_personagens) {
                    char msg[100];
                    sprintf(msg, "CHUTE:%d", chute);
                    enviar_mensagem(clientSocket, msg);
                    
                    // Aguardar resultado
                    bytesReceived = receber_mensagem(clientSocket, buffer, BUFFER_SIZE);
                    if (bytesReceived > 0) {
                        if (strcmp(buffer, "VITORIA") == 0) {
                            printf("\n*** PARABENS! VOCE VENCEU! ***\n");
                            jogo_ativo = 0;
                        }
                        else if (strcmp(buffer, "CHUTE_ERRADO") == 0) {
                            printf("\nChute errado! Continue tentando...\n");
                        }
                    }
                } else {
                    printf("Numero invalido!\n");
                }
            }
        }
        else if (strcmp(buffer, "AGUARDE_TURNO") == 0) {
            printf("\nAguarde seu turno...\n");
        }
        else if (strncmp(buffer, "PERGUNTA_RECEBIDA:", 18) == 0) {
            // Oponente fez uma pergunta
            char* pergunta = buffer + 18;
            printf("\n=== PERGUNTA DO OPONENTE ===\n");
            printf("%s\n", pergunta);
            printf("Digite sua resposta (1 para SIM, 0 para NAO): ");
            
            int resposta;
            scanf("%d", &resposta);
            getchar(); // Limpar buffer
            
            char msg[100];
            sprintf(msg, "RESPOSTA:%d", resposta);
            enviar_mensagem(clientSocket, msg);
            printf("Resposta enviada!\n");
        }
        else if (strcmp(buffer, "DERROTA") == 0) {
            printf("\n*** VOCE PERDEU! ***\n");
            printf("Seu oponente acertou seu personagem!\n");
            jogo_ativo = 0;
        }
        else if (strcmp(buffer, "OPONENTE_ERROU_CHUTE") == 0) {
            printf("\nSeu oponente errou o chute! Continue tentando...\n");
        }
        else if (strncmp(buffer, "RESPOSTA:", 9) == 0) {
            // Resposta a uma pergunta anterior
            int resposta = atoi(buffer + 9);
            printf("Resposta do oponente: %s\n", resposta ? "SIM" : "NAO");
        }
    }

    printf("\nJogo encerrado. Pressione Enter para sair...");
    getchar();

    closesocket(clientSocket);
    WSACleanup();
    closegraph();
    return 0;
}
