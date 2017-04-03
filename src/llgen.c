/*
  Arquivo: LLGEN.C
  Autor: Roberto Bauer
  Observacoes:
              Primitivas genericas para uma lista duplamente encadeada.
              Naoh contem nenhuma funcaoh dependedente de codigo.
*/


#define __LLGEN_C__

#include "main.h"


//  Redefinicoes para aumentar legibilidade do codigo

#define LLHead          (L->slkHead)            // o inicio da lista atual
#define LLTail          (L->slkTail)            // o fim da lista atual
#define NodeCount       (L->uiCount)            // nohs na lista atual

#define CreateData      (*(L->fCreateData))
#define DeleteData      (*(L->fDeleteData))
#define DuplicatedNode  (*(L->fDuplicatedNode))
#define NodeDataCmp     (*(L->fNodeDataCmp))


/*
    Funcao: 
              AddNodeAtHead

    Proposito: 
              Adiciona um noh no inicio(cabeca) da lista NAOH ordenada

    Parametros:
              SLIST *L -> ponteiro para estrutura de dados que trata a lista NAOH ordenada
              void *nd -> noh a ser inserido na lista NAOH ordenada

    Retorno:
            TRUE  - adicionou o no inicio da lista o noh
            FALSE - naoh conseguiu criar um noh para inserir na lista

    Descricao:
              Armazena um novo noh sempre no inicio da lista
              Primeiro aloca espaco para o dado,
              a seguir aloca um noh com ponteiro para o dado,
              entaoh adiciona o noh na lista.
*/
BOOL AddNodeAtHead( SLIST *L, void *nd )
{
    SLINK slkPn;                    // ponteiro de para o noh a ser inserido na lista

    slkPn = CreateNode(L, nd);      // cria o noh e passa o endereco 
    if (slkPn == NULL)              // naoh conseguiu alocar o noh
        return FALSE;               // retorna indicando fracasso

    // adiciona o noh
    if (LLHead == NULL)             // eh o primeiro noh a ser inserido na lista ?
    {
        LLHead = LLTail = slkPn;    // sim 
    }
    else                            // naoh
    {
        LLHead->prior = slkPn;       // primeiro coloca o noh antes do inicio(cabeca)
        slkPn->next = LLHead;       // coloca o inicio como seguinte do novo noh
        LLHead = slkPn;             // entaoh aponta o inicio para o novo noh
    }

    ++NodeCount;                    // soma o novo noh no total
    return TRUE;
}


/*
    Funcao: 
              AddNodeAscend

    Proposito: 
              Adiciona um noh ascendente

    Parametros:
              SLIST *L -> ponteiro para estrutura de dados que trata a lista
              void *nd -> noh a ser inserido na lista

    Retorno:
            TRUE  - adicionou na lista o noh, 
                   caso seja duplicata, remove, adiciona ou ignora, 
                   depende da configuracaoh da lista
            FALSE - naoh conseguiu criar um noh para inserir na lista

    Descricao:
              Adiciona um noh numa lista ordenada
*/

BOOL AddNodeAscend( SLIST *L, void *nd )
{
    SLINK slkPn;                    // ponteiro de para o noh a ser inserido na lista
    SLINK slkPrev;                  // ponteiro do link anterior da lista durante a busca 
    SLINK slkCurr;                  // ponteiro do link atual da lista durante a busca 
    SNODE snDummy;           // noh temporario
    int iCompare;                   // resultado da comparacaoh entre dois nohs
    int iAction;                    // acaoh a ser tomada em caso de nohs duplicados

    slkPn = CreateNode(L, nd);      // cria o noh e passa o endereco 
    if (slkPn == NULL)              // naoh conseguiu alocar o noh
        return FALSE;               // retorna indicando fracasso

    // ignora o tratamento especial que se faz para o inicio da lista
    // adiciona o noh temporario ao inicio da lista
    // e passa a tratar o noh inicial(cabeca) como um noh igual aos demais
    // simplificando a logica de tratamento
    snDummy.next = LLHead;
    snDummy.prior = NULL;
    if (snDummy.next != NULL)           // se a lista naoh estiver vazia
        snDummy.next->prior = &snDummy;  // igual, LLHEAD->prior = &snDummy;

    // se prepara para percorrer a lista
    slkPrev = &snDummy;                 // noh anterior aponta p/ o noh temporario
    slkCurr = snDummy.next;             // noh atual aponta p/ o noh inicial da lista

    // busca
    // enquanto o link do noh for valido
    // copia o noh atual para o anterior
    // copia o noh seguinte para o atual
    for ( ; slkCurr != NULL; slkPrev = slkCurr, slkCurr = slkCurr->next)
    {
        // usa a funcaoh que compara os dados do noh atual com o noh a ser inserido
        iCompare = NodeDataCmp(slkPn->pdata, slkCurr->pdata);
        if (iCompare <= 0) {
            break;                      // o novo noh eh igual ou precede o atual
        }
    }

    // se naoh percorreu a lista ateh o final
    // e o encontrou uma copia(duplicada) dos dados na lista
    if ((slkCurr != NULL) && (iCompare == 0)) 
    {
        // chama a funcaoh que retorna a acaoh desejada 
        // no caso de noh duplicado
        iAction = DuplicatedNode(slkPn, slkCurr);
        //        fDuplicateNode retorna:
        //              2 -> adicionar duplicada
        if (iAction == 2) {
            // nada faz, deve ser inserido
            // o tratamento continua fora 
            // da condicional 'if ((slkCurr != NULL) && (iCompare == 0))'
        }   
        //        fDuplicateNode retorna:
        //              0 -> nenhuma acaoh na lista
        //              1 -> destruir duplicada
        else    // if (iAction == 0 || iAction == 1) 
        {
            // primeiro, arruma a lista encadeada tiramos o noh 
            // temporario do inicio e restauramos o inicio original
            LLHead = snDummy.next;
            LLHead->prior = NULL;

            // se apropriado, apaga o noh duplicado
            if (iAction == 1) // ||(iAction == 0)
            {
                DeleteData(slkPn->pdata);   // apagamos os dados que dependem da implementacaoh do objeto
                free(slkPn);                // desalocamos o noh 
            }

            // se a iAction == 0, um erro aconteceu, 
            // o tratamento vai ser feito fora desta funcaoh
            return TRUE;                    // encontrou uma duplicata e tomou a acaoh de ignorar ou excluir
        }            
    }

    // vai adicionar o noh entre o anterior e o atual
    slkPrev->next = slkPn;          // o noh anterior aponta para o novo noh
    slkPn->prior = slkPrev;          // o novo noh aponta 'prior' para o noh anterior
    slkPn->next = slkCurr;          // o novo noh aponta 'next' para o noh atual

    // se o noh corrente aponta para uma posicao valida na lista
    if (slkCurr != NULL)
    {
        slkCurr->prior = slkPn;      // o noh atual aponta 'prior' para o novo noh
    }
    // do contrario, significa que percorreu a lista ateh o final
    else 
    {
        LLTail = slkPn;             //  este noh eh a novo fim(cauda)
    }

    ++NodeCount;                    // soma o novo noh no total

    // retiramos o noh temporario
    LLHead = snDummy.next;
    LLHead->prior = NULL;

    return TRUE;
}


/*
    Funcao: 
              CreateLList

    Proposito: 
              Cria e inicia uma estrutura de gerenciamento de lista encadeada

    Parametros:   
          ponteiros p/ a funcoesh especificas da lista
              void *(*fCreateData)(void *)           -> cria dado
              int   (*fDeleteData)(void *)           -> apaga dado
              int   (*fDuplicatedNode)(SLINK, SLINK) -> o que fazer c/ dado duplicado
              int   (*fNodeDataCmp)(void *, void *)  -> compara nohs

    Retorno:
          ponteiro SLIST
            pL   - ponteiro para uma estrutura do tipo lista inicializada
            NULL - naoh conseguiu criar a lista encadeada

    Descricao:
              Cria uma estrutura de lista encadeada e retorna um ponteiro da 
              mesma. Em erro, retorna NULL.
              Esta funcaoh aceita ponteiros para as quatro funcoesh especificas
              de uma lista e inicializa a estrutura com elas.
*/
SLIST *CreateLList( void *  ( * fCreateData ) ( void * ),
                    BOOL    ( * fDeleteData ) ( void * ),
                    int     ( * fDuplicatedNode ) ( SLINK, SLINK ),
                    int     ( * fNodeDataCmp ) ( void *, void *))
{
    SLIST *pL;

    // aloca uma estrutura que gerencia uma lista encadeada
    pL = (SLIST *) malloc(sizeof(SLIST));
    if (pL == NULL)         //  caso fracasse em alocar memoria
        return NULL;        // retorna a indicacaoh

    // tendo alocado, inicializa as variaveis 
    pL->slkHead = NULL;
    pL->slkTail = NULL;
    pL->uiCount = 0;
    
    // e armazena as funcoesh especificas para tratamento da lista
    pL->fCreateData = fCreateData;
    pL->fDeleteData = fDeleteData;
    pL->fDuplicatedNode = fDuplicatedNode;
    pL->fNodeDataCmp = fNodeDataCmp;

    return (pL);            // devolve o enderecoh da lista
}

/*
    Funcao: 
              CreateNode

    Proposito: 
              Criar um noh para a lista encadeada

    Parametros:
              SLIST *L   -> estrutura lista c/ ponteiro para a funcaoh CreateData
              void *data -> ponteiro generico c/ o dado do noh

    Retorno:
              slkNewNode - ponteiro para uma estrutura do tipo noh
              NULL - naoh conseguiu criar um noh

    Descricao:
              Cria um noh e entaoh chama a funcaoh especifica da aplicacaoh
              CreateData() para criar o dado da estrutura do noh.
              Em caso de falha retorna NULO

*/
SLINK CreateNode( SLIST *L, void *data )
{
    SLINK slkNewNode;

    // aloca os ponteiros para seguinte, anterior e dado para o noh
    slkNewNode = (SLINK) malloc(sizeof(SNODE));
    if (slkNewNode == NULL)     // caso fracasse em alocar memoria
        return (NULL);          // retorna indicacaoh de NULO

    // tendo sucesso na alocacaoh, inicializa as variaveis
    slkNewNode->prior = NULL;
    slkNewNode->next = NULL;

    // agora chama a aplicacaoh especifica para alocacah de dados
    slkNewNode->pdata = CreateData(data);
    // caso naoh consiga alocar memoria para o dado do noh
    if (slkNewNode->pdata == NULL)
    {
        free(slkNewNode);       // libera a memoria previamente alocada
        return (NULL);          // retorna NULO para indicar falha
    }
    // alocou toda a memoria nescessaria
    else {
        return (slkNewNode);    // retorna o ponteiro para a funcaoh
    }
}

/*
    Funcao: 
              DeleteNode

    Proposito: 
              Apaga um noh da lista encadeada

    Parametros:
              SLIST *L          -> estrutura lista c/ ponteiro para a funcaoh DeleteData
              SLINK slkToDelete -> o noh a ser excluido

    Retorno:
              TRUE  - exclui o noh e atualizou a lista
              FALSE - naoh era um noh valido p/ exclusaoh

    Descricao:
              Apaga um noh apontado por slkToDelete.
              Verifica se o noh se encontra no inicio, meio ou fim p/ tratamento especifico.
              Funcaoh chama a aplicacaoh especifica da lista para apagar o dado
*/
BOOL DeleteNode( SLIST *L, SLINK slkToDelete )
{
    SLINK slkPn;

    if (slkToDelete == NULL)    // certifica-se de ser um noh valido
        return FALSE;           // antes de tentar apagar qualquer coisa


    // eh o ultimo da lista 
    if (slkToDelete == LLHead && slkToDelete == LLTail)
    {
        LLHead = NULL;                  // o anterior do inicio deve ser NULO
        LLTail = NULL;                  // o seguinte do fim deve ser NULO
    }
    else    
    if (slkToDelete->prior == NULL)      // se encontra no inicio da lista (cabeca)
    {
        LLHead = slkToDelete->next;     // atualiza o inicio
        LLHead->prior = NULL;            // o anterior do inicio deve ser NULO
    }
    else if (slkToDelete->next == NULL) // se encontra no fim da lista (cauda)
    {
        LLTail = slkToDelete->prior;     // atualiza o fim
        LLTail->next = NULL;            // o seguinte do fim deve ser NULO
    }
    else                                // se encontra no meio da lista
    {
        slkPn = slkToDelete->prior;      // vai para o noh anterior ao da exclusaoh
        slkPn->next = slkToDelete->next;// atualiza para o proximo

        slkPn = slkToDelete->next;      // vai para o noh seguinte ao da exclusaoh
        slkPn->prior = slkToDelete->prior;// e atualiza para o anterior
    }

    DeleteData(slkToDelete->pdata);     // apaga o dado do noh
    free(slkToDelete);                  // libera a memoria do noh

    --NodeCount;                        // subtrai o noh do total

    return TRUE;
}

/*
    Funcao: 
              FindNode

    Proposito: 
              Pesquisa a lista inteira obs.:por ser NAOH ordenada

    Parametros:
              SLIST *L -> estrutura com as variaveis de controle e funcaoh de comparacaoh
              void *nd -> noh a ser encontrado

    Retorno:
              slkPCurr - aponta para o noh desejado na lista
              NULL     - naoh encontrou o noh

    Descricao:
              Encontra um noh, iniciando a busca no inicio da lista, 
              avancando em cada noh e comparando os itens do dado com
              a chave de pesquisa.
              Precisa percorrer toda a lista, por se tratar de uma NAOH ordenada
*/
SLINK FindNode( SLIST *L, void *nd )
{
    SLINK   slkPCurr;       // o noh que estamos examinando

    if (LLHead == NULL) {   // lista vazia ?
        return (NULL);        // retornando indicando NULO como falha
    }

    // posiciona slkPCurr no inicio da lista
    // enquanto for valido, avancah p/ o seguinte
    for (slkPCurr = LLHead; slkPCurr != NULL; slkPCurr = slkPCurr->next)
    {
        // compara, se igual
        if (NodeDataCmp(nd, slkPCurr->pdata) == 0) 
        {
            return (slkPCurr);      // retorna o ponteiro p/ a posicaoh da lista
        }
    }

    // naoh consigou encontrar o noh
    return (NULL);
}

/*
    Funcao: 
              FindNodeAscend

    Proposito: 
              Pesquisa a lista ordenada.

    Parametros:
              SLIST *L -> estrutura com as variaveis de controle e funcaoh de comparacaoh
              void *nd -> noh a ser encontrado

    Retorno:
              slkPCurr - aponta para o noh desejado na lista
              NULL     - naoh encontrou o noh

    Descricao:
              Encontra um noh, iniciando a busca no inicio da lista, 
              avancando em cada noh e comparando os itens do dado com
              a chave de pesquisa ENQUANTO naoh for maior que o desejado,
              por estar previamente ordenada, naoh pode se encontrar no
              restante da fila
*/
SLINK FindNodeAscend( SLIST *L, void *nd )
{
    SLINK   slkPCurr;       // o noh que estamos examinando
    int     iCmpResult;     // resultado da comparacao

    if (LLHead == NULL) {   // lista vazia ?
        return (NULL);        // retornando indicando NULO como falha
    }

    // posiciona slkPCurr no inicio da lista
    // enquanto for valido, avancah p/ o seguinte
    for (slkPCurr = LLHead; slkPCurr != NULL; slkPCurr = slkPCurr->next)
    {
        // salva o resultado, para poder fazer duas analises posteriores
        iCmpResult = NodeDataCmp(nd, slkPCurr->pdata);

        // compara, se o noh pesquisado eh maior que a posicaoh atual da lista
        if (iCmpResult < 0) 
        {
            return (NULL);        // indica falha, pois naoh pode estar no restante da fila
        }

        // compara, se igual
        if (iCmpResult == 0) 
        {
            return (slkPCurr);      // retorna o ponteiro p/ a posicaoh da lista
        }
    }

    // naoh consigou encontrar o noh em toda lista
    return (NULL);
}

/*
    Funcao: 
              DestroyLList

    Proposito: 
              Apaga a estrutura de gerenciamento de lista encadeada e seus nohs

    Parametros:   
              ponteiro SLIST
              L   - ponteiro da estrutura do tipo lista a ser desalocada

    Retorno:
              TRUE  -
              FALSE - naoh conseguiu apagar a lista encadeada

    Descricao:
              Apos verificar que se trata de uma lista valida, percorre a
              mesma, apagando os nohs individualmente, por fim, libera a
              memoria usada pela propria estrutura de gerenciamento.
*/
BOOL DestroyLList(SLIST *L)
{
    SLINK slk1, slk2;    // cursores p/ percorrer as lista

    if (L == NULL)      //  caso naoh exista uma estrutura valida
        return FALSE;    // retorna a indicacaoh de falha

    // percorre a lista apagando todos os nohs
    for (slk1 = LLHead; slk1 != NULL;) {
        slk2 = slk1;            // faz um backup da posicaoh
        slk1 = slk1->next;      // avanca para o proximo
        DeleteNode(L, slk2);    // usa o backup para apagar o noh
    }
    
    free(L);             // libera a estrutura que gerencia uma lista encadeada

    return TRUE;          // informa que a lista foi destruida
}


