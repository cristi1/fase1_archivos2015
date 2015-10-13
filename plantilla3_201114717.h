/* 
 * File:   plantilla3_201114717.h
 * Author: clnx
 *
 * Created on 8 de septiembre de 2015, 12:22 AM
 */

#ifndef PLANTILLA3_201114717_H
#define	PLANTILLA3_201114717_H

#ifdef	__cplusplus
extern "C" {
#endif

    int ajuste(infoPart particion,int cantidad);
    void escribirBloques(infoPart info,int i,int numBloqs,char contenido);
    infoPart informacionParticion(char nomDisco[],char particion[]);
    void archivoFAT();
    void archivoENLAZADO();
    void archivoEXT3();
    void crearArchivo();
#ifdef	__cplusplus
}
#endif

#endif	/* PLANTILLA3_201114717_H */

