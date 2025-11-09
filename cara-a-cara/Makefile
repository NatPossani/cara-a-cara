CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LDFLAGS = -lws2_32
SRC_DIR = src
BUILD_DIR = build

# Arquivos fonte
SERVER_SRC = $(SRC_DIR)/server.c
CLIENT_SRC = $(SRC_DIR)/client.c

# Executáveis
SERVER_EXE = $(BUILD_DIR)/server.exe
CLIENT_EXE = $(BUILD_DIR)/client.exe

.PHONY: all clean server client

all: server client

server: $(SERVER_EXE)

client: $(CLIENT_EXE)

$(SERVER_EXE): $(SERVER_SRC)
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(CLIENT_EXE): $(CLIENT_SRC) include/graphics.h
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)

