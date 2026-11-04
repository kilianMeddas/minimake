CC = cc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

SRC = src/micromake.c
OBJ = $(SRC:.c=.o)
TARGET = micromake

.PHONY: all clean check

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

check: all

clean:
	rm -f $(OBJ) $(TARGET)
