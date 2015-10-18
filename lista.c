#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addLista(Lista *l, char nom[]){
    nodol *n=malloc(sizeof(nodol));
    strcpy(n->nombre,nom);
    if(l->primero==NULL){
        n->ant=NULL;
        n->sig=NULL;
        l->primero=n;
        l->ultimo=n;
    }else{
        l->ultimo->sig=n;
        n->ant=l->ultimo;
        l->ultimo=n;
    }
}

void recLista(Lista *l){
    nodol *auxiliar;
    auxiliar=l->primero;
    while(auxiliar!=NULL){
        printf(" %s\n",auxiliar->nombre);
        auxiliar=auxiliar->sig;
    }
}
