#include "plantilla1_201114717.h"
#include "plantilla4_201114717.h"
#include "plantilla2_201114717.h"
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
    char dir_mbr[120],grafica[120],estado[10],parti[9],ajus[15],nombre[32],dir[150],id[3],dot[300],sis[10];
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
                                strcpy(sis,"");
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
                                switch(miMBR.iPart[k].sisArchivos){
                                    case 1:
                                        strcat(sis,"FAT");
                                        break;
                                    case 2:
                                        strcat(sis,"Enlazado");
                                        break;
                                    case 3:
                                        strcat(sis,"EXT3");
                                        break;
                                    default:
                                        strcat(sis,"SinFormato");
                                }
                                fprintf(fichero,"server%i[label=\"%s, %s, inicia en %i, %i bloques, %i bloques libres, %s, %s, %.1f Kb, %s\"];}\n",k+1,estado,miMBR.iPart[k].nombre,miMBR.iPart[k].byteInicio,miMBR.iPart[k].cantBloques,miMBR.iPart[k].cantBloqLib,parti,ajus,(miMBR.iPart[k].tam/(1024)),sis);
                            }
                        }
                        fprintf(fichero, "\n}}");
                        //fprintf( stdout, "Datos escritos en el archivo: %s\n",dir_mbr);
                        if( !fclose(fichero) ){
                        fflush(stdin);
                        //printf( "Archivo cerrado\n" );
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

void reporteParticion(){
    char id[3],nomPart[32],dir[150],dirFat[150],dirRootFat[150],dirBloques[150],stam[10],si[10],sf[10],est[2];
    char idb[10],ant[10],sig[10];
    int vacio,num,bandera,i,lineas;
    disco aux;
    mbr miMBR;
    FILE *disco_act,*fat,*rootFolder,*bloques,*index;
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
                    printf("Nombre de la particion: ");
                    getchar();
                    fflush(stdin);
                    scanf("%[^\n]",nomPart);
                    num=miMBR.cantPart;
                    bandera=-1;
                    while(num>0){
                        if(miMBR.iPart[num-1].estado==1 && strcmp(miMBR.iPart[num-1].nombre,nomPart)==0 && miMBR.iPart[num-1].tipoPart!=2){
                            bandera=num-1;
                            num=1;
                        }
                        num--;
                    }
                    if(bandera!=-1){
                        lineas=0;
                        fseek(disco_act,miMBR.iPart[bandera].byteInicio,SEEK_SET);
                        strcpy(dirFat,"");
                        strcat(dirFat,ubic_general);
                        strcat(dirFat,"FAT.csv");
                        fat=fopen(dirFat,"wt");
                        if(fat!=NULL){
                            for(i=0;i<miMBR.iPart[bandera].cantBloques;i++){ 
                                byte temp;
                                fread(&temp,sizeof(byte),1,disco_act);
                                if(lineas<66){
                                    fputc(temp.a,fat);
                                    fputc(',',fat);
                                    lineas++;
                                }else{
                                    fputc(temp.a,fat);
                                    fputc('\n',fat);
                                    lineas=0;
                                } 
                            }
                            fclose(fat);
                            printf("se creo reporte FAT.csv !!\n");
                        }else{
                            printf("no se pudo crear el reporte FAT.csv !!\n");
                        }
                        fseek(disco_act,(miMBR.iPart[bandera].byteInicio)+miMBR.iPart[bandera].cantBloques,SEEK_SET);
                        strcpy(dirRootFat,"");
                        strcat(dirRootFat,ubic_general);
                        strcat(dirRootFat,"RootFolder.csv");
                        rootFolder=fopen(dirRootFat,"wt");
                        if(rootFolder!=NULL){
                            fwrite("Nombre,",7,1,rootFolder);
                            fwrite("Extension,",10,1,rootFolder);
                            fwrite("Creacion,",9,1,rootFolder);
                            fwrite("Tamanio(b),",11,1,rootFolder);
                            fwrite("Inicio,",7,1,rootFolder);
                            fwrite("Final,",6,1,rootFolder);
                            fwrite("Estado\n",7,1,rootFolder);
                            for(i=0;i<256;i++){
                                fatRootFolder tempFrf;
                                fread(&tempFrf,sizeof(fatRootFolder),1,disco_act);
                                fwrite(tempFrf.nombre,strlen(tempFrf.nombre),1,rootFolder);
                                fwrite(",",1,1,rootFolder);
                                fwrite(tempFrf.extension,strlen(tempFrf.extension),1,rootFolder);
                                fwrite(",",1,1,rootFolder);
                                fwrite(tempFrf.fecha,strlen(tempFrf.fecha),1,rootFolder);
                                fwrite(",",1,1,rootFolder);
                                strcpy(stam,"");
                                sprintf(stam,"%i",tempFrf.tamanio);
                                fwrite(stam,strlen(stam),1,rootFolder);
                                fwrite(",",1,1,rootFolder);
                                strcpy(si,"");
                                sprintf(si,"%i",tempFrf.idInicio);
                                fwrite(si,strlen(si),1,rootFolder);
                                fwrite(",",1,1,rootFolder);
                                strcpy(sf,"");
                                sprintf(sf,"%i",tempFrf.idFinal);
                                fwrite(sf,strlen(sf),1,rootFolder);
                                fwrite(",",1,1,rootFolder);
                                strcpy(est,"");
                                //sprintf(est,"%i",tempFrf.estado);
                                byte temp;
                                temp.a=tempFrf.estado;
                                fwrite(&temp,sizeof(byte),1,rootFolder);
                                //fputc(tempFrf.estado,rootFolder);
                                fwrite("\n",1,1,rootFolder);
                            }
                            fclose(rootFolder);
                            printf("se creo el reporte RootFolder.csv !!\n");
                        }else{
                            printf("no se pudo crear el reporte RootFolder.csv !!\n");
                        }
                        fseek(disco_act,(miMBR.iPart[bandera].byteInicio)+(miMBR.iPart[bandera].cantBloques)+(256*sizeof(fatRootFolder)),SEEK_SET);
                        strcpy(dirBloques,"");
                        strcat(dirBloques,ubic_general);
                        strcat(dirBloques,"Bloques.csv");
                        bloques=fopen(dirBloques,"wt");
                        if(bloques!=NULL){
                            fwrite("Id,",3,1,bloques);
                            fwrite("Estado,",7,1,bloques);
                            fwrite("Anterior,",9,1,bloques);
                            fwrite("Siguiente,",10,1,bloques);
                            fwrite("Datos\n",6,1,bloques);
                            char contenido[miMBR.iPart[bandera].tamBloq-sizeof(bloque)];
                            for(i=0;i<miMBR.iPart[bandera].cantBloques;i++){
                                bloque bloq;
                                strcpy(contenido,"");
                                fread(&bloq,sizeof(bloque),1,disco_act);
                                fread(contenido,sizeof(contenido),1,disco_act);
                                strcpy(idb,"");
                                sprintf(idb,"%i",bloq.id);
                                fwrite(idb,strlen(idb),1,bloques);
                                fwrite(",",1,1,bloques);
                                char estb[7];
                                switch(bloq.estado){
                                    strcpy(estb,"");
                                    case '0':
                                        strcpy(estb,"Libre");
                                        break;
                                    case '1':
                                        strcpy(estb,"Ocupado");
                                        break;
                                    default:
                                        strcat(estb,"");
                                }
                                fwrite(estb,strlen(estb),1,bloques);
                                fwrite(",",1,1,bloques);
                                sprintf(ant,"%i",bloq.anterior);
                                fwrite(ant,strlen(ant),1,bloques);
                                fwrite(",",1,1,bloques);
                                sprintf(sig,"%i",bloq.siguiente);
                                fwrite(sig,strlen(sig),1,bloques);
                                fwrite(",",1,1,bloques);
                                fwrite(contenido,strlen(contenido),1,bloques);
                                fwrite("\n",1,1,bloques);
                            }
                            fclose(bloques);
                            printf("se creo el reporte Bloques.csv !!\n");
                        }else{
                            printf("no se pudo crear el reporte Bloques.csv !!\n");
                        }
                    }else{
                        printf("particion %s no encontrada verifique o hace referencia a una extendida!!\n",nomPart);
                    }
                    fclose(disco_act);
                }else{
                    printf("error al aperturar disco %s.vd !!\n",aux.nombre);
                }
            }else{
                printf("disco con id %s no encontrado !!\n",id);
            }
        }else{
            printf("no se ha creado ningun disco !!\n");
        }
        fclose(index);
    }else{
        printf("error al aperturar el index !!\n");
    }
}

/*void reportePartDisco(){
    FILE *fichero,*disco_act,*index;
    char dir_mbr[120],grafica[120],dir[150],id[3];
    int vacio,p,e;
    disco aux;
    mbr miMBR;
    strcpy(dir_mbr,"");
    strcat(dir_mbr,ubic_general);
    strcat(dir_mbr,"estructura1.dot");
    strcpy(grafica,"");
    strcpy(grafica,ubic_general);
    strcpy(grafica,"grafica1.png");
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
                    e=0;
                    if(fichero=NULL){
                        fprintf(fichero,"digraph G {node[ shape=box, style=filled,color=Gray95]; edge[color=blue]; rankdir=BT;\n" );
                        fprintf(fichero,"subgraph cluster0 {color=lightgrey;  node [color=white]; \n");
                        int k;
                        if(aux.spLibre>0){
                            e=1;
                            fprintf(fichero,"subgraph cluster1 {color=lightgrey;  node [color=white];\n");
                            fprintf(fichero,"server1[label=\"Libre\"];}\n");
                        }
                        if(miMBR.cantPart>0){
                            for(k=0;k<miMBR.cantPart;k++){
                                if(miMBR.iPart[k].tipoPart==1){
                                    fprintf(fichero,"subgraph cluster%i {color=lightgrey;  node [color=white];\n",k+2);
                                    fprintf(fichero,"server%i[label=\"Primaria\"];}",k+1+p);
                                }else{
                                    if(miMBR.iPart[k].tipoPart==2){
                                        fprintf(fichero,"subgraph cluster%i {color=lightgrey;  node [color=white];\n",k+1+p);
                                        fprintf(fichero,"server%i[label=\"Extendida\"];\n",k+1+p);
                                        for(p=0;p<aux.prtLog;p++){
                                            fprintf(fichero,"subgraph cluster%i {color=lightgrey;  node [color=white];\n",k+1+p);
                                            fprintf(fichero,"server%i[label=\"Logica\"];}\n",k+1+p);
                                        }
                                        fprintf(fichero,"}\n");
                                    }       
                                }
                            }
                        }
                        fprintf(fichero,"subgraph cluster%i {color=lightgrey;  node [color=white];",k+1);
                        fprintf(fichero,"server%i[label=\"MBR\"];}",k+1);
                        fprintf(fichero,"\n}}");
                        if( !fclose(fichero) ){
                        fflush(stdin);
                        //printf( "Archivo cerrado\n" );
                        /*strcpy(dot,"");
                        strcat(dot,"dot -Tpng ");
                        strcat(dot,dir_mbr);
                        strcat(dot," -o ");
                        strcat(dot,grafica);
                         //"dot -Tpng %s -o %s",dir_mbr,grafica
                        }
                        system("dot -Tpng estructura1.dot -o grafica1.png");
                    } 
                }
            }else{
                printf("error id %s no encontrado !!\n",id);
            }
        }
        fclose(index);
    }
}*/