CC = gcc
CFLAGS = -Wall -Iinclude

SRC = src/main.c src/cpu.c
OUT = sysmon

all:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS)

clean:
	rm -f $(OUT)
