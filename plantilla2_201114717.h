/* 
 * File:   plantilla2_201114717.h
 * Author: clnx
 *
 * Created on 4 de septiembre de 2015, 12:34 PM
 */

#ifndef PLANTILLA2_201114717_H
#define	PLANTILLA2_201114717_H

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
        char estado; //1 creado, 0 eliminado
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
    mbr obtenerMBR(char nombreDisco[]);
    void actualizarDiscoIndex(disco aux1);
    void EliminarParticion();
            
#ifdef	__cplusplus
}
#endif

#endif	/* PLANTILLA2_201114717_H */

