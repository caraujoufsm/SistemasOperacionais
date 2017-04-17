#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

void cadeia(int qtd)
{
  pid_t Processo;
  int i;
  clock_t ini, fim, tempo;
  int pai = getpid();

  printf("Meu pid eh [%d]\n", getpid());
  for (i=1 ; i <= qtd ; i++) {

    ini = clock();
    Processo = fork();    // Chamada Fork

    if(Processo == -1)
    {
      // Ocorreu um erro
      exit(errno);
    }

    else if (Processo)
    {
      // Processo Pai
       break;        // Impede que o pai de um novo fork
    }
    else
    // Processo Filho
    printf("CAD:Sou o filho[%d], pid = [%d], meu pai eh [%d]\n",i, getpid(), getppid()); // Mostra o pid do filho e o pid do pai
  }
  wait(NULL);        // Impede que o pai finalize primeiro, evitando a geraçao de processos orfãos
  printf("CAD:Processo saindo:[%d]\n", getpid());
  if(pai == getpid())
  {
    fim = clock();
    printf("Tempo final da cadeia: %f segundos\n", (double)(fim)/CLOCKS_PER_SEC);
    tempo = ((double) (fim - ini)) / CLOCKS_PER_SEC;
    printf("Processo cadeia executado em %f segundos\n", tempo);
  }
}

int main(int argc, char *argv[])
{
  int qtd;

  if (argc != 2)
  {
    fprintf(stderr, "Passagem de argumentos errada!\n");
    return EXIT_FAILURE;
  }

  qtd = atoi(argv[1]);
  if (qtd < 0)
  {
    fprintf(stderr, "%s: Quantidade nao pode ser negativa!\n");
    return EXIT_FAILURE;
  }
  cadeia(qtd);
  return EXIT_SUCCESS;
}
