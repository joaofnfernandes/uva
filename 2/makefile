CC=gcc
CFLAGS=-g -ansi
OUT_DIR=bin

all: build run

build: main.c
	@$(CC) $(CFLAGS) main.c -o $(OUT_DIR)/main

$(OUT_DIR)/main: build
	
run: $(OUT_DIR)/main
	@./$(OUT_DIR)/main

clean:
	@rm -rf $(OUT_DIR)/*
