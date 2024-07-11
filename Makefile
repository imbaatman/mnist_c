CC = gcc

CFLAGS = -Wall -Wextra -O2

LDFLAGS = -lm

# Directories
SRC_DIR = src
BUILD_DIR = build
DATA_DIR = data

SRC = $(SRC_DIR)/mnist.c
OBJ = $(BUILD_DIR)/mnist.o
EXECUTABLE = $(BUILD_DIR)/MNIST

TRAIN_IMAGES = $(DATA_DIR)/train-images.idx3-ubyte
TRAIN_LABELS = $(DATA_DIR)/train-labels.idx1-ubyte
TEST_IMAGES = $(DATA_DIR)/t10k-images.idx3-ubyte
TEST_LABELS = $(DATA_DIR)/t10k-labels.idx1-ubyte

# Default target
all: $(EXECUTABLE)

# Compile source file
$(OBJ): $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

# Run the program
run: $(EXECUTABLE)
	@if [ -f $(TRAIN_IMAGES) ] && [ -f $(TRAIN_LABELS) ] && [ -f $(TEST_IMAGES) ] && [ -f $(TEST_LABELS) ]; then \
		cd $(BUILD_DIR) && ./MNIST; \
	else \
		echo "Error: MNIST data files not found in $(DATA_DIR)"; \
		exit 1; \
	fi

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
