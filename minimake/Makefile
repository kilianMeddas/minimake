CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

SRC = $(wildcard src/*.c)
SRC_H = $(wildcard src/include/*.h)

OBJECT = $(SRC:.c=.o)
TARGET = minimake

TEST_SRC_CRITERION = $(wildcard tests/test_unit/*.c)
TEST_BIN_CRITERION = tests/test_suite

TEST_SRC = $(wildcard tests/*.c)
TEST_H = $(wildcard tests/include/*.h)
TEST_BIN = $(TEST_SRC:.c=)

.PHONY: all check clean format debug

all: minimake

$(TARGET):$(OBJECT)
	$(CC) -o $(TARGET) $(OBJECT)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BIN_CRITERION): $(TEST_SRC_CRITERION) $(filter-out src/minimake.o,$(OBJECT))
	$(CC) $(CFLAGS) $^ -lcriterion -o $@

tests/%: tests/%.c
	$(CC) $(CFLAGS) $< -o $@

format:
	clang-format -i $(SRC) $(SRC_H) $(TEST_SRC_CRITERION) $(TEST_SRC) $(TEST_H)

debug: CFLAGS += -g
debug: clean
	$(MAKE) all

check: $(TEST_BIN_CRITERION) minimake
	./$(TEST_BIN_CRITERION)
	./tests/test_integration/test_pretty_print_complete.sh
	./tests/test_integration/test_pretty_print_no_rules.sh
	./tests/test_integration/test_pretty_print_no_var.sh
	./tests/test_integration/test_error_missing_file.sh
	./tests/test_integration/test_error_missing_filename.sh
	./tests/test_integration/test_error_nonexistent_file.sh
	./tests/test_integration/test_error_option.sh
	./tests/test_integration/test_error_empty.sh
	
check-asan:
	$(CC) $(CFLAGS) -fsanitize=address -g -fno-omit-frame-pointer $(TEST_SRC_CRITERION) $(filter-out src/minimake.c,$(SRC)) -lcriterion -o $(TEST_BIN_CRITERION)_asan
	ASAN_OPTIONS=detect_leaks=1 ./$(TEST_BIN_CRITERION)_asan

clean:
	rm -f $(TARGET) $(OBJECT) $(TEST_BIN_CRITERION) $(TEST_BIN_CRITERION)_asan $(TEST_BIN) -r tests/script_results
