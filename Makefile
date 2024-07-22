# Compiler and flags
CC = clang
CFLAGS = -Wall -Wextra -I./src

# Directories
SRC_DIR = src
BUILD_DIR = build
TEST_DIR = tests
INCLUDE_DIR = src

# Source and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Library names
STATIC_LIB = $(BUILD_DIR)/libcml.a
SHARED_LIB = $(BUILD_DIR)/libcml.so

# Targets
all: staticlib sharedlib

# Build the static library
staticlib: $(OBJ_FILES)
	ar rcs $(STATIC_LIB) $(OBJ_FILES)

# Build the shared library
sharedlib: $(OBJ_FILES)
	$(CC) -shared -o $(SHARED_LIB) $(OBJ_FILES)

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

# Clean build directory
clean:
	rm -f $(BUILD_DIR)/*.o $(STATIC_LIB) $(SHARED_LIB)

# Test targets
test: all
	$(CC) $(CFLAGS) $(TEST_DIR)/test_k_means.c $(STATIC_LIB) -o $(BUILD_DIR)/test_k_means
	$(CC) $(CFLAGS) $(TEST_DIR)/test_linear_regression.c $(STATIC_LIB) -o $(BUILD_DIR)/test_linear_regression
	$(BUILD_DIR)/test_k_means
	$(BUILD_DIR)/test_linear_regression

.PHONY: all staticlib sharedlib clean test
