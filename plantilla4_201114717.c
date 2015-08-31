#include "plantilla1_201114717.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void archDiscos(){
    char dir_csv[120];
    char stam[5],sprim[2],sext[2],slog[2],slib[2],sest[1];
    strcpy(dir_csv,"");
    strcat(dir_csv,ubic_general);
    strcat(dir_csv,"discos.csv");
    FILE *csv,*index;
    index=fopen(ubi_index,"rb+");
    csv=fopen(dir_csv,"wt");
    if(index!=NULL && csv!=NULL){
        disco aux;
        fseek(index,0,SEEK_SET);
        fwrite("ID,",3,1,csv);
        fwrite("Nombre,",7,1,csv);
        fwrite("Tamanio,",8,1,csv);
        fwrite("Primarias,",10,1,csv);
        fwrite("Extendidas,",11,1,csv);
        fwrite("Logicas,",8,1,csv);
        fwrite("Sin Particionar,",16,1,csv);
        fwrite("Estado\n",7,1,csv);
        fread(&aux,sizeof(disco),1,index);
        while(!feof(index)){  
            fwrite(aux.id,sizeof(aux.id),1,csv);
            fwrite(",",1,1,csv);
            fwrite(aux.nombre,strlen(aux.nombre),1,csv);
            fwrite(",",1,1,csv);
            strcpy(stam,"");
            sprintf(stam,"%d",aux.tam);
            fwrite(stam,sizeof(stam),1,csv);
            fwrite(",",1,1,csv);
            strcpy(sprim,"");
            sprintf(sprim,"%d",aux.prtPrim);
            fwrite(sprim,sizeof(sprim),1,csv);
            fwrite(",",1,1,csv);
            strcpy(sext,"");
            sprintf(sext,"%d",aux.prtExt);
            fwrite(sext,sizeof(sext),1,csv);
            fwrite(",",1,1,csv);
            strcpy(slog,"");
            sprintf(slog,"%d",aux.prtLog);
            fwrite(slog,sizeof(slog),1,csv);
            fwrite(",",1,1,csv);
            strcpy(slib,"");
            sprintf(slib,"%d",aux.spLibre);
            fwrite(slib,sizeof(slib),1,csv);
            fwrite(",",1,1,csv);
            sprintf(sest,"%d",aux.estado);
            fwrite(sest,sizeof(sest),1,csv);
            fwrite("\n",1,1,csv);
            fread(&aux,sizeof(disco),1,index);
        }
        fclose(index);
        fclose(csv);
        printf("se ha creado el archivo discos.csv !!\n");
    }else{
        printf("no se ha podido crear el archivo de discos.csv !!\n");
    }
}

void mbrDisco(){
    FILE *fichero;
    char dir_mbr[120],grafica[120],*estado,*particion,*ajuste;
    mbr miMBR;
    strcpy(dir_mbr,"");
    strcat(dir_mbr,ubic_general);
    strcat(dir_mbr,"estructura.dot");
    strcpy(grafica,"");
    strcpy(grafica,ubic_general);
    strcpy(grafica,"grafica.png");
    
    fichero = fopen(dir_mbr, "w" );
    if(fichero!=NULL){
        fprintf(fichero, "digraph G {\n node[shape=box, style=filled, color=Gray95];\n edge[color=blue]; \n rankdir=UD;\n" );
        fprintf(fichero,"%s,%i Mb, %i particiones",miMBR.nombre,miMBR.tam,miMBR.cantPart);
        if(miMBR.cantPart>0){
            int k;
            for(k=0;k<=miMBR.cantPart;k++){
                fprintf(fichero, "subgraph cluster_%i { \n style=filled; \n color=lightgrey; \n node [style=filled,color=white]; \n",k);
                strcpy(estado,"");
                strcpy(particion,"");
                strcpy(ajuste,"");
                if(miMBR.iPart[k].estado==1){
                    strcat(estado,"activa");
                }else{
                    strcat(estado,"eliminada");
                }
                switch(miMBR.iPart[k].tipoPart){
                    case 1:
                        strcat(particion,"primaria");
                        break;
                    case 2:
                        strcat(particion,"extendida");
                        break;
                    case 3:
                        strcat(particion,"logica");
                        break;
                    default:
                        strcat(particion,"error");
                }
                switch(miMBR.iPart[k].tipoAjuste){
                    case 1:
                        strcat(ajuste,"primer ajuste");
                        break;
                    case 2:
                        strcat(ajuste,"mejor ajuste");
                        break;
                    case 3:
                        strcat(ajuste,"peor ajuste");
                        break;
                    default:
                        strcat(ajuste,"error");
                }
                fprintf(fichero,"%s,Particion %i,inicia en %i,%i bloques,%i bloques libres,%s,%s,%i\n",estado,k+1,miMBR.iPart[k].byteInicio,miMBR.iPart[k].cantBloques,miMBR.iPart[k].cantBloqLib,particion,ajuste,miMBR.iPart[k].tam);
                fprintf(fichero, "\n}");
            }
        }
        fprintf(fichero, "\n}");
        fprintf( stdout, "Datos escritos en el archivo: %s\n",dir_mbr);
        if( !fclose(fichero) ){
        fflush(stdin);
        printf( "Archivo cerrado\n" );
        system("dot -Tpng %s -o %s",dir_mbr,grafica);
        }else{
            printf("no se ha podido generar la grafica del MBR !!\n");
        }
    }
}

void existe(char *id){
    
}