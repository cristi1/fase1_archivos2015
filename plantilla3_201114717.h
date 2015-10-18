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
    
    typedef struct{ //se maneja una por particion 88 bytes
        int num_inod; //cantidad de i-nodos que tiene el sistema de archivos
        int num_bloq; //cantidad de bloques de datos que tiene el sistema de archivos
        int tam_bloq; //tamaño dado a cada bloque
        int num_mag; //firma numero de carne
        int bloq_lib; //cantidad de bloques de datos libres
        int inod_lib; //cantidad de inodos libres
        char fech_mont[16]; //fecha de montaje
        char fech_desmont[16]; //fecha de desmontaje
        int cant_mont; //cantidad de veces q se monto el sistema
        int ini_bloq_bit; //apuntador al inicio del bloque de bitmap de ficheros y enlaces simbolicos
        int ini_log; //apuntador al inicio del bloque de la bitacora
        int dir_rz; //apuntador al inodo del directorio raiz (primer inodo creado automaticamente)
        int ini_bloq; //apuntador al primer bloque de datos 
        int bit_lib_inod; //primer bit libre en el bitmap de inodos
        int bit_lib_bloq; //primer bit libre en el bitmap de bloque de datos
        int p_inod_lib; //primer inodo libre
        int p_bloq_lib; //primer bloque libre
    }superBloque;
    
    typedef struct{
        int llave; //id del inodo
        char fech_act[16]; //fecha y hora del ultimo cambio
        int tam; //tamaño del archivo en bytes
        char tipo[50]; //si es archivo o carpeta
        char fech_acc[16]; //fecha de ultimo acceso para obtener info del inodo
        char fech_crea[16]; //fecha de creacion del inodo
        int apt_dir[5]; //apuntador a id de bloque 5 apt
        int apt_ind[2]; //apuntador indirecto a id bloque 2 apt
    }inodo;

    typedef struct{
        int llave; //id del bloque
        char padre[16]; //nombre de la carpeta padre  o contenedora 16 carac
        char nombre[16]; //nombre del archivo o carpeta  16 carac
        char contenido[64]; //contenido de texto q puede existir en un archivo
        int apt[6]; //apuntadores hacia inodos que forman parte de esta carpeta
    }bloquEXT;
    
    typedef struct{
        int tip_oper; //tipo de operacion a realizarse
        int tipo; //si es archivo (0), si es directorio(1)
        char nombre[100]; //nombre del archivo o directorio path completo 100 carac
        char contenido[64]; //contenido si es bloq 
        char fech_trans[16]; //fecha de transaccion 
    }log;
    
    //bloquEXT getDirEXT(superBloque sb,char nomDisco[],nodol *padre,int id);
    int ajuste(infoPart particion,int cantidad);
    int escribirBloques(infoPart info,int i,int numBloqs,char contenido[]);
    //void escribirBloques(infoPart info,int i,int numBloqs,char contenido);
    infoPart informacionParticion(char nomDisco[],char particion[]);
    void archivoFAT();
    void archivoENLAZADO();
    void archivoEXT3();
    void crearArchivo();
    
#ifdef	__cplusplus
}
#endif

#endif	/* PLANTILLA3_201114717_H */

