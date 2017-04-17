#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

void proc_bin(int max)
{
  int i,g1,g2,status; // Galho 1 e Galho 2
  int pai = getpid();
  clock_t ini, fim, tempo;
  ini = clock();

  for(i=0; i<max; i++)
  {
      g1 = fork();
      if(g1 == -1)
      {
        // Ocorreu um erro
        exit(errno);
      }
      else if(g1 == 0)
      {
      printf("Sou o filho [%d], meu pai eh [%d]\n", getpid(), getppid());
      }
      else
      {
        if(g1!=0)
          g2 = fork();
          if(g2 == -1)
          {
            // Ocorreu um erro
            exit(errno);
          }
          else if(g2 == 0)
          {
          printf("Sou o filho [%d], meu pai eh [%d]\n", getpid(), getppid());
          }
            else
            {
              if(g1!=0 && g2!=0)
              break;
            }
      }
  }
  waitpid(g1,&status,0);
  waitpid(g2,&status,0);
  printf("Processo saindo: [%d]\n", getpid());

  if(pai == getpid())
  {
    fim = clock();
    printf("Tempo final da arvore: %f segundos\n", (double)(fim)/CLOCKS_PER_SEC);
    tempo = ((double) (fim - ini)) / CLOCKS_PER_SEC;
    printf("Processo arvore executado em %f segundos\n", tempo);
  }
}

int main(int argc, char *argv[])
{
  int altura;

  if (argc != 2)
  {
    fprintf(stderr, "Estao faltando argumentos!\n");
    return EXIT_FAILURE;
  }

  altura = atoi(argv[1]);
  if (altura < 0)
  {
    fprintf(stderr, "%s: Altura nao pode ser negativa!\n");
    return EXIT_FAILURE;
  }

  proc_bin(altura);

  return EXIT_SUCCESS;
}
