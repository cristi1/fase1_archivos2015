#include "plantilla1_201114717.h"
#include "plantilla2_201114717.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void crearParticion(){
    char id[3],nomPart[32],dir[145];
    int tm,tamPart,tipoPart,tipoAjust,tamBloq,totPart,n;
    printf("ID del disco donde se creara la Particion: ");
    scanf("%s",id);
    FILE *discoActual;
    disco temp;
    mbr mbrDisco;
    temp=existeDisco(id);
    if(strcmp(temp.id,id)==0){
        strcpy(dir,ubic_general);
        strcat(dir,temp.nombre);
        strcat(dir,".vd");
        totPart=temp.prtPrim+temp.prtExt+temp.prtLog;
        if(temp.spLibre>=32 && totPart<16){ //verificar spLibre Mb o Kb
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
                printf("¡ el %s.vd dispone de %eKb libres !",temp.nombre,(temp.spLibre*1024));
                do{
                    printf("Tamaño de la particion en Kb(min 32): ");
                    scanf("%i",&tamPart);
                }while(tamPart<32 || tamPart>(temp.spLibre*1024));
                printf("El disco puede manejar 1 particion extendida, 12 logicas y 3 primarias !!\n");
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
                    scanf("%i",&tamBloq);
                }while(tamBloq<128);
                
                n=(((tamPart*1024)-(sizeof(fatRootFolder)*360))/(1+tamBloq));
                
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

mbr obtenerMBR(char idDisco[]){
    
}

mbr actualizarMBR(char nombreDisco[]){}

disco existeDisco(char id[]){ //0 no existe, 1 existe
    FILE *index;
    int byte_act;
    disco aux1;
    index=fopen(ubi_index,"rb+");
    if(index!=NULL){
        fseek(index,0,SEEK_SET);
        byte_act=ftell(index);
        fread(&aux1,sizeof(disco),1,index);
        while(!feof(index) && strcmp(aux1.id,id)!=0){
            byte_act=ftell(index);
            fread(&aux1,sizeof(disco),1,index);
        }
        if(strcmp(aux1.id,id)!=0){
            strcpy(aux1.id,"xxx");
        }
    }/*else{
        printf("error al intentar aperturar archivo index !!\n",id);
    }*/
    return aux1;
}