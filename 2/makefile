CC=gcc
CFLAGS=-g
OUT_DIR=bin

all: build run

build: blocks.c
	@$(CC) $(CFLAGS) blocks.c -o $(OUT_DIR)/blocks

$(OUT_DIR)/blocks: build
	
run: $(OUT_DIR)/blocks
	@./$(OUT_DIR)/blocks < sample-input.txt

clean:
	@rm -rf $(OUT_DIR)/*
