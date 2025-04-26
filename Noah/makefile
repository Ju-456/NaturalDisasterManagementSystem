CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I./ -I/home/ju456/raylib/src

# Files
SRCS = main.c vertex.c road.c earthquake.c general.c window.c parson.c
OBJS = $(SRCS:.c=.o)
EXEC = graph_viewer

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

