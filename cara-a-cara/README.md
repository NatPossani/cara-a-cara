# Jogo Cara a Cara - Cliente-Servidor em C

Jogo de adivinhação "Cara a Cara" implementado em C usando sockets TCP, onde dois jogadores competem para descobrir o personagem secreto do oponente.

## Estrutura do Projeto

```
cara-a-cara/
├── src/
│   ├── server.c      # Servidor do jogo
│   └── client.c      # Cliente do jogo
├── include/
│   └── graphics.h    # Biblioteca gráfica para exibição de imagens
├── images/           # 10 imagens de personagens (.jpg)
├── build/            # Diretório para executáveis compilados
└── Makefile          # Arquivo de build
```

## Requisitos

- MinGW (Windows) com GCC
- Windows 10 ou superior
- 10 imagens de personagens na pasta `images/`

## ⚠️ IMPORTANTE: Compilação Necessária

**Os executáveis `.exe` ainda não foram gerados!** Você precisa compilar o código fonte primeiro.

### Pré-requisito: Instalar MinGW

O MinGW (compilador GCC para Windows) precisa estar instalado. Veja o arquivo **`INSTALACAO.md`** para instruções detalhadas.

**Opção rápida:**
1. Baixe MinGW-w64 de: https://winlibs.com/
2. Extraia e adicione ao PATH do Windows
3. Ou use MSYS2: https://www.msys2.org/

### Compilação

Após instalar o MinGW, execute:

```bash
# Opção 1: Usar script automático (recomendado)
compilar.bat

# Opção 2: Compilar manualmente
gcc -Wall -Wextra -std=c99 -Iinclude -o build/server.exe src/server.c -lws2_32
gcc -Wall -Wextra -std=c99 -Iinclude -o build/client.exe src/client.c -lws2_32

# Opção 3: Usar Makefile
make all
```

**Após a compilação, os executáveis estarão em:**
- `build/server.exe`
- `build/client.exe`

## Como Jogar

1. **Iniciar o servidor:**
   ```bash
   build/server.exe
   ```
   O servidor aguardará a conexão de dois clientes na porta 51171.

2. **Iniciar o primeiro cliente:**
   ```bash
   build/client.exe
   ```
   O cliente se conectará ao servidor em `127.0.0.1:51171`.

3. **Iniciar o segundo cliente:**
   ```bash
   build/client.exe
   ```
   Abra uma segunda janela de terminal e execute o cliente novamente.

4. **Jogar:**
   - Cada jogador receberá uma lista de 10 personagens
   - Cada jogador receberá seu personagem secreto (com imagem exibida)
   - Os jogadores alternam turnos
   - Durante seu turno, o jogador pode:
     - **[0] Fazer uma pergunta** - Digite uma pergunta de sim/não
     - **[1] Chutar o personagem** - Digite o número do personagem que deseja chutar
   - O primeiro jogador a acertar o personagem do oponente vence!

## Funcionalidades

### Servidor
- ✅ Aceita conexões de dois clientes
- ✅ Envia lista de 10 personagens para cada cliente
- ✅ Sorteia personagens secretos para cada jogador
- ✅ Gerencia turnos alternados
- ✅ Processa perguntas e respostas entre jogadores
- ✅ Processa chutes e verifica vitória/derrota
- ✅ Encerra o jogo corretamente quando há um vencedor

### Cliente
- ✅ Exibe lista de personagens disponíveis
- ✅ Exibe o personagem secreto do jogador (com imagem)
- ✅ Interface de texto para interação
- ✅ Opção de fazer perguntas
- ✅ Opção de chutar personagem
- ✅ Exibe perguntas e respostas durante o jogo
- ✅ Notifica vitória/derrota

## Personagens

Os personagens são definidos pelos nomes dos arquivos de imagem na pasta `images/`:
1. 2a11dc0774d583dc400f17158a244a36
2. 48cffb00285969f09d8b7037a16f6069
3. 5ce6a7fde6d11060e833d3cbcaa9448f
4. 64d0962733f113dc16bb9429715fbe65
5. acb51114bc454d50fa726a2c2582d13d
6. bd809f77937fd06963775c22dd0cb8d7
7. d30ed3c904a5f1628b269ce6965e2a33
8. eadffbeafb9357aa015d25e397dcdb08
9. fa2d378b01e96a946bad90b448ddc807
10. gerard_way

## Protocolo de Comunicação

### Mensagens do Servidor para o Cliente:
- `PERSONAGENS:0:nome1;1:nome2;...` - Lista de personagens
- `SEU_PERSONAGEM:id:nome` - Personagem secreto do jogador
- `SEU_TURNO` - É a vez do jogador
- `AGUARDE_TURNO` - Aguarde seu turno
- `PERGUNTA_RECEBIDA:pergunta` - Pergunta recebida do oponente
- `RESPOSTA:0` ou `RESPOSTA:1` - Resposta (0=Não, 1=Sim)
- `VITORIA` - Você venceu!
- `DERROTA` - Você perdeu!
- `CHUTE_ERRADO` - Seu chute estava errado
- `OPONENTE_ERROU_CHUTE` - O oponente errou o chute

### Mensagens do Cliente para o Servidor:
- `PERGUNTA:texto` - Enviar pergunta
- `RESPOSTA:0` ou `RESPOSTA:1` - Resposta à pergunta (0=Não, 1=Sim)
- `CHUTE:id` - Chutar personagem por ID

## Notas

- As imagens são abertas no visualizador padrão do Windows quando o personagem secreto é revelado
- O servidor deve ser iniciado antes dos clientes
- Ambos os clientes devem estar conectados para o jogo iniciar
- A porta padrão é 51171 (definida em `PORT`)

## Troubleshooting

- **Erro de compilação:** Certifique-se de que o MinGW está instalado e no PATH
- **Erro de conexão:** Verifique se o servidor está rodando e se a porta 51171 está disponível
- **Imagens não abrem:** Verifique se os arquivos de imagem estão na pasta `images/` com os nomes corretos

