#include "io.h"
// hash table global para os calculos de moda
hashNodo* hashTable[TABLE_SIZE];

// funcao que recebe o nome de uma tabela Csv e aloca ela em uma struct em C (struct matrizCsv)
matrizCsv* recebeTabela(char *nomeArquivo){

	FILE* arquivo = fopen(nomeArquivo, "r");

	if (!arquivo)
		return NULL;

	// alocagem dos elementos necessarios para a matriz
	matrizCsv *retorno = (matrizCsv*)malloc(sizeof(matrizCsv));

	retorno->nLinhas = contaLinhasCsv(nomeArquivo);
	retorno->nColunas = contaColunasCsv(nomeArquivo);

	retorno->cabecalho = (categoriasMatriz*)malloc(retorno->nColunas * sizeof(categoriasMatriz));
	
	for (int i = 0; i < retorno->nColunas; i++)
		retorno->cabecalho[i].tipo = 'N';

	retorno->matriz = (char***)malloc(retorno->nLinhas * sizeof(char**));

	retorno->indLinhas = (int*)malloc(retorno->nLinhas * sizeof(int));


	for (int i = 0; i < retorno->nLinhas; i++){

		retorno->indLinhas[i] = i;

		retorno->matriz[i] = (char**)malloc(retorno->nColunas * sizeof(char*));

		for (int j = 0; j < retorno->nColunas; j++)
			retorno->matriz[i][j] = NULL;

	}

	char linha[1025];
	fgets(linha,1025,arquivo);

	char* keeper;
	char* search;
	int ind = 0;

	// le o cabecalho da matriz
	for (keeper = linha, search = strchr(linha,',');
		search; keeper = search+1, search = strchr(keeper, ',')){

		search[0] = 0;

		retorno->cabecalho[ind].categoria = strdup(keeper);
		ind++;
	}

	keeper[strcspn(keeper, "\n")] = '\0';
	retorno->cabecalho[ind].categoria = strdup(keeper);

	// le as linhas da matriz
	for (int i = 0; i < retorno->nLinhas; i++){

		fgets(linha,1025,arquivo);

		char* keeper;
		char* search;
		int j = 0;
			
		for (keeper = linha, search = strchr(linha,',');
			search; keeper = search+1, search = strchr(keeper, ',')){

			search[0] = 0;

			// se espaco tiver vazio, salva na matriz como "NaN" para facilitar proximos calculos
			if (!strcmp("", keeper))
				retorno->matriz[i][j] = strdup("NaN");

			else
				retorno->matriz[i][j] = strdup(keeper);

			for (int k = 0; keeper[k] != '\0'; k++){
				if ((!isdigit(keeper[k])) && keeper[k] != '.'){
					retorno->cabecalho[j].tipo = 'S';
					break;
				}
			}

			j++;
		}

		keeper[strcspn(keeper, "\n")] = '\0';

		if (!strcmp("", keeper))
			retorno->matriz[i][j] = strdup("NaN");
		else
			retorno->matriz[i][j] = strdup(keeper);

		for (int k = 0; keeper[k] != '\0'; k++){

			if (k == 0 && keeper[0] == '-')
				continue;

			else{
				if ((!isdigit(keeper[k])) && keeper[k] != '.' && strcmp(keeper, "\"\"") != 0) {
					retorno->cabecalho[j].tipo = 'S';
					break;
				}
			}
		}
	}

	fclose(arquivo);
	return retorno;

}

// conta o numero de linhas de uma tabela CSV
int contaLinhasCsv(char* nomeArquivo){
	char linha[1025];
	int retorno = 0;

	FILE* arquivo = fopen(nomeArquivo, "r");

	while (!feof(arquivo)){
		fgets(linha, 1025, arquivo);
		retorno++;
	}

	fclose(arquivo);
	return retorno-2;
}

// conta o numero de colunas de uma tabela CSV
int contaColunasCsv(char* nomeArquivo){
	char linha[1025];
	int retorno = 0;

	FILE* arquivo = fopen(nomeArquivo, "r");

	fgets(linha, 1025, arquivo);

	char* search;
	char* keeper;

	for (keeper = linha, search = strchr(linha, ',');
		search; keeper = search+1, search = strchr(keeper, ',')){

		retorno++;
	}

	fclose(arquivo);
	return retorno + 1;
}

int contaDigitos(int n) {
    int digitos = 0;
    if (n == 0)
		return 1;

    while (n != 0) {
        n /= 10;
        digitos++;
    }
    return digitos;
}

// printa o sumario de uma matrizCsv
void printaSumario(matrizCsv* cabeca){
	for (int i = 0; i < cabeca->nColunas; i++){
		printf("%s [%c]\n", cabeca->cabecalho[i].categoria, cabeca->cabecalho[i].tipo);
	}
}

// funcao de print de uma matrizCsv
void printaMostrar(matrizCsv* cabeca){

	if (cabeca && cabeca->nLinhas > 0){
		// printa o cabecalho
		int espacoInd = contaDigitos(cabeca->indLinhas[cabeca->nLinhas - 1]);
		printf("%-*s", espacoInd, "");
		for (int i = 0; i < cabeca->nColunas; i++){
			int space = tamMaiorString(cabeca, i);
			if (strlen(cabeca->cabecalho[i].categoria) + 2 > space)
                space = strlen(cabeca->cabecalho[i].categoria) + 2;
			printf("%*s", space, cabeca->cabecalho[i].categoria);
		}
		printf("\n");

		int linhas = cabeca->nLinhas;
		int colunas = cabeca->nColunas;

		// se tiver mais que 10 linhas, printa as primeiras 5 e ultimas 5 linhas
		if (linhas > 10){
			for (int i = 0; i < 5; i++){
				printf("%-*d", espacoInd, cabeca->indLinhas[i]);
				for (int j = 0; j < colunas; j++){
					int space = tamMaiorString(cabeca, j);
					if (strlen(cabeca->cabecalho[j].categoria) + 2 > space)
						space = strlen(cabeca->cabecalho[j].categoria) + 2;
					printf("%*s", space, cabeca->matriz[i][j]);
				}
				printf("\n");
			}
		
			printf("%-*s", espacoInd, "");

			for (int i = 0; i < colunas; i++){
				int space = tamMaiorString(cabeca, i);
				if (strlen(cabeca->cabecalho[i].categoria) + 2 > space)
                    space = strlen(cabeca->cabecalho[i].categoria) + 2;
				printf("%*s", space, "...");
			}

			printf("\n");

			for (int i = linhas-5; i < linhas; i++){
				printf("%-*d", espacoInd, cabeca->indLinhas[i]);
				for(int j = 0; j < colunas; j++){
					int space = tamMaiorString(cabeca, j);
					if (strlen(cabeca->cabecalho[j].categoria) + 2 > space)
                        space = strlen(cabeca->cabecalho[j].categoria) + 2;
					printf("%*s", space, cabeca->matriz[i][j]);
				}
				printf("\n");
			}
		}

		// se nao printa tudo
		else{
			for (int i = 0; i < linhas; i++){
				printf("%-*d", espacoInd, cabeca->indLinhas[i]);
				for (int j = 0; j < colunas; j++){
					int space = tamMaiorString(cabeca, j);
					if (strlen(cabeca->cabecalho[j].categoria) + 2 > space)
                        space = strlen(cabeca->cabecalho[j].categoria) + 2;
					printf("%*s", space, cabeca->matriz[i][j]);
				}
				printf("\n");
			}
		}

		printf("\n[%d rows x %d columns]\n\n", linhas, colunas);

	}
	else{
		printf("Empty DataFrame\n");
		printf("Columns: [");
		for (int i = 0; i < cabeca->nColunas; i++){
			if (i < cabeca->nColunas - 1)
				printf("%s, ", cabeca->cabecalho[i].categoria);
			else
				printf("%s", cabeca->cabecalho[i].categoria);
		}
		printf("]\n\n");
	}
}

// recebe uma matrizCsv e uma coluna e retorna o tamanho da maior string entre as 5 primeiras e 5 ultimas posicoes
int tamMaiorString(matrizCsv* cabeca, int col){
	int tamMax = 0;

	if (cabeca->nLinhas > 10){

		for (int i = 0; i < 5; i++){
			if (strlen(cabeca->matriz[i][col]) > tamMax)
				tamMax = strlen(cabeca->matriz[i][col]);
		}

		for (int i = cabeca->nLinhas - 5; i < cabeca->nLinhas; i++){
			if (strlen(cabeca->matriz[i][col]) > tamMax)
				tamMax = strlen(cabeca->matriz[i][col]);
		}
	}

	else{
		for (int i = 0; i < cabeca->nLinhas; i++){
			if (strlen(cabeca->matriz[i][col]) > tamMax)
				tamMax = strlen(cabeca->matriz[i][col]);
		}
	}		

	return tamMax + 2;
}


// funcoes de filtro
int igual(const char* a, const char* b, const char tipo){
	if (tipo == 'S')
		return strcmp(a, b) == 0;
	return atof(a) == atof(b);
}

int menor(const char* a, const char* b, const char tipo){
	if (strcmp(a, "NaN") == 0)
		return 1;
	
	if (tipo == 'S')
		return strcmp(a, b) < 0;
	return atof(a) < atof(b);
}

int maior(const char* a, const char* b, const char tipo){

	if (strcmp(b, "NaN") == 0)
		return 1;
	if (strcmp(a, "NaN") == 0)
		return 0;

	if (tipo == 'S')
		return strcmp(a, b) > 0;

	return atof(a) > atof(b);
}

int maiorIgual(const char* a, const char* b, const char tipo){
	if (strcmp(b, "NaN") == 0)
		return 1;
	if (strcmp(a, "NaN") == 0)
		return 0;	

	if (tipo == 'S')
		return strcmp(a, b) >= 0;

	return atof(a) >= atof(b);
}

int menorIgual(const char* a, const char* b, const char tipo){
	if (strcmp(a, "NaN") == 0)
		return 1;

	if (tipo == 'S')
		return strcmp(a, b) <= 0;

	return atof(a) <= atof(b);
}

int diferente(const char* a, const char* b, const char tipo){
    return strcmp(a, b) != 0;
}


matrizCsv* filtrosCsv(matrizCsv* cabeca, FuncFiltro func){
	
	// copia o cabecalho da matriz para uma matriz auxiliar
	matrizCsv *filtrada = (matrizCsv*)malloc(sizeof(matrizCsv));	
	filtrada->cabecalho = (categoriasMatriz*)malloc(sizeof(categoriasMatriz) * cabeca->nColunas);
	filtrada->indLinhas = (int*)malloc(cabeca->nLinhas * sizeof(int));
	filtrada->matriz = NULL;

	int indVar = -1;

	filtrada->nLinhas = 0;
	filtrada->nColunas = 0;


	for (int i = 0; i < cabeca->nColunas; i++){
		filtrada->cabecalho[i].tipo = cabeca->cabecalho[i].tipo;

		filtrada->cabecalho[i].categoria = strdup(cabeca->cabecalho[i].categoria);

		filtrada->nColunas++;

	}

	char vari[1025];
	printf("Insira o nome da variavel: ");
	scanf("%s", vari);
	printf("\n");

	for (int i = 0; i < cabeca->nColunas; i++){
		if (strcmp(vari, cabeca->cabecalho[i].categoria) == 0){
			indVar = i;
		}
	}

	if (indVar == -1){
		printf("Coluna selecionada INVALIDA...\n");
		return filtrada;
	}

	char filtro[3];
	printf("Insira o filtro (==, !=, >, <, >=, <=): ");
	scanf("%s", filtro);
	printf("\n");

	// encontra a funcao de filtro requisitada	
	if (strcmp(filtro, "==") == 0)
		func = igual; 

	else if (strcmp(filtro, "!=") == 0)
		func = diferente;

	else if (strcmp(filtro, ">") == 0)
		func = maior;

	else if (strcmp(filtro, "<") == 0)
		func = menor;

	else if (strcmp(filtro, ">=") == 0)
		func = maiorIgual;

	else if (strcmp(filtro, "<=") == 0)
		func = menorIgual;

	else{
		printf("Filtro Invalido...\n");
		return filtrada;
	}
	char valor[1024];
	printf("Insira o valor: ");
	scanf("%s", valor);

	for (int i = 0; i < cabeca->nLinhas; i++){
		if (func(cabeca->matriz[i][indVar], valor, cabeca->cabecalho[indVar].tipo)){
			insereNaMatriz(filtrada,cabeca,i);
		}
	}

	return filtrada;
	
}

// cria e salva novo arquivo csv com o nome recebido
void salvaNovoCsv(matrizCsv* cabeca, char* nomeArquivo){

	FILE* arquivo;

	arquivo = fopen(nomeArquivo, "w");

	if (!arquivo){
		printf("Erro ao salvar o arquivo, abortando...\n");
		return;
	}

	for (int i = 0; i < cabeca->nColunas; i++){
		fprintf(arquivo, "%s", cabeca->cabecalho[i].categoria);
		if (i + 1 != cabeca->nColunas)
			fprintf(arquivo, ",");
	}

	fprintf(arquivo, "\n");

	for (int i = 0; i < cabeca->nLinhas; i++){
		for (int j = 0; j < cabeca->nColunas; j++){
			if (strcmp(cabeca->matriz[i][j], "NaN") != 0)
				fprintf(arquivo, "%s",cabeca->matriz[i][j]);
			if (j + 1 != cabeca->nColunas)
				fprintf(arquivo, ",");
		}
		fprintf(arquivo, "\n");
	}
	fclose(arquivo);
	printf("Arquivo gravado com sucesso\n");
}

//funcao que insere uma nova LINHA em uma matriz
void insereNaMatriz(matrizCsv* receber, matrizCsv* dar, int linha){

    char ***novaMatriz = (char***)realloc(receber->matriz, (receber->nLinhas + 1) * sizeof(char**));
    if (novaMatriz == NULL){
        printf("falha na alocacao de memoria\n");
        return;
    }
    receber->matriz = novaMatriz;

    // aloca as colunas da nova linha
    receber->matriz[receber->nLinhas] = (char**)malloc(dar->nColunas * sizeof(char*));
    if (receber->matriz[receber->nLinhas] == NULL){
        printf("falha na alocacao de memoria\n");
        return;
    }

	receber->indLinhas[receber->nLinhas] = dar->indLinhas[linha];

    for (int i = 0; i < dar->nColunas; i++){
        receber->matriz[receber->nLinhas][i] = strdup(dar->matriz[linha][i]);
    }

    receber->nLinhas++;
}

//funcao que insere uma nova COLUNA em uma matriz
void insereNaMatrizCol(matrizCsv* receber, matrizCsv* dar, int coluna){

    for (int i = 0; i < dar->nLinhas; i++){
		// aloca as linhas da nova coluna
        char **novaLinha = (char**)realloc(receber->matriz[i], (receber->nColunas + 1) * sizeof(char*));
        if (novaLinha == NULL) {
            printf("falha na alocacao de memoria %d.\n", i);
            return;
        }
        receber->matriz[i] = novaLinha;

        receber->matriz[i][receber->nColunas] = strdup(dar->matriz[i][coluna]);
        if (receber->matriz[i][receber->nColunas] == NULL){
            printf("falha na alocacao de memoria %d.\n", i);
            return;
        }
    }
    receber->nColunas++; 
}

// funcao que copia apenas colunas escolhidas para uma nova matriz
matrizCsv* selecao(matrizCsv* cabeca){
    matrizCsv* resultado = (matrizCsv*)malloc(sizeof(matrizCsv));
    resultado->cabecalho = (categoriasMatriz*)malloc(cabeca->nColunas * sizeof(categoriasMatriz));

    resultado->nLinhas = cabeca->nLinhas;
    resultado->nColunas = 0;

    resultado->matriz = (char***)malloc(cabeca->nLinhas * sizeof(char**));
	resultado->indLinhas = (int*)malloc(cabeca->nLinhas * sizeof(int));

    for (int i = 0; i < cabeca->nLinhas; i++){
        resultado->matriz[i] = NULL;
    }

    printf("Entre com as variaveis que deseja selecionar (separadas por espaço): ");
    char input[1024];

    if (fgets(input, 1024, stdin) != NULL){
        input[strcspn(input, "\n")] = 0;

        char *variable = strtok(input, " ");
		// loop com a string separando seus espaços em branco " "
        while (variable != NULL){
            int indCol = -1;
            for (int i = 0; i < cabeca->nColunas; i++){
                if (strcmp(cabeca->cabecalho[i].categoria, variable) == 0){
                    indCol = i;
                    break;
                }
            }

			// se a coluna existir, copia ela para a nova matriz
            if (indCol != -1){
                resultado->cabecalho[resultado->nColunas].categoria = strdup(cabeca->cabecalho[indCol].categoria);
				resultado->cabecalho[resultado->nColunas].tipo = cabeca->cabecalho[indCol].tipo;
                insereNaMatrizCol(resultado, cabeca, indCol);
            }
			else
				printf("COLUNA: %s INVALIDA\n", variable);

            variable = strtok(NULL, " ");
        }
    } 
	else {
        printf("Erro ao processar entrada\n");
        return NULL;
    }

	if (resultado->nColunas > 0){
		for (int i = 0; i < resultado->nLinhas; i++)
			resultado->indLinhas[i] = cabeca->indLinhas[i];
	}
	else
		resultado->nLinhas = 0;

    return resultado;
}       

// funcao que recebe uma matriz e devolve as estatisticas sobre uma coluna daquela matriz
descricaoDados* descreveDados(matrizCsv *cabeca){
	descricaoDados* resultado = (descricaoDados*)malloc(sizeof(descricaoDados));
	
	char vari[1024];
	printf("Entre com a variavel: ");
	scanf("%s", vari);
	printf("\n");
	
	int ind = -1;

	for (int i = 0; i < cabeca->nColunas; i++){
		if (strcmp(cabeca->cabecalho[i].categoria, vari) == 0){
			ind = i;
			resultado->tipo = cabeca->cabecalho[i].tipo;
		}
	}

	if (ind == -1){
		printf("Variavel invalida\n");
		return NULL;
	}

	resultado->total = 0;
	for (int i = 0; i < cabeca->nLinhas; i++){
		if (strcmp(cabeca->matriz[i][ind], "NaN") != 0)
			resultado->total++;
	}

	if (resultado->tipo == 'N'){

		float media = obtemMediaCol(cabeca, ind);

		resultado->media = media;

		float desvioP = 0;
		for (int i = 0; i < cabeca->nLinhas; i++){
			if (strcmp(cabeca->matriz[i][ind], "NaN") != 0) {
				float diff = atof(cabeca->matriz[i][ind]) - media;
				desvioP += diff * diff;
			}
		}
		desvioP = desvioP / resultado->total;
		desvioP = sqrt(desvioP);

		resultado->desvio = desvioP;

		float min = atof(cabeca->matriz[0][ind]);
		float max = atof(cabeca->matriz[0][ind]);

		for (int i = 0; i < cabeca->nLinhas; i++){
			if (atof(cabeca->matriz[i][ind]) > max)
				max = atof(cabeca->matriz[i][ind]);

			if (atof(cabeca->matriz[i][ind]) < min && strcmp(cabeca->matriz[i][ind], "NaN") != 0)
				min = atof(cabeca->matriz[i][ind]);
		}

		resultado->min = min;
		resultado->max = max;

		float vetorAux[cabeca->nLinhas];
		int tamVetor = 0;

		for (int i = 0; i < cabeca->nLinhas; i++){
			if (strcmp(cabeca->matriz[i][ind], "NaN") != 0){
				vetorAux[tamVetor] = atof(cabeca->matriz[i][ind]);
				tamVetor++;
			}
		}

		// funcao qsort padrao da stdlib, uso para encontrar a mediana
		qsort(vetorAux, tamVetor, sizeof(vetorAux[0]), compare_floats);


		if (tamVetor % 2 == 1)
			resultado->mediana = vetorAux[tamVetor / 2];

		else
			resultado->mediana = (vetorAux[(tamVetor / 2) - 1] + vetorAux[tamVetor / 2]) / 2.0;

	}
		
	memset(hashTable, 0, sizeof(hashTable));
	resultado->moda = (char*)malloc(1024 * sizeof(char));

	// insere os valores na hash table para operacoes mais pesadas (como a moda)
	for (int i = 0; i < cabeca->nLinhas; i++){
		if (strcmp(cabeca->matriz[i][ind], "NaN") != 0){
			insereHash(cabeca->matriz[i][ind]);
		}	
	}

	// encontra a moda com a hash table
	
	int maxCount = 0;
    hashNodo *modaNodo = NULL;

    for (int i = 0; i < TABLE_SIZE; i++){
        hashNodo *novo = hashTable[i];
        while (novo != NULL) {
            if (novo->count > maxCount){
                maxCount = novo->count;
                modaNodo = novo;
            }
            novo = novo->prox;
        }
    }
	
	if (modaNodo != NULL){
		sprintf(resultado->moda, "%s", modaNodo->key);
		resultado->nModa = modaNodo->count;
	}

	// encontra valores unicos tambem com o uso da hash table
	int capacity = 10;
	int tam = 0;
	char **valoresDistintos = (char**)malloc(capacity * sizeof(char*));

	for (int i = 0; i < TABLE_SIZE; i++){
		hashNodo *novo = hashTable[i];
		while (novo != NULL){
			if (tam >= capacity){
				capacity *= 2; // dobra a capacidade do vetor de numeros exclusivos se necessario
				valoresDistintos = (char**)realloc(valoresDistintos, capacity * sizeof(char*));
			}
			valoresDistintos[tam++] = strdup(novo->key);
			novo = novo->prox;
		}
	}

	resultado->valoresUn = valoresDistintos;
	resultado->nValoresUn = tam;

	freeHashTable();
	return resultado;
}

// funcao de print para a struct DescreveDados
void printDescreveDados(descricaoDados* cabeca){
	if (cabeca == NULL){
		printf("Nenhum dado encontrado\n");
		return;
	}

	// print para dados numeros
	if (cabeca->tipo == 'N'){
		printf("Contador: %d\n", cabeca->total);
		printf("Media: %.2f\n", cabeca->media);
		printf("Desvio: %.2f\n", cabeca->desvio);
		printf("Mediana: %.2f\n", cabeca->mediana);
		printf("Moda: %s %d vezes\n", cabeca->moda, cabeca->nModa);
		printf("Min: %.2f\n", cabeca->min);
		printf("Max: %.2f\n", cabeca->max);
		
		printf("Valores unicos: [");

		for (int i = 0; i < cabeca->nValoresUn; i++){
			if (i < cabeca->nValoresUn - 1)
				printf("%s, ", cabeca->valoresUn[i]);
			else
				printf("%s", cabeca->valoresUn[i]);
		}

		printf("]\n");
            
	}

	// print para dados "string"
	else{
		printf("Contador: %d\n", cabeca->total);
		printf("Moda: %s %d vezes\n", cabeca->moda, cabeca->nModa);

		printf("Valores unicos: [");

		for (int i = 0; i < cabeca->nValoresUn; i++){
			if (i < cabeca->nValoresUn - 1)
				printf("%s, ", cabeca->valoresUn[i]);
			else
				printf("%s", cabeca->valoresUn[i]);
		}

		printf("]\n");
	}

}

// obtem a media dos valores numericos de uma coluna da matriz
float obtemMediaCol(matrizCsv* cabeca, int col){
	float media = 0;
	int total = 0;

	for (int i = 0; i < cabeca->nLinhas; i++){
		if (strcmp(cabeca->matriz[i][col], "NaN") != 0){
			media += atof(cabeca->matriz[i][col]);
			total++;
		}
	}

	return media/total;
}


matrizCsv* ordenacao(matrizCsv* cabeca){

	char variavel[1024];
	int ordem = 0;

	printf("Entre com a variavel: ");
	scanf("%s", variavel);
	while (getchar() != '\n');

	// faz uma copia da matriz principal
	matrizCsv* novo = (matrizCsv*)malloc(sizeof(matrizCsv));
	novo->cabecalho = (categoriasMatriz*)malloc(cabeca->nColunas * sizeof(categoriasMatriz));
	novo->indLinhas = (int*)malloc(cabeca->nLinhas * sizeof(int));
	novo->matriz = NULL;

	novo->nLinhas = 0;
	novo->nColunas = 0;

	for (int i = 0; i < cabeca->nColunas; i++){
		novo->cabecalho[i].tipo = cabeca->cabecalho[i].tipo;

		novo->cabecalho[i].categoria = strdup(cabeca->cabecalho[i].categoria);

		novo->nColunas++;
	}

	int indVar = -1;

	// encontra a coluna escolhida para a ordenacao
	for (int i = 0; i < cabeca->nColunas; i++){
		if (strcmp(variavel, cabeca->cabecalho[i].categoria) == 0)
			indVar = i;
	}

	if (indVar == -1){
		printf("Coluna para ordenacao INVALIDA...\n");
		return novo;
	}

	for (int i = 0; i < cabeca->nLinhas; i++){
		insereNaMatriz(novo, cabeca, i);
	}
	
	printf("Selecione uma opcao [A]scendente ou [D]ecrescente: ");
	char confirm = getchar();
	
	if (confirm == 'A' || confirm == 'a')
		ordem = 1;

	// ordena de acordo com a coluna e opcao escolhida
	mergeSortMatriz(novo->matriz, novo->indLinhas, 0, novo->nLinhas - 1, indVar, ordem, novo->cabecalho[indVar].tipo);	

	return novo;
}

// faz uma copia da matriz, mas salvando APENAS as linhas que contem algum "NaN"
matrizCsv* listarNaN(matrizCsv *cabeca){
	matrizCsv *resultado = (matrizCsv*)malloc(sizeof(matrizCsv));
	resultado->cabecalho = (categoriasMatriz*)malloc(cabeca->nColunas * sizeof(categoriasMatriz));
	resultado->indLinhas = (int*)malloc(cabeca->nLinhas * sizeof(int));
	resultado->matriz = NULL;

	resultado->nLinhas = 0;
	resultado->nColunas = 0;

	for (int i = 0; i < cabeca->nColunas; i++){
		resultado->cabecalho[i].tipo = cabeca->cabecalho[i].tipo;

		resultado->cabecalho[i].categoria = strdup(cabeca->cabecalho[i].categoria);

		resultado->nColunas++;
	}

	for (int i = 0; i < cabeca->nLinhas; i++){
		for (int j = 0; j < cabeca->nColunas; j++){
			if (strcmp(cabeca->matriz[i][j], "NaN") == 0){
				insereNaMatriz(resultado, cabeca, i);
				break;
			}
		}
	}

	return resultado;
}

// faz uma copia da matriz, ignorando as linhas que contem algum registro "NaN"
matrizCsv* removeNaN(matrizCsv *cabeca){
	matrizCsv *resultado = (matrizCsv*)malloc(sizeof(matrizCsv));
	resultado->cabecalho = (categoriasMatriz*)malloc(cabeca->nColunas * sizeof(categoriasMatriz));
	resultado->indLinhas = (int*)malloc(cabeca->nLinhas * sizeof(int));
	resultado->matriz = NULL;

	resultado->nLinhas = 0;
	resultado->nColunas = 0;

	for (int i = 0; i < cabeca->nColunas; i++){
		resultado->cabecalho[i].tipo = cabeca->cabecalho[i].tipo;

		resultado->cabecalho[i].categoria = strdup(cabeca->cabecalho[i].categoria);

		resultado->nColunas++;
	}

	for (int i = 0; i < cabeca->nLinhas; i++){
		int confirmation = 0;

		for (int j = 0; j < cabeca->nColunas; j++){
			if (strcmp(cabeca->matriz[i][j], "NaN") == 0)
				confirmation = 1;
		}

		if (confirmation == 0)
			insereNaMatriz(resultado, cabeca, i);
	}

	return resultado;
}

// funcao que substitui um valor "NaN" pelo proximo valor valido da coluna
matrizCsv* substituiPelaMedia(matrizCsv *cabeca){
	matrizCsv *resultado = (matrizCsv*)malloc(sizeof(matrizCsv));
	resultado->cabecalho = (categoriasMatriz*)malloc(cabeca->nColunas * sizeof(matrizCsv));
	resultado->indLinhas = (int*)malloc(cabeca->nLinhas * sizeof(int));
	resultado->matriz = NULL;

	resultado->nLinhas = 0;
	resultado->nColunas = 0;

	for (int i = 0; i < cabeca->nColunas; i++){
		resultado->cabecalho[i].tipo = cabeca->cabecalho[i].tipo;

		resultado->cabecalho[i].categoria = strdup(cabeca->cabecalho[i].categoria);

		resultado->nColunas++;
	}

	for (int i = 0; i < cabeca->nLinhas; i++)
		insereNaMatriz(resultado,cabeca,i);

	for (int j = 0; j < cabeca->nColunas; j++){
		// encontra a media da coluna
		float mediaCol = obtemMediaCol(cabeca, j);
		for (int i = 0; i < cabeca->nLinhas; i++){
			if (strcmp(resultado->matriz[i][j], "NaN") == 0){
				// caso exista um "NaN" na coluna, sera substituido pela media
				resultado->matriz[i][j] = (char*)realloc(resultado->matriz[i][j], 32);
				sprintf(resultado->matriz[i][j], "%.2f", mediaCol);
			}
		}
	}

	return resultado;
}

// funcao que substitui um valor NaN pelo proximo valor valido da coluna
matrizCsv* substituiPeloProx(matrizCsv* cabeca){
	matrizCsv *resultado = (matrizCsv*)malloc(sizeof(matrizCsv));
	resultado->cabecalho = (categoriasMatriz*)malloc(cabeca->nColunas * sizeof(matrizCsv));
	resultado->indLinhas = (int*)malloc(cabeca->nLinhas * sizeof(int));
	resultado->matriz = NULL;

	resultado->nLinhas = 0;
	resultado->nColunas = 0;

	for (int i = 0; i < cabeca->nColunas; i++){
		resultado->cabecalho[i].tipo = cabeca->cabecalho[i].tipo;
		
		resultado->cabecalho[i].categoria = strdup(cabeca->cabecalho[i].categoria);

		resultado->nColunas++;
	}

	for (int i = 0; i < cabeca->nLinhas; i++)
		insereNaMatriz(resultado,cabeca,i);


	for (int i = 0; i < cabeca->nLinhas; i++){
		for (int j = 0; j < cabeca->nColunas; j++){
			if (strcmp(resultado->matriz[i][j], "NaN") == 0 && i < (cabeca->nLinhas - 1)){
				resultado->matriz[i][j] = (char*)realloc(resultado->matriz[i][j], 32);
				int k = i+1;
				// encontra o prox valor valido
				while (strcmp(resultado->matriz[k][j], "NaN") == 0 && k < cabeca->nLinhas - 1)
					k++;
				
				sprintf(resultado->matriz[i][j], "%s", resultado->matriz[k][j]);
			}
		}
	}

	return resultado;
}

// funcao de free para a struct matrizCsv e todos os elementos alocados dinamicamente nela
void freeMatrizCsv(matrizCsv *matriz){
    if (matriz != NULL){
        if (matriz->matriz != NULL){
            for (int i = 0; i < matriz->nLinhas; i++){
                for (int j = 0; j < matriz->nColunas; j++){
                    free(matriz->matriz[i][j]);
                }
                free(matriz->matriz[i]);
            }
            free(matriz->matriz);
        }
        
        if (matriz->indLinhas != NULL){
            free(matriz->indLinhas);
        }

        if (matriz->cabecalho != NULL){
            for (int i = 0; i < matriz->nColunas; i++){
                free(matriz->cabecalho[i].categoria);
            }
            free(matriz->cabecalho);
        }

        free(matriz);
    }
}

// funcao de free para a struct descricaoDados
void freeDescricaoDados(descricaoDados *dados){
    if (dados != NULL){
		free(dados->moda);
        if (dados->valoresUn != NULL){
            for (int i = 0; i < dados->nValoresUn; i++){
                free(dados->valoresUn[i]);
            }
            free(dados->valoresUn);
			dados->valoresUn = NULL;
        }

        free(dados);
		dados = NULL;
    }
}

// funcao merge, a mais importante para o funcionamento do mergesort
void mergeMatriz(char ***matriz, int *indices, int left, int mid, int right, int col, int ascendente, char tipo){
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // criacao de matrizes temporarias
    char ***matrizLeft = malloc(n1 * sizeof(char **));
    char ***matrizRight = malloc(n2 * sizeof(char **));
    int *leftIndices = malloc(n1 * sizeof(int));
    int *rightIndices = malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++) {
        matrizLeft[i] = matriz[left + i];
        leftIndices[i] = indices[left + i];
    }
    for (j = 0; j < n2; j++) {
        matrizRight[j] = matriz[mid + 1 + j];
        rightIndices[j] = indices[mid + 1 + j];
    }

    i = 0;
    j = 0;
    k = left;

	// se a coluna for do tipo string, ordenacao baseada no strcmp
    if (tipo == 'S'){
        while (i < n1 && j < n2){
            if ((ascendente && strcmp(matrizLeft[i][col], matrizRight[j][col]) <= 0) ||
                    (!ascendente && strcmp(matrizLeft[i][col], matrizRight[j][col]) > 0)){
                matriz[k] = matrizLeft[i];
                indices[k] = leftIndices[i];
                i++;
            } 
			else{
                matriz[k] = matrizRight[j];
                indices[k] = rightIndices[j];
                j++;
            }
            k++;
        }
    } 
	else{
		while (i < n1 && j < n2){

			// na ordenacao, NaN considerados maiores
			int i_nan = (strcmp(matrizLeft[i][col], "NaN") == 0);

			int j_nan = (strcmp(matrizRight[j][col], "NaN") == 0);

			if (i_nan && !j_nan){
				if (ascendente){
					matriz[k] = matrizRight[j];
					indices[k] = rightIndices[j];
					j++;
				}
				else{
					matriz[k] = matrizLeft[i];
					indices[k] = leftIndices[i];
					i++;
				}
			} 
			else if (!i_nan && j_nan){
				if (ascendente){
					matriz[k] = matrizLeft[i];
					indices[k] = leftIndices[i];
					i++;
				}
				else{
					matriz[k] = matrizRight[j];
					indices[k] = rightIndices[j];
					j++;
				}
			} 
			else if (i_nan && j_nan){
				matriz[k] = matrizLeft[i];
				indices[k] = leftIndices[i];
				i++;

			} 
			else{
				// se nenhum dos elementos da comparacao eh NaN, comparacao feita com funcao atof
				if ((ascendente && atof(matrizLeft[i][col]) <= atof(matrizRight[j][col])) ||
					(!ascendente && atof(matrizLeft[i][col]) > atof(matrizRight[j][col]))) {
					matriz[k] = matrizLeft[i];
					indices[k] = leftIndices[i];
					i++;
				} 
				else{
					matriz[k] = matrizRight[j];
					indices[k] = rightIndices[j];
					j++;
				}
            }

			k++;
        }
    }

    // copia elementos restantes da matriz auxiliar da esquerda
    while (i < n1){
        matriz[k] = matrizLeft[i];
        indices[k] = leftIndices[i];
        i++;
        k++;
    }

    // copia elementos restantes da matriz auxiliar da direita
    while (j < n2) {
        matriz[k] = matrizRight[j];
        indices[k] = rightIndices[j];
        j++;
        k++;
    }

    // free nas matrizes auxiliares
    free(matrizLeft);
    free(matrizRight);
    free(leftIndices);
    free(rightIndices);
}

// funcao merge sort para a matriz, utilizando da auxiliar "merge"
void mergeSortMatriz(char ***matriz, int *indices, int left, int right, int col, int ascendente, char tipo){
    if (left < right) {
        int mid = left + (right - left) / 2;

        // ordena a primeira e segunda metades
        mergeSortMatriz(matriz, indices, left, mid, col, ascendente, tipo);
        mergeSortMatriz(matriz, indices, mid + 1, right, col, ascendente, tipo);

        // da "merge" nas metades ordenadas
        mergeMatriz(matriz, indices, left, mid, right, col, ascendente, tipo);
    }
}

//funcao para a main
void pressioneEnterParaContinuar(){
    printf("\nPressione ENTER para continuar\n");
    while (getchar() != '\n'); // limpa o buffer
    getchar(); // aguarda apertar enter
}

//funcao de comparacao simples para a funcao padrao qsort
int compare_floats(const void* a, const void* b){
    int arg1 = *(const float*)a;
    int arg2 = *(const float*)b;

    if(arg1 < arg2) return -1;
    if(arg1 > arg2) return 1;
    return 0;
}

// hash com metodo Knuth
unsigned int hash(char *string){
    unsigned int hash = 0;
    while (*string){
        hash = (hash * 2654435761) + (*string++);
    }
    return hash % TABLE_SIZE;
}

//aloca e cria novo nodo para a hashtable
hashNodo* criaNodo(char *key){
    hashNodo *novo = (hashNodo*)malloc(sizeof(hashNodo));
    novo->key = strdup(key);
    novo->count = 1;
    novo->prox = NULL;
    return novo;
}

// insere nodo na hashtable global
void insereHash(char *key){
    unsigned int index = hash(key);
    hashNodo *novo = hashTable[index];

    while (novo != NULL) {
        if (strcmp(novo->key, key) == 0) {
            novo->count++;
            return;
        }
        novo = novo->prox;
    }

    novo = criaNodo(key);
    novo->prox = hashTable[index];
    hashTable[index] = novo;
}

// da free na hash table
void freeHashTable(){
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashNodo *novo = hashTable[i];
        while (novo != NULL){
            hashNodo *temp = novo;
            novo = novo->prox;
            free(temp->key);  // liberar tambem a key da hash
            free(temp); 
        }
    }
    memset(hashTable, 0, sizeof(hashTable));
}
