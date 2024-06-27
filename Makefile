# Define directories
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# Define source files
SRC_FILES = $(SRC_DIR)/linear_regression.c
TEST_FILES = $(TEST_DIR)/test_linear_regression.c

# Define object and executable files
OBJ_FILES = $(BUILD_DIR)/linear_regression.o
TEST_OBJ_FILES = $(BUILD_DIR)/test_linear_regression.o
TEST_EXEC = $(BUILD_DIR)/test_linear_regression

# Compiler and flags
CC = clang
CFLAGS = -Wall -Wextra -Isrc -Itest

# Build all test targets
tests: $(TEST_EXEC)

# Build the test executable
$(TEST_EXEC): $(OBJ_FILES) $(TEST_OBJ_FILES)
	$(CC) $(OBJ_FILES) $(TEST_OBJ_FILES) -o $@

# Compile the src object files
$(BUILD_DIR)/linear_regression.o: $(SRC_FILES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile the test object files
$(BUILD_DIR)/test_linear_regression.o: $(TEST_FILES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build directory
clean:
	rm -rf $(BUILD_DIR)
