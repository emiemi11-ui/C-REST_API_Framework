CC = g++
CFLAGS = -Wall
TARGET = server
SRC = main.cpp server.cpp utils.cpp

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)