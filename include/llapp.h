/*
  Arquivo: LLAPP.H
  Autor: Roberto Bauer
  Observacoes:
              Dados especificos da aplicacaoh p/ lista encadeada
*/


#ifndef __LLAPP_H__
#define __LLAPP_H__

#undef EXTERN
#ifdef __LLAPP_C__
    #define EXTERN
#else
    #define EXTERN  extern
#endif


// A primeira lista de noh consiste em:
typedef struct tagSNODEDATA
{
    char *pWord;         //  ponteiros para palavras
    unsigned int uCont;     // e a contagem de ocorrencias
}SNODEDATA;

typedef SNODEDATA *      pND;

EXTERN void *CreateData( void * );
EXTERN BOOL  DeleteData( void * );
EXTERN int   DuplicatedNode( SLINK, SLINK );
EXTERN int   NodeDataCmp( void *, void * );

EXTERN int  DataLen(void *);

EXTERN int  PrintData(void *);


#endif  // #define __LLAPP_H__
