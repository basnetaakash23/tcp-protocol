CC := cc
CPPFLAGS := -Iinclude
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic

BUILD_DIR := build
BIN_DIR := bin
SERVER := $(BIN_DIR)/echoserv
CLIENT := $(BIN_DIR)/echoclnt
CONVERTER_OBJ := $(BUILD_DIR)/converter.o

.PHONY: all clean

all: $(SERVER) $(CLIENT)

$(SERVER): $(BUILD_DIR)/server.o $(CONVERTER_OBJ) | $(BIN_DIR)
	$(CC) $^ -o $@

$(CLIENT): $(BUILD_DIR)/client.o | $(BIN_DIR)
	$(CC) $^ -o $@

$(BUILD_DIR)/server.o: src/server.c include/converter.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/client.o: src/client.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(CONVERTER_OBJ): src/converter.c include/converter.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
