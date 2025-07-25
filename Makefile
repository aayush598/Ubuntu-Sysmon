CC = gcc
CFLAGS = -Wall -Iinclude

SRC = $(wildcard src/*.c)
OUT = sysmon
	
all:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS)

clean:
	rm -f $(OUT)
