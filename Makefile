# Compiler and flags
CC      := gcc
CFLAGS  := -Wall -Wextra -Iinclude
LDFLAGS := -lncurses
BUILD   := build
SRC     := $(wildcard src/*.c)
OBJ     := $(patsubst src/%.c, $(BUILD)/%.o, $(SRC))
OUT     := sysmon

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
release: CFLAGS += -O2
release: clean all

# Clean all generated files
clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILD) $(OUT)

# Print help
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  debug    - Build with debug symbols"
	@echo "  release  - Build optimized version"
	@echo "  run      - Build and run"
	@echo "  clean    - Remove build files"
	@echo "  help     - Show this message"
