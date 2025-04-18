# Compiler et linkeuse
CC = gcc
CFLAGS = -Wall -I./ -I/home/ju456/raylib/src
LDFLAGS = -L/home/ju456/raylib/src -lraylib -lm -lpthread -ldl -lX11

# Fichiers source
SRCS = assets_gestion.c main.c

# Nom de l'exécutable
TARGET = assets

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
