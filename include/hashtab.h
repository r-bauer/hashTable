/*
  Arquivo: HASHTAB.H
  Autor: Roberto Bauer
  Observacoes:
              Rotinas para tabela de dispersaoh, criar tabela, 
              inserir dados e mostrar estatisticas.
*/

#ifndef __HASHTAB_H__
#define __HASHTAB_H__

#undef EXTERN
#ifdef __HASHTAB_C__
    #define EXTERN
#else
    #define EXTERN  extern
#endif



// uma estrutura de dados de uma tabela de dispersaoh encadeada
typedef struct tagSHASHTABLE
{
    SLIST   *ptrList;           // a lista encadeada a ser usada pela tabela
    SLINK   *ptrTable;          // uma tabela de dispersão
    int     iSize;              // tamanho da tabela de dispersaoh
    int     ( * fDataLen )  ( void * ); 
} SHASHTABLE;


SHASHTABLE *InitHashTable(  void *  ( * fCreateData )       ( void * ),         // criar dado
                            BOOL    ( * fDeleteData )       ( void * ),         // apaga dado
                            int     ( * fDuplicatedNode )   ( SLINK, SLINK ),   // noh duplicado
                            int     ( * fNodeDataCmp )      ( void *, void *),  // compara
                            int     iTableSize,                                 // tamanho da tabela
                            int     ( * fDataLen )          ( void * ) );       // tamanho do dado
                            

BOOL AddDataToTable(SHASHTABLE *, void *);
SLINK FindDataInTable(SHASHTABLE *, void *);
BOOL DelDataInTable(SHASHTABLE *,  void *);
BOOL EndHashTable(SHASHTABLE *);

void ListTableStat(SHASHTABLE *);


#endif  // #define __HASHTAB_H__

