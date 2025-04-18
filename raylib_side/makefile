# Compiler et linkeuse
CC = gcc
CFLAGS = -Wall -I./ -I/home/ju456/raylib/src # don't forget to adapt to your own path !!!
LDFLAGS = -L/home/ju456/raylib/src -lraylib -lm -lpthread -ldl -lX11 # don't forget to adapt to your own path !!!

# Fichiers source
SRCS = assets_gestion.c main.c

# Nom de l'exécutable
TARGET = raylib

# Règle par défaut (compilation de l'exécutable)
all: $(TARGET)

# Règle pour créer l'exécutable
$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# Règle pour nettoyer les fichiers générés (exécutable)
clean:
	rm -f $(TARGET)

# Règle pour générer le fichier d'objet de chaque fichier source (optionnel)
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: all clean
