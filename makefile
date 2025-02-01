CC = gcc
CFLAGS = -lncursesw -lSDL2 -lSDL2_mixer

TARGET = main

SRC = main.c

$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)
