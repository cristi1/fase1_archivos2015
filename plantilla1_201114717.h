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
        //int tam; //tamaño de disco en MB
        float tam; //tamaño de disco en Bytes
        char nombre[32]; //nombre del disco
        int prtPrim; //No. particiones primarias
        int prtExt; //No. particiones extendidas
        int prtLog; //No. particiones logicas
        //int spLibre; //espacio libre en MB (sin particionar)
        float spLibre; //espacio libre en Bytes(sin particionar)
        int estado; //1 existe 0 eliminado
        char id[3]; 
    }disco;
    
    typedef struct{
        int estado; //estado de la particion 1=activa, 0=eliminada
        char nombre[32]; //nombre de la particion (32 caracteres)
        int byteInicio; //byte donde inicia la particion
        int cantBloques; //cantidad de bloques de la particion
        int cantBloqLib; //cantidad de bloques libres
        int tipoPart; //tipo de particion 1=primaria, 2=extendida, 3=logica
        int tipoAjuste; //tipo de ajuste 1=primer ajuste, 2=mejor ajuste, 3=peor ajuste
        float tam; //tamaño de la particion kb
        int tamBloq; //tamaño del bloque
        int sisArchivos; //sistema de archivos 1.FAT 2.ENLAZADO 3.EXT3
    }infoPart;
    
    typedef struct{
        char nombre[32]; //nombre del disco
        float tam; //tamaño del disco en bytes
        int cantPart; //numero de particiones creadas
        infoPart iPart[16]; //tabla de info de 16 particiones maximo 1 extendida por disco y 12 logicas por extendida
    }mbr;
    
    typedef struct{
        int id;
        char contenido[100]; //arreglo de caracteres q poseera el bloq
        int sig; //id de apuntador hacia el siguiente bloq
    }bloqEnl;
    
    typedef struct{
        int inicio;
        int fin;
        char nombre[16];
    }directorio;
    
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

