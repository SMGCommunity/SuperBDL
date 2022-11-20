CC ?= $(CROSS_COMPILE)gcc
CFLAGS ?= -Wall -Wno-unknown-pragmas -O2 -flto

SOURCE_DIR = src
INCLUDE_DIR = include

C_FILES = $(wildcard $(SOURCE_DIR)/*.c)

TARGET = superbdl

all: $(TARGET)

clean:
	rm -rf $(TARGET)

$(TARGET): $(C_FILES)
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) `pkg-config --cflags --libs json-c assimp` -o $@ $(C_FILES)
