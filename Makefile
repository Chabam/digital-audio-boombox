CC = clang++

FLAGS = -std=c++14 -g -Wall -lrt -lm -lasound -ljack -pthread -lsndfile -o

BUILD_DIR = ./bin
SRC_DIR = ./src
MAIN = $(SRC_DIR)/main.cpp
EXEC = dab

all:
	$(CC) $(MAIN) $(FLAGS) $(BUILD_DIR)/$(EXEC)
