CC = gcc
CFLAGS = -Wall -Werror -std=c17 -IInclude

SRC_DIR = Src
INC_DIR = Include

SRC = $(SRC_DIR)/Dump_func.c
MAIN_SRC = $(SRC_DIR)/main.c
TEST_SRC = $(SRC_DIR)/test_main.c

TARGET = prog
TEST_TARGET = test_prog

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(MAIN_SRC) $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(TEST_TARGET): $(TEST_SRC) $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) $(TEST_TARGET) *.o *.dump

rebuild: clean all
