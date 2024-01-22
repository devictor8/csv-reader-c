#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct items
{
  int CodPedido;
  long int DtPedido;
  int CodFilial;
  int CodProduto;
  int Qtd;
  float VlUnit;
};

struct rel_order
{
  int CodPedido;
  long int DtPedido;
  int CodFilial;
  int QtdTotalItens;
  float TotalPedido;
};

struct rel_branch
{
  int CodFilial;
  int TotalItensVendidos;
  float FatTotal;
};

void changePositions();
void copyFile();
void CreateFile();
void increaseArrayBranch();
void increaseArrayItems();
void increaseArrayOrder();
void processBranch();
void processOrder();
int readCSV();
void readCopyBranch();
void readCopyOrder();
void sortByDate();
void sortByOrder();
void sumBilling();
void writeBranch();
void writeOrder();

int main()
{
  int fileSize;      // Contador auxiliar para o número de linhas do arquivo
  CreateFile();       // Criar arquivos para armazenar os dados
  int arraySize = 50; // Definir tamanho inicial do vetor
  struct items *load_items = calloc(arraySize, sizeof(struct items));
  // Rodar menu principal para o usuário escolher um arquivo para ler
  while (1)
  {
    char choice[100];
    printf("[1] - Realizar carga\n[2] - Encerrar programa\n");
    printf("Escolha uma opção: ");
    scanf("%s", choice);
    while (strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0)
    {
      printf("Opção inválida. Digite novamente: ");
      scanf("%s", choice);
    }
    // Para quando o usuário escolher realizar carga
    if (!strcmp(choice, "1"))
    {
      char input[100];
      printf("Digite o nome do arquivo: ");
      scanf("%s", input);
      // Organiza o pedido em ordem
      fileSize = readCSV(input, &load_items, &arraySize);
      processOrder(load_items, fileSize);
      processBranch(load_items, fileSize);
      printf("Carga realizada com sucesso!\n");
    }
    // Para quando o usuário escolher encerrar programa
    else if (!strcmp(choice, "2"))
    {
      remove("teste.csv"); // Remover arquivo temporário
      break;
    }
  }
  free(load_items);
  return 0;
}

// Função para ler o arquivo e armazenar os dados em um vetor
int readCSV(char filename[100], struct items **array, int *arraySize)
{
  FILE *file = fopen(filename, "r");
  char buffer[255];
  int count = 0;
  // Caso não houver nada no arquivo
  if (file == NULL)
  {
    printf("Erro ao abrir o arquivo.\n");
    exit(1);
  }
  fgets(buffer, sizeof(buffer), file);
  // Abrir arquivo e ler linha por linha
  while (!feof(file))
  {
    // Saber a quantidade de linhas no arquivo para aumentar ou não a lista
    if (count == *arraySize)
    {
      increaseArrayItems(array, *arraySize);
      *arraySize *= 3;
    }
    fscanf(file, "%d,%ld,%d,%d,%d,%f\n", &(*array)[count].CodPedido, &(*array)[count].DtPedido, &(*array)[count].CodFilial, &(*array)[count].CodProduto, &(*array)[count].Qtd, &(*array)[count].VlUnit);
    count++;
  }
  fclose(file);
  return count;
}

// Função para aumentar o tamanho do vetor de items
void increaseArrayItems(struct items **listPont, int arraySize)
{
  *listPont = realloc(*listPont, (arraySize * 3) * sizeof(struct items));
  if (*listPont == NULL)
  {
    printf("Erro ao realocar memória.\n");
    exit(1);
  }
}

// Função para aumentar o tamanho do vetor de rel_order
void increaseArrayOrder(struct rel_order **listPont, int arraySize)
{
  *listPont = realloc(*listPont, (arraySize * 3) * sizeof(struct rel_order));
  if (*listPont == NULL)
  {
    printf("Erro ao realocar memória.\n");
    exit(1);
  }
}

// Função para aumentar o tamanho do vetor de rel_branch
void increaseArrayBranch(struct rel_branch **listPont, int arraySize)
{
  *listPont = realloc(*listPont, (arraySize * 3) * sizeof(struct rel_branch));
  if (*listPont == NULL)
  {
    printf("Erro ao realocar memória.\n");
    exit(1);
  }
}

// Função para gravar os dados em um arquivo csv dos pedidos
void writeOrder(struct rel_order *array, int arraySize, char type[1])
{
  FILE *file = fopen("rel_pedidos.csv", type);
  if (type == "w")
  {
    fprintf(file, "CodPedido,DtPedido,CodFilial,QtdTotalItens,TotalPedido\n");
  }

  for (int i = 0; i < arraySize; i++)
  {
    fprintf(file, "%d,%ld,%d,%d,%.2f\n", array[i].CodPedido, array[i].DtPedido, array[i].CodFilial, array[i].QtdTotalItens, array[i].TotalPedido);
  }
  fclose(file);
}

// Função para gravar os dados em um arquivo csv das filiais
void writeBranch(struct rel_branch *array, int arraySize, char type[1])
{
  FILE *file = fopen("rel_filiais.csv", type);
  if (type == "w")
  {
    fprintf(file, "CodFilial,TotalItemsVendidos,FatTotal\n");
  }
  for (int i = 0; i < arraySize; i++)
  {
    fprintf(file, "%d,%d,%.2f\n", array[i].CodFilial, array[i].TotalItensVendidos, array[i].FatTotal);
  }
  fclose(file);
}

// Função para criar ou verificar a existência de um arquivo com cabeçalho
void CreateFile()
{
  FILE *file = fopen("rel_pedidos.csv", "r");
  if (file == NULL)
  {
    file = fopen("rel_pedidos.csv", "w");
    fprintf(file, "CodPedido,DtPedido,CodFilial,QtdTotalItens,TotalPedido\n");
  }
  fclose(file);

  file = fopen("rel_filiais.csv", "r");
  if (file == NULL)
  {
    file = fopen("rel_filiais.csv", "w");
    fprintf(file, "CodFilial,TotalItemsVendidos,FatTotal\n");
  }
  fclose(file);

  file = fopen("teste.csv", "r");
  if (file == NULL)
  {
    file = fopen("teste.csv", "w");
    fprintf(file, "CodFilial,TotalItemsVendidos,FatTotal\n");
  }
  fclose(file);
}

// Função para fazer o somatório da qtd de itens x valor unitário e mostrar a qtd total de itens de um pedido
void processOrder(struct items *p, int fileSize)
{
  int arraySize = 50;
  struct rel_order *array = (struct rel_order *)calloc(arraySize, sizeof(struct rel_order));
  int count = 0;

  // Percorre o vetor de pedidos
  for (int i = 0; i < fileSize; i++)
  {
    if (count == arraySize)
    {
      increaseArrayOrder(&array, arraySize);
      arraySize *= 3;
    }
    int j;
    for (j = 0; j < count; j++)
    {
      // Soma os pedidos com o mesmo código
      if (array[j].CodPedido == p[i].CodPedido)
      {
        array[j].QtdTotalItens += p[i].Qtd;
        array[j].TotalPedido += p[i].Qtd * p[i].VlUnit;
        break;
      }
    }
    // Caso não tenha nenhum pedido com o mesmo código
    if (j == count)
    {
      array[count].CodPedido = p[i].CodPedido;
      array[count].DtPedido = p[i].DtPedido;
      array[count].CodFilial = p[i].CodFilial;
      array[count].QtdTotalItens = p[i].Qtd;
      array[count].TotalPedido = p[i].Qtd * p[i].VlUnit;
      count++;
    }
  }
  writeOrder(array, count, "a");
  free(array);
  copyFile("rel_pedidos.csv", "teste.csv");
  readCopyOrder();
}

// Função para fazer o somatório da qtd de itens x valor unitário e mostrar a qtd total de itens de uma filial
void processBranch(struct items *p, int fileSize)
{
  int arraySize = 50;
  struct rel_branch *array = (struct rel_branch *)calloc(arraySize, sizeof(struct rel_branch));
  int count = 0;

  // Percorre o vetor de pedidos
  for (int i = 0; i < fileSize; i++)
  {
    // Caso o vetor de filiais esteja cheio
    if (count == arraySize)
    {
      increaseArrayBranch(&array, arraySize);
      arraySize *= 3;
    }
    int j;
    for (j = 0; j < count; j++)
    {
      // Soma os pedidos com o mesmo código
      if (array[j].CodFilial == p[i].CodFilial)
      {
        array[j].TotalItensVendidos += p[i].Qtd;
        array[j].FatTotal += p[i].Qtd * p[i].VlUnit;
        break;
      }
    }
    // Caso não tenha nenhum pedido com o mesmo código
    if (j == count)
    {
      array[count].CodFilial = p[i].CodFilial;
      array[count].TotalItensVendidos = p[i].Qtd;
      array[count].FatTotal = p[i].Qtd * p[i].VlUnit;
      count++;
    }
  }
  writeBranch(array, count, "a");
  free(array);
  copyFile("rel_filiais.csv", "teste.csv");
  readCopyBranch();
}

// Função para copiar o conteúdo de um arquivo para outro
void copyFile(char fileName[100], char fileNameCopy[100])
{
  FILE *file = fopen(fileName, "r");
  FILE *file2 = fopen(fileNameCopy, "w");
  char buffer[255];
  int count = 0;
  // Caso não houver nada no arquivo
  if (file == NULL)
  {
    printf("Erro ao abrir o arquivo.\n");
    exit(1);
  }
  fgets(buffer, sizeof(buffer), file);
  // Abrir arquivo e ler linha por linha
  while (!feof(file))
  {
    fprintf(file2, "%s", buffer);
    fgets(buffer, sizeof(buffer), file);
  }
  fclose(file);
  fclose(file2);
}

// Função para fazer a leitura da cópia do arquivo
void readCopyOrder()
{
  // Inicializa o tamanho do array e aloca memória para o array de estruturas
  int arraySize = 50;
  struct rel_order *array = calloc(arraySize, sizeof(struct rel_order));
  FILE *file = fopen("teste.csv", "r");
  char buffer[255];
  int count = 0;
  // Caso não houver nada no arquivo
  if (file == NULL)
  {
    printf("Erro ao abrir o arquivo.\n");
    exit(1);
  }

  fgets(buffer, sizeof(buffer), file);
  // Abrir arquivo e ler linha por linha
  while (!feof(file))
  {
    // Saber a quantidade de linhas no arquivo para aumentar ou não a lista
    if (count == arraySize)
    {
      increaseArrayOrder(&array, arraySize);
      arraySize *= 3;
    }
    fscanf(file, "%d,%ld,%d,%d,%f\n", &array[count].CodPedido, &array[count].DtPedido, &array[count].CodFilial, &array[count].QtdTotalItens, &(array)[count].TotalPedido);
    count++;
  }
  fclose(file);
  sortByDate(array, count);
  writeOrder(array, count, "w");
  free(array);
}

// Função para ordenar os pedidos por data
void sortByDate(struct rel_order *p, int arraySize)
{
  int i, j;
  for (i = 0; i < arraySize; i++)
  {
    for (j = 0; j < arraySize - i - 1; j++)
    {
      // Se a data do pedido na posição atual é maior que a data do pedido na próxima posição
      if (p[j].DtPedido > p[j + 1].DtPedido)
      {
        changePositions(p, j, j + 1);
      }
    }
  }
}

// Função para mudar a posição dos pedidos
void changePositions(struct rel_order *p, int xp, int yp)
{
  struct rel_order aux;
  aux = p[xp];
  p[xp] = p[yp];
  p[yp] = aux;
}

// Função para fazer a leitura da cópia do arquivo da filial
void readCopyBranch()
{
  int arraySize = 50;
  struct rel_branch *array = calloc(arraySize, sizeof(struct rel_branch));
  FILE *file = fopen("teste.csv", "r");
  char buffer[255];
  int count = 0;
  // Caso não haja nada no arquivo
  if (file == NULL)
  {
    printf("Erro ao abrir o arquivo.\n");
    exit(1);
  }

  fgets(buffer, sizeof(buffer), file);
  // Abrir arquivo e ler linha por linha
  while (!feof(file))
  {
    // Saber a quantidade de linhas no arquivo para aumentar ou não a lista
    if (count == arraySize)
    {
      increaseArrayBranch(&array, arraySize);
      arraySize *= 3;
    }
    fscanf(file, "%d,%d,%f\n", &array[count].CodFilial, &array[count].TotalItensVendidos, &(array)[count].FatTotal);
    count++;
  }
  fclose(file);
  sumBilling(array, count);
  free(array);
}

// Função para fazer o somatório do faturamento de cada filial
void sumBilling(struct rel_branch *p, int fileSize)
{
  int arraySize = 50;
  struct rel_branch *array = (struct rel_branch *)calloc(arraySize, sizeof(struct rel_branch));
  int count = 0;

  // Percorre o vetor de pedidos
  for (int i = 0; i < fileSize; i++)
  {
    // Caso o vetor de filiais esteja cheio
    if (count == arraySize)
    {
      increaseArrayBranch(&array, arraySize);
      arraySize *= 3;
    }
    int j;
    for (j = 0; j < count; j++)
    {
      // Soma os pedidos com o mesmo código
      if (array[j].CodFilial == p[i].CodFilial)
      {
        array[j].TotalItensVendidos += p[i].TotalItensVendidos;
        array[j].FatTotal += p[i].FatTotal;
        break;
      }
    }
    // Caso não tenha nenhum pedido com o mesmo código
    if (j == count)
    {
      array[count].CodFilial = p[i].CodFilial;
      array[count].TotalItensVendidos = p[i].TotalItensVendidos;
      array[count].FatTotal = p[i].FatTotal;
      count++;
    }
  }
  writeBranch(array, count, "w");
  free(array);
}
