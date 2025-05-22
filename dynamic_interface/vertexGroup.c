#include "graph.h"

void init_matrix_1d_zero(int num_vertices, int mat_1d[MAX_VERTICES]) // initialiser le poids de la matrice 1d avec des 0
{
  printf("Initialization of the matrix (1d) with 0.\n");
  for (int i = 0; i < num_vertices; i++)
  {
    mat_1d[i] = 0;
  }
}

void init_matrix_2d_zero(int num_vertices, Road mat_2d[][MAX_VERTICES]) // initialiser le poids de la matrice 1d avec des 0
{
  printf("Initialization de la matrice (2d) with 0.\n");
  for (int i = 0; i < num_vertices; i++)
  {
    for (int j = 0; j < num_vertices; j++)
    {
      mat_2d[i][j].weight = 0;
    }
  }
}

void inverse_matrix_1d_full(int num_vertices, int mat_1d[MAX_VERTICES], int mat_1d_inverse[MAX_VERTICES]) //ajouter a la nouvelle matrice 1d l'inverse de l'autre matrice 1d
{
  for (int i = 0; i < num_vertices; i++)
  {
    mat_1d_inverse[i] = mat_1d[num_vertices-i-1];
  }
}

void inverse_matrix_2d_full(int num_vertices, Road mat_2d[][MAX_VERTICES], Road mat_2d_inverse[][MAX_VERTICES]) //ajouter a la nouvelle matrice 2d l'inverse de l'autre matrice 2d
{
  for (int i = 0; i < num_vertices; i++)
  {
    for (int j = 0; j < num_vertices; j++)
    {
      mat_2d_inverse[i][j].weight = mat_2d[j][i].weight;
    }
  }
}

void read_matrix_1d(int num_vertices, int matrix[MAX_VERTICES]) //lecture de la matrice 1d
{
  for (int i = 0; i < num_vertices; i++)
  {
    printf("%d ", matrix[i]);
  }
  printf("\n");
}

void read_matrix_2d(int num_vertices, Road matrix[][MAX_VERTICES]) //lecture de la matrice 2d
{
  for (int i = 0; i < num_vertices; i++)
  {
    for (int j = 0; j < num_vertices; j++)
    {
      printf("%.0f ", matrix[i][j].weight);
    }
    printf("\n");
  }
  printf("\n");
}

void read_matrix_2d_cfc(int num_vertices, Road matrix[][MAX_VERTICES]) // lecture des sommets qui appartiennent a des groupes de sommets
{
  for (int i = 0; i < num_vertices; i++)
  {
    printf("\nThe vertex %d belong at group of vertices : %d", i, matrix[i][i].cfc_group);
  } 
}

void depth_first_search(int num_vertices, Road matrix[][MAX_VERTICES], int mat_visit[num_vertices], int index, int parent) // parcours en profondeur du graphe en récursif
{
  mat_visit[index] = 1;
  
  if (parent != -1) 
  {
    printf("Arete of the tree : %d -> %d\n", parent, index);
  } 
  else 
  {
    printf("Root of the tree : %d\n", index);
  }

  for (int i = 0; i < num_vertices; i++)
  {
    if (matrix[index][i].weight != 0 && mat_visit[i] == 0)
    {
      depth_first_search(num_vertices, matrix, mat_visit, i, index);
    }
  }
}

void depth_first_search_full(int num_vertices, Road matrix[][MAX_VERTICES]) // parcours en profondeur du graphe en récursif avec une boucle pour faire toutes les possibilites
{
  int mat_visit[num_vertices];
  for (int i = 0; i < num_vertices; i++) 
  {
    mat_visit[i] = 0;
  }
  for (int i = 0; i < num_vertices; i++) 
  {
    if (mat_visit[i] == 0) 
    {
      depth_first_search(num_vertices, matrix, mat_visit, i, -1);
    }
  }
}

void left_right_root(int num_vertices, Road matrix[][MAX_VERTICES], int visited[num_vertices], int index, int result[MAX_VERTICES], int *pos) // remontee gauche droite racine (récursif)
{
  visited[index] = 1;
  for (int i = 0; i < num_vertices; i++)
  {
    if (matrix[index][i].weight != 0 && !visited[i])
    {
      left_right_root(num_vertices, matrix, visited, i, result, pos);
    }
  }
  result[*pos] = index;
  (*pos)++;
}

void left_right_root_full(int num_vertices, Road matrix[][MAX_VERTICES], int result[MAX_VERTICES], int *pos) // remontee gauche droite racine en entier (récursif)
{
  int visited[num_vertices];
  for (int i = 0; i < num_vertices; i++) 
  {
    visited[i] = 0;
  }
  for (int i = 0; i < num_vertices; i++)
  {
    if (visited[i] == 0)
    {
      left_right_root(num_vertices, matrix, visited, i, result, pos);
    }
  }
}
/*
void depth_first_search_inverse(int num_vertices, Road matrix[][MAX_VERTICES], Road matrix_inverse[][MAX_VERTICES], int result_inverse[MAX_VERTICES]) // affichage des differentes composantes fortement connexe (les groupes de sommets)
{
  int mark[num_vertices];
  for (int i = 0; i < num_vertices; i++)
  {
    mark[i] = result_inverse[i];
  }
  int i = 0;
  int component_it = 0;
  while (i < num_vertices)
  {
    if (mark[i] != -1)
    {
      printf("\nThe vertex's group %d contain the composante : %d ", component_it, mark[i]);
      matrix[mark[i]][mark[i]].cfc_group = component_it;
      component_it++;
    }
    for (int j = result_inverse[i]; j < num_vertices; j++)
    {
      if (matrix_inverse[result_inverse[i]][j].weight != 0 && j != result_inverse[i] && j == result_inverse[i+1])
      {
        printf("%d ", j);
        for (int k = 0; k < num_vertices; k++)
        {
          if (mark[k] == j)
          {
            mark[k] = -1;
          }
        }
      }
    }
    mark[i] = -1;
    result_inverse[i] = -1;
    i++;
  }
  //printf("\nIl y a donc %d groupes de sommets dans ce graphe.\n",component_it++);
}
*/
void dfs_cfc(int num_vertices, Road matrix_inverse[][MAX_VERTICES], int visited[], int index, int cfc_group_id, Road matrix[][MAX_VERTICES]) {
    visited[index] = 1;
    matrix[index][index].cfc_group = cfc_group_id;
    printf("%d ", index);
    for (int i = 0; i < num_vertices; i++) {
        if (matrix_inverse[index][i].weight != 0 && !visited[i]) {
            dfs_cfc(num_vertices, matrix_inverse, visited, i, cfc_group_id, matrix);
        }
    }
}

void depth_first_search_inverse(int num_vertices, Road matrix[][MAX_VERTICES], Road matrix_inverse[][MAX_VERTICES], int finishing_order[MAX_VERTICES]) {
    int visited[MAX_VERTICES] = {0};
    int cfc_group_id = 0;

    for (int i = num_vertices - 1; i >= 0; i--) {
        int index = finishing_order[i];
        if (!visited[index]) {
            printf("\nThe vertex's group %d contain the composante : ", cfc_group_id);
            dfs_cfc(num_vertices, matrix_inverse, visited, index, cfc_group_id, matrix);
            cfc_group_id++;
        }
    }
}
