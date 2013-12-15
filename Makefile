CC = gcc
CFLAGS = -Wall
LDFLAGS = -lpng -lm
EXECUTABLE = imgnspire
SRC = $(wildcard *.c)
OBJ = $(SRC: .c=.o)

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) -o $(EXECUTABLE) $< $(LDFLAGS)

.o: .c
	$(CC) $(CFLAGS) -s $< -o $@

clean:
	rm -r $(EXECUTABLE) *.o
