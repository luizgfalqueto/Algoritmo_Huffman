#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Procura por um caracter no vetor (retorna o indice do vetor caso encontrar ou -1 se encontrar)
int isInArray(char *vetor, char c)
{
    int tam = strlen(vetor);
    for (int i = 0; i < tam; i++)
    {
        if (vetor[i] == c)
            return i;
    }
    return -1;
}

void organizaVetores(char *vetorC, int *vetorI)
{
    int i, j, auxI;
    char auxC;
    int tam = strlen(vetorC);
    for (i = 0; i < tam - 1; i++)
    {
        for (j = i + 1; j < tam; j++)
        {
            if (vetorI[i] > vetorI[j])
            {
                auxI = vetorI[i];
                vetorI[i] = vetorI[j];
                vetorI[j] = auxI;

                auxC = vetorC[i];
                vetorC[i] = vetorC[j];
                vetorC[j] = auxC;
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    FILE *pont_arq = fopen("arquivo.txt", "rb");
    if (!pont_arq)
    {
        printf("Erro ao ler arquivo!\n");
        fclose(pont_arq);
        exit(1);
    }
    else
    {
        printf("Arquivo lido com sucesso!\n\n");
    }

    //------------------------------------------ Vetor com caracteres do arquivo ---------------------------
    //------------------------------------------------------------------------------------------------------
    int *frequencia;
    char *caracteresArquivo;

    int tamVet = 1;
    caracteresArquivo = (char *)malloc(tamVet * sizeof(char));
    frequencia = (int *)malloc(tamVet * sizeof(int));

    if (!caracteresArquivo || !frequencia)
    {
        printf("Erro ao alocar memória!\n");
        fclose(pont_arq);
        exit(1);
    }

    int i = 0, j = 0, index;
    char aux;
    while (fscanf(pont_arq, "%c", &aux) != EOF)
    {
        if (aux != '\n')
        {
            if (tamVet > 1) //Se o vetor ja tem algum caracter inserido
            {
                index = isInArray(caracteresArquivo, aux); //Retornou o indice do caracter no vetor
                if (index >= 0)
                {
                    frequencia[index]++;
                }
                else //Caracter não existe no vetor (adicionar no vetor)
                {
                    caracteresArquivo = (char *)realloc(caracteresArquivo, (tamVet + 1) * sizeof(char));
                    frequencia = (int *)realloc(frequencia, (tamVet + 1) * sizeof(int));
                    caracteresArquivo[j] = aux;
                    frequencia[j] = 1;
                    tamVet++;
                    j++;
                }
            }
            else
            {
                caracteresArquivo = (char *)realloc(caracteresArquivo, (tamVet + 1) * sizeof(char));
                frequencia = (int *)realloc(frequencia, (tamVet + 1) * sizeof(int));
                caracteresArquivo[j] = aux;
                frequencia[j] = 1;
                tamVet++;
                j++;
            }
        }
    }

    organizaVetores(caracteresArquivo, frequencia);

    for (int i = 0; i < tamVet - 1; i++)
    {
        printf("%c -> %d\n", caracteresArquivo[i], frequencia[i]);
    }
    printf("\n");

    printf("\n");
    fclose(pont_arq);
    free(frequencia);
    free(caracteresArquivo);
    return 0;
}