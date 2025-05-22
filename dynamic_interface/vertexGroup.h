#ifndef VERTEXGROUP_H
#define VERTEXGROUP_H

#include "graph.h"

void init_matrix_1d_zero(int num_vertices, int mat_1d[MAX_VERTICES]); // initialiser le poids de la matrice 1d avec des 0
void init_matrix_2d_zero(int num_vertices, Road mat_2d[][MAX_VERTICES]); // initialiser le poids de la matrice 1d avec des 0
void inverse_matrix_1d_full(int num_vertices, int mat_1d[MAX_VERTICES], int mat_1d_inverse[MAX_VERTICES]); // ajouter a la nouvelle matrice 1d l'inverse de l'autre matrice 1d
void inverse_matrix_2d_full(int num_vertices, Road mat_2d[][MAX_VERTICES], Road mat_2d_inverse[][MAX_VERTICES]); //ajouter a la nouvelle matrice 2d l'inverse de l'autre matrice 2d
void read_matrix_1d(int num_vertices, int matrix[MAX_VERTICES]); //lecture de la matrice 1d
void read_matrix_2d(int num_vertices, Road matrix[][MAX_VERTICES]); //lecture de la matrice 2d
void read_matrix_2d_cfc(int num_vertices, Road matrix[][MAX_VERTICES]); // lecture des sommets qui appartiennent a des groupes de sommets

void depth_first_search(int num_vertices, Road matrix[][MAX_VERTICES], int mat_visit[num_vertices], int index, int parent); // parcours en profondeur du graphe en récursif
void depth_first_search_full(int num_vertices, Road matrix[][MAX_VERTICES]); // parcours en profondeur du graphe en récursif avec une boucle pour faire toutes les possibilites
void left_right_root(int num_vertices, Road matrix[][MAX_VERTICES], int visited[num_vertices], int index, int result[MAX_VERTICES], int *pos); // remontee gauche droite racine (récursif)
void left_right_root_full(int num_vertices, Road matrix[][MAX_VERTICES], int result[MAX_VERTICES], int *pos); // remontee gauche droite racine en entier (récursif)
// void depth_first_search_inverse(int num_vertices, Road matrix[][MAX_VERTICES], Road matrix_inverse[][MAX_VERTICES], int result_inverse[MAX_VERTICES]); // affichage des differentes composantes fortement connexe (les groupes de sommets)
void dfs_cfc(int num_vertices, Road matrix_inverse[][MAX_VERTICES], int visited[], int index, int cfc_group_id, Road matrix[][MAX_VERTICES]);
void depth_first_search_inverse(int num_vertices, Road matrix[][MAX_VERTICES], Road matrix_inverse[][MAX_VERTICES], int finishing_order[MAX_VERTICES]);

#endif