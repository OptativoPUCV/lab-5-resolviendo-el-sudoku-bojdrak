#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

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

int is_valid(Node* n){
  int i, j, k;
  //filas
  for(i=0;i<9;i++){
    int* check = (int*) malloc(10*sizeof(int));
    for(k=0;k<10;k++) check[k]=0;

    for(j=0;j<9;j++){
      if(n->sudo[i][j] != 0){
        if(check[n->sudo[i][j]] == 1){
          free(check);
          return 0;
        }
        check[n->sudo[i][j]] = 1;
      }
    }
    free(check);
  }
  //columnas
  for(j=0;j<9;j++){
    int* check = (int*) malloc(10*sizeof(int));
    for(k=0;k<10;k++)
      check[k]=0;
    for(i=0;i<9;i++){
      if(n->sudo[i][j] != 0){
        if(check[n->sudo[i][j]] == 1){
          free(check);
          return 0;
        }
        check[n->sudo[i][j]] = 1;
      }
    }
    free(check);
  }
  //subcuadrantes
  for(i=0;i<9;i+=3){
    for(j=0;j<9;j+=3){
      int* check = (int*) malloc(10*sizeof(int));
      for(k=0;k<10;k++)
        check[k]=0;
      for(int x=i;x<i+3;x++){
        for(int y=j;y<j+3;y++){
          if(n->sudo[x][y] != 0){
            if(check[n->sudo[x][y]] == 1){
              free(check);
              return 0;
            }
            check[n->sudo[x][y]] = 1;
          }
        }
      }
      free(check);
    }
  }  
  return 1;
}


List* get_adj_nodes(Node* n){
    List* list=createList();
    int fila = -1, col = -1;
    for (int i = 0; i < 9 && fila == -1; i++){
        for (int j = 0; j < 9;j++){
            if (n->sudo[i][j] == 0){
                fila = i;
                col = j;
                break;
            }
        }
    }
    if (fila == -1) return list;
    for(int valor = 1; valor <= 9; valor++){
        Node* NueNodo = copy(n);
        NueNodo->sudo[fila][col] = valor;
        if (is_valid(NueNodo)){
            pushBack(list, NueNodo);
        } else {
            free(NueNodo);
        }
    }
    // Verifica si hay ceros
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(n->sudo[i][j] == 0){
                free(n);
                return list;
            }
        }
    }
    // Si no hay ceros y no hay repetidos, es valido
    for(int i=0;i<9;i++){
        int* check = (int*) malloc(10*sizeof(int));
        for(int k=0;k<10;k++)
            check[k]=0;
        for(int j=0;j<9;j++){
            if(n->sudo[i][j] != 0){
                if(check[n->sudo[i][j]] == 1){
                    free(check);
                    return list;
                }
                check[n->sudo[i][j]] = 1;
            }
        }
        free(check);
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

Node* DFS(Node* initial, int* cont){
  Stack* stack = createStack();
  push(stack, initial);
  while(!is_empty(stack)){
    Node* n = top(stack);
    pop(stack);
    (*cont)++;
    if(is_final(n)){
      return n;
    }
    List* adj = get_adj_nodes(n);
    for(Node* node = first(adj); node != NULL; node = next(adj)){
      push(stack, node);
    }
    clean(adj);
  }
  clean(stack);
  free(initial);
  return NULL;
}


int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}