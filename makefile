CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Files
SRCS = main.c vertex.c road.c general.c
OBJS = $(SRCS:.c=.o)
EXEC = graph

# Build executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Object files
%.o: %.c graph.h
	$(CC) $(CFLAGS) -c $<

# Clean build files
clean:
	rm -f $(OBJS) $(EXEC)

# Rebuild from scratch
rebuild: clean $(EXEC)

