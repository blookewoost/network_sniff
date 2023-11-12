CC = g++
CFLAGS = -std=c++20 -O

SRC_DIR = ../src
PROTO_DIR = ../protocols

SRC = $(SRC_DIR)/Packet.cpp $(SRC_DIR)/raw.cpp $(SRC_DIR)/lib.cpp
PROTO = $(PROTO_DIR)/IPv4.cpp $(PROTO_DIR)/IPv6.cpp

JSON_FILES = $(wildcard ../../../data/*.json)
OBJ_FILES = $(wildcard *.o)

raw.o: $(SRC) $(PROTO)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	sudo rm $(OBJ_FILES) $(JSON_FILES)
