# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -Wextra -Iinclude
LDFLAGS := -lncurses
BUILD   := build
SRC     := $(wildcard src/*.c)
OBJ     := $(patsubst src/%.c, $(BUILD)/%.o, $(SRC))
OUT     := ubuntu-system-monitor
VERSION := $(shell cat VERSION 2>/dev/null || echo "dev")

.PHONY: all run debug release clean help install uninstall

# Default target
all: $(OUT)

# Build executable from object files
$(OUT): $(OBJ)
	@echo "Linking $(OUT)..."
	$(CC) $(OBJ) -o $(OUT) $(LDFLAGS)

# Compile .c files to .o in build/
$(BUILD)/%.o: src/%.c | $(BUILD)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if not exists
$(BUILD):
	@mkdir -p $(BUILD)

# Run the program
run: all
	./$(OUT)

# Debug build
debug: CFLAGS += -g
debug: clean all

# Optimized release build
release: CFLAGS += -O2 -DNDEBUG
release: clean all
	@strip $(OUT)
	@mkdir -p dist
	@cp $(OUT) dist/$(OUT)-$(VERSION)
	@echo "Release binary created at dist/$(OUT)-$(VERSION)"

# Clean generated files
clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILD) $(OUT) dist/

# Install to system (supports DESTDIR for packaging)
install: all
	@echo "Installing to $(DESTDIR)/usr/bin/..."
	@mkdir -p $(DESTDIR)/usr/bin
	@cp $(OUT) $(DESTDIR)/usr/bin/
	@chmod +x $(DESTDIR)/usr/bin/$(OUT)


# Uninstall from system
uninstall:
	@echo "Removing /usr/local/bin/$(OUT)..."
	@sudo rm -f /usr/local/bin/$(OUT)

# Help message
help:
	@echo "Ubuntu System Monitor Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all        - Build the project (default)"
	@echo "  debug      - Build with debug symbols"
	@echo "  release    - Build optimized version"
	@echo "  run        - Build and run"
	@echo "  install    - Copy binary to /usr/local/bin (supports DESTDIR)"
	@echo "  uninstall  - Remove binary from /usr/local/bin"
	@echo "  clean      - Remove build and dist files"
	@echo "  help       - Show this help message"
