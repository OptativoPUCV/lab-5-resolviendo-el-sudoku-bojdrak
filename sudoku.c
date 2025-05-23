#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
} Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}



int is_valid(Node* n) {
  // rows
  for (int i = 0; i < 9; i++){
    int check[10] = {0};
    for (int j = 0; j < 9; j++){
      int val = n->sudo[i][j];
      if (val && check[val])
        return 0;
      check[val] = 1;
    }
  }

  // colunumnas
  for (int j = 0; j < 9; j++){
    int check[10] = {0};
    for (int i = 0; i < 9; i++){
      int val = n->sudo[i][j];
      if (val && check[val])
        return 0;
      check[val] = 1;
    }
  }

  // subcuadrantes
  for (int k = 0; k < 9; k++){
    int check[10] = {0};
    for (int p = 0; p < 9; p++){
      int i = 3 * (k / 3) + (p / 3);
      int j = 3 * (k % 3) + (p % 3);
      int val = n->sudo[i][j];
      if (val && check[val])
        return 0;
      check[val] = 1;
    }
  }

  return 1;
}


List* get_adj_nodes(Node* n){
  List* list = createList();

  int row = -1, column = -1;
  for (int i = 0; i < 9 && row == -1; i++){
    for (int j = 0; j < 9; j++){
      if (n->sudo[i][j] == 0){
        row = i;
        column = j;
        break;  
      }
    }
  }

  if (row == -1) return list;

  for (int valor = 1; valor <= 9; valor++){
    Node* new_node = copy(n);
    new_node->sudo[row][column] = valor;
    if (is_valid(new_node)){
      pushBack(list, new_node);
    } else {
      free(new_node);
    }
  }

  return list;
}

int is_final(Node* n){
  for(int i=0;i<9;i++){
    for(int j=0;j<9;j++){
      if(n->sudo[i][j] == 0){
        return 0;
      }
    }
  }
  return 1;
}

Node* DFS(Node* initial, int* counter){
  *counter = 0;
  Stack* stack = createStack();
  push(stack, initial);

  while (!is_empty(stack)) {
    (*counter)++;
    Node* current = top(stack);
    pop(stack);

    if (is_final(current)) {
      clean(stack);
      free(stack);
      return current;
    }

    List* adj = get_adj_nodes(current);
    for (Node* n = first(adj); n != NULL; n = next(adj)) {
      push(stack, n);
    }
    clean(adj);
    free(adj);
  }

  clean(stack);
  free(stack);
  return NULL;
}

int main( int argc, char *argv[] ){
  Node* initial= read_file("s12a.txt");;

  int counter=0;
  Node* final = DFS(initial, &counter);
  printf("iterations:%d\n",counter);
  print_node(final);

  return 0;
}

