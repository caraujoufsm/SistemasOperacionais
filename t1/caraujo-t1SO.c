#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

void cadeia(int qtd)
{
  pid_t Processo;
  int i;

  printf("Meu pid eh [%d]\n", getpid());
  for (i=1 ; i <= qtd ; i++) {

    Processo = fork();    // Chamada Fork

    if ( Processo ) {
      printf("Iniciando a contagem de tempo do processo pid [%d] pai [%d] teste[%d]\n", getpid(), getppid(), Processo);
       break;        // Impede que o pai de um novo fork
    }
    printf("Sou o filho[%d], pid = [%d], meu pai eh [%d]\n",i, getpid(), getppid()); // Mostra o pid do filho e o pid do pai
  }
  wait(NULL);        // Impede que o pai finalize primeiro, evitando a geraçao de processos orfãos
  printf("Processo saindo:[%d]\n", getpid());
}

void proc_bin(int i, int h_atual, int max)
{
// Passa como parametros um valor inicial, a altura atual e a altura maxima

  pid_t IdProcesso = fork();

  if(IdProcesso == -1)
  {
    // Erro
    exit(errno);
  }
  else if (IdProcesso == 0)
  {
    sleep(1);
    // Processo Filho
    printf("Sou o filho, meu pid eh [%d], meu pai eh [%d]\n", getpid(), getppid());

    if(h_atual < max)
    {
      proc_bin(2*i+1, h_atual+1, max);
      proc_bin(2*i+2, h_atual+1, max);
    }
    exit(EXIT_SUCCESS);
  }
  else
  {
    // Processo Pai
    pid_t pid;
    int status;
    //printf("Sou o pai, meu pid eh [%d]\n",getpid());
    pid = waitpid(IdProcesso, &status, 0);
    printf("Encerrando o processo %d\n", IdProcesso);

    if (pid == -1)
    {
      exit(errno);
    }
  }

}

int main(int argc, char *argv[])
{
  int altura;
  int pai = getpid();
  int qtd = (atoi(argv[1])*2);
  double tempo_arv, tempo_cad;
  clock_t ini_arv, fim_arv, ini_cad, fim_cad;
  printf("Pai: [%d]\n", pai);

  if (argc != 2)
  {
    fprintf(stderr, "Esta faltando argumentos!\n");
    return EXIT_FAILURE;
  }

  altura = atoi(argv[1]);
  if (altura < 0)
  {
    fprintf(stderr, "%s: Altura nao pode ser negativa!\n");
    return EXIT_FAILURE;
  }

  ini_arv = clock();
  printf("Imprimindo a taxa de clock: %d\n", ini_arv);
  printf("Tempo inicial da arvore: %ld segundos\n", (double)(ini_arv/CLOCKS_PER_SEC));
  proc_bin(0,0,altura);

  if(pai == getpid())
  {
    fim_arv = clock();
    tempo_arv = ((double) (fim_arv - ini_arv)) / CLOCKS_PER_SEC;
    printf("Processo executado em %g segundos\n", tempo_arv);
  }

  ini_cad = clock();
  printf("Tempo inicial da cadeia: %ld segundos\n", (long)(ini_cad/CLOCKS_PER_SEC));
  cadeia(qtd);

  if(pai == getpid())
  {
    fim_cad = clock();
    tempo_cad = ((double) (fim_cad - ini_cad)) / CLOCKS_PER_SEC;
    printf("Processo executado em %g segundos\n", tempo_cad);
  }

  if(tempo_cad < tempo_arv)
  {
    printf("\n\n\n\nO tempo da arvore foi %g mais rapido que o tempo da cadeia\n\n\n", (tempo_arv - tempo_cad));
  }
  else
  {
    printf("\n\n\n\nO tempo da cadeia foi %g mais rapido que o tempo da arvore\n\n\n", (tempo_cad - tempo_arv));
  }

  return EXIT_SUCCESS;
}
