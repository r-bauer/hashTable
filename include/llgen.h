/*
  Arquivo: LLGEN.H
  Autor: Roberto Bauer
  Observacoes:
              Primitivas genericas para uma lista duplamente encadeada.
*/

#ifndef __LLGEN_H__
#define __LLGEN_H__

#undef EXTERN
#ifdef __LLGEN_C__
    #define EXTERN
#else
    #define EXTERN  extern
#endif


// numa lista duplamente encadeada consiste numa corrente de elementos
// chamados de no, cada no possui um tipo de dados e dois ponteiros
// usados para apontar para o no anterior e proximo
typedef struct tagSNODE
{
    struct tagSNODE    *prior;      //  ligacaoh para o noh anterior
    struct tagSNODE    *next;      //  ligacaoh para o noh seguinte
    void            *pdata;     //  ponteiro generico de dados
} SNODE;

// define um tipo ponteiro para o noh
typedef SNODE *      SLINK;


// uma estrutura de dados de uma lista encadeada
typedef struct tagSLIST
{
    SLINK           slkHead;     // noh do inicio da lista
    SLINK           slkTail;     // noh do fim da lista
    unsigned int    uiCount;    // total de nohs na lista

    // As quatro funcoes especificas da implementacaoh dos dados 
    // de uma lista encadeada saoh:

    void *  ( * fCreateData )       ( void * );    //      Criar Dado
    //        Eh passado um ponteiro do objeto definido para a 
    //        aplicacaoh, espera-se o retorno de um ponteiro
    //        para o que venha a ser o dado armazenado na lista.

    BOOL    ( * fDeleteData )       ( void * );    //      Apagar Dado
    //        Eh passado um ponteiro do objeto definido para a 
    //        aplicacaoh que deve estar armazenado na lista 
    //        encadeada. fDeleteData deve destruir o objeto

    int     ( * fDuplicatedNode )   ( SLINK, SLINK );    //      Noh Duplicado
    //        Eh passado dois ponteiro. O primeiro ponteiro eh o
    //        noh que deve ser adicionado e o segundo eh o noh que
    //        jah se encontra na lista, mas que eh igual(duplicada)
    //        a informacao do primeiro ponteiro.
    //        fDuplicateNode retorna:
    //              0 -> nenhuma acaoh na lista
    //              1 -> destruir duplicada
    //              2 -> adicionar duplicada

    int     ( * fNodeDataCmp )      ( void *, void * );    //      Compara Dados do Noh
    //      fNodeDataCmp:       Eh passado dois ponteiros do objeto definido para o
    //                          dado e deve compara-lo, retornando um numero que eh
    //                          < 0, zero, ou > 0, dependendo da relacaoh entre o 
    //                          primeiro e segundo objeto.
    //

} SLIST;


// primitivas genericas da lista encadeada
EXTERN BOOL AddNodeAscend( SLIST *, void * );
EXTERN BOOL AddNodeAtHead( SLIST *, void * );

EXTERN SLIST *CreateLList(  void *  ( * fCreateData )       ( void * ),         // criar dado
                            BOOL    ( * fDeleteData )       ( void * ),         // apaga dado
                            int     ( * fDuplicatedNode )   ( SLINK, SLINK ),   // noh duplicado
                            int     ( * fNodeDataCmp )      ( void *, void *)); // compara

EXTERN SLINK CreateNode( SLIST *, void * );
EXTERN BOOL DeleteNode( SLIST *, SLINK );
EXTERN SLINK FindNode( SLIST *, void * );
EXTERN SLINK FindNodeAscend( SLIST *, void * );
EXTERN BOOL DestroyLList( SLIST * );


#endif  /* #define __LLGEN_H__ */


