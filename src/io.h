#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// numero primo para o tamanho da hash table
#define TABLE_SIZE 100003

//definicao de um ponteiro de funcao
typedef int (*FuncFiltro)(const char*, const char*, const char);

// struct do cabecalho de uma tabela csv
typedef struct categoriasMatriz{
	char tipo;
	char *categoria;
} categoriasMatriz;

// struct para o conteudo da tabela csv
typedef struct matrizCsv{
	int nLinhas;
	int *indLinhas;
	int nColunas;
	char ***matriz;
	categoriasMatriz *cabecalho;
} matrizCsv;

// struct para a estatistica dos dados da tabela csv
typedef struct descricaoDados{
	char tipo;
	int total;
	float media;
	float mediana;
	char* moda;
	int nModa;
	float desvio;
	float min;
	float max;
	int nValoresUn;
	char** valoresUn;
} descricaoDados;

// struct para o nodo de uma hashtable
typedef struct hashNodo{
	char *key;
    int count;
    struct hashNodo *prox;
} hashNodo;

// abre um arquivo csv e salva em uma matriz
matrizCsv* recebeTabela(char* nomeArquivo);

// abre uma tabela csv e conta quantas linhas tem (desconsiderando o cabecalho)
int contaLinhasCsv(char* nomeArquivo);

// abre uma tabela csv e conta quantas colunas tem
int contaColunasCsv(char* nomeArquivo);

// funcao de print para o sumario
void printaSumario(matrizCsv* cabeca);

// funcao de print para o conteudo da tabela csv
void printaMostrar(matrizCsv* cabeca);

// funcao para contar os digitos de um numero para o print do cabecalho
int contaDigitos(int n);

// encontra o tamanho da maior string em uma coluna csv para o print
int tamMaiorString(matrizCsv* cabeca, int col);

// funcao para filtrar dados em uma tabela csv
matrizCsv* filtrosCsv(matrizCsv* cabeca, FuncFiltro func);

// funcoes de comparaccao
int igual(const char* a, const char* b, const char tipo);

int diferente(const char* a, const char* b, const char tipo);

int maior(const char* a, const char* b, const char tipo);

int menor(const char* a, const char* b, const char tipo);

int maiorIgual(const char* a, const char* b, const char tipo);

int menorIgual(const char* a, const char* b, const char tipo);

// funcao que pega uma matrizCsv e registra em uma nova tabela
void salvaNovoCsv(matrizCsv* cabeca, char* nomeArquivo);

// insere uma nova linha na matrizCsv
void insereNaMatriz(matrizCsv* receber, matrizCsv* dar, int linha);

// insere uma nova coluna na matrizCsv
void insereNaMatrizCol(matrizCsv* receber, matrizCsv* dar, int coluna);

// seleciona colunas especificas e salva em uma nova matrizCsv
matrizCsv* selecao(matrizCsv* cabeca);

// calcula a estatistica dos dados de uma coluna em uma matrizCsv
descricaoDados* descreveDados(matrizCsv* cabeca);

// printa as estatisticas calculadas pela funcao acima
void printDescreveDados(descricaoDados* cabeca);

//obtem a media dos valores numericos de uma coluna em uma matrizCsv (so pode ser usada em coluna tipo 'N')
float obtemMediaCol(matrizCsv* cabeca, int col);

// funcao merge para ordenacao de uma matrizCsv baseada nos valores de uma coluna
void mergeMatriz(char ***matriz, int *indices, int left, int mid, int right, int col, int ascendente, char tipo);

// funcao principal do mergeSort para uma matrizCsv
void mergeSortMatriz(char ***matriz, int *indices, int left, int right, int col, int ascendente, char tipo);

// funcao que recebe a matriz e requisita a coluna-base que orientará a ordenacao da matriz
matrizCsv* ordenacao(matrizCsv* cabeca);

// retorna uma matriz apenas com as linhas que contenham algum "NaN"
matrizCsv* listarNaN(matrizCsv* cabeca);

// retorna uma matriz na qual os valores "NaN" sao substituidos pela media dos valores naquela coluna
matrizCsv* substituiPelaMedia(matrizCsv* cabeca);

// retorna uma matriz na qual os valores "NaN" sao substituidos pelo proximo valor valido daquela coluna
matrizCsv* substituiPeloProx(matrizCsv* cabeca);

// retorna uma matriz com todas as linhas que contenham "NaN" removidas
matrizCsv* removeNaN(matrizCsv* cabeca);

// funcao para segurar o buffer para a main
void pressioneEnterParaContinuar();

// funcao para limpar a memoria de uma matrizCsv
void freeMatrizCsv(matrizCsv *matriz);

// funcao para limpar a memoria do struct Descricao Dados
void freeDescricaoDados(descricaoDados *dados);

// funcao que faz comparacao entre 2 floats
int compare_floats(const void* a, const void* b);

// gera a hash para a hashtable
unsigned int hash(char *string);

// cria um nodo para a hashtable
hashNodo* createNode(char *key);

// insere um nodo em um hashtable
void insereHash(char *key);

// funcao para limpar a memoria do struct hashtable
void freeHashTable();
