CC = gcc
CFLAGS = -Wall -I./ -I/home/ju456/raylib/src # Inclut le chemin des en-têtes de raylib
LDFLAGS = -L/home/ju456/raylib/lib -lraylib -lm -lpthread -ldl -lX11 # Chemin vers la bibliothèque raylib

# Fichiers source
SRCS = assets_gestion.c main.c window.c parson.c

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
