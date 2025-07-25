# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -Wextra -Iinclude
LDFLAGS := -lncurses
BUILD   := build
SRC     := $(wildcard src/*.c)
OBJ     := $(patsubst src/%.c, $(BUILD)/%.o, $(SRC))
OUT     := sysmon
VERSION := $(shell cat VERSION 2>/dev/null || echo "dev")

.PHONY: all run debug release clean help install uninstall

# Default target
all: $(OUT)

# Build executable from object files
$(OUT): $(OBJ)
	@echo "Linking $(OUT)..."
	$(CC) $(OBJ) -o $(OUT) $(LDFLAGS)

# Compile each .c file into .o in build/
$(BUILD)/%.o: src/%.c | $(BUILD)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD):
	@mkdir -p $(BUILD)

# Run the program
run: all
	./$(OUT)

# Debug build (adds -g)
debug: CFLAGS += -g
debug: clean all

# Release build (adds -O2)
release: CFLAGS += -O2 -DNDEBUG
release: clean all
	@strip $(OUT)
	@mkdir -p dist
	@cp $(OUT) dist/$(OUT)-$(VERSION)
	@echo "Release binary created at dist/$(OUT)-$(VERSION)"

# Clean all generated files
clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILD) $(OUT) dist/

# Install to /usr/local/bin
install: all
	@echo "Installing $(OUT) to /usr/local/bin..."
	@sudo cp $(OUT) /usr/local/bin/
	@sudo chmod +x /usr/local/bin/$(OUT)

# Uninstall binary
uninstall:
	@echo "Removing /usr/local/bin/$(OUT)..."
	@sudo rm -f /usr/local/bin/$(OUT)

# Print help
help:
	@echo "Ubuntu System Monitor Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all        - Build the project (default)"
	@echo "  debug      - Build with debug symbols"
	@echo "  release    - Build optimized version"
	@echo "  run        - Build and run"
	@echo "  install    - Copy binary to /usr/local/bin"
	@echo "  uninstall  - Remove binary from /usr/local/bin"
	@echo "  clean      - Remove build and dist files"
	@echo "  help       - Show this help message"
