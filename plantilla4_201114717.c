#include "plantilla1_201114717.h"
#include "plantilla4_201114717.h"
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
        fwrite("Tamanio(Mb),",12,1,csv);
        fwrite("Primarias,",10,1,csv);
        fwrite("Extendidas,",11,1,csv);
        fwrite("Logicas,",8,1,csv);
        fwrite("Sin Particionar(Mb),",20,1,csv);
        fwrite("Estado\n",7,1,csv);
        fread(&aux,sizeof(disco),1,index);
        while(!feof(index)){  
            fwrite(aux.id,strlen(aux.id),1,csv);
            fwrite(",",1,1,csv);
            fwrite(aux.nombre,strlen(aux.nombre),1,csv);
            fwrite(",",1,1,csv);
            strcpy(stam,"");
            sprintf(stam,"%.2f",(aux.tam/(1024*1024)));
            fwrite(stam,strlen(stam),1,csv);
            fwrite(",",1,1,csv);
            strcpy(sprim,"");
            sprintf(sprim,"%d",aux.prtPrim);
            fwrite(sprim,strlen(sprim),1,csv);
            fwrite(",",1,1,csv);
            strcpy(sext,"");
            sprintf(sext,"%d",aux.prtExt);
            fwrite(sext,strlen(sext),1,csv);
            fwrite(",",1,1,csv);
            strcpy(slog,"");
            sprintf(slog,"%d",aux.prtLog);
            fwrite(slog,strlen(slog),1,csv);
            fwrite(",",1,1,csv);
            strcpy(slib,"");
            sprintf(slib,"%.2f",(aux.spLibre/(1024*1024)));
            fwrite(slib,strlen(slib),1,csv);
            fwrite(",",1,1,csv);
            sprintf(sest,"%d",aux.estado);
            fwrite(sest,strlen(sest),1,csv);
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
    FILE *fichero,*disco_act,*index;
    char dir_mbr[120],grafica[120],estado[10],parti[9],ajus[15],nombre[32],dir[150],id[3],dot[300];
    int vacio;
    disco aux;
    mbr miMBR;
    strcpy(dir_mbr,"");
    strcat(dir_mbr,ubic_general);
    strcat(dir_mbr,"estructura.dot");
    strcpy(grafica,"");
    strcpy(grafica,ubic_general);
    strcpy(grafica,"grafica.png");
    printf("ID del disco: ");
    scanf("%s",id);
    index=fopen(ubi_index,"rb+");
    if(index!=NULL){
        fseek(index,0,SEEK_END);
        vacio=ftell(index);
        if(vacio>0){
            fseek(index,0,SEEK_SET);
            fread(&aux,sizeof(disco),1,index);
            while(!feof(index) && strcmp(aux.id,id)!=0){
                fread(&aux,sizeof(disco),1,index);
            }
            fflush(stdin);
            if(strcmp(aux.id,id)==0 && aux.estado==1){
                strcpy(dir,"");
                strcat(dir,ubic_general);
                strcat(dir,aux.nombre);
                strcat(dir,".vd");
                disco_act=fopen(dir,"rb+");
                if(disco_act!=NULL){
                    fread(&miMBR,sizeof(mbr),1,disco_act);
                    fclose(disco_act);
                    fichero = fopen("estructura.dot", "w" ); //dir_mbr
                    if(fichero!=NULL){
                        fprintf(fichero, "digraph G {node[ shape=box, style=filled,color=Gray95]; edge[color=blue]; rankdir=LR;\n" );
                        fprintf(fichero, "subgraph cluster0 {color=lightgrey;  node [color=white]; \n");
                        fprintf(fichero,"server0[label=\"%s, %.1f Mb, %i particiones\"];\n",miMBR.nombre,(miMBR.tam/(1024*1024)),miMBR.cantPart);
                        if(miMBR.cantPart>0){
                            int k;
                            for(k=0;k<miMBR.cantPart;k++){
                                fprintf(fichero, "subgraph cluster%i {color=lightgrey;  node [color=white];\n",k+1);
                                strcpy(estado,"");
                                strcpy(parti,"");
                                strcpy(ajus,"");
                                if(miMBR.iPart[k].estado==1){
                                    strcat(estado,"activa");
                                }else{
                                    strcat(estado,"eliminada");
                                }
                                switch(miMBR.iPart[k].tipoPart){
                                    case 1:
                                        strcat(parti,"primaria");
                                        break;
                                    case 2:
                                        strcat(parti,"extendida");
                                        break;
                                    case 3:
                                        strcat(parti,"logica");
                                        break;
                                    default:
                                        strcat(parti,"error");
                                }
                                switch(miMBR.iPart[k].tipoAjuste){
                                    case 1:
                                        strcat(ajus,"primer ajuste");
                                        break;
                                    case 2:
                                        strcat(ajus,"mejor ajuste");
                                        break;
                                    case 3:
                                        strcat(ajus,"peor ajuste");
                                        break;
                                    default:
                                        strcat(ajus,"error");
                                }
                                fprintf(fichero,"server%i[label=\"%s, %s, inicia en %i, %i bloques, %i bloques libres, %s, %s, %.1f Kb\"];}\n",k+1,estado,miMBR.iPart[k].nombre,miMBR.iPart[k].byteInicio,miMBR.iPart[k].cantBloques,miMBR.iPart[k].cantBloqLib,parti,ajus,(miMBR.iPart[k].tam/(1024)));
                            }
                        }
                        fprintf(fichero, "\n}}");
                        fprintf( stdout, "Datos escritos en el archivo: %s\n",dir_mbr);
                        if( !fclose(fichero) ){
                        fflush(stdin);
                        printf( "Archivo cerrado\n" );
                        strcpy(dot,"");
                        strcat(dot,"dot -Tpng ");
                        strcat(dot,dir_mbr);
                        strcat(dot," -o ");
                        strcat(dot,grafica);
                        system("dot -Tpng estructura.dot -o grafica.png"); //"dot -Tpng %s -o %s",dir_mbr,grafica
                        }else{
                            printf("no se ha podido generar la grafica del MBR !!\n");
                        }
                    }
                }else{
                    printf("error no se ha podido acceder al disco %s.vd !!\n",nombre);
                }
            }else{
                printf("disco con id %s no encontrado !!\n",id);
            }
        }
        fclose(index);
    }else{
        printf("error no se ha podido buscar el id del disco !!\n");
    }
}