CC := clang
CFLAGS := -Wall -Wextra -Werror -Wpedantic -std=c11 -g $(shell sdl2-config --cflags)
LDFLAGS := $(shell sdl2-config --libs)

SRC_FILES := src/engine.c src/main.c
TARGET := out/chess

all: $(TARGET)

$(TARGET): $(SRC_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
