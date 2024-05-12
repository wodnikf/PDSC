CC = gcc
CFLAGS = -g -Wall -pedantic
LDFLAGS = -lm
SOURCES = main.c file.c input.c account.c operations.c search.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = bank

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
