CC = g++
CFLAGS = -O

raw.o: raw.cpp Packet.cpp Packet.h IPv4.cpp IPv4.h
	$(CC) $(CFLAGS) raw.cpp IPv4.cpp Packet.cpp -o raw

.PHONY: clean
clean:
	sudo rm raw ipv4.txt
