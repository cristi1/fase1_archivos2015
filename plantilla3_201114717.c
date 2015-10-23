#include "menu_201114717.h"
#include "plantilla1_201114717.h"
#include "plantilla2_201114717.h"
#include "plantilla3_201114717.h"
#include "lista.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

char dir[150];

void modificarFat(infoPart info,int inicio,int cantidad){
    FILE *discoActual;
    //infoPart info;
    if(info.estado!=-1){
        discoActual=fopen(dir,"rb+");
        if(discoActual!=NULL){
            fseek(discoActual,info.byteInicio+inicio,SEEK_SET);
            while(cantidad>0){
                byte temp;
                temp.a='1';
                fputc(temp.a,discoActual);
                cantidad--;
            }
            fclose(discoActual);
        }   
    }
}

/*infoPart informacionParticion(char nomDisco[],char particion[]){
    char direc[150];
    int x;
    FILE *discoActual;
    mbr miMBR;
    infoPart temp;
    temp.estado=-1;
    strcpy(direc,ubic_general);
    strcat(direc,nomDisco);
    strcat(direc,".vd");
    strcpy(dir,"");
    strcat(dir,direc);
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fread(&miMBR,sizeof(mbr),1,discoActual);
        x=miMBR.cantPart;
        while(x>0){
            if(strcmp(miMBR.iPart[x-1].nombre,particion)==0 && miMBR.iPart[x-1].tipoPart!=2 && miMBR.iPart[x-1].estado!=0){
                temp=miMBR.iPart[x-1];
            }
            x--;
        }
        fclose(discoActual);
    }else{
        printf("error al intentar aperturar disco %s.vd !!\n",nomDisco);
    }
    return temp;
}*/

/*disco buscarIdDisco(){
    char id[3];
    int vacio;
    FILE *index;
    disco aux,temp;
    strcpy(temp.id,"");
    strcat(temp.id,"xxx");
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
            if(strcmp(aux.id,id)!=0 || aux.estado!=1){
                aux=temp;
                printf("id %s no encontrado !!\n",id);
            }
        }else{
            printf("no se ha creado ningun disco !!\n");
        }
        fclose(index);
    }else{
        printf("Error al intentar buscar id %s !!\n",id);
    }
    return aux;
}*/

int escribirBloques(infoPart info,int i,int numBloqs,char contenido[]){
    int byteBloqInicial,k,l,c,tam,resp;
    FILE *discoActual;
    char cont[info.tamBloq-sizeof(bloque)];
    discoActual=fopen(dir,"rb+");
    resp=-1;
    if(discoActual!=NULL){
        byteBloqInicial=info.byteInicio+(info.cantBloques)+(256*sizeof(fatRootFolder))+(i*info.tamBloq);
        tam=strlen(contenido);
        c=0;
        for(k=0;k<numBloqs;k++){
            bloque auxil;
            fseek(discoActual,byteBloqInicial,SEEK_SET);
            fread(&auxil,sizeof(bloque),1,discoActual);
            fseek(discoActual,byteBloqInicial,SEEK_SET);
            
            bloque temporal;
            temporal.id=auxil.id;
            temporal.estado='1';
            
            if(k==0){
                temporal.anterior=-1;
            }else{
                temporal.anterior=i+k-1;
            }
            if(k+1==numBloqs){
                temporal.siguiente=-1;
            }else{
                temporal.siguiente=i+k+1;
            }
            strcpy(cont,"");
            l=0;
            while(l<(info.tamBloq-sizeof(bloque))&& tam>0){
                cont[l]=contenido[c];
                l++;
                c++;
                tam--;
            }
            fwrite(&temporal,sizeof(bloque),1,discoActual);
            fwrite(cont,info.tamBloq-sizeof(bloque),1,discoActual);
            byteBloqInicial=ftell(discoActual);
        }
        fclose(discoActual);
        if(k==numBloqs){
            resp=1;
        }
    }else{
        printf("error al intentar craer archivo en bloques!!\n");
    }
    return resp;
}

void archivoFAT(){}
void modificarFAT(){}
void eliminarFAT(){}

//------------------------ARCHIVO ENLAZADO--------------------------------------

directorio recuperarDirEnlazado(char nomDisco[],infoPart particion){
    char dir[145];
    FILE *discoActual;
    directorio actual;
    strcpy(actual.nombre,"");
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,particion.byteInicio,SEEK_SET);
        fread(&actual,sizeof(directorio),1,discoActual);
        fclose(discoActual);
    }else{
        printf("error al aperturar el disco %s.vd !!\n",nomDisco);
    }
    return actual;
}

void actualizarDirEnlazado(char nomDisco[],directorio actual,int posicion){
    char dir[145];
    FILE *discoActual;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,posicion,SEEK_SET);
        fwrite(&actual,sizeof(directorio),1,discoActual);
        fclose(discoActual);
    }else{
        printf("error al aperturar el disco %s.vd !!\n",nomDisco);
    }
}

void escribirBloqEnlazado(char nomDisco[],bloqEnl nuevo,int posicion){
    char dir[145];
    FILE *discoActual;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,posicion,SEEK_SET);
        fwrite(&nuevo,sizeof(bloqEnl),1,discoActual);
        fclose(discoActual);
    }else{
        printf("error al aperturar el disco %s.vd !!\n",nomDisco);
    }
}

bloqEnl leerBloqEnlazado(char nomDisco[],int posicion){
    char dir[145];
    FILE *discoActual;
    bloqEnl nuevo;
    strcpy(nuevo.contenido,"");
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,posicion,SEEK_SET);
        fread(&nuevo,sizeof(bloqEnl),1,discoActual);
        fclose(discoActual);
    }else{
        printf("error al aperturar el disco %s.vd !!\n",nomDisco);
    }
    return nuevo;
}

void archivoENLAZADO(infoPart particion,char nomDisco[]){
    char path[100],nom[16],cadena[100];
    int i,tm,n,r,k,j,m,pos;
    FILE *arch;
    directorio actual;
    printf("Direccion del archivo: ");
    getchar();
    fflush(stdin);
    scanf("%[^\n]",path);
    do{
        printf("Nombre del archivo(max 16 caracteres): ");
        getchar();
        fflush(stdin);
        scanf("%[^\n]",nom);
        tm=strlen(nom); 
    }while(tm>16);
    arch=fopen(path,"r");
    if(arch!=NULL){
        fseek(arch,0,SEEK_END);
        i = ftell(arch); //numero de caracteres en el archivo
        char contenido[i];
        m=0;
        fseek(arch,0,SEEK_SET);
        while(m<i && !feof(arch)){
            contenido[m]=fgetc(arch);
            m++;
        }
        fclose(arch);
        if(i>99){
            n=i/100; //n numero de bloques a usar con 100 caracteres
            r=i%100;
        }else{
            n=0;
            r=i;
        }
        if(n<particion.cantBloques){
            pos=particion.byteInicio+sizeof(directorio);
            j=0;
            m=0;
            for(k=0;k<n;k++){
                int f=0;
                while(f<100) { //limpia cadena
                cadena[f] = '\0';
                f++;
                }
                while(j<100 && m<i){
                    cadena[j]=contenido[m];
                    j++;
                    m++;
                }
                j=0;
                bloqEnl nuevo;
                strcpy(nuevo.contenido,cadena);
                nuevo.sig=k+1;
                if(k==n-1 && r==0){
                    nuevo.sig=-1;
                }
                escribirBloqEnlazado(nomDisco,nuevo,pos);
                pos=pos+sizeof(bloqEnl);
            }
            if(r>0){ 
                int f=0;
                while(f<100) { //limpia cadena
                cadena[f] = '\0';
                f++;
                }
                while(j<r && m<i){
                    cadena[j]=contenido[m];
                    j++;
                    m++;
                }
                bloqEnl nuevo;
                strcpy(nuevo.contenido,cadena);
                nuevo.sig=-1;
                escribirBloqEnlazado(nomDisco,nuevo,pos);
                n=n+1;
            }
            actual.inicio=0;
            actual.fin=n;
            strcpy(actual.nombre,nom);
            actualizarDirEnlazado(nomDisco,actual,particion.byteInicio);
        }else{
            printf("error el archivo es muy grande, la particion no cuenta con suficiente espacio!!\n"); // ->> %i,%i,%i!!\n",i,n,particion.cantBloques
        }
    }else{
        printf("error al intentar aperturar el archivo %s\n",path);
    }
}

void leerEnlazado(infoPart particion,char nomDisco[]){
    char cadena[101];
    FILE *temporal;
    int i,pos,f;
    directorio dirAct; 
    temporal=fopen("//home//clnx//Escritorio//temporal.csv","w"); //verificar .csv
    if(temporal!=NULL){
        dirAct=recuperarDirEnlazado(nomDisco,particion);
        pos=particion.byteInicio+sizeof(directorio);
        for(i=0;i<dirAct.fin;i++){
            bloqEnl temp;
            temp=leerBloqEnlazado(nomDisco,pos);
            strcpy(cadena,temp.contenido);
            cadena[100]='\0';
            fwrite(cadena,strlen(cadena),1,temporal);
            pos=pos+sizeof(bloqEnl);
        }
        fclose(temporal);
        system("gedit //home//clnx//Escritorio//temporal.csv");
    }else{
        printf("error al intentar leer el archivo, vuelva a intentarlo!!\n");
    } 
}

eliminarENLAZADO(infoPart particion,char nom[]){
    directorio actual;
    int i,pos;
    actual=recuperarDirEnlazado(nom,particion);
    pos=particion.byteInicio+sizeof(directorio);
    for(i=0;i<actual.fin;i++){
        bloqEnl temp;
        strcpy(temp.contenido,"");
        temp.id=i;
        temp.sig=-1;
        escribirBloqEnlazado(nom,temp,pos);
        pos=pos+sizeof(bloqEnl);
    }
    pos=particion.byteInicio;
    actual.fin=0;
    actual.inicio=0;
    strcpy(actual.nombre,"");
    actualizarDirEnlazado(nom,actual,pos);
}

//----------------------------ARCHIVO EXT3--------------------------------------
superBloque recuperarSB(infoPart particion,char nomDisco[]){
    char dir[145];
    FILE *discoActual;
    superBloque actual;
    actual.num_mag=-1;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,particion.byteInicio,SEEK_SET);
        fread(&actual,sizeof(superBloque),1,discoActual);
        fclose(discoActual);
    }else{
        printf("error al aperturar el disco %s.vd !!\n",nomDisco);
    }
    return actual;
}

void actualizarSB(infoPart particion,char nomDisco[],superBloque actual){
    char dir[145];
    int n;
    FILE *discoActual;
    n=actual.num_bloq;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,particion.byteInicio,SEEK_SET);
        fwrite(&actual,sizeof(superBloque),1,discoActual);
        fseek(discoActual,particion.byteInicio+sizeof(superBloque)+2*n+n*sizeof(inodo)+n*sizeof(bloquEXT)+100*sizeof(log),SEEK_SET);
        fwrite(&actual,sizeof(superBloque),1,discoActual);
        fclose(discoActual);
    }else{
        printf("error al aperturar el disco %s.vd !!\n",nomDisco);
    }
}

inodo getPrimInodLib(superBloque sb,char nomDisco[]){ //devuelve el primer inodo libre
    char dir[145];
    FILE *discoActual;
    inodo ino;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,sb.p_inod_lib,SEEK_SET);
        fread(&ino,sizeof(inodo),1,discoActual);
        fclose(discoActual);
    }
    return ino;
} 

bloquEXT getPrimBloqLib(superBloque sb,char nomDisco[]){ //devuelve el primer bloquEXT libre
    char dir[145];
    FILE *discoActual;
    bloquEXT bloqAct;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,sb.p_bloq_lib,SEEK_SET);
        fread(&bloqAct,sizeof(bloquEXT),1,discoActual);
        fclose(discoActual);
    }
    return bloqAct;
} 

inodo getInodoEXT(superBloque sb,char nomDisco[],int id){ //devuelve un inodo con id especifico
    char dir[145];
    FILE *discoActual;
    inodo ino;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,sb.dir_rz+(id*sizeof(inodo)),SEEK_SET);
        fread(&ino,sizeof(inodo),1,discoActual);
        fclose(discoActual);
    }
    return ino;
}

bloquEXT getBloqEXT(superBloque sb,char nomDisco[],int id){ //devuelve un bloque con id especifico
    char dir[145];
    FILE *discoActual;
    bloquEXT bloqAct;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,sb.ini_bloq+(id*sizeof(bloquEXT)),SEEK_SET);
        fread(&bloqAct,sizeof(bloquEXT),1,discoActual);
        fclose(discoActual);
    }
    return bloqAct;
}

void actualizarInodo(superBloque sb,char nomDisco[],inodo ino){
    char dir[145];
    FILE *discoActual;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,sb.dir_rz+(ino.llave*sizeof(inodo)),SEEK_SET);
        fwrite(&ino,sizeof(inodo),1,discoActual);
        fclose(discoActual);
    }
}

void actualizarBloqEXT(superBloque sb,char nomDisco[],bloquEXT bloqAct){
    char dir[145];
    FILE *discoActual;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,sb.ini_bloq+(bloqAct.llave*sizeof(bloquEXT)),SEEK_SET);
        fwrite(&bloqAct,sizeof(bloquEXT),1,discoActual);
        fclose(discoActual);
    }
}

byte getBMInodo(infoPart particion,char nomDisco[],int id){ //obtiene el n bitmap de inodo
    char dir[145];
    byte bitmap;
    FILE *discoActual;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,particion.byteInicio+sizeof(superBloque)+id,SEEK_SET);
        fread(&bitmap,sizeof(byte),1,discoActual);//fputc(fat.a,discoActual);
        fclose(discoActual);
    }
    return bitmap;
}

byte getBMBloqEXT(infoPart particion,char nomDisco[],superBloque sb,int id){ //obtiene el n bitmap de bloquEXT
    char dir[145];
    byte bitmap;
    FILE *discoActual;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,particion.byteInicio+sizeof(superBloque)+sb.num_inod+id,SEEK_SET);
        fread(&bitmap,sizeof(byte),1,discoActual);//fputc(fat.a,discoActual);
        fclose(discoActual);
    }
    return bitmap;
}

log getBitacora(infoPart particion,char nomDisco[],superBloque sb,int id){
    char dir[145];
    int n;
    log bitac;
    FILE *discoActual;
    n=sb.num_inod;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,sb.ini_log+id*(sizeof(log)),SEEK_SET);
        fread(&bitac,sizeof(log),1,discoActual);
        fclose(discoActual);
    }
    return bitac;
}

void actualizarBitMapInodo(int inicio,char nomDisco[],int id,char val){
    char dir[145];
    FILE *discoActual;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,inicio+sizeof(superBloque)+id,SEEK_SET);
        byte bitmap;
        bitmap.a=val;
        fwrite(&bitmap,sizeof(byte),1,discoActual);//fputc(fat.a,discoActual);
        fclose(discoActual);
    }
}

void actualizarBitMapBloqEXT(int inicio,char nomDisco[],int id,char val,int n){
    char dir[145];
    FILE *discoActual;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,inicio+sizeof(superBloque)+n+id,SEEK_SET);
        byte bitmap;
        bitmap.a=val;
        fwrite(&bitmap,sizeof(byte),1,discoActual);//fputc(fat.a,discoActual);
        fclose(discoActual);
    }
}

void actualizarBitacora(superBloque sb,char nomDisco[],char cont[],char nom[],int tipOp,int tipo){
    char dir[145];
    int i;
    FILE *discoActual;
    log nuevo;
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char fecha[16];
    strftime(fecha,16,"%d/%m/%y-%H:%M",tlocal);
    strcpy(nuevo.contenido,cont);
    strcpy(nuevo.fech_trans,fecha);
    strcpy(nuevo.nombre,nom);
    nuevo.tip_oper=tipOp;
    nuevo.tipo=tipo;
    strcpy(dir,ubic_general);
    strcat(dir,nomDisco);
    strcat(dir,".vd");
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,sb.ini_bloq_bit,SEEK_SET);
        fwrite(&nuevo,sizeof(log),1,discoActual);
        fclose(discoActual);
    }
}

int aptVacio(bloquEXT encontrado){
    int i;
    i=0;
    while(i<6 && encontrado.apt[i]!=-1){
        i++;
    }
    if(encontrado.apt[i]!=-1){ //carpeta con 6 hijos (llena)
        i=-1;
    }
    return i;
}

void divPath(Lista *l,char path[]){
    char *ptr;
    ptr = strtok(path,"/");    // Primera llamada => Primer token
    addLista(l,ptr);
    while( (ptr=strtok(NULL,"/"))!=NULL){    // Posteriores llamadas
        addLista(l,ptr);
    }
}

bloquEXT getDirEXT(superBloque sb,char nomDisco[],nodol *padre,int id,char nuevo[]){ //busca un directorio especifico
    int i,exist;
    inodo inod;
    bloquEXT bloq,bloq_aux;
    if(strcmp(padre->nombre,"root")==0){
        exist=1;
        inod=getInodoEXT(sb,nomDisco,id);
        bloq=getBloqEXT(sb,nomDisco,inod.apt_dir[0]);
        while(padre->sig!=NULL && exist==1 && strcmp(inod.tipo,"carpeta")==0){
            exist=0;
            i=0;
            while(i<6 && bloq.apt[i]!=-1){
                inod=getInodoEXT(sb,nomDisco,bloq.apt[i]);
                bloq_aux=getBloqEXT(sb,nomDisco,inod.apt_dir[0]);
                if(strcmp(padre->sig->nombre,bloq_aux.nombre)==0){ //subcarpeta encontrada
                    exist==1;
                    padre=padre->sig;
                    bloq=bloq_aux;
                    break;
                }
                i++;
            }
        }
        if(exist==0){
            bloq.llave=-1; //path no encontrada
        }else{
            i=0;
            while(i<6 && bloq.apt[i]!=-1){
                inod=getInodoEXT(sb,nomDisco,bloq.apt[i]);
                bloq_aux=getBloqEXT(sb,nomDisco,inod.apt_dir[0]);
                if(strcmp(bloq_aux.nombre,nuevo)==0){ //carpeta ya existe
                    exist=1;
                    bloq.llave=-1;
                    break;
                }
                i++;
            }
        }
    }else{
        bloq.llave=-1; //ruta no encontrada desde la raiz
    }
    return bloq;
}

/*digraph structs {
    node [shape=record];rankdir=LR;
    struct1 [label="<f0> 1|<f1> root|<f2> home|<f3> contenido|<4> 0|<5>1"];
}*/

void directorioEXT3(infoPart particion,char nomDisco[]){
    char nomDir[16],path[150];
    int tm,i;
    Lista *l;
    nodol *temp;
    superBloque sb;
    inodo nuevoIno;
    bloquEXT encontrado,nuevoBloq;
    sb=recuperarSB(particion,nomDisco);
    if(sb.num_mag==201114717){
        if(sb.inod_lib>0 && sb.bloq_lib>0){
            do{
                printf("Nombre del directorio a crear: ");
                getchar();
                fflush(stdin);
                scanf("%[^\n]",nomDir);
                tm=strlen(nomDir); 
            }while(tm>16);
            strcpy(path,"");
            printf("Path para el nuevo directorio: ");
            getchar();
            fflush(stdin);
            scanf("%[^\n]",path);
            l=malloc(sizeof(Lista));
            divPath(l,path);
            temp=l->primero;
            encontrado=getDirEXT(sb,nomDisco,temp,0,nomDir); //pos actual 
            if(encontrado.llave!=-1){
                i=aptVacio(encontrado);
                if(i!=-1){
                    time_t tiempo = time(0);
                    struct tm *tlocal = localtime(&tiempo);
                    char fecha[16];
                    strftime(fecha,16,"%d/%m/%y-%H:%M",tlocal);
                    nuevoIno=getPrimInodLib(sb,nomDisco);
                    nuevoBloq=getPrimBloqLib(sb,nomDisco);
                    strcpy(nuevoIno.tipo,"carpeta");
                    strcpy(nuevoIno.fech_crea,fecha);
                    strcpy(nuevoIno.fech_act,fecha);
                    nuevoIno.apt_dir[0]=nuevoBloq.llave;
                    strcpy(nuevoBloq.nombre,nomDir);
                    strcpy(nuevoBloq.padre,encontrado.nombre);
                    encontrado.apt[i]=nuevoIno.llave;
                    actualizarInodo(sb,nomDisco,nuevoIno);
                    actualizarBloqEXT(sb,nomDisco,nuevoBloq);
                    actualizarBloqEXT(sb,nomDisco,encontrado);
                    actualizarBitMapBloqEXT(particion.byteInicio,nomDisco,nuevoBloq.llave,'1',sb.num_bloq);
                    actualizarBitMapInodo(particion.byteInicio,nomDisco,nuevoIno.llave,'1');
                    actualizarBitacora(sb,nomDisco,"",nomDir,1,1);
                    sb.bit_lib_bloq++;
                    sb.bit_lib_inod++;
                    sb.bloq_lib--;
                    sb.inod_lib--;
                    sb.p_bloq_lib=sb.p_bloq_lib+sizeof(bloquEXT);
                    sb.p_inod_lib=sb.p_inod_lib+sizeof(inodo);
                    sb.ini_bloq_bit=sb.ini_bloq_bit+sizeof(log);
                    actualizarSB(particion,nomDisco,sb);
                }else{
                    printf("carpeta con maximo de hijos !!!\n");
                } 
            }else{
                printf("error no se puede crear el directorio el path no existe o el nombre de directorio ya existe!!!\n");
            }
        }
    }
}

int existeDirectorio(bloquEXT actual){}

void crearDirEXT3(){
    char id[3],nomPart[32],dirNuevo[16];
    disco temp;
    mbr mbrDisco;
    infoPart particion;
    printf("ID del disco donde se creara el Directorio: ");
    scanf("%s",id);
    temp=existeDiscoIndex(id);
    if(strcmp(temp.id,id)==0){
        mbrDisco=recuperarMBR(temp.nombre);
        if(mbrDisco.cantPart>0){
            printf("Nombre de la particion: ");
            getchar();
            fflush(stdin);
            scanf("%[^\n]",nomPart);
            particion=buscarInfoPart(mbrDisco,nomPart);
            if(strcmp(particion.nombre,nomPart)==0 && particion.estado==1 && particion.sisArchivos==3){
                directorioEXT3(particion,temp.nombre);
            }else{
                printf("error no se puede crear el directorio!!\n");
            }
        }
    }
}

void existeDirEXT3(){}

void archivoEXT3(){}
void modificarEXT3(){}
void eliminarEXT3(){}
//--------------------------------GENERAL---------------------------------------
void crearArchivo(){
    char id[3],nomPart[32],nomNuevo[16];
    disco temp;
    mbr mbrDisco;
    infoPart particion;
    printf("ID del disco donde se creara el Archivo: ");
    scanf("%s",id);
    temp=existeDiscoIndex(id);
    if(strcmp(temp.id,id)==0){
        mbrDisco=recuperarMBR(temp.nombre);
        if(mbrDisco.cantPart>0){
            printf("Nombre de la particion: ");
            getchar();
            fflush(stdin);
            scanf("%[^\n]",nomPart);
            particion=buscarInfoPart(mbrDisco,nomPart);
            if(strcmp(particion.nombre,nomPart)==0 && particion.estado==1){
                switch(particion.sisArchivos){
                    case 1:
                        archivoFAT();
                        break;
                    case 2:
                        archivoENLAZADO(particion,temp.nombre);
                        break;
                    case 3:
                        archivoEXT3();
                        break;
                    default:
                        printf("particion no formateada o existe algun error en el sistema de archivos %i!!\n",particion.sisArchivos);
                }
            }else{
                printf("error la particion %s ha sido eliminada!!\n",particion.nombre);
            }
        }
    }
}

void modificarArchivo(){
    char id[3],nomPart[32],nomNuevo[16];
    disco temp;
    FILE *arch;
    mbr mbrDisco;
    infoPart particion;
    printf("ID del disco donde se encuentra el Archivo: ");
    scanf("%s",id);
    temp=existeDiscoIndex(id);
    if(strcmp(temp.id,id)==0){
        mbrDisco=recuperarMBR(temp.nombre);
        if(mbrDisco.cantPart>0){
            printf("Nombre de la particion: ");
            getchar();
            fflush(stdin);
            scanf("%[^\n]",nomPart);
            particion=buscarInfoPart(mbrDisco,nomPart);
            if(strcmp(particion.nombre,nomPart)==0 && particion.estado==1){
                switch(particion.sisArchivos){
                    case 1:
                        modificarFAT();
                        break;
                    case 2:
                        leerEnlazado(particion,temp.nombre);
                        break;
                    case 3:
                        modificarEXT3(); //edita y lee 
                        break;
                    default:
                        printf("particion no formateada o existe algun error en el sistema de archivos %i!!\n",particion.sisArchivos);
                }
            }else{
                printf("error la particion %s ha sido eliminada!!\n",particion.nombre);
            }
        }
    }
}

void eliminarArchivo(){
    char id[3],nomPart[32],nomNuevo[16];
    disco temp;
    mbr mbrDisco;
    infoPart particion;
    printf("ID del disco donde se eliminara el Archivo: ");
    scanf("%s",id);
    temp=existeDiscoIndex(id);
    if(strcmp(temp.id,id)==0){
        mbrDisco=recuperarMBR(temp.nombre);
        if(mbrDisco.cantPart>0){
            printf("Nombre de la particion: ");
            getchar();
            fflush(stdin);
            scanf("%[^\n]",nomPart);
            particion=buscarInfoPart(mbrDisco,nomPart);
            if(strcmp(particion.nombre,nomPart)==0 && particion.estado==1){
                switch(particion.sisArchivos){
                    case 1:
                        eliminarFAT();
                        break;
                    case 2:
                        eliminarENLAZADO(particion,temp.nombre);
                        break;
                    case 3:
                        eliminarEXT3();
                        break;
                    default:
                        printf("particion no formateada o existe algun error en el sistema de archivos %i!!\n",particion.sisArchivos);
                }
            }else{
                printf("error la particion %s ha sido eliminada!!\n",particion.nombre);
            }
        }
    }
}

/*void crearArchivo(){
    char nomNuevo[16],part[32],texto[1000];
    int tm,tmCont,res,numBloq,fatInicial,r,resp;
    disco auxiliar;
    infoPart info;
    numBloq=0;
    auxiliar=buscarIdDisco();
    if(strcmp(auxiliar.id,"xxx")!=0){
        printf("Nombre de la particion: ");
        getchar();
        fflush(stdin);
        scanf("%[^\n]",part);
        //strcpy(nomDisco,auxiliar.nombre);
        //strcpy(particion,part);
        info=informacionParticion(auxiliar.nombre,part);
        //info=informacionParticion();
        if(info.estado!=-1){
            do{
                printf("Nombre del archivo(max 16 caracteres): ");
                getchar();
                fflush(stdin);
                scanf("%[^\n]",nomNuevo);
                tm=strlen(nomNuevo); 
            }while(tm>16);
            do{
                strcpy(texto,"");
                printf("Ingrese el contenido del archivo: ");
                getchar();
                fflush(stdin);
                scanf("%[^\n]",texto);
                tmCont=strlen(texto); 
            }while(tmCont<1);
            numBloq=tmCont/((int)info.tamBloq-sizeof(bloque));
            res=tmCont%((int)info.tamBloq-sizeof(bloque));
            if(res>0){
                numBloq++; //(res*(int)info.tamBloq-sizeof(bloque))
            }
            printf("numero de bloques necesarios %i \n",numBloq);
            fatInicial=ajuste(info,numBloq);
            printf("byte donde inicia bloques %i !!\n",fatInicial);
            if(fatInicial!=-1){
                r=nuevoArchivo(info,nomNuevo,fatInicial,(fatInicial+numBloq-1),tmCont);
                if(r!=-1){
                    modificarFat(info,fatInicial,numBloq);
                    resp=escribirBloques(info,fatInicial,numBloq,texto);
                    if(resp==1){
                        printf("se ha creado el archivo %s !!\n",nomNuevo);
                    }
                }
            }else{
                printf("error no se ha podido crear el archivo %s !!\n",nomNuevo);
            }
        }else{
            printf("error en particion \"%s\" posibilidades\n ->no creada\n ->hace referencia a una extendida\n ->ha sido eliminada\n",part);
        }
    }  
}*/

int nuevoArchivo(infoPart info,char nombre[],int idInicio,int idFinal,int tam){   
    char *cads,nom[10],ext[5];
    int r,byteActual,limite,j;
    FILE *discoActual;
    fatRootFolder frfActual;
    r=-1; //root folder no creado
    limite=0;
    discoActual=fopen(dir,"rb+");
    if(discoActual!=NULL){
        fseek(discoActual,(info.byteInicio+info.cantBloques),SEEK_SET);
        byteActual=ftell(discoActual);
        fread(&frfActual,sizeof(fatRootFolder),1,discoActual);
        while(frfActual.estado!='2' && limite<256){
            byteActual=ftell(discoActual);
            fread(&frfActual,sizeof(fatRootFolder),1,discoActual);
            limite++;
        }
        if(frfActual.estado=='2'){
            time_t tiempo = time(0);
            struct tm *tlocal = localtime(&tiempo);
            char fecha[20];
            strftime(fecha,128,"%d/%m/%y-%H:%M",tlocal);
            cads=strtok(nombre,".");
            for(j=0;cads;j++){
                if(j==0){
                    strcpy(nom,cads);
                }else{
                    strcpy(ext,cads);
                }
                cads=strtok(0,".");
            }
            frfActual.estado='1';
            strcpy(frfActual.fecha,fecha);
            strcpy(frfActual.nombre,nom);
            strcpy(frfActual.extension,ext);
            frfActual.tamanio=tam;
            frfActual.idInicio=idInicio;
            frfActual.idFinal=idFinal;
            fseek(discoActual,byteActual,SEEK_SET);
            fwrite(&frfActual,sizeof(fatRootFolder),1,discoActual);
            r=1; //se creó el fatRootFolder correspondiente al archivo
        }
        fclose(discoActual);
    }else{
        printf("error al intentar aperturar el disco !!\n");
    }
}
 
int ajuste(infoPart particion,int cantidad){ //informacion de la particion,cantidad de bloques necesarios
    int inicio,i,limite,ant,act,bandera,ini,b,recorrido,cero,pos;
    FILE *discoActual;
    inicio=-1;
    i=0;
    b=0;
    limite=0;
    if(particion.cantBloqLib>=cantidad){ //se verifica q existan suficiente cantidad de bloques para el nuevo archivo
        discoActual=fopen(dir,"rb+");
        if(discoActual!=NULL){ 
            fseek(discoActual,particion.byteInicio,SEEK_SET);
            switch(particion.tipoAjuste){
                case 1: //primero
                    recorrido=0;
                    cero=0;
                    pos=0;
                    
                    act=0;
                    ant=0;
                    bandera=0;
                    inicio=0;
                    while(recorrido<particion.cantBloques){
                        byte temp;
                        fread(&temp,sizeof(byte),1,discoActual);
                        char a=temp.a;
                        if(a=='0'){
                            cero++;
                            if(cero==cantidad){
                                inicio=recorrido-cantidad+1;
                                recorrido=particion.cantBloques;
                            }
                        }else{
                            cero=0;
                        }
                        recorrido++;
                    }
                    break;
                case 2: //mejor
                    while(limite<particion.cantBloques){ //&& bandera!=1
                        byte temp;
                        fread(&temp,sizeof(byte),1,discoActual);
                        if(temp.a=='0'){
                            act++;
                        }else{
                            if(act>cantidad){
                                if(ant==0){
                                    ant=act;
                                }else{
                                    if(act<ant){
                                        ant=act;
                                    }
                                }
                            }else if(act==cantidad){
                                limite=particion.cantBloques-1;
                            }
                            act=0;
                        }
                        inicio=limite+1-act;
                        limite++;
                    }
                    break;
                case 3: //peor
                    /*while(limite<particion.cantBloques){
                        byte temp;
                        fread(&temp,sizeof(byte),1,discoActual);
                        if(temp.a=='0'){
                            act++;
                        }else{
                            if(act>cantidad && ant<act){
                                ant=act;
                            }
                        }
                        inicio=
                        limite++;
                    }*/
                    break;
                default:
                    printf("error fatal !!\n");
            }
            fclose(discoActual);
        }else{
            printf("error al intentar aperturar el disco !!\n");
        }
    }else{
        printf("error no existen suficiente espacio para el archivo !!\n");
    }
    return inicio; //retorna id inicio
}

/*void eliminarModificarArchivo(int tipo){ //eliminar tipo=0 ; 
    char nomArch[16],part[32],nomNuevo[16],*cads,nom[10],ext[5];
    disco auxiliar;
    infoPart info;
    FILE *discoActual;
    fatRootFolder frfActual;
    int byteActual,limite,inicio,final,j,tm;
    auxiliar=buscarIdDisco();
    if(strcmp(auxiliar.id,"xxx")!=0){
        printf("Nombre de la particion: ");
        getchar();
        fflush(stdin);
        scanf("%[^\n]",part);
        info=informacionParticion(auxiliar.nombre,part);
        if(info.estado!=-1){
            strcpy(nomArch,"");
            printf("Nombre del archivo(max 16 caracteres): ");
            getchar();
            fflush(stdin);
            scanf("%[^\n]",nomArch);
            discoActual=fopen(dir,"rb+");
            if(discoActual!=NULL){
                fseek(discoActual,(info.byteInicio+info.cantBloques),SEEK_SET);
                byteActual=ftell(discoActual);
                fread(&frfActual,sizeof(fatRootFolder),1,discoActual);
                while(strcmp(frfActual.nombre,nomArch)!=0 && limite<256){
                    byteActual=ftell(discoActual);
                    fread(&frfActual,sizeof(fatRootFolder),1,discoActual);
                    limite++;
                }
                inicio=frfActual.idInicio;
                final=frfActual.idFinal;
                fseek(discoActual,info.byteInicio+frfActual.idInicio,SEEK_SET);
                if(strcmp(frfActual.nombre,nomArch)==0){
                    if(tipo==0){
                        while(final>=inicio){
                            byte temp;
                            temp.a='0';
                            fwrite(&temp,sizeof(byte),1,discoActual);
                            inicio++;
                        }
                        frfActual.estado='0';
                        strcpy(frfActual.extension,"");
                        strcpy(frfActual.fecha,"dd/mm/aaaa-hh:mm");
                        frfActual.idFinal=-1;
                        frfActual.idInicio=-1;
                        strcpy(frfActual.nombre,"");
                        frfActual.tamanio=0; 
                        fseek(discoActual,byteActual,SEEK_SET);
                        fwrite(&frfActual,sizeof(fatRootFolder),1,discoActual);
                    }else{
                        do{
                            printf("Nuevo nombre para el archivo: ");
                            getchar();
                            fflush(stdin);
                            scanf("%[^\n]",nomNuevo);
                            tm=strlen(nomNuevo); 
                        }while(tm>16);
                        cads=strtok(nomNuevo,".");
                        for(j=0;cads;j++){
                            if(j==0){
                                strcpy(nom,cads);
                            }else{
                                strcpy(ext,cads);
                            }
                            cads=strtok(0,".");
                        }
                        strcpy(frfActual.nombre,nom);
                        strcpy(frfActual.extension,ext);
                        fseek(discoActual,byteActual,SEEK_SET);
                        fwrite(&frfActual,sizeof(fatRootFolder),1,discoActual);
                    }
                }else{
                    printf("archivo con nombre no ha sido encontrado %s !!\n",nomArch);
                }
                fclose(discoActual);
            }
        }
    }   
}*/