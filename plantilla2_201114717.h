/* 
 * File:   plantilla2_201114717.h
 * Author: clnx
 *
 * Created on 4 de septiembre de 2015, 12:34 PM
 */

#ifndef PLANTILLA2_201114717_H
#define	PLANTILLA2_201114717_H
#include "plantilla3_201114717.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
    typedef struct{
        char nombre[16]; //nombre del archivo
        int tamanio; //tamaño del archivo en bytes
        char extension[5]; //extension del archivo
        int idInicio; //id del bloque en el que inicia el archivo
        char fecha[20]; //fecha en la que el archivo fue creado
        int idFinal; //id del ultimo bloque del archivo
        char estado; //1 creado, 0 eliminado, 2 disponible
    }fatRootFolder;
    
    typedef struct{
        int id; //es el numero de bloque segun en la particion en que este
        char estado; //1 ocupado, 0 libre 
        int anterior; //id del bloq anterior del q se compone el archivo, si es el primer bloq del archivo sera -1
        int siguiente; //id del siguiente bloq del q se compone el archivo, si es el ultimo bloq del archivo tendra el valor de -1
    }bloque;
    
//--------------------------------METODOS---------------------------------------
    disco existeDiscoIndex(char id[]);
    void crearParticion();
    void actualizarDiscoIndex(disco aux1);
    void EliminarParticion();
    
    int crearFAT(infoPart particion, char nomDisco[]);
    int crearENLAZADO(infoPart particion, char nomDisco[]);
    int crearEXT3(infoPart particion, char nomDisco[]);
    mbr recuperarMBR(char nom[]);
    infoPart buscarInfoPart(mbr datosDisco,char nomPart[]);
    void FormatearParticion();
    void ActualizarMBR(char nomDisco[],mbr actual,infoPart act);
    
    inodo nuevoInodo();
    bloquEXT nuevobloqEXT3();
    log nuevaBitacora();
    
                        /*while(i<cantidad && limite<particion.cantBloques){
                        byte temp;
                        fread(&temp,sizeof(byte),1,discoActual);
                        if(temp.a=='0'){
                            if(i==cantidad-1){
                                i=cantidad;
                            }else{
                                i++;
                            }
                        }else{
                            i=0;
                        }
                        limite++;
                    }
                    if(i==cantidad){
                        inicio=limite-cantidad; 
                    }*/
            
#ifdef	__cplusplus
}
#endif

#endif	/* PLANTILLA2_201114717_H */

