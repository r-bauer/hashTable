/*
  Arquivo: MAIN.H
  Autor: Roberto Bauer
  Observacoes: Cabecalho principal p/ concentrar os demais cabecalhos
*/

#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __MAIN_C__
    #define EXTERN
#else
    #define EXTERN      extern
#endif

#define PRIVATE       static

typedef enum tagBOOL { FALSE, TRUE } BOOL;

typedef unsigned char       BYTE;
typedef unsigned short int  WORD;
typedef unsigned long int   DWORD;


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "llgen.h"
#include "llapp.h"
#include "hashtab.h"


#endif                                  /* #ifndef __MAIN_H__ */
