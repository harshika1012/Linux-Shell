# gcc 
# 	-g main.c hop.c seek.c back.c proclore.c log.c trim.c 

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Debug flags
DEBUG_FLAGS = -g

# Release flags
RELEASE_FLAGS = -O2

# Target executable name
TARGET = shell

# Source files
SRCS = main.c hop.c seek.c back.c proclore.c log.c trim.c 

# Object files
OBJS = $(SRCS:.c=.o)

# Default target (debug build)
all: $(TARGET)

# Rule to build the target executable for debugging
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

# Rule to build the target executable for release
release: CFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean the build directory
clean:
	rm -f $(TARGET) $(OBJS)
