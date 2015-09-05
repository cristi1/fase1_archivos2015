#include "menu_201114717.h"
#include "plantilla1_201114717.h"
#include "plantilla4_201114717.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void menu0(){
    int n=0;
    system("clear");
    fflush(stdin);
    printf("\n******* PROYECTO FASE 1 MENEJO E IMPLEMENTACION DE ARCHIVOS ******* \n");
    printf("                           MENU PRINCIPAL\n");
    printf("\n0. Salir\n");
    printf("1. Operaciones sobre Disco\n");
    printf("2. Administracion de Particiones\n");
    printf("3. Administracion de Archivos\n");
    printf("4. Reportes\n");
    scanf(" %d",&n);
    verificar0(n);
}

void verificar0(int n){
    switch(n){
        case 0:
            exit(0);
        case 1:
            menu1();
            break;
        case 2:
            menu2();
            break;
        case 3:
            menu3();
            break;
        case 4:
            menu4();
            break;
        default:
            menu0();
    }
}

void menu1(){
    int n=0;
    system("clear");
    fflush(stdin);
    printf("\n******* PROYECTO FASE 1 MENEJO E IMPLEMENTACION DE ARCHIVOS ******* \n");
    printf("                       OPERACIONES SOBRE DISCO\n");
    printf("\n0. Menu Principal\n");
    printf("1. Crear Disco\n");
    printf("2. Aumentar Tamaño del Disco\n");
    printf("3. Reducir Tamaño del Disco\n");
    printf("4. Eliminar Disco\n");
    scanf(" %d",&n);
    verificar1(n);
}

void verificar1(int n){
    switch(n){
        case 1:
            crearDisco();
            sleep(2);
            menu1();
            break;
        case 2:
            aumentarTamDisco();
            sleep(2);
            menu1();
            break;
        case 3:
            reducirTamDisco();
            sleep(2);
            menu1();
            break;
        case 4:
            eliminarDisco();
            sleep(2);
            menu1();
            break;
        default:
            menu0();
    }
}

void menu2(){
    int n=0;
    system("clear");
    fflush(stdin);
    printf("\n******* PROYECTO FASE 1 MENEJO E IMPLEMENTACION DE ARCHIVOS ******* \n");
    printf("\n0. Menu Principal\n");
    printf("1. Crear Particion\n");
    printf("2. Eliminar Particion\n");
    printf("3. Formatear Particion\n");
    scanf(" %d",&n);
    verificar2(n);
}

void verificar2(int n){
    switch(n){
        case 1:
            crearParticion();
            sleep(2);
            menu2();
            break;
        default:
            menu0();
    }
}

void menu3(){
    int n=0;
    system("clear");
    fflush(stdin);
    printf("\n******* PROYECTO FASE 1 MENEJO E IMPLEMENTACION DE ARCHIVOS ******* \n");
    printf("\n0. Menu Principal\n");
    printf("1. Crear Archivo\n");
    printf("2. Modificar Archivo\n");
    printf("3. Eliminar Archivo\n");
    printf("4. Renombrar Archivo\n");
    scanf(" %d",&n);
}

void menu4(){
    int n=0;
    system("clear");
    fflush(stdin);
    printf("\n******* PROYECTO FASE 1 MENEJO E IMPLEMENTACION DE ARCHIVOS ******* \n");
    printf("\n0. Menu Principal\n");
    printf("1. Reporte de Archivo de Discos\n");
    printf("2. Reporte del MBR\n");
    printf("3. Reporte de Particion\n");
    printf("4. Reporte Particiones del Disco\n");
    scanf(" %d",&n);
    verificar4(n);
}

void verificar4(int n){
    switch(n){
        case 1:
            archDiscos();
            sleep(2);
            menu4();
            break;
        case 2:
            mbrDisco();
            sleep(2);
            menu4();
        default:
            menu0();
    }
}