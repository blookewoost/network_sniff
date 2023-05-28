CC = g++
CFLAGS = -O

SRC_FILES = $(wildcard *.cpp)
TXT_FILES = $(wildcard *.txt)
OBJ_FILES = $(wildcard *.o)

raw.o: $(SRC_FILES) 
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	sudo rm $(OBJ_FILES) $(TXT_FILES)
