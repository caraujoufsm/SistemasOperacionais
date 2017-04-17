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
      printf("Iniciando a contagem de tempo do processo pid [%d] pai [%d]\n", getpid(), getppid());
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
  int rep = (atoi(argv[2]));
  double tempo_arv, tempo_cad;
  clock_t ini_arv, fim_arv, ini_cad, fim_cad;
  printf("Pai: [%d]\n", pai);

  if (argc != 3)
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

  if (rep < 1)
  {
    fprintf(stderr, "%s: Valor de repeticao nao pode ser menor que 1!\n");
    return EXIT_FAILURE;
  }

  while(rep > 1)
  {

  ini_arv = clock();
  printf("Tempo inicial da arvore: %g segundos\n", (double)(ini_arv)/CLOCKS_PER_SEC);
  proc_bin(0,0,altura);

  if(pai == getpid())
  {
    fim_arv = clock();
    printf("Tempo final da arvore: %g segundos\n", (double)(fim_arv)/CLOCKS_PER_SEC);
    tempo_arv = ((double) (fim_arv - ini_arv)) / CLOCKS_PER_SEC;
    printf("Processo arvore executado em %g segundos\n", tempo_arv);
  }
  printf("-----------------------------------------------------------------\n");
  ini_cad = clock();
  printf("Tempo inicial da cadeia: %g segundos\n", (double)(ini_cad)/CLOCKS_PER_SEC);
  cadeia(qtd);

  if(pai == getpid())
  {
    fim_cad = clock();
    printf("Tempo final da cadeia: %g segundos\n", (double)(fim_cad)/CLOCKS_PER_SEC);
    tempo_cad = ((double) (fim_cad - ini_cad)) / CLOCKS_PER_SEC;
    printf("Processo cadeia executado em %g segundos\n", tempo_cad);
  }
  rep --;
}
  return EXIT_SUCCESS;
}
