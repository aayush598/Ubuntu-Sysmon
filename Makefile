CC = gcc
CFLAGS = -Wall -Iinclude
LDFLAGS = -lncurses

SRC = $(wildcard src/*.c)
OUT = sysmon

all:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OUT)
