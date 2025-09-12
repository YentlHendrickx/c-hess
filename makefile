CC := clang
CFLAGS := -Wall -Wextra -Werror -Wpedantic -std=c11 -g $(shell sdl2-config --cflags)
LDFLAGS := $(shell sdl2-config --libs) -lSDL2_image

# Source files
SRC_FILES := src/engine.c src/game.c src/piece.c src/main.c src/renderer.c src/input.c src/resources.c

# Target
TARGET := chess

all: $(TARGET)

$(TARGET): $(SRC_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

# Debug build
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)

# Release build
release: CFLAGS += -O2 -DNDEBUG
release: $(TARGET)

.PHONY: all clean debug release
