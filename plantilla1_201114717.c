#include "menu_201114717.h"
#include "plantilla1_201114717.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

//char ubic_general[110],ubi_index[100];
//home//clnx//Escritorio//pp a1//


void inicio(){
    DIR *carpeta;
    char disco[100];
    FILE *index;
    printf("Ingrese la ubicacion de la carpeta a utilizar: ");
    getchar();
    fflush(stdin);
    scanf("%[^\n]",disco);
    strcpy(ubic_general,disco); //printf("%s",ubic_general);
    if((carpeta=opendir(ubic_general))!=NULL){
        strcat(disco,"index.idx");
        strcpy(ubi_index,disco);
        index=fopen(disco,"rb+");
        if(index==NULL){
            index=fopen(disco,"w");
        }
        fclose(index);
        printf("%s",disco);
        closedir(carpeta);
        menu0();
    }else{
        printf("Error el directorio no existe!\n");
        inicio();
    }
}

void crearDisco(){
    disco aux;
    FILE *index;
    FILE *nuevoDisco;
    char nom[32],dirNuevo[145];
    int tam,tm,posf,d,l;
    printf("Nombre disco: ");
    getchar();
    fflush(stdin);
    scanf("%[^\n]",nom);
    strcpy(dirNuevo,ubic_general);
    strcat(dirNuevo,nom);
    strcat(dirNuevo,".vd");
    tm=strlen(nom);
    if(tm<32){
        printf("Tamaño disco en Mb: ");
        scanf("%i",&tam);
        if(tam>0){
            index=fopen(ubi_index,"rb+");
            if(index!=NULL){
                fseek(index,0,SEEK_END);
                posf=ftell(index);
                d=(posf/sizeof(disco));
                strcpy(aux.id,"");
                strcat(aux.id,"vd");
                aux.id[2]=97+d;
                strcpy(aux.nombre,nom);
                aux.tam=tam;
                aux.prtPrim=0;
                aux.prtExt=0;
                aux.prtLog=0;
                aux.spLibre=tam;
                aux.estado=1;
                fwrite(&aux,sizeof(disco),1,index);
                fclose(index);
                nuevoDisco=fopen(dirNuevo,"w");
                if(nuevoDisco!=NULL){
                    mbr nuevoMBR;
                    strcpy(nuevoMBR.nombre,nom);
                    nuevoMBR.tam=tam;
                    nuevoMBR.cantPart=0;
                    fwrite(&nuevoMBR,sizeof(mbr),1,nuevoDisco);
                    int k;
                    for(k=0;k<((tam*1024*1024)-1000);k++){
                    byte c;
                    fwrite(&c,sizeof(byte),1,nuevoDisco); 
                    }
                    fclose(nuevoDisco);
                    printf("Se creado el disco %s.vd\n",nom);
                }else{
                    printf("Error al crear Disco!\n");
                    crearDisco();
                }
            }
        }else{
            printf("El tamaño del disco debe ser mayor a 0 Mb\n");
            crearDisco();
        }
    }else{
        printf("El nombre del disco debe ser menor o igual a 32 caracteres!\n");
        crearDisco();
    }
}

void aumentarTamDisco(){
    char id[3],actual[40],dir[150];
    int tam,vacio,byte_act; 
    FILE *aux,*index;
    disco aux1;
    printf("ID del disco: ");
    scanf("%s",id);
    index=fopen(ubi_index,"rb+");
    if(index!=NULL){
        fseek(index,0,SEEK_END);
        vacio=ftell(index);
        if(vacio>0){
            fseek(index,0,SEEK_SET);
            byte_act=ftell(index);
            fread(&aux1,sizeof(disco),1,index);
            while(!feof(index) && strcmp(aux1.id,id)!=0){
                byte_act=ftell(index);
                fread(&aux1,sizeof(disco),1,index);
            }
            if(strcmp(aux1.id,id)!=0){
                printf("disco con id %s no encontrado!\n",id);
            }else{
                if(aux1.estado==0){
                    printf("el disco con id %s ha sido eliminado !!",id);
                }else{
                    strcpy(actual,"");
                    strcat(actual,aux1.nombre);
                    strcpy(dir,"");
                    strcat(dir,ubic_general);
                    strcat(dir,actual);
                    strcat(dir,".vd");
                    printf("Mb a aumentar en el disco: ");
                    scanf("%i",&tam);
                    if(tam>0){
                        aux=fopen(dir,"rb+");
                        if(aux!=NULL){
                            fseek(aux,0,SEEK_END);
                            int k;
                            for(k=0;k<(tam*1024*1024);k++){
                                byte c;
                                fwrite(&c,sizeof(byte),1,aux); 
                            }
                            fclose(aux);
                            fseek(index,byte_act,SEEK_SET);
                            aux1.tam=aux1.tam+tam;
                            aux1.spLibre=aux1.spLibre+tam;
                            fwrite(&aux1,sizeof(disco),1,index);
                            printf("se han aumentado %i Mb al disco %s.vd !!\n",tam,aux1.nombre);
                        }
                    }else{
                        printf("el tamaño debe ser mayor a 0!\n");
                    }
                }
            }
        }else{
            printf("no se ha creado ningun disco!\n");
        }
        fclose(index);
    }else{
        printf("error al buscar id!\n");
    }
}

void reducirTamDisco(){
    char id[3],actual[40],dir[150];
    int tam,vacio,sinPart,byte_act,reducir; 
    FILE *aux,*index;
    disco aux1;
    printf("ID del disco: ");
    scanf("%s",id);
    index=fopen(ubi_index,"rb+");
    if(index!=NULL){
        fseek(index,0,SEEK_END);
        vacio=ftell(index);
        if(vacio>0){
            fseek(index,0,SEEK_SET);
            byte_act=ftell(index);
            fread(&aux1,sizeof(disco),1,index);
            while(!feof(index) && strcmp(aux1.id,id)!=0){
                byte_act=ftell(index);
                fread(&aux1,sizeof(disco),1,index);
            }
            if(strcmp(aux1.id,id)!=0){
                printf("disco con id %s no encontrado!\n",id);
            }else{
                if(aux1.estado==0){
                    printf("el disco con id %s ha sido eliminado !!",id);
                }else{
                    sinPart=aux1.spLibre;
                    strcpy(actual,"");
                    strcat(actual,aux1.nombre);
                    strcpy(dir,"");
                    strcat(dir,ubic_general);
                    strcat(dir,actual);
                    strcat(dir,".vd");
                    printf("Mb a reducir en el disco: ");
                    scanf("%i",&tam);
                    if(tam>0 && tam<=sinPart){
                        aux=fopen(dir,"rb+");
                        if(aux!=NULL){
                            reducir=aux1.tam-tam;
                            truncate(dir,reducir*1024*1024);
                            fclose(aux);
                            fseek(index,byte_act,SEEK_SET);
                            aux1.tam=aux1.tam-tam;
                            aux1.spLibre=aux1.spLibre-tam;
                            fwrite(&aux1,sizeof(disco),1,index);
                            printf("se redujeron %i Mb al disco %s.vd !!\n",tam,aux1.nombre);
                        }else{
                            printf("no se ha podido acceder al disco!");
                        }
                    }else{
                        printf("no se ha podido reducir el tamaño del disco!\n");
                    }
                }
            }
        }else{
            printf("no se ha creado ningun disco!\n");
        }
        fclose(index);
    }else{
        printf("error al buscar id!\n");
    }
    
}

void eliminarDisco(){
    char nombre[32],dir[150];
    int vacio,byte_act,resp,r; 
    FILE *index;
    disco aux1;
    printf("Nombre del disco a eliminar: ");
    getchar();
    fflush(stdin);
    scanf("%[^\n]",nombre);
    index=fopen(ubi_index,"rb+");
    if(index!=NULL){
        fseek(index,0,SEEK_END);
        vacio=ftell(index);
        if(vacio>0){
            fseek(index,0,SEEK_SET);
            byte_act=ftell(index);
            fread(&aux1,sizeof(disco),1,index);
            while(!feof(index) && strcmp(aux1.nombre,nombre)!=0){
                byte_act=ftell(index);
                fread(&aux1,sizeof(disco),1,index);
            }
            if(strcmp(aux1.nombre,nombre)!=0){
                printf("disco con nombre %s no encontrado!\n",nombre);
            }else{
                if(aux1.estado==0){
                    printf("el disco %s.vd ya ha sido eliminado !!",nombre);
                }else{
                    strcpy(dir,"");
                    strcat(dir,ubic_general);
                    strcat(dir,nombre);
                    strcat(dir,".vd");
                    printf("Confirmar eliminar disco %s.vd 1 --> eliminar : ",nombre);
                    scanf("%i",&r);
                    if(r==1){
                        resp=remove(dir);
                        if(resp==0){
                            printf("el disco %s.vd ha sido eliminado !!\n",nombre);
                        }else{
                            printf("error al intentar eliminar el disco %s.vd !!\n",nombre);
                        }
                        fseek(index,byte_act,SEEK_SET);
                        aux1.estado=0;
                        fwrite(&aux1,sizeof(disco),1,index);
                    }else{
                        printf("el disco %s.vd no ha sido eliminado !!\n",nombre);
                    }
                }
            }
        }else{
            printf("no se ha creado ningun disco!\n");
        }
        fclose(index);
    }
}