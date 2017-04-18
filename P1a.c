#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>


//-------------------------------------------------------------------
//                     Declaracion de funciones
//-------------------------------------------------------------------
void llenar(int *red, int n, float prob); 
void imprimir(int *red, int n);

int main(){
   //Declaraciones
   int n, *red;
   float prob;

   //Defino
   n=5;
   prob=0.5;
   red=malloc(n*n*sizeof(float));

   llenar(red,n,prob);
   imprimir(red,n);



}
//-----------------------------------------------------------
//                   Definicion de funciones
//-----------------------------------------------------------
void llenar(int *red, int n, float p){

	/*Generador de numero aleatorio entre 0 y 1*/
	//srand(time(NULL));
    int i;
    float test;
    int s;
    s=n*n;
    for (i=0;i<s;i=i+1){
	   test = ( (float)(rand() % 101) )/100 ;
	   /*printf("Numero aleatorio entre 0 y 1 es %.2f\n",test)*/; 
       /*Acepto o rechazo con probabilidad p*/
        if (test<p)
    	 red[i]=1;
        else
    	 red[i]=0;
    }    
}

void imprimir(int *red, int n){
    int j;
    int i;
    int s;
    j=1;
    s=n*n;
    for (i=0;i<s;i=i+1){

        if (j!=n)
         {printf("%d\t",red[i]);
         j=j+1;}
        else
         {j=1;
         printf("%d\n",red[i]);}
    }
    printf("\n");
}
