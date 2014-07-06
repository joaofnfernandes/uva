CC=gcc
CFLAGS=-g
OUT_DIR=bin

all: build run

build: main.c
	@$(CC) $(CFLAGS) main.c -o $(OUT_DIR)/main

$(OUT_DIR)/main: build
	
run: $(OUT_DIR)/main
	@./$(OUT_DIR)/main < sample-input.txt

clean:
	@rm -rf $(OUT_DIR)/*
