/*
  Arquivo: HASHTAB.C
  Autor: Roberto Bauer
  Observacoes:
              Rotinas de uma estrutura de dados 
              para tabela de dispersaoh encadeada, como um superconjunto 
              da lista encadeada generica.
*/

#define __HASHTAB_C__

#include "main.h"



/////// HASHING  //////


PRIVATE WORD HashDiv(DWORD dwKey, int iSize)
{
    return (WORD) (dwKey % iSize);
}

PRIVATE WORD HashMul(DWORD dwKey, int iSize)
{
    double dFrac;

    dFrac = fmod(dwKey * 0.618, 1);

    return (WORD) (iSize * dFrac);
}

PRIVATE DWORD HashElf ( const BYTE *ptrData, int iLen ) 
{ 
    DWORD dwKey = 0, dwTmp;

    for ( ; iLen > 0; --iLen )
    { 
        dwKey = ( dwKey << 4 ) + *ptrData++; 
        if ( dwTmp = dwKey & 0xF0000000 )
            dwKey ^= dwTmp >> 24;
        dwKey &= ~dwTmp; 
    } 

    return dwKey; 
}

WORD HashData(SHASHTABLE *pHT, void *pData)
{
    DWORD dwKey;
    WORD wIndex;
    WORD ( * fHash ) (DWORD, int);

    // trasnforma os dados em uma chave
    // parte especifica do codigo dependente dos dados escolhidos
    dwKey = HashElf(pData, pHT->fDataLen(pData));

    // se tamanho da tabela for potencia de 2
    fHash = (pHT->iSize & 1) ? HashDiv : HashMul;

    // calcular o indice
    wIndex = fHash( dwKey , pHT->iSize );

    return wIndex;
}


PRIVATE BOOL CreateTable(SLINK **ptrTable, int iSize)
{
    *ptrTable = calloc(iSize, sizeof(SLINK));

    return (*ptrTable == NULL ? FALSE : TRUE);
}

////////////////////

/*
    Funcao: 
              InitHashTable

    Proposito: 
              Cria e inicia uma estrutura de gerenciamento para tabela de dispersaoh encadeada

    Parametros:   
            ponteiros p/ a funcoesh especificas da lista
                void *(*fCreateData)(void *)           -> cria dado
                int   (*fDeleteData)(void *)           -> apaga dado
                int   (*fDuplicatedNode)(SLINK, SLINK) -> o que fazer c/ dado duplicado
                int   (*fNodeDataCmp)(void *, void *)  -> compara nohs
            int iTableSize                      -> tamanho da tabela
            ponteiros p/ a funcoesh especificas da tabela
                int (* fDataLen)(void *)            -> tamanho do dado

    Retorno:
          ponteiro SHASHTABLE
            pHT -> estrutura de dados que gerencia tabela de dispersaoh
            NULL - naoh conseguiu criar a tabela de dispersaoh encadeada

    Descricao:
              Cria uma estrutura de tabela de dispersaoh encadeada e retorna 
              um ponteiro da mesma. Em erro, retorna NULL.
              Esta funcaoh aceita ponteiros para as quatro funcoesh especificas
              de uma lista e inicializa a estrutura com elas.
*/
SHASHTABLE *InitHashTable(  void *  ( * fCreateData )       ( void * ),         // criar dado
                            BOOL    ( * fDeleteData )       ( void * ),         // apaga dado
                            int     ( * fDuplicatedNode )   ( SLINK, SLINK ),   // noh duplicado
                            int     ( * fNodeDataCmp )      ( void *, void *),  // compara
                            int     iTableSize,
                            int     ( * fDataLen )          ( void * ) )        // tamanho do dado
                        
{
    SHASHTABLE *pHT;

    pHT = (SHASHTABLE *) malloc(sizeof(SHASHTABLE));

    if (pHT)
    {
        pHT->iSize = iTableSize;
        pHT->fDataLen = fDataLen;       

        if ( CreateTable(&pHT->ptrTable, pHT->iSize) ) 
        {
            // configura a estrutura de dados da lista encadeada
            pHT->ptrList = CreateLList( fCreateData,
                                        fDeleteData,
                                        fDuplicatedNode,
                                        fNodeDataCmp);

            if (pHT->ptrList)
            {
                return (pHT);   // devolve o enderecoh
            }
            else
            {
                fprintf(stderr, "Erro criando lista encadeada\n");

                free(pHT->ptrTable);
            }
        }
        else {
            fprintf(stderr, "Erro criando tabela\n");
        }
    }
    else {
        fprintf(stderr, "Erro criando TDA da dispersaoh encadeada\n");
    }

    free(pHT);

    return NULL;
}

/*
    Funcao: 
              AddDataToTable

    Proposito: 
              Adiciona um noh em uma lista encadeada na tabela

    Parametros:
              SHASHTABLE *pHT -> ponteiro para estrutura de dados que trata uma tabela
              void *pData -> dado a ser inserido na tabela

    Retorno:
            TRUE  - adicionou na tabela um noh, 
            FALSE - naoh conseguiu criar um noh para inserir na lista

    Descricao:
              Adiciona um noh dentro de uma tabela de dispersaoh encadeada.
              Como o tipo de dado possui um gerenciador de lista herdado, 
              eh preciso adaptar a lista em cima da tabela, portanto
              cada posicaoh da tabela se refere a um inicio de lista, 
              para que as rotinas de lista tratem corretamente, 
              atualiza a variavel inicio de lista para o apontamento da posicao da tabela
              e apos o valor ser adicionado, atualizamos o apontamento na tabela
              caso este tenha sido alterado.
*/
BOOL AddDataToTable(SHASHTABLE *pHT, void *pData)
{
    WORD wIndex;

    if (pHT == NULL)
        return FALSE;

    // calcular a codificacaoh hash
    wIndex = HashData( pHT, pData );

    // insere na tabela
    pHT->ptrList->slkHead = pHT->ptrTable[wIndex];

    if (AddNodeAscend(pHT->ptrList, pData) == FALSE)
    {
        fprintf(stderr, "ERRO! Memoria naoh alocada\n");
        return FALSE;
    }
    // atualiza a tabela, caso a informcaoh 
    // tenha sido inserida no inicio
    pHT->ptrTable[wIndex] = pHT->ptrList->slkHead;

    return TRUE;
}

/*
    Funcao: 
              FindNodeAscend

    Proposito: 
              Pesquisa na tabela de dispersaoh encadeada.

    Parametros:
                SHASHTABLE *pHT -> estrutura de dados que gerencia tabela e lista encadeadas
                void *pData -> informacaoh a ser comparada

    Retorno:
              slkPCurr - aponta para o noh desejado na tabela
              NULL     - naoh encontrou o noh

    Descricao:
              Encontra um noh, iniciando a busca no inicio da lista, 
              apontada pela posicaoh calculada na tabela, formata a 
              informacaoh para utilizar a rotina FindNodeAscend
*/
SLINK FindDataInTable(SHASHTABLE *pHT, void *pData)
{
    WORD wIndex;
    void *pInfo;
    SLINK slk;

    if (pHT == NULL)
        return NULL;

    // calcular a codificacaoh hash
    wIndex = HashData( pHT, pData );

    // direciona para tabela
    pHT->ptrList->slkHead = pHT->ptrTable[wIndex];

    // formata o dado para realizar a busca
    pInfo = pHT->ptrList->fCreateData(pData);

    slk = FindNodeAscend(pHT->ptrList, pInfo);

    // descarta o dado formatado
    pHT->ptrList->fDeleteData(pInfo);

    // retorna o enderecoh do dado
    return slk;
}

/*
    Funcao: 
            DelDataInTable

    Proposito: 
              Apaga um noh dentro da tabela de dispersaoh

    Parametros:
                SHASHTABLE *pHT -> estrutura de dados com a tabela e rotinas de busca e exclusaoh de dados
                void *pData -> informacaoh a ser excluida

    Retorno:
              TRUE  - exclui o dado e atualizou a tabela
              FALSE - naoh era um dado valido p/ exclusaoh

    Descricao:
                Aposh calcular a posicaoh na tabela, formata o dado
                para buscar sua posicaoh na lista, caso tenha
                encontrado a informacaoh desejada, elimina o noh utilizando
                a funcaoh DeleteNode, para a funcaoh DeleteNode funcionar
                corretamente eh preciso atualiza as variaveis de gerenciamento 
                da lista de inicio e fim, com os valores apontados pela posicaoh
                calculada da tabela, ao fim da exclusao, atualiza o valor de apontamento 
                na tabela
*/
BOOL DelDataInTable(SHASHTABLE *pHT,  void *pData)
{
    WORD wIndex;
    void *pInfo;
    SLINK slk;
    BOOL bOk;

    // calcular a codificacaoh hash
    wIndex = HashData( pHT, pData );

    // direciona para tabela
    pHT->ptrList->slkHead = pHT->ptrTable[wIndex];

    // formata o dado para realizar a busca
    pInfo = pHT->ptrList->fCreateData(pData);

    slk = FindNodeAscend(pHT->ptrList, pInfo);

    // descarta o dado formatado
    pHT->ptrList->fDeleteData(pInfo);

    if (slk) 
    {
        // primeiro da lista
        if (slk->prior == NULL) 
            slk = pHT->ptrList->slkHead;

        // ultimo da lista
        if (slk->next == NULL) 
            slk = pHT->ptrList->slkTail;

        // elimina o dado
        bOk = DeleteNode(pHT->ptrList, slk);

        // atualiza tabela
        pHT->ptrTable[wIndex] = pHT->ptrList->slkHead;
    }
    else 
        bOk = FALSE;

    return bOk;
}


/*
    Funcao: 
              EndHashTable

    Proposito: 
              Apaga a estrutura de gerenciamento da tabela de dispersaoh e suas listas encadeadaa

    Parametros:   
            SHASHTABLE *pHT -> estrutura de dados que gerencia tabela de dispersaoh

    Retorno:
              TRUE  - liberou toda a memoria utilizada pela tabela
              FALSE - naoh conseguiu apagar a lista encadeada

    Descricao:
              Apos verificar que se trata de uma tabela valida, percorre a
              mesma, apagando os nohs individualmente, por fim, libera a
              memoria usada pela propria estrutura de gerenciamento da 
              lista encadeada e da tabela de dispersaoh;
*/
BOOL EndHashTable(SHASHTABLE *pHT)
{
    int iCnt;
    SLINK pCurr, pBak;

    if (pHT == NULL)
        return FALSE;

    // percorre cada posicaoh da tabela
    for (iCnt = 0; iCnt < pHT->iSize; ++iCnt)
    {
        pCurr = pHT->ptrList->slkHead = pHT->ptrTable[iCnt];

        if (pCurr != NULL)
        {
            // percorre a lista apagando todos os nohs
            while ( pCurr != NULL )
            {
                pBak = pCurr;
                pCurr = pCurr->next;
                if (pCurr == NULL)
                    pHT->ptrList->slkTail = pBak;
                DeleteNode(pHT->ptrList, pBak);
            }
        }
    }

    free(pHT->ptrList);  // libera a estrutura que trata a lista encadeada
    free(pHT);           // libera a estrutura que trata a tabela

    return TRUE;
}


//#define LIST_TABLE

void ListTableStat(SHASHTABLE *pHT)
{
    int iCnt;
    int iChainTables[33];
    int iChains;
    SLINK pCurr;

    for (iCnt = 0; iCnt < 33; ++iCnt) {
        iChainTables[iCnt] = 0;
    }

    for (iCnt = 0; iCnt < pHT->iSize; ++iCnt)
    {
#ifdef LIST_TABLE
        printf("%4u) ", iCnt);
#endif
        pCurr = pHT->ptrTable[iCnt];

        if (pCurr != NULL)
        {
            int iChainLen;

            for (iChainLen = 0; pCurr != NULL; pCurr = pCurr->next)
            {
#ifdef LIST_TABLE
                PrintData(pCurr->pdata);
#endif
                iChainLen++;
            }

            if (iChainLen > 32)
                iChainLen = 32;

            iChainTables[iChainLen] += 1;
        }
#ifdef  LIST_TABLE
        printf("\n");
#endif
    }

    iChains = 0;
    for (iCnt = 0; iCnt < 33; iCnt++)
    {
        if (iChainTables[iCnt]) 
        {
            printf("%3u listas encadeadas com %2u nohs\n", iChainTables[iCnt], iCnt);
            iChains += iChainTables[iCnt];
        }
    }

    if (iChains)
    {
        printf("\n%d Nohs em %u listas encadeadas\n",       pHT->ptrList->uiCount, iChains );
        printf("Tamanho da tabela de dispersaoh = %u\n",    pHT->iSize );
        printf("Tamanho medio das listas = %f\n",           (double) pHT->ptrList->uiCount / iChains );
        printf("Recipientes ocupados = %f\n",               (double) iChains / pHT->iSize);
        printf("Coeficiente de carregamento = %f\n\n",      (double) pHT->ptrList->uiCount / pHT->iSize);
    }
    else
    {
        printf("ATENCAOH! Nenhum encadeamento encontrado\n\n");
    }
}
