/* 
 * File:   plantilla1_201114717.h
 * Author: clnx
 * Created on 20 de agosto de 2015, 11:46 AM
 */

#ifndef PLANTILLA1_201114717_H
#define	PLANTILLA1_201114717_H
#ifdef	__cplusplus
extern "C" {
#endif
    
    char ubic_general[110],ubi_index[100];
    
    typedef struct{
        char a;
    }byte;
    
    typedef struct{//para archivos de discos
        char id[3]; 
        int tam; //tamaño de disco en MB
        char nombre[32]; //nombre del disco
        int prtPrim; //No. particiones primarias
        int prtExt; //No. particiones extendidas
        int prtLog; //No. particiones logicas
        int spLibre; //espacio libre en MB (sin particionar)
        int estado; //1 existe 0 eliminado
    }disco;
    
    typedef struct{
        int estado; //estado de la particion 1=activa, 0=eliminada
        char nombre[32]; //nombre de la particion (32 caracteres)
        int byteInicio; //byte donde inicia la particion
        int cantBloques; //cantidad de bloques de la particion
        int cantBloqLib; //cantidad de bloques libres
        int tipoPart; //tipo de particion 1=primaria, 2=extendida, 3=logica
        int tipoAjuste; //tipo de ajuste 1=primer ajuste, 2=mejor ajuste, 3=peor ajuste
        int tam; //tamaño de la particion
    }infoPart;
    
    typedef struct{
        char nombre[32]; //nombre del disco
        int tam; //tamaño del disco
        int cantPart; //numero de particiones creadas
        infoPart iPart[16]; //tabla de info de 16 particiones maximo 1 extendida por disco y 12 logicas por extendida
    }mbr;
    
//-------------------------------METODOS----------------------------------------
    void crearDisco();
    void aumentarTamDisco();
    void reducirTamDisco();
    void eliminarDisco();
    void inicio();

#ifdef	__cplusplus
}
#endif

#endif	/* PLANTILLA1_201114717_H */

