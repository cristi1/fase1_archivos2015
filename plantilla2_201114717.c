#include "plantilla1_201114717.h"
#include "plantilla2_201114717.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int byteIndAct=0;

void crearParticion(){
    char id[3],nomPart[32],dir[145];
    int tm,tipoPart,tipoAjust,n,x,i,totPart,tamInfo;
    float tamPart,tamBloq;
    printf("ID del disco donde se creara la Particion: ");
    scanf("%s",id);
    FILE *discoActual;
    disco temp;
    mbr mbrDisco;
    temp=existeDiscoIndex(id);
    if(strcmp(temp.id,id)==0){
        strcpy(dir,ubic_general);
        strcat(dir,temp.nombre);
        strcat(dir,".vd");
        totPart=temp.prtPrim+temp.prtExt+temp.prtLog;
        printf("espacio libre %f  #particiones %i\n",temp.spLibre,totPart);
        if((temp.spLibre/1024)>=32 && totPart<16){ //spLibre dado en Bytes
            discoActual=fopen(dir,"rb+");
            if(discoActual!=NULL){
                fread(&mbrDisco,sizeof(mbr),1,discoActual);
                do{
                printf("Nombre de la particion(max 32 caracteres): ");
                getchar();
                fflush(stdin);
                scanf("%[^\n]",nomPart);
                tm=strlen(nomPart); 
                }while(tm>32);
                printf("------- El disco %s.vd dispone de %.0fKb libres -------\n",temp.nombre,(temp.spLibre/1024));
                do{
                    printf("Tamaño de la particion en Kb(min 32): ");
                    scanf("%f",&tamPart);
                }while(tamPart<32 || tamPart>(temp.spLibre/1024));
                printf("----- El disco puede manejar 1 particion extendida, 12 logicas y 3 primarias -----\n");
                printf("El disco contiene las siguientes particiones\n  %i primarias\n  %i extendidas\n  %i logicas\n",temp.prtPrim,temp.prtExt,temp.prtLog);
                do{
                    printf("Tipo de Particion(1->Primaria, 2->Extendida, 3->Lógica): ");
                    scanf("%i",&tipoPart);
                    if((tipoPart==2 && temp.prtExt==1) || (tipoPart==1 && temp.prtPrim==3) || (tipoPart==3 && temp.prtLog==12)){
                        printf("error verifique el tipo de particion !!\n");
                        tipoPart=0;
                    }
                }while(tipoPart<1 || tipoPart>3 );
                do{
                    printf("Tipo de Ajuste(1->Primer, 2->Mejor, 3->Peor): ");
                    scanf("%i",&tipoAjust);
                }while(tipoAjust<1 || tipoAjust>3);
                do{
                    printf("Tamaño del bloque en bytes(min 128): ");
                    scanf("%f",&tamBloq);
                }while(tamBloq<128);
                tamInfo=tamBloq-sizeof(bloque);
                char contenido[tamInfo];
                //---------------SE CREARA LA PARTCICION------------------------
                n=(((tamPart*1024)-(sizeof(fatRootFolder)*256))/(1+tamBloq));
                switch(tipoPart){
                    case 1:
                        temp.prtPrim=temp.prtPrim+1;
                        break;
                    case 2:
                        temp.prtExt=temp.prtExt+1;
                        break;
                    default:
                        temp.prtLog=temp.prtLog+1;
                }
                temp.spLibre=temp.spLibre-(tamPart*1024);
                actualizarDiscoIndex(temp); //actualiza index
                fseek(discoActual,0,SEEK_SET);
                x=mbrDisco.cantPart;
                i=sizeof(mbr);
                while(x>0){
                    if(mbrDisco.iPart[x-1].estado==1){
                        i=i+(mbrDisco.iPart[x-1].tam);
                    }
                    x=x-1;
                }
                x=mbrDisco.cantPart;
                mbrDisco.iPart[x].byteInicio=i;
                mbrDisco.iPart[x].cantBloqLib=n;
                mbrDisco.iPart[x].cantBloques=n;
                mbrDisco.iPart[x].estado=1;
                strcpy(mbrDisco.iPart[x].nombre,"");
                strcat(mbrDisco.iPart[x].nombre,nomPart);
                mbrDisco.iPart[x].tam=tamPart*1024;
                mbrDisco.iPart[x].tamBloq=tamBloq;
                mbrDisco.iPart[x].tipoAjuste=tipoAjust;
                mbrDisco.iPart[x].tipoPart=tipoPart;
                mbrDisco.cantPart=mbrDisco.cantPart+1;
                fwrite(&mbrDisco,sizeof(mbr),1,discoActual); //actualiza mbr
                //------------------------particionar---------------------------
                fseek(discoActual,i,SEEK_SET);
                int j;
                for(j=0;j<n;j++){ //escribe FAT
                    byte fat;
                    fat.a='0'; //0 indica bloq libre 1 bloque equivocado
                    fwrite(&fat,sizeof(byte),1,discoActual);
                }
                for(j=0;j<256;j++){ //escribe FAT root folder
                    fatRootFolder frf;
                    frf.estado=-1; //bloque no usado o no inicializado
                    fwrite(&frf,sizeof(fatRootFolder),1,discoActual);
                }
                for(j=0;j<n;j++){
                    bloque bloq;
                    bloq.id=j+1;
                    strcpy(contenido,"");
                    fwrite(&bloq,sizeof(bloque),1,discoActual);
                    fwrite(contenido,sizeof(contenido),1,discoActual);
                }
                fclose(discoActual);
            }else{
                printf("error al intentar acceder al disco %s.vd  !!\n",temp.nombre);
            }  
        }else{
            printf("error ya no hay espacio suficiente para seguir creando particiones !!\n");
        }
    }else{
        printf("error id %s no encontrado !!\n",id);
    }
}

void EliminarParticion(){
    
}

void actualizarDiscoIndex(disco aux1){
    FILE *index;
    index=fopen(ubi_index,"rb+");
    if(index!=NULL){
        fseek(index,byteIndAct,SEEK_SET);
        fwrite(&aux1,sizeof(disco),1,index);
        fclose(index);
    }
}

disco existeDiscoIndex(char id[]){ //0 no existe, 1 existe
    FILE *index;
    disco aux1;
    index=fopen(ubi_index,"rb+");
    if(index!=NULL){
        fseek(index,0,SEEK_SET);
        byteIndAct=ftell(index);
        fread(&aux1,sizeof(disco),1,index);
        while(!feof(index) && strcmp(aux1.id,id)!=0){
            byteIndAct=ftell(index);
            fread(&aux1,sizeof(disco),1,index);
        }
        if(strcmp(aux1.id,id)!=0){
            strcpy(aux1.id,"xxx");
        }
    }
    return aux1;
}