/* 
 * File:   lista.h
 * Author: clnx
 *
 * Created on 16 de octubre de 2015, 06:47 PM
 */

#ifndef LISTA_H
#define	LISTA_H

#ifdef	__cplusplus
extern "C" {
#endif
    typedef struct nodo{
        char nombre[16];
        struct nodo *ant;
        struct nodo *sig;
    }nodol;

    typedef struct{
        nodol *primero;
        nodol *ultimo;
    }Lista;
    
    void addLista(Lista *l, char nom[]);

#ifdef	__cplusplus
}
#endif

#endif	/* LISTA_H */

