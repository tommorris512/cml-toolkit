# Define directories
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# Define source files
SRC_FILES = $(SRC_DIR)/linear_regression.c $(SRC_DIR)/k_means.c
TEST_FILES = $(TEST_DIR)/test_linear_regression.c $(TEST_DIR)/test_k_means.c

# Define object and executable files
OBJ_FILES = $(BUILD_DIR)/linear_regression.o $(BUILD_DIR)/k_means.o
TEST_OBJ_FILES = $(BUILD_DIR)/test_linear_regression.o $(BUILD_DIR)/test_k_means.o
TEST_EXEC_LINEAR_REGRESSION = $(BUILD_DIR)/test_linear_regression
TEST_EXEC_K_MEANS = $(BUILD_DIR)/test_k_means

# Compiler and flags
CC = clang
CFLAGS = -Wall -Wextra -Isrc -Itests
LDFLAGS = -lm

# Build all test targets
tests: $(TEST_EXEC_LINEAR_REGRESSION) $(TEST_EXEC_K_MEANS)

# Build the test executables
# Linear Regression Test Executable
$(TEST_EXEC_LINEAR_REGRESSION): $(OBJ_FILES) $(BUILD_DIR)/test_linear_regression.o
	$(CC) $(OBJ_FILES) $(BUILD_DIR)/test_linear_regression.o -o $@ $(LDFLAGS)

# K Means Test Executable
$(TEST_EXEC_K_MEANS): $(OBJ_FILES) $(BUILD_DIR)/test_k_means.o
	$(CC) $(OBJ_FILES) $(BUILD_DIR)/test_k_means.o -o $@ $(LDFLAGS)

# Compile the src object files
# Linear Regression Source
$(BUILD_DIR)/linear_regression.o: $(SRC_DIR)/linear_regression.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# K Means Source
$(BUILD_DIR)/k_means.o: $(SRC_DIR)/k_means.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile the test object files
# Linear Regression Test
$(BUILD_DIR)/test_linear_regression.o: $(TEST_DIR)/test_linear_regression.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# K Means Test
$(BUILD_DIR)/test_k_means.o: $(TEST_DIR)/test_k_means.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build directory
clean:
	rm -rf $(BUILD_DIR)
