#include "plantilla1_201114717.h"
#include "plantilla2_201114717.h"
#include "plantilla3_201114717.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int byteIndAct=0;

void crearParticion(){
    char id[3],nomPart[32],dir[145];
    int tm,tipoPart,tipoAjust,n,x,i,totPart,tamInfo,tipSisArch;
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
        printf(" espacio libre %.0f bytes , No. particiones %i\n",temp.spLibre,totPart);
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
                    printf("Tamaño de la particion en Kb(min 250): ");
                    scanf("%f",&tamPart);
                }while(tamPart<250 || tamPart>(temp.spLibre/1024));
                printf("----- El disco puede manejar 1 particion extendida, 12 logicas y 3 primarias -----\n");
                printf("El disco contiene las siguientes particiones\n  %i primarias\n  %i extendidas\n  %i logicas\n",temp.prtPrim,temp.prtExt,temp.prtLog);
                do{
                    printf("Tipo de Particion(1->Primaria, 2->Extendida, 3->Lógica): ");
                    scanf("%i",&tipoPart);
                    if(tipoPart==3 && temp.prtExt==0){
                        tipoPart=0;
                        printf("no se ha creado la particion extendida correspondiente!!\n");
                    }
                    if((tipoPart==2 && temp.prtExt==1) || (tipoPart==1 && temp.prtPrim==3) || (tipoPart==3 && temp.prtLog==12)){
                        printf("error verifique el tipo de particion !!\n");
                        tipoPart=0;
                    }
                }while(tipoPart<1 || tipoPart>3);
                x=mbrDisco.cantPart;
                i=sizeof(mbr);
                while(x>0){
                    if(mbrDisco.iPart[x-1].estado==1){
                        i=i+(mbrDisco.iPart[x-1].tam);
                    }
                    x=x-1;
                }
                x=mbrDisco.cantPart;
                if(tipoPart!=2){
                    do{
                        printf("Tipo de Ajuste(1->Primer, 2->Mejor, 3->Peor): ");
                        scanf("%i",&tipoAjust);
                    }while(tipoAjust<1 || tipoAjust>3);
                    do{
                        printf("Tamaño del bloque en bytes(min 64): ");
                        scanf("%f",&tamBloq);
                    }while(tamBloq<64);
                    //tamInfo=tamBloq-sizeof(bloque);
                    //char contenido[tamInfo];
                    //n=(((tamPart*1024)-(sizeof(fatRootFolder)*256))/(1+tamBloq));
                    //---------------SE CREARA LA PARTCICION------------------------
                    //mbrDisco.iPart[x].cantBloqLib=n;
                    //mbrDisco.iPart[x].cantBloques=n;
                    mbrDisco.iPart[x].tamBloq=tamBloq; 
                    mbrDisco.iPart[x].tipoAjuste=tipoAjust;
                    //------------------------particionar---------------------------
                    /*fseek(discoActual,i,SEEK_SET);
                    int j;
                    for(j=0;j<n;j++){ //escribe FAT
                        byte fat;
                        fat.a='0'; //0 indica bloq libre 1 bloque ocupado
                        fputc(fat.a,discoActual);
                    }
                    for(j=0;j<256;j++){ //escribe FAT root folder
                        fatRootFolder frf;
                        frf.estado='2'; //bloque no usado o no inicializado
                        strcpy(frf.extension,"");
                        strcpy(frf.fecha,"dd/mm/aaaa-hh:mm");
                        strcpy(frf.nombre,"");
                        frf.idInicio=-1;
                        frf.idFinal=-1;
                        frf.tamanio=0;
                        fwrite(&frf,sizeof(fatRootFolder),1,discoActual);
                    }
                    for(j=0;j<n;j++){ //escribe bloque contenido
                        bloque bloq;
                        bloq.id=j;
                        bloq.anterior=-1;
                        bloq.siguiente=-1;
                        bloq.estado='0';
                        strcpy(contenido,"");
                        fwrite(&bloq,sizeof(bloque),1,discoActual);
                        fwrite(contenido,sizeof(contenido),1,discoActual);
                    }*/
                }else{
                    mbrDisco.iPart[x].tamBloq=-1; //no aplica part Extendida
                    mbrDisco.iPart[x].tipoAjuste=-1; //no aplica part Extendida
                }
                mbrDisco.iPart[x].cantBloqLib=-1; //no aplica part Extendida y usado por FAT y ENLAZADO
                mbrDisco.iPart[x].cantBloques=-1; //no aplica part Extendida y usado por FAT y ENLAZADO
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
                mbrDisco.iPart[x].byteInicio=i;
                mbrDisco.iPart[x].estado=1;
                strcpy(mbrDisco.iPart[x].nombre,"");
                strcat(mbrDisco.iPart[x].nombre,nomPart);
                mbrDisco.iPart[x].tam=tamPart*1024; // en bytes
                mbrDisco.iPart[x].tipoPart=tipoPart;
                mbrDisco.iPart[x].sisArchivos=-1; //sin sistema de archivos ya que no ha sido formateada
                mbrDisco.cantPart=mbrDisco.cantPart+1;
                fseek(discoActual,0,SEEK_SET);
                fwrite(&mbrDisco,sizeof(mbr),1,discoActual); //actualiza mbr
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
    char id[3],dir[145],nomPart[32],diraux[145],dirAux[150];
    int bandera,i,ini,fin;
    disco temp;
    mbr mbrDisco;
    FILE *discoActual,*auxiliar;
    printf("Id del disco: ");
    scanf("%s",id);
    temp=existeDiscoIndex(id);
    if(strcmp(temp.id,id)==0){
        strcpy(dir,ubic_general);
        strcat(dir,temp.nombre);
        strcat(dir,".vd");
        discoActual=fopen(dir,"rb+");
        if(discoActual!=NULL){
            fread(&mbrDisco,sizeof(mbr),1,discoActual);
            printf("Nombre de la particion a eliminar: ");
            getchar();
            fflush(stdin);
            scanf("%[^\n]",nomPart);
            bandera=-1;
            for(i=0;i<mbrDisco.cantPart;i++){
                if((strcmp(mbrDisco.iPart[i-1].nombre,nomPart)==0) && (mbrDisco.iPart[i-1].estado==1)){
                    bandera=i-1;
                    i=mbrDisco.cantPart;
                }
            }
            if(bandera!=-1){  //pendiente
               /* mbrDisco.iPart[bandera].estado=0;
                ini=mbrDisco.iPart[bandera].byteInicio;
                fin=mbrDisco.iPart[bandera].byteInicio+mbrDisco.iPart[bandera].tam;
                strcpy(dir,ubic_general);
                strcat(dir,"auxTempo.vd");
                auxiliar=fopen(dir,"wb+");
                size_t sz = mbrDisco.iPart[bandera].byteInicio;                         // 100,000 bytes
                char *buff = malloc (sz); 
                sz = fread (buff, 1, sz, fin);              // should check for errors
                
                FILE *finp = fopen ("inp.txt", "rb");       // should check for NULLs
FILE *fout = fopen ("out.txt", "wb");
size_t sz = 100000;                         // 100,000 bytes
char *buff = malloc (sz);                   // should check for NULL
sz = fread (buff, 1, sz, fin);              // should check for errors
fwrite (buff, 1, sz, fout);
free (buff);
fclose (fin);
fclose (fout);
rename ("out.txt", "inp.txt);               // should check for error*/
            }
        }else{
            printf("error intentar abrir el disco %s.vd !!\n",temp.nombre);
        }
    }else{
        printf("error id %s no encontrado !!\n",id);
    }
}

int crearFAT(infoPart particion, char nomDisco[]){ //retorna numero de bloques creados
    char dir[145];
    int j,n,tamInfo;
    FILE *discoActual;
    n=0;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,particion.byteInicio,SEEK_SET);
        n=(((particion.tam*1024)-(sizeof(fatRootFolder)*256))/(1+particion.tamBloq));
        tamInfo=particion.tamBloq-sizeof(bloque);
        char contenido[tamInfo];
        for(j=0;j<n;j++){ //escribe FAT
            byte fat;
            fat.a='0'; //0 indica bloq libre 1 bloque ocupado
            fputc(fat.a,discoActual);
        }
        for(j=0;j<256;j++){ //escribe FAT root folder
            fatRootFolder frf;
            frf.estado='2'; //bloque no usado o no inicializado
            strcpy(frf.extension,"");
            strcpy(frf.fecha,"dd/mm/aaaa-hh:mm");
            strcpy(frf.nombre,"");
            frf.idInicio=-1;
            frf.idFinal=-1;
            frf.tamanio=0;
            fwrite(&frf,sizeof(fatRootFolder),1,discoActual);
        }
        for(j=0;j<n;j++){ //escribe bloque contenido
            bloque bloq;
            bloq.id=j;
            bloq.anterior=-1;
            bloq.siguiente=-1;
            bloq.estado='0';
            strcpy(contenido,"");
            fwrite(&bloq,sizeof(bloque),1,discoActual);
            fwrite(contenido,sizeof(contenido),1,discoActual);
        }
        fclose(discoActual);
    }else{
        printf("error al intentar aperturar en disco %s.vd",nomDisco);
    }
    return n;
}
//-----------------------------ENLAZADO-----------------------------------------
int crearENLAZADO(infoPart particion, char nomDisco[]){ //retorna numero de bloques creados
    char dir[145];
    int n,i;
    n=0;
    directorio dr;
    FILE *discoActual;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        n=((particion.tam*1024)-sizeof(directorio))/sizeof(bloqEnl);
        fseek(discoActual,particion.byteInicio,SEEK_SET);
        dr.inicio=0;
        dr.fin=n-1;
        strcpy(dr.nombre,"");
        fwrite(&dr,sizeof(directorio),1,discoActual);
        for(i=0;i<n;i++){
            bloqEnl nuevo;
            nuevo.id=i;
            nuevo.sig=-1;
            strcpy(nuevo.contenido,"");
            fwrite(&nuevo,sizeof(bloqEnl),1,discoActual);
        }
        fclose(discoActual);
    }else{
        printf("error al aperturar disco %s.vd\n",nomDisco);
    }
    return n;
}
//--------------------------------------EXT3------------------------------------
inodo nuevoInodo(){
    inodo nuevo;
    nuevo.apt_dir[0]=-1;
    nuevo.apt_dir[1]=-1;
    nuevo.apt_dir[2]=-1;
    nuevo.apt_dir[3]=-1;
    nuevo.apt_dir[4]=-1;
    nuevo.apt_ind[0]=-1;
    nuevo.apt_ind[1]=-1;
    strcpy(nuevo.fech_acc,"dd/mm/aaaa-hh:mm");
    strcpy(nuevo.fech_act,"dd/mm/aaaa-hh:mm");
    strcpy(nuevo.fech_crea,"dd/mm/aaaa-hh:mm");
    nuevo.llave=-1;
    nuevo.tam=-1;
    strcpy(nuevo.tipo,""); //carpeta o archivo
    return nuevo;
}

bloquEXT nuevobloqEXT3(){
    bloquEXT nuevo;
    nuevo.apt[0]=-1;
    nuevo.apt[1]=-1;
    nuevo.apt[2]=-1;
    nuevo.apt[3]=-1;
    nuevo.apt[4]=-1;
    nuevo.apt[5]=-1;
    strcpy(nuevo.contenido,"");
    nuevo.llave=-1;
    strcpy(nuevo.nombre,"");
    strcpy(nuevo.padre,"");
    return nuevo;
}

log nuevaBitacora(){
    log bitacora;
    strcpy(bitacora.contenido,"");
    strcpy(bitacora.fech_trans,"dd/mm/aaaa-hh:mm");
    strcpy(bitacora.nombre,"");
    bitacora.tip_oper=-1;
    bitacora.tipo=-1;
    return bitacora;
}

int crearEXT3(infoPart particion, char nomDisco[]){ //retorna numero de bloques ##creados para archivos
    char dir[145];
    int n,i;
    n=0;
    FILE *discoActual;
    superBloque sb;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        
        time_t tiempo = time(0);
	struct tm *tlocal = localtime(&tiempo);
	char fecha[16];
	strftime(fecha,16,"%d/%m/%y-%H:%M",tlocal);
        
        n=(particion.tam-(2*sizeof(superBloque))-(100*sizeof(log)))/(3*(sizeof(inodo)+sizeof(bloquEXT)));
        fseek(discoActual,particion.byteInicio,SEEK_SET);
        sb.bit_lib_inod=particion.byteInicio+sizeof(superBloque)+1;
        sb.bit_lib_bloq=particion.byteInicio+sizeof(superBloque)+n+1;
        sb.cant_mont=1;
        sb.dir_rz=particion.byteInicio+sizeof(superBloque)+2*n;
        strcpy(sb.fech_desmont,"dd/mm/aaaa-hh:mm");
        strcpy(sb.fech_mont,fecha);
        sb.ini_bloq=particion.byteInicio+sizeof(superBloque)+2*n+n*sizeof(inodo);
        sb.ini_bloq_bit=-1; //??????????
        sb.ini_log=particion.byteInicio+sizeof(superBloque)+2*n+n*sizeof(inodo)+n*sizeof(bloquEXT);
        sb.bloq_lib=n-1;
        sb.inod_lib=n-1;
        sb.num_bloq=n;
        sb.num_inod=n;
        sb.num_mag=201114717;
        sb.tam_bloq=64;
        sb.p_inod_lib=particion.byteInicio+sizeof(superBloque)+2*n+sizeof(inodo);
        sb.p_bloq_lib=particion.byteInicio+sizeof(superBloque)+2*n+n*sizeof(inodo)+sizeof(bloquEXT);
        //----------------------------------------------------------------------
        fwrite(&sb,sizeof(superBloque),1,discoActual); //escribe sb ya actualizado con raiz creada --> /
        //--------------Escribe bitmap de inodos y bloques----------------------
        byte rz;
        rz.a='1';
        fwrite(&rz,sizeof(byte),1,discoActual); //raiz
        for(i=0;i<(n-1);i++){
            byte bitmap;
            bitmap.a='0';
            //fputc(fat.a,discoActual);
            fwrite(&bitmap,sizeof(byte),1,discoActual);
        }
        fwrite(&rz,sizeof(byte),1,discoActual); //raiz
        for(i=0;i<(n-1);i++){ 
            byte bitmap;
            bitmap.a='0';
            //fputc(fat.a,discoActual);
            fwrite(&bitmap,sizeof(byte),1,discoActual);
        }
        //----------------------Escribe inodos----------------------------------
        inodo raiz;
        raiz=nuevoInodo();
        strcpy(raiz.tipo,"carpeta");
        strcpy(raiz.fech_crea,fecha);
        strcpy(raiz.fech_act,fecha);
        raiz.llave=0;
        raiz.apt_dir[0]=0; //apunta al bloq de datos con id 0
        fwrite(&raiz,sizeof(inodo),1,discoActual); //escribe inodo raiz
        for(i=0;i<n-1;i++){
            inodo nuevo;
            nuevo=nuevoInodo();
            nuevo.llave=i+1;
            fwrite(&nuevo,sizeof(inodo),1,discoActual);
        }
        //----------------------Escribe bloques---------------------------------
        bloquEXT brz;
        brz=nuevobloqEXT3();
        brz.llave=0;
        strcpy(brz.nombre,"root"); //nombre de la carpeta raiz
        fwrite(&brz,sizeof(bloquEXT),1,discoActual);
        for(i=0;i<n-1;i++){
            bloquEXT nuevo;
            nuevo=nuevobloqEXT3();
            nuevo.llave=i+1;
            fwrite(&nuevo,sizeof(bloquEXT),1,discoActual);
        }
        //----------------------Escribe Bitacoras-------------------------------
        log bitac;
        bitac=nuevaBitacora();
        strcpy(bitac.fech_trans,fecha);
        strcpy(bitac.nombre,"root");
        bitac.tip_oper=1;
        bitac.tipo=1;
        fwrite(&bitac,sizeof(log),1,discoActual);
        for(i=0;i<99;i++){
            log bitacora;
            bitacora=nuevaBitacora();
            fwrite(&bitacora,sizeof(log),1,discoActual);
        }
        fwrite(&sb,sizeof(superBloque),1,discoActual);//copia superbloque sb
        fclose(discoActual);
    }else{
        printf("error al intentar aperturar disco %s.vd",nomDisco);
    }
    return n;
}
//------------------------------------------------------------------------------
mbr recuperarMBR(char nom[]){ //devuelve el mbr del disco con nombre-->nom
    char dir[145];
    FILE *discoActual;
    mbr mbrDisco;
    mbrDisco.cantPart=-1; //para verificar si encontro la particion
    strcpy(dir,ubic_general);
    strcat(dir,nom);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fread(&mbrDisco,sizeof(mbr),1,discoActual);
        fclose(discoActual);
    }else{
        printf("error al intentar aperturar el disco %s.vd \n",nom);
    }
    return mbrDisco;
}

infoPart buscarInfoPart(mbr datosDisco,char nomPart[]){
    int cant,i;
    infoPart encontrado;
    strcpy(encontrado.nombre,"");
    cant=datosDisco.cantPart;
    i=0;
    while(i<cant && strcmp(datosDisco.iPart[i].nombre,nomPart)!=0){
        i++;
    }
    if(strcmp(datosDisco.iPart[i].nombre,nomPart)==0){
        encontrado=datosDisco.iPart[i];
    }else{
        printf("particion %s no encontrada !!",nomPart);
    }
    return encontrado;
}

void ActualizarMBR(char nomDisco[],mbr actual,infoPart act){
    char dir[145];
    int cant,i;
    FILE *discoActual;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    i=0;
    cant=actual.cantPart;
    while(i<cant && strcmp(actual.iPart[i].nombre,act.nombre)!=0){
        i++;
    }
    if(strcmp(actual.iPart[i].nombre,act.nombre)==0){
        actual.iPart[i].sisArchivos=act.sisArchivos;
        actual.iPart[i].cantBloqLib=act.cantBloqLib;
        actual.iPart[i].cantBloques=act.cantBloques;
    }
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fwrite(&actual,sizeof(mbr),1,discoActual);
        fclose(discoActual);
    }else{
        printf("error al intentar aperturar el disco %s.vd \n",nomDisco);
    }
}

void FormatearParticion(){
    char id[3],nomPart[32];
    int tipSisArch,n,bandera;
    disco temp;
    mbr mbrDisco;
    infoPart particion;
    bandera=1;
    n=0;
    printf("ID del disco donde se encuentra la Particion: ");
    scanf("%s",id);
    temp=existeDiscoIndex(id);
    printf("disco recuperado: %s\n",temp.id);
    if(strcmp(temp.id,id)==0){
        mbrDisco=recuperarMBR(temp.nombre);
        if(mbrDisco.cantPart>0){
            printf("Nombre de la particion a formatear: ");
            getchar();
            fflush(stdin);
            scanf("%[^\n]",nomPart);
            particion=buscarInfoPart(mbrDisco,nomPart);
            if(strcmp(particion.nombre,nomPart)==0){
                do{
                    printf("Sistema de Archivos \n 1. FAT \n 2. ENLAZADO \n 3. EXT3\n ");
                    scanf("%i",&tipSisArch);
                }while(tipSisArch<1 || tipSisArch>3);
                if(particion.tipoPart!=2 && particion.estado==1){
                    if(tipSisArch==1 && particion.tamBloq>127){ //FAT
                        n=crearFAT(particion,temp.nombre);
                        particion.sisArchivos=1;
                    }else if(tipSisArch==2 && particion.tamBloq==100){ //ENLAZADA
                        n=crearENLAZADO(particion,temp.nombre);
                        particion.sisArchivos=2;
                    }else if(tipSisArch==3 && particion.tipoAjuste==3 && particion.tamBloq==64){ //EXT3
                        n=crearEXT3(particion,temp.nombre);
                        particion.sisArchivos=3;
                    }else{
                        printf("no se dado formato ala particion verifique!!\n");
                        bandera=0;
                    }
                    if(bandera!=0){
                        particion.cantBloqLib=n;
                        particion.cantBloques=n;
                        ActualizarMBR(temp.nombre,mbrDisco,particion);
                    }
                }else{
                    printf("error esta haciendo referencia a una particion extendida o ha sido eliminada!!\n");
                }
            }
        }
    }
}

/*void FormatearParticion(){
    char id[3],nomPart[32],dir[145];
    int i,bandera,tipoFor;
    FILE *discoActual;
    disco temp;
    mbr mbrDisco;
    printf("ID del disco a formatear: ");
    scanf("%s",id);
    temp=existeDiscoIndex(id);
    if(strcmp(temp.id,id)==0){
        strcpy(dir,ubic_general);
        strcat(dir,temp.nombre);
        strcat(dir,".vd");
        if(temp.prtPrim>0 || temp.prtLog>0){
            discoActual=fopen(dir,"rb+");
            if(discoActual!=NULL){
                fread(&mbrDisco,sizeof(mbr),1,discoActual);
                printf("Nombre de la particion a formatear: ");
                getchar();
                fflush(stdin);
                scanf("%[^\n]",nomPart);
                bandera=-1;
                for(i=0;i<mbrDisco.cantPart;i++){
                    if((strcmp(mbrDisco.iPart[i-1].nombre,nomPart)==0) && (mbrDisco.iPart[i-1].estado==1) && (mbrDisco.iPart[i-1].tipoPart!=2)){
                        bandera=i-1;
                        i=mbrDisco.cantPart;
                    }
                }
                if(bandera!=-1){
                    do{
                    printf("Tipo de formateo 1->rapido, 2->completo: ");
                    scanf("%i",&tipoFor);
                    }while(tipoFor<1 || tipoFor>2);
                    fseek(discoActual,mbrDisco.iPart[bandera].byteInicio,SEEK_SET);
                    for(i=0;i<mbrDisco.iPart[bandera].cantBloques;i++){
                        byte fat;
                        fat.a='0'; //0 indica bloq libre 1 bloque ocupado
                        fputc(fat.a,discoActual);
                    }
                    for(i=0;i<256;i++){
                        fatRootFolder frf;
                        frf.estado='2'; //bloque no usado o no inicializado
                        strcpy(frf.extension,"");
                        strcpy(frf.fecha,"dd/mm/aaaa-hh:mm");
                        strcpy(frf.nombre,"");
                        frf.idInicio=-1;
                        frf.idFinal=-1;
                        frf.tamanio=0;
                        fwrite(&frf,sizeof(fatRootFolder),1,discoActual);
                    }
                    if(tipoFor==2){ //formateo completo
                        char contenido[mbrDisco.iPart[bandera].tamBloq-sizeof(bloque)];
                        for(i=0;i<mbrDisco.iPart[bandera].cantBloques;i++){ //escribe bloque contenido
                            bloque bloq;
                            bloq.id=i;
                            bloq.anterior=-1;
                            bloq.siguiente=-1;
                            bloq.estado='0';
                            strcpy(contenido,"");
                            fwrite(&bloq,sizeof(bloque),1,discoActual);
                            fwrite(contenido,sizeof(contenido),1,discoActual);
                        }
                    }
                    printf("formateo terminado!!\n");
                }else{
                    printf("posibilidades de error al buscar particion %s:\n ->no existe\n ->eliminada\n ->no apta\n",nomPart);
                }
                fclose(discoActual);
            }else{
                printf("error al intentar ingresar al disco %s.vd !!\n",temp.nombre);
            }
        }else{
            printf("no existen particiones para formatear !!\n");
        }
    }else{
        printf("error id %s no encontrado !!\n",id);
    }
}*/

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
            strcpy(aux1.id,"");
            printf("Disco con id--> %s no encontrado!!\n",id);
        }
        fclose(index);
    }
    return aux1;
}