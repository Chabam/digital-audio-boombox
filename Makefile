CC = clang

PORTAUDIO_LIBS = -lrt -lasound -ljack -lpthread -lportaudio -pthread
SNDFILE_LIBS = -lsndfile
STD_LIBS = -lstdc++ -lstdc++fs
NCURSES_LIBS = -lncurses
LIBS = $(PORTAUDIO_LIBS) $(SNDFILE_LIBS) $(STD_LIBS) $(NCURSES_LIBS)

FLAGS = -std=c++17 -g -Wall -Wextra -Werror $(LIBS) -o
BUILD_DIR = ./bin
SRC_DIR = ./src
INCLUDE = $(SRC_DIR)/main.cpp \
		  $(SRC_DIR)/Player.cpp \
		  $(SRC_DIR)/AudioFile.cpp \
		  $(SRC_DIR)/DAB.cpp
EXEC = dab

all:
	$(CC) $(INCLUDE) $(FLAGS) $(BUILD_DIR)/$(EXEC)

clean:
	rm $(BUILD_DIR)/*

install:
	cp $(BUILD_DIR)/$(EXEC) /bin