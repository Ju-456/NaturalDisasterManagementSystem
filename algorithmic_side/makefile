CC = gcc

CFLAGS = -Wall -I./ -I/home/ju456/raylib/src
LDFLAGS = -L/home/ju456/raylib/src -lraylib -lm -lpthread -ldl -lX11

# Files
SRCS = main.c general.c parson.c vertex.c road.c earthquake.c window.c shortestPath.c travel.c
OBJS = $(SRCS:.c=.o)
HEADERS = graph.h vertex.h road.h earthquake.h window.h shortestPath.h travel.h
EXEC = graph_viewer

# Build executable
$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile .c to .o
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(EXEC)

# Rebuild from scratch
rebuild: clean $(EXEC)