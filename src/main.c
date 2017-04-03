/*
  Arquivo: MAIN.C
  Autor: Roberto Bauer
  Observacoes: 
            Arquivo central do programa, funcaoh main()
            Cria uma tabela de dispersaoh para uma 
            listagem de palavras de um texto.
*/

#define __MAIN_C__

#include "main.h"

char *StrUpr (char *str)
{
    char *ptr = str;

    while (*ptr) {
        *ptr = toupper (*ptr);
        ptr++;
    }

    return (str);
}

#define TABLE_PRIME_SIZE            997 // total de recipientes, numero primo


int main(int argc, char *argv[])
{
    FILE *fin;                 // arquivo de entrada
    char strWord[64];          // bufer de leitura da palavra no arquivo
    char *ptrStr;
    int iChar;
    SHASHTABLE *sHT;


    if (argc != 2)
    {
        fprintf(stderr, "ERRO! Uso: executavel arquivo.ext\n");
        exit(EXIT_FAILURE);
    }

    fin = fopen(argv[1], "rt");
    if (fin) 
    {
        // cria uma estrutura de dados de dispersao encadeada
        sHT = InitHashTable(CreateData,
                            DeleteData,
                            DuplicatedNode,
                            NodeDataCmp,
                            TABLE_PRIME_SIZE, 
                            DataLen);
        if (sHT == NULL)
        {
            fprintf(stderr, "ERRO! Nao inicializou tabela de dispersaoh\n");
            exit(EXIT_FAILURE);
        }

        // comecah a processar o arquivo
        while (!feof(fin)) 
        {
            // percorre o texto pulando os espacosh e pontuncaoesh
            do {
                iChar = getc(fin);
            } while ( iChar != EOF && (isspace(iChar) || ispunct(iChar)));

            // tendo um caracter valido
            // comecah a montar a palavra
            ptrStr = strWord;
            do {
                *ptrStr++ = iChar;
                iChar = getc(fin);
            } while ( iChar != EOF && !isspace(iChar) && !ispunct(iChar) );

            // fim do arquivo, sai do loop
            if (iChar == EOF)
                break;

            // fecha a palavra com o terminador nulo
            *ptrStr = '\0';

            // todos os caracteres em maiuscula
            ptrStr = StrUpr(strWord);

            // insere na tabela
            AddDataToTable(sHT, ptrStr);

        }   // while (!feof(fin)) 

        fclose(fin);


        // mostra as estatistica de uso da tabela
        ListTableStat(sHT);

        // encerra a 
        EndHashTable(sHT);

        return (EXIT_SUCCESS);
    }
    else 
    {
        fprintf(stderr, "Naoh conseguiu encontrar/abrir o arquivo %s\n", argv[1]);
        return (EXIT_FAILURE);
    }

}


