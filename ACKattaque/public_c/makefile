# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g

SOURCES = $(wildcard src/*.c) 
OBJECTS = $(SOURCES:.c=.o)

CJSON_SOURCES = src/cjson/cJSON.c
CJSON_OBJECTS = $(CJSON_SOURCES:.c=.o)

UTILS_SOURCES = $(wildcard src/utils/*.c)
UTILS_OBJECTS = $(UTILS_SOURCES:.c=.o)

OBJS = $(OBJECTS) $(CJSON_OBJECTS) $(UTILS_OBJECTS)

TARGET = my_bot

all: $(TARGET)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)
