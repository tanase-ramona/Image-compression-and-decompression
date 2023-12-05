/*TANASE Elena-Ramona - 313 CC*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

FILE * file_in, * file_out;

// definim structura pentru arbore
typedef struct Tree {
  struct Tree * left_up;
  struct Tree * right_up;
  struct Tree * right_down;
  struct Tree * left_down;
  unsigned char node_type;
  unsigned char red_value;
  unsigned char green_value;
  unsigned char blue_value;
}
Tree;

// definim structura pentru matricea in care vom citi pixelii
typedef struct matrix {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
}
matrix;

// definim structura pentru coada in care vom retine nodurile din arbore
// la parcurgerea BFS
typedef struct queue {
  Tree * node;
}
queue;

// calculam culoarea medie pentru rosu, intr-un anumit bloc
int red(int width, int height, int x, int y, matrix ** grid) {
  int i, j;
  unsigned long long sum = 0;
  for (i = x; i < x + height; i++) {
    for (j = y; j < y + width; j++) {
      sum = sum + grid[i][j].red;
    }
  }
  sum = sum / (width * height);
  return sum;
}

// calculam culoarea medie pentru verde, intr-un anumit bloc
int green(int width, int height, int x, int y, matrix ** grid) {
  int i, j;
  unsigned long long sum = 0;
  for (i = x; i < x + height; i++) {
    for (j = y; j < y + width; j++) {
      sum = sum + grid[i][j].green;
    }
  }
  sum = sum / (width * height);
  return sum;
}

// calculam culoarea medie pentru albastru, intr-un anumit bloc
int blue(int width, int height, int x, int y, matrix ** grid) {
  int i, j;
  unsigned long long sum = 0;
  for (i = x; i < x + height; i++) {
    for (j = y; j < y + width; j++) {
      sum = sum + grid[i][j].blue;
    }
  }
  sum = sum / (width * height);
  return sum;
}

// calculam similaritatea pentru un anumit bloc
unsigned long long similarity(int width, int height, int x, int y, matrix ** grid) {
  unsigned long long mean = 0;
  unsigned char r, g, b;
  int i, j;
  r = red(width, height, x, y, grid);
  g = green(width, height, x, y, grid);
  b = blue(width, height, x, y, grid);
  for (i = x; i < x + height; i++) {
    for (j = y; j < y + width; j++) {
      mean = mean + (r - grid[i][j].red) * (r - grid[i][j].red) +
        (g - grid[i][j].green) * (g - grid[i][j].green) +
        (b - grid[i][j].blue) * (b - grid[i][j].blue);
    }
  }
  mean = (unsigned long long) mean / (3 * width * height);
  return mean;
}

// Initializarea arborelui
Tree * init_tree(unsigned char node_type, unsigned char red_value,
  unsigned char green_value, unsigned char blue_value) {
  Tree * new_tree = malloc(sizeof(Tree));
  new_tree -> left_up = NULL;
  new_tree -> right_up = NULL;
  new_tree -> right_down = NULL;
  new_tree -> left_down = NULL;
  new_tree -> node_type = node_type;
  new_tree -> red_value = red_value;
  new_tree -> green_value = green_value;
  new_tree -> blue_value = blue_value;
  return new_tree;
}
// Adaugarea unui nod nou în arbore
void add_node(Tree ** tree, matrix ** grid, int width, int height, int x, int y,
  float factor, int * nr_nod) {
  // daca matricea nu trebuie divizata
  if (similarity(width, height, x, y, grid) <= factor) {
    // nodul este de tip frunza
    ( * tree) -> node_type = 1;
    // calculam valorile pentru culorile medii din acel bloc
    ( * tree) -> red_value = red(width, height, x, y, grid);
    ( * tree) -> green_value = green(width, height, x, y, grid);
    ( * tree) -> blue_value = blue(width, height, x, y, grid);
  } else {
    // daca matricea trebuie divizata => se formeaza 4 fii
    // crestem numarul de noduri
    ( * nr_nod) = ( * nr_nod) + 4;
    // nodul este de tip intern
    ( * tree) -> node_type = 0;
    // alocam memorie pentru nodurile fii
    ( * tree) -> left_up = (Tree * ) calloc(1, sizeof(Tree));
    ( * tree) -> right_up = (Tree * ) calloc(1, sizeof(Tree));
    ( * tree) -> right_down = (Tree * ) calloc(1, sizeof(Tree));
    ( * tree) -> left_down = (Tree * ) calloc(1, sizeof(Tree));
    // continuam divizarea recursiva a blocurilor
    add_node( & ( * tree) -> left_up, grid, width / 2, height / 2, x, y, factor, nr_nod);
    add_node( & ( * tree) -> right_up, grid, width / 2, height / 2, x, y + width / 2,
      factor, nr_nod);
    add_node( & ( * tree) -> right_down, grid, width / 2, height / 2, x + height / 2,
      y + width / 2, factor, nr_nod);
    add_node( & ( * tree) -> left_down, grid, width / 2, height / 2, x + height / 2, y,
      factor, nr_nod);
  }
}

// functie care calculeaza inaltimea arborelui
int inaltime(Tree * tree) {
  // daca arborele este gol
  if (tree == NULL) {
    return 0;
  }
  // variabila are retine inaltimea maxima la fiecare nivel
  int h_max = 0;
  // calculam recursiv inaltimea pentru fiecare fiu
  int h1 = inaltime(tree -> left_up);
  int h2 = inaltime(tree -> right_up);
  int h3 = inaltime(tree -> right_down);
  int h4 = inaltime(tree -> left_down);
  // inaltimea arborelui este maximul dintre cele 4 inaltimi ale fiilor + 1
  if (h1 > h_max) {
    h_max = h1;
  }
  if (h2 > h_max) {
    h_max = h2;
  }
  if (h3 > h_max) {
    h_max = h3;
  }
  if (h4 > h_max) {
    h_max = h4;
  }
  return h_max + 1;
}

// functie care calculeaza numarul de frunze
int no_leaves(Tree * tree) {
  // daca arborele este gol
  if (tree == NULL) {
    return 0;
  }
  // daca arborele are doar radacina, aceasta este frunza, caci nu are fii
  if (tree -> left_up == NULL && tree -> right_up == NULL &&
    tree -> right_down == NULL && tree -> left_down == NULL) {
    return 1;
  }
  //altfel, numarul de frunze va fii suma frunzelor fiecarui fiu
  int nr = 0;
  nr = nr + no_leaves(tree -> left_up) + no_leaves(tree -> right_up) +
       no_leaves(tree -> right_down) + no_leaves(tree -> left_down);
  return nr;
}

// Funcția care returnează primul nivel care are frunze
int first_level_leaves(Tree * tree) {
  // daca arborele este gol
  if (tree == NULL) {
    return -1;
  }
  // daca arborele are doar radacina, va fi considerata frunza
  if (tree -> left_up == NULL && tree -> right_up == NULL &&
    tree -> right_down == NULL && tree -> left_down == NULL) {
    return 0;
  }
  // calculam recursiv primul nivel cu frunze pentru fiecare fiu
  // comparam rezultatul cu variabila min_level unde retinem minimul absolut 
  int min_level = INT_MAX;
  int level1 = first_level_leaves(tree -> left_up);
  if (level1 >= 0 && level1 < min_level) {
    min_level = level1;
  }
  int level2 = first_level_leaves(tree -> right_up);
  if (level2 >= 0 && level2 < min_level) {
    min_level = level2;
  }
  int level3 = first_level_leaves(tree -> right_down);
  if (level3 >= 0 && level3 < min_level) {
    min_level = level3;
  }
  int level4 = first_level_leaves(tree -> left_down);
  if (level4 >= 0 && level4 < min_level) {
    min_level = level4;
  }
  return min_level + 1;
}

// retinem in coada nodurile din arbore parcurse in latime
void BFS(Tree * tree, queue ** coada) {
  if (tree == NULL) {
    return;
  }
  int stg = 0, dr = 1;
  // retinem radacina la indicele stg
  ( * coada)[stg].node = tree;
  while (stg < dr) {
    // cu stg ne mutam si extragem primul nod din coada pentru care nu am
    // parcurs inca fii
    Tree * aux = ( * coada)[stg].node;
    // crestem indicele stg pentru a trece la urmatorul nod
    stg = stg + 1;
    // adaugam toti fii nodului curent in coada, la indicele dr pe care il
    // incrementam la fiecare pas
    if (aux -> left_up != NULL) {
      ( * coada)[dr].node = aux -> left_up;
      dr++;
    }
    if (aux -> right_up != NULL) {
      ( * coada)[dr].node = aux -> right_up;
      dr++;
    }
    if (aux -> right_down != NULL) {
      ( * coada)[dr].node = aux -> right_down;
      dr++;
    }
    if (aux -> left_down != NULL) {
      ( * coada)[dr].node = aux -> left_down;
      dr++;
    }
  }
}

// eliberare memorie pentru arbore
void free_tree(Tree * tree) {
  if (tree == NULL) {
    return;
  }
  free_tree(tree -> left_up);
  free_tree(tree -> right_up);
  free_tree(tree -> right_down);
  free_tree(tree -> left_down);
  free(tree);
  tree = NULL;
}

int main(int argc, char
  const * argv[]) {
  // deschidem fisierul
  file_in = fopen(argv[3], "rb");
  file_out = fopen(argv[4], "w");

  // verificam daca s-a deschis fisierul
  if (file_in == NULL) {
    printf("Fisierul in nu s-a putut deschide\n");
    return 0;
  }
  if (file_out == NULL) {
    printf("Fisierul out nu s-a putut deschide\n");
    return 0;
  }
  // citim tipul fisierului
  char file_type[3];
  fscanf(file_in, "%s", file_type);
  // citim dimensiunile imaginii
  int width, height;
  fscanf(file_in, "%d %d", & height, & width);
  // citim valoarea maxima a culorii
  int colour;
  fscanf(file_in, "%d", & colour);
  fseek(file_in, 1, SEEK_CUR);
  // alocare memorie pentru matrice de pixeli
  matrix ** grid = (matrix ** ) malloc(height * sizeof(matrix * ));
  for (int i = 0; i < height; i++) {
    grid[i] = (matrix * ) malloc(width * sizeof(matrix));
  }
  // citire matrice de pixeli
  int i, j;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      fread( & grid[i][j].red, sizeof(unsigned char), 1, file_in);
      fread( & grid[i][j].green, sizeof(unsigned char), 1, file_in);
      fread( & grid[i][j].blue, sizeof(unsigned char), 1, file_in);
    }
  }
  // declarare arbore
  Tree * tree;
  // alocare memorie pentru arbore
  tree = (Tree * ) calloc(1, sizeof(Tree));
  // presupunem ca radacina o sa aiba fii
  tree -> node_type = 0;
  int nr_nod = 1;
  float factor = atoi(argv[2]);
  // cream arborele cuaternal de compresie
  add_node( & tree, grid, width, height, 0, 0, factor, & nr_nod);
  // daca radacina nu are fii (imaginea nu a fost comprimata) ii schimbam tipul
  if (tree -> left_up == NULL) tree -> node_type = 1;
  // verificam ce cerinta trebuie rezolvata
  if (strstr(argv[1], "c1")) {
    // afisare numar de nivele din arborele de compresie
    fprintf(file_out, "%d\n", inaltime(tree));
    // afisare , numărul de blocuri din imagine pentru care scorul similaritătii
    // pixelilor este mai mic sau egal decât factorul furnizat
    fprintf(file_out, "%d\n", no_leaves(tree));
    // afisare latura pentru pătratul care descrie cea mai mare zonă din imagine
    // care a rămas nedivizată
    int min_h_leaves = first_level_leaves(tree), p2 = 1;
    for (i = 0; i < min_h_leaves; i++) {
      p2 = p2 * 2;
    }
    fprintf(file_out, "%d\n", height / p2);
    // eliberam memoria alocata arborelui
    free_tree(tree);
  } else if (strstr(argv[1], "c2")) {
    // declarare coada
    queue * coada;
    // alocare memorie pentru coada
    coada = malloc(nr_nod * sizeof(queue));
    // apelam functia BFS pentru a crea coada
    BFS(tree, & coada);
    // scriem in fisier dimensiunea
    fwrite( & height, 1, sizeof(height), file_out);
    // parcurgem coada
    for (i = 0; i < nr_nod; i++) {
      // daca nodul curent din coada este nod intern
      if (coada[i].node -> node_type == 0) {
        fwrite( & coada[i].node -> node_type, 1, sizeof(coada[i].node -> node_type), file_out);
      } else {
        // daca nodul curent din coada este nod frunza
        fwrite( & coada[i].node -> node_type, 1, sizeof(coada[i].node -> node_type), file_out);
        fwrite( & coada[i].node -> red_value, 1, sizeof(coada[i].node -> red_value), file_out);
        fwrite( & coada[i].node -> green_value, 1, sizeof(coada[i].node -> green_value), file_out);
        fwrite( & coada[i].node -> blue_value, 1, sizeof(coada[i].node -> blue_value), file_out);
      }
    }
    // eliberare memorie alocata pentru coada
    for (i = 0; i < nr_nod; i++) {
      free(coada[i].node);
    }
    free(coada);
  }
  // eliberare memorie alocata pentru matrice
  for (int i = 0; i < height; i++) {
    free(grid[i]);
  }
  free(grid);
  // incidem fisierele
  fclose(file_in);
  fclose(file_out);
  return 0;
}