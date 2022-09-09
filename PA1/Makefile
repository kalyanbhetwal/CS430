CC=gcc
CFLAGS=-I.

BUILD_DIR := build
OBJ_DIR := build/src
SRCS := hellofunc.c hellomake.c
OBJS :=  $(OBJ_DIR)/hellomake.o $(OBJ_DIR)/hellofunc.o # output object files

.PHONY: program
	program: $(BUILD_DIR)/bin/program

$(OBJ_DIR)/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) -c $< -o $@

$(BUILD_DIR)/bin/program: $(OBJS)
	mkdir -p $(@D)
	$(CC) $^ -o $@

clean:
	rm -rf $(BUILD_DIR)/*

