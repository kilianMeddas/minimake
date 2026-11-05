CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

SRC = $(wildcard src/*.c)
SRC_H = $(wildcard include/*.h)

OBJECT = $(SRC:.c=.o)
TARGET = Minimakefile

TEST_SRC_CRITERION = $(wildcard tests/test_unit/*.c)
TEST_BIN_CRITERION = tests/test_suite

TEST_SRC = $(wildcard tests/*.c)
TEST_H = $(wildcard tests/include/*.h)
TEST_BIN = $(TEST_SRC:.c=)


.PHONY: all minimake check clean format debug

all: minimake

minimake: $(TARGET)

$(TARGET):$(OBJECT)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECT)

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

check: $(TEST_BIN_CRITERION)
	./$(TEST_BIN_CRITERION)

clean:
	rm -f $(TARGET) $(OBJECT) $(TEST_BIN_CRITERION) $(TEST_BIN)
