# Variables
# ---------

SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := include

CC := gcc
CFLAGS := -I$(INCLUDE_DIR) -Wall -MMD -MP -Wextra -Wpedantic -Wconversion -Wshadow

# Optional inputs
# ---------------

MODE ?= dev
ifeq ($(filter $(MODE), dev release),)
$(error Error: MODE must either be 'dev' or 'release')
endif

VERBOSE ?= 0
ifeq ($(filter $(VERBOSE), 0 1),)
$(error Error: VERBOSE must either be '0' or '1')
endif

# Further variables
# -----------------

CLEAN_DIR := $(BUILD_DIR)

ifeq ($(MODE), dev)
BUILD_DIR := $(BUILD_DIR)/dev
CFLAGS += -g -O0 -DDEBUG
else ifeq ($(MODE), release)
BUILD_DIR := $(BUILD_DIR)/release
CFLAGS += -O3 -DNDEBUG
VERBOSE := 1
endif

ifeq ($(VERBOSE), 0)
Q := @
else ifeq ($(VERBOSE), 1)
Q :=
endif

BINARY := $(BUILD_DIR)/program

SRCS := $(shell find $(SRC_DIR) -name "*.c")
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/gen/%.o, $(SRCS))
DEPS := $(patsubst %.o, %.d, $(OBJS))

# Rules
# -----

.PHONY: all
all: $(BINARY)
ifeq ($(MODE), dev)
	@./$(BINARY)
endif

$(BINARY): $(OBJS)
	$(Q)$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/gen/%.o: $(SRC_DIR)/%.c
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

.PHONY: clean
clean:
	rm -rf $(CLEAN_DIR)
