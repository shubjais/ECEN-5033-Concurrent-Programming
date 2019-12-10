TARGET = concurrent_tree 
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
LDFLAGS = -lrt -lpthread
CFLAGS = -Wall -g
CC = g++

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)
