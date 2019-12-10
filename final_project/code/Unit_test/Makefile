TARGET = unittest 
SRC = $(wildcard *.c) ../tree.c
OBJ = $(SRC:.c=.o)
LDFLAGS = -lcunit
CFLAGS = -Wall -g
CC = g++

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)
