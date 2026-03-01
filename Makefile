CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = test_nova
LIB_SRC = nova_drop.c
LIB_OBJ = $(LIB_SRC:.c=.o)
TEST_SRC = main.c

all: $(TARGET)

$(TARGET): $(LIB_OBJ) $(TEST_SRC)
	$(CC) $(CFLAGS) -o $@ $(TEST_SRC) $(LIB_OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(LIB_OBJ)

.PHONY: all test clean
