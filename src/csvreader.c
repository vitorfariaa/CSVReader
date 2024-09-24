#include "io.h"

#include <stdio.h>

int main(int argc, char** argv){

	matrizCsv *principal = recebeTabela(argv[1]);

	if (principal == NULL){
		printf("Nome do arquivo invalido...\nInsira: ./main nomedoarquivo.csv\n");
		return 1;
	}

    int opcao;

    do {
        printf("1) Sumário do Arquivo\n");
        printf("2) Mostrar\n");
        printf("3) Filtros\n");
        printf("4) Descrição dos Dados\n");
        printf("5) Ordenação\n");
        printf("6) Seleção\n");
        printf("7) Dados Faltantes\n");
        printf("8) Salvar Dados\n");
        printf("9) Fim\n");
        printf("Escolha uma opção: ");

		int opcaoConfirm;

		do {
			opcaoConfirm = scanf("%d", &opcao);

			if (opcaoConfirm == 0) {
				printf("Entrada invalida, por favor insira um numero...\n");
				while (getchar() != '\n');
			}

		} while (opcaoConfirm != 1);

		opcaoConfirm = 0;

        switch (opcao) {
            case 1:{
                printaSumario(principal);
				pressioneEnterParaContinuar();
                break;
			}
            case 2:{
                printaMostrar(principal);
				pressioneEnterParaContinuar();
                break;
			}
			case 3:{

				FuncFiltro filtro = NULL;	
                matrizCsv *filtrada = filtrosCsv(principal, filtro);
				printaMostrar(filtrada);
				printf("Deseja gravar um arquivo com os dados filtrados? [S|N]: ");
				getchar();
				char option = getchar();

				if (option == 'S' || option == 's'){
					printf("Entre com o nome do Arquivo: ");
					char nomeTemp[1024];
					scanf("%s", nomeTemp);
					salvaNovoCsv(filtrada, nomeTemp);
				}

				getchar();
				printf("Deseja descartar os dados originais? [S|N]: ");
				option = getchar();

				if (option == 'S' || option == 's'){
					matrizCsv *aux = principal;
					principal = filtrada;
					freeMatrizCsv(aux);
				}
				else
					freeMatrizCsv(filtrada);

				pressioneEnterParaContinuar();
                break;
			}
            case 4:{
                descricaoDados *estatistica = descreveDados(principal);
				printDescreveDados(estatistica);
				freeDescricaoDados(estatistica);
				pressioneEnterParaContinuar();
                break;
			}
            case 5:{
                matrizCsv* ordenada = ordenacao(principal);
				printaMostrar(ordenada);
				printf("Deseja gravar um arquivo com os dados ordenados? [S|N]: ");
				getchar();
                char option = getchar();

                if (option == 'S' || option == 's'){
					printf("Entre com o nome do Arquivo: ");
                    char nomeTemp[1024];
                    scanf("%s", nomeTemp);
                    salvaNovoCsv(ordenada, nomeTemp);
                }

				getchar();
				printf("Deseja descartar os dados originais? [S|N]: ");
				option = getchar();

				if (option == 'S' || option == 's'){
					matrizCsv *aux = principal;
					principal = ordenada;
					freeMatrizCsv(aux);
				}
				else
					freeMatrizCsv(ordenada);

                pressioneEnterParaContinuar();
                break;
			}
            case 6:{
				getchar();
                matrizCsv* selecionada = selecao(principal);
				printaMostrar(selecionada);
				printf("Deseja gravar um arquivo com as variáveis selecionadas? [S|N]: ");
				getchar();
				char option = getchar();

				if (option == 'S' || option == 's'){
					printf("Entre com o nome do Arquivo: ");
					char nomeTemp[1024];
					scanf("%s", nomeTemp);
					salvaNovoCsv(selecionada, nomeTemp);
				}
			
				freeMatrizCsv(selecionada);
				pressioneEnterParaContinuar();
                break;
			}
            case 7:{
				int choice;
				do{
					matrizCsv* DadosF = NULL;
					printf("%-10s1) Listar registros com NaN\n", "");
					printf("%-10s2) Substituir pela media\n", "");
					printf("%-10s3) Substituir pelo proximo valor valido\n", "");
					printf("%-10s4) Remover registros com NaN\n", "");
					printf("%-10s5) Voltar ao menu principal\n", "");
					printf("%-10sDigite sua opcao: ", "");

					int choiceConfirm;

					do {
						choiceConfirm = scanf("%d", &choice);

						if (choiceConfirm == 0){
							printf("%-10sEntrada invalida, por favor insira um numero: ", "");
							while (getchar() != '\n');
						}

					} while(choiceConfirm != 1);

					choiceConfirm = 0;

					switch (choice) {
						case 1:{
							DadosF = listarNaN(principal);
							printaMostrar(DadosF);
							break;
						}
						
						case 2:{
							DadosF = substituiPelaMedia(principal);
							printaMostrar(DadosF);
							break;
						}

						case 3:{
							DadosF = substituiPeloProx(principal);
							printaMostrar(DadosF);
							break;
						}

						case 4:{
							DadosF = removeNaN(principal);
							printaMostrar(DadosF);
							break;
						}

						case 5:
							   break;

						default:
							   printf("%-10sNumero invalido, tente novamente...\n", "");
							   break;
					}

					if (DadosF != NULL){
                        printf("Deseja gravar um arquivo com os dados ordenados? [S|N]: ");
                        getchar();
                        char option = getchar();

                        if (option == 'S' || option == 's'){
                            printf("Entre com o nome do Arquivo: ");
                            char nomeTemp[1024];
                            scanf("%s", nomeTemp);
                            salvaNovoCsv(DadosF, nomeTemp);
                        }

                        getchar();
                        printf("Deseja descartar os dados originais? [S|N]: ");
                        option = getchar();

                        if (option == 'S' || option == 's'){
                            matrizCsv *aux = principal;
                            principal = DadosF;
                            freeMatrizCsv(aux);
                        }
                        else
                            freeMatrizCsv(DadosF);
					}
				}while (choice != 5);

				pressioneEnterParaContinuar();
                break;
			}
            case 8:{
                printf("Entre com o nome do Arquivo: ");
				char nomeTemp[1024];
				scanf("%s", nomeTemp);
				salvaNovoCsv(principal, nomeTemp);
				pressioneEnterParaContinuar();
				break;
			}
            case 9:
                printf("Programa encerrado!\n");
                break;
            default:
                printf("Opcao invalida, tente novamente\n");
        }
    } while (opcao != 9);

	freeMatrizCsv(principal);

    return 0;
}
