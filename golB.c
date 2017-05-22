//Testing Git

/*
* The Game of Life
*
* a cell is born, if it has exactly three neighbours
* a cell dies of loneliness, if it has less than two neighbours
* a cell dies of overcrowding, if it has more than three neighbours
* a cell survives to the next generation, if it does not die of loneliness
* or overcrowding
*
* In this version, a 2D array of ints is used.  A 1 cell is on, a 0 cell is off.
* The game plays a number of steps (given by the input), printing to the screen each time.  'x' printed
* means on, space means off.
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
typedef unsigned char cell_t;

//Cada thread deve saber em qual iteração começa e qual termina.
typedef struct {
  int start, end;
  pthread_t thread;
} Thread;

//Array de Threads
Thread *threads;
//Número de Threads
int nThreads;
//Board
cell_t ** board;
// Board
cell_t ** prev;
// Newboard
cell_t ** next;
//size
int size;

//Calcula quais iterações do laço cada thread executa
void iteration_calc (int totalIterations)
{
    //Calcula quantas iterações cada thread deve executar
    int iterations = totalIterations / nThreads;
    // Calcula quantas iterações vão ficar de fora, considerando que
    //o número de iterações não é múltiplo do número de threads
    int rest = totalIterations % nThreads;
    int i;
    int temp = 0;
    //For que coloca no array auxiliar quantas vezes o laço deve ser executado
    //O temp serve pra o I que ele for executar ser um depois da thread anterior.
    for (i = 0; i < nThreads; i++) {
      threads[i].end = iterations;
    }
    //Outro for pra distribuir as iterações restantes
    for (i = 0; i < rest; i++) {
      threads[i].end += 1;
    }
    //Agora distribui igualmente quais threads executam quais laços,
    // sei que parece ter muitos for, mas eles devem ser executados poucas vezes,
    // acho que valem a pena em troca de ter uma boa uniformidade entre as threads
    for (i = 0; i < nThreads; i++) {
      threads[i].start = temp;
      threads[i].end += temp;
      temp = threads[i].end;
    }
}

void *for_allocate_board (void *idThread)
{
  int id = (int)idThread;
  int i;
  for (i=threads[id].start; i<threads[id].end; i++) {
    board[i] = (cell_t *) malloc(sizeof(cell_t)*size);
  }
  pthread_exit(NULL);
}

cell_t ** allocate_board ()
{
  board = (cell_t **) malloc(sizeof(cell_t*)*size);
  int	i;
  //Chama a função que distribui os i's de cada thread
  iteration_calc(size);
  for (i = 0; i < nThreads; i++) {
    pthread_create(&threads[i].thread, NULL, for_allocate_board, (void *)i);
  }
  for (i = 0; i < nThreads; i++) {
    pthread_join(threads[i].thread,NULL);
  }
  return board;
}

void free_board (cell_t ** board)
{
  int i;
  for (i=0; i<size; i++)
    free(board[i]);

  free(board);
}


/* return the number of on cells adjacent to the i,j cell */
int adjacent_to (cell_t ** board, int i, int j)
{
  int	k, l, count=0;

  int sk = (i>0) ? i-1 : i;
  int ek = (i+1 < size) ? i+1 : i;
  int sl = (j>0) ? j-1 : j;
  int el = (j+1 < size) ? j+1 : j;

  for (k=sk; k<=ek; k++)
  for (l=sl; l<=el; l++)
  count+=board[k][l];
  count-=board[i][j];

  return count;
}

void play_adjacent_to(void idThread)
{
  int id = (int)idThread;
  int i;
  for (j = threads[id].start; j < threads[id].end; j++) {
    a = adjacent_to (prev, i, j); // como passar os parametros i e j?
    if (a == 2) next[i][j] = prev[i][j];
    if (a == 3) next[i][j] = 1;
    if (a < 2) next[i][j] = 0;
    if (a > 3) next[i][j] = 0;
  }
}

void play ()
{
  int	i, j, a;
  iteration_calc(size);
  /* for each cell, apply the rules of Life */
  for (i=0; i<size; i++) { 
      for (j = 0; j < nThreads; i++) {
        pthread_create(&threads[i].thread, NULL, play_adjacent_to, (void *)i);
      }
      for (i = 0; i < nThreads; i++) {
        pthread_join(threads[i].thread,NULL);
      }

      pthread_exit(NULL);
  }
}

/* print the life board */
void print (cell_t ** board)
{
  int	i, j;
  /* for each row */
  for (j=0; j<size; j++) {
    /* print each column position... */
    for (i=0; i<size; i++)
    printf ("%c", board[i][j] ? 'x' : ' ');
    /* followed by a carriage return */
    printf ("\n");
  }
}

/* read a file into the life board */
void read_file (FILE * f, cell_t ** board)
{
  int	i, j;
  char	*s = (char *) malloc(size+10);

  /* read the first new line (it will be ignored) */
  fgets (s, size+10,f);

  /* read the life board */
  for (j=0; j<size; j++) {
    /* get a string */
    fgets (s, size+10,f);
    /* copy the string to the life board */
    for (i=0; i<size; i++)
    board[i][j] = s[i] == 'x';

  }
}

int main (int argc, char **argv)
{
  nThreads = atoi(argv[1]);
  int steps;
  
  FILE    *f;
  f = stdin;
  fscanf(f,"%d %d", &size, &steps);

  //Malloc do array de threads
  if (nThreads >= steps) {
    threads = (Thread *) malloc(sizeof(Thread)*size);
    nThreads = size;
  } else {
    threads = (Thread *) malloc(sizeof(Threa
  for (i=0; i<steps; i++) {
    play();d)*nThreads);
  }

  prev = allocate_board ();
  read_file (f, prev);
  fclose(f);
  
  next = allocate_board ();
  cell_t ** tmp;
  
  int i;
  
  #ifdef DEBUG
    printf("Initial:\n");
    print(prevx);
  #endif

  for (i=0; i<steps; i++) {
    play();

    #ifdef DEBUG
      printf("%d ----------\n", i + 1);
      print (next);
    #endif
    tmp = next;
    next = prev;
    prev = tmp;
  }

#ifdef RESULT
  printf("Final:\n");
  print (prev);
#endif

  free_board(prev);
  free_board(next);
}
