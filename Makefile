PHONY: all clean

SRC_DIR = src
BUILD_DIR = build

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

END_FILE_NAME = main.exe
END_FILE = $(BUILD_DIR)/$(END_FILE_NAME)

COMPILER = gcc

SDL_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

all:
	mkdir -p $(BUILD_DIR)
	$(COMPILER) $(SRC_FILES) -o $(END_FILE) $(SDL_FLAGS)
	./$(END_FILE)

clean:
	rm -rf $(BUILD_DIR)
	clear