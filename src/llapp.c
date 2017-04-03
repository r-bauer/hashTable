/*
  Arquivo: LLAPP.C
  Autor: Roberto Bauer
  Observacoes:
              Dados especificos da aplicacaoh p/ lista encadeada
*/

#define __LLAPP_C__

#include "main.h"


void *CreateData(void *data)
{
    SNODEDATA *pNewData;

    // aloca sua estrutura de dados
    pNewData = (SNODEDATA *) malloc(sizeof(SNODEDATA));
    if (pNewData == NULL)
        return (NULL);

    // move os valores para a estrutura de dados
    pNewData->uCont = 1;
    pNewData->pWord = strdup((char *) data);

    if (pNewData->pWord == NULL)     // erro copiando a string
    {
        free(pNewData);
        return (NULL);
    }
    else
    {
        return (pNewData);            // retorna o endereco da estrutura
    }
}

BOOL  DeleteData(void *data)
{
    // neste caso NodeData1 consiste em : um ponteiro e um inteiro.
    // O inteiro retorna para a memoria quando o noh eh liberado
    // Sendo nescessario liberar a string manualmente
    free( ((pND) data)->pWord);
    // e somente agora liberar a estrutura SNODEDATA1
    free( data );

    return TRUE;
}

// Estah funcaoh determina o que fazer quando se insere um noh na lista
// onde jah se encontra outro noh com a mesma informacaoh.
// Neste caso estamos contando as palavras, se uma palavra duplicada eh
// encontrada, incrementa-se o contador.
//
// Note que estah funcaoh deve retornar um dos seguintes valores
//      0   um erro aconteceu
//      1   apagar o noh duplicado
//      2   inserir o noh duplicado
// Qualquer outro processamento na informacaoh duplicada deve ser 
// feito dentro desta funcaoh.
//
int   DuplicatedNode(SLINK slkNewNode, SLINK slkListNode)
{
    pND pnd;

    // transforma o ponteiro de dados do noh, no ponteiro da aplicacaoh
    pnd = slkListNode->pdata;
    // adiciona-se uma ocorrencia para a palavra existente
    pnd->uCont += 1;

    return 1;
}

int   NodeDataCmp(void *first, void *second)
{
    return (strcmp( ((pND) first)->pWord, ((pND) second)->pWord));
}

int   DataLen(void *data)
{
    return ( (int) strlen( data ));
}

int PrintData(void *data)
{
    return printf("%s %u\t", ((pND) data)->pWord, ((pND) data)->uCont);
}
