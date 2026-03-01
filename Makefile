CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O3
TARGET = test_nova
RAW_TARGET = nova_raw
BENCH_TARGET = nova_bench
UNIT_TEST_TARGET = unit_test
LIB_SRC = nova_drop.c
LIB_OBJ = $(LIB_SRC:.c=.o)
TEST_SRC = main.c
RAW_SRC = nova_raw.c
BENCH_SRC = nova_bench.c
UNIT_TEST_SRC = test_nova.c
UNITY_SRC = unity.c

all: $(TARGET) $(RAW_TARGET) $(BENCH_TARGET) $(UNIT_TEST_TARGET)

$(TARGET): $(LIB_OBJ) $(TEST_SRC)
	$(CC) $(CFLAGS) -o $@ $(TEST_SRC) $(LIB_OBJ)

$(RAW_TARGET): $(LIB_OBJ) $(RAW_SRC)
	$(CC) $(CFLAGS) -o $@ $(RAW_SRC) $(LIB_OBJ)

$(BENCH_TARGET): $(LIB_OBJ) $(BENCH_SRC)
	$(CC) $(CFLAGS) -o $@ $(BENCH_SRC) $(LIB_OBJ)

$(UNIT_TEST_TARGET): $(LIB_OBJ) $(UNIT_TEST_SRC) $(UNITY_SRC)
	$(CC) $(CFLAGS) -o $@ $(UNIT_TEST_SRC) $(UNITY_SRC) $(LIB_OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

unit-test: $(UNIT_TEST_TARGET)
	./$(UNIT_TEST_TARGET)

bench: $(BENCH_TARGET)
	./$(BENCH_TARGET)

clean:
	rm -f $(TARGET) $(RAW_TARGET) $(BENCH_TARGET) $(UNIT_TEST_TARGET) $(LIB_OBJ)

.PHONY: all test unit-test bench clean
