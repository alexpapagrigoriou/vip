CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC = $(wildcard *.c)

OBJ = $(SRC:.c=.o)

TARGET = vip

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

re: clean all
