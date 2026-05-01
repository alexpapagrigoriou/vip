TARGET    := vip
CC        := gcc
SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := build
BIN_DIR   := bin

SRCS         := $(wildcard $(SRC_DIR)/*.c)
DEBUG_OBJS   := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/debug/%.o, $(SRCS))
RELEASE_OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/release/%.o, $(SRCS))

CFLAGS_COMMON  := -Wall -Wextra -Wpedantic -Werror -MMD -MP -I$(INC_DIR)
CFLAGS_DEBUG   := $(CFLAGS_COMMON) -g3 -O0
CFLAGS_RELEASE := $(CFLAGS_COMMON) -O2

LDFLAGS_DEBUG   :=
LDFLAGS_RELEASE :=

.DEFAULT_GOAL := debug

debug: $(BIN_DIR)/debug/$(TARGET)

$(BIN_DIR)/debug/$(TARGET): $(DEBUG_OBJS) | $(BIN_DIR)/debug
	$(CC) $(LDFLAGS_DEBUG) $^ -o $@

$(BUILD_DIR)/debug/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)/debug
	$(CC) $(CFLAGS_DEBUG) -c $< -o $@

release: $(BIN_DIR)/release/$(TARGET)

$(BIN_DIR)/release/$(TARGET): $(RELEASE_OBJS) | $(BIN_DIR)/release
	$(CC) $(LDFLAGS_RELEASE) $^ -o $@
	@strip $@

$(BUILD_DIR)/release/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)/release
	$(CC) $(CFLAGS_RELEASE) -c $< -o $@

$(BUILD_DIR)/debug $(BUILD_DIR)/release $(BIN_DIR)/debug $(BIN_DIR)/release:
	@mkdir -p $@

DEPS := $(DEBUG_OBJS:.o=.d) $(RELEASE_OBJS:.o=.d)
-include $(DEPS)

clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

compile-db:
	@bear -- make -B debug

.PHONY: debug release clean compile-db
