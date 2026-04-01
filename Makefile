# Compiler settings
CC = gcc
# Point to the include directory for headers
CFLAGS = -Iinclude -Itest/unity -Wall -Wextra
BIN_DIR = bin

# Default target: builds everything (tests and app)
all: app run_tests

app:
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) src/main.c src/md5_utils.c -o $(BIN_DIR)/app

run_tests:
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) test/unity/unity.c src/md5_utils.c test/test_md5_utils.c -o $(BIN_DIR)/test_suite
	./$(BIN_DIR)/test_suite

clean:
	rm -rf $(BIN_DIR)/*