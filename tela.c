#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void continuar(){
char psq [10];
printf("\n\nTecle enter para continuar");
gets(psq);
}

void rodape(){ // telas rodape
system ("clear");
printf("       - - - SISTEMA LOCADORA SIDEGUM - - -\n\n");
}

void inicio(){ //tela inicial
system ("clear");
printf("Bem vindo ao sistema da LOCADORA SIDEGUM");
printf("\n\nPor favor, insira os dados corretamente!!");
printf("\n\nUtilize sempre letras minusculas e caracteres sem acento");
continuar();
}

void limparb(){
while((getchar())!='\n');}

