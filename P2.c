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
int hoshen (int *red, int n);
int actualizar(int *red, int *clase, int s, int frag);
int etiqueta_falsa(int *red, int *clase, int sa, int si);
int corregir_etiqueta(int *red, int *clase, int n);
int percola (int *red, int n);

#define N     128 //lado de la red simulada
#define Z  27000 //iteraciones para cada proba.
//-----------------------------------------------------------------
//                           Principal
//-----------------------------------------------------------------
int main(){
    int *red; /*declaro red como puntero a int*/
    int i, j, n, m, k;
    float paso;
    float *probas;
    float a, b;
    float *tamanopromedio; //array que tiene el tamañopromdelclusterpercolante


    //Defino algunas cosas
    n=N;
    m=100;//cantidad de probabilidades
    a=0.0;// extremo inferior
    b=1.0;//extremo superior
    paso=(b-a)/(m+1.0); //paso
    probas=malloc(m*sizeof(float)); // es la cantidad de realizaciones, o sea cantidad de pc que obtengo, y luego a prom
    red=malloc(n*n*sizeof(int)); //creo el espacio en memoria   
    tamanopromedio=malloc(m*sizeof(float));//es la cantidad de veces que percolo para la probabilidad p


//Lleno probas y tamano promedio
    
for (i=0;i<m;i++){
    probas[i]=a+paso*(i+1);
    tamanopromedio[i]=0;
}

for (k=0;k<m;k++)

{//recorro probas*/
      //Semilla
       srand(time(NULL)+k);

    for (j=0;j<Z;j++)

    {//repito para una proba dada Z veces

   

    //pueblo

       llenar(red,n,probas[k]);

    //hk   

       hoshen(red,n);   

    //percola o no percola?

     tamanopromedio[k]=tamanopromedio[k]+percola(red,n);
    }
}


//Imprimo en archivo de texto
//Imprimo en archivo
char filename[64];

FILE *f;   //Declara puntero a tipo FILE 
sprintf(filename, "%d.txt", n);
f=fopen(filename, "wt");
fprintf(f,"Tamaño de red: %d x %d\n",n,n);
fprintf(f,"Se estudiaron %d probabilidades.\n", m);
fprintf(f,"Se plantaron %d semillas para cada probabilidad.\n",Z);
fprintf(f,"p\t\tFuerza P\n");

printf ("p\t\tFuerza P\n");

for (k=0;k<m;k++){
fprintf(f,"%5f\t%7f\n",probas[k],tamanopromedio[k]/(Z*(n*n)));
printf ("%5f\t%7f\n",probas[k],(tamanopromedio[k])/(Z*(n*n)));
}


fflush(f);
fclose(f);

free(red);
free(probas);

return 0;
}

//-----------------------------------------------------------
//                   Definicion de funciones
//-----------------------------------------------------------
//1)Llenar

  //Función que llena la red de nxn con probabilidad p.

void llenar(int *red, int n, float p){

  /*Generador de numero aleatorio entre 0 y 1*/
  //srand(time(NULL));
    int i;
    float test;
    int s;
    s=n*n;
    for (i=0;i<s;i++){
     test = ( (float)(rand() % 101) )/100 ;
     /*printf("Numero aleatorio entre 0 y 1 es %.2f\n",test)*/; 
       /*Acepto o rechazo con probabilidad p*/
        if (test<p)
       red[i]=1;
        else
       red[i]=0;
    }    
}

//2)Imprimir

void imprimir(int *red, int n){
    int j;
    int i;
    int s;
    j=1;
    s=n*n;
    for (i=0;i<s;i=i+1){

        if (j!=n)
         {printf("%2d\t",red[i]);
         j=j+1;}
        else
         {j=1;
         printf("%2d\n",red[i]);}
    }
    printf("\n");
}

//3)Hoshen Kopelman

    //Funcion que implementa en algoritmo de Hoshen-Kopelman.
    //Recibe el puntero que apunta a la red y asigna etiquetas 
    //a cada fragmento de red. 

int hoshen(int *red, int n){
int *clase;
int i,j,si,sa;    
int frag;

clase=malloc(n*n*sizeof(int)); /*vector clase*/


//Primer Pasada

// primer elemento de la red

  si=0;
  frag=1;
  if (*red) 
    {
    frag=actualizar(red,clase,si,frag);
    }

// primera fila de la red

  for(i=1;i<n;i++) 
    {
      if (*(red+i)) 
         {
           si=*(red+i-1);  
           frag=actualizar(red+i,clase,si,frag);
         }
    }  


// el resto de las filas 

  for(i=n;i<n*n;i=i+n)
    {

      // primer elemento de cada fila

      if (*(red+i)) 
         {
           sa=*(red+i-n); //miro el de arriba 
           frag=actualizar(red+i,clase,sa,frag);
         }

      for(j=1;j<n;j++)
         if (*(red+i+j))
            {
               si=*(red+i+j-1); 
               sa=*(red+i+j-n);

               if (sa*si>0)
               {
               etiqueta_falsa(red+i+j,clase,sa,si);
               }
               else 
               { if (si!=0) {frag=actualizar(red+i+j,clase,si,frag);}
                 else       {frag=actualizar(red+i+j,clase,sa,frag);}
               }
            }
    }

//Segunda Pasada
corregir_etiqueta(red,clase,n);

free(clase);

return 0;
}

//4)Actualizar

    // Esta funcion actualiza la etiqueta del elemento actual. 
    // Si es necesario crea una nueva etiqueta y actualiza el vector clase.

int actualizar(int *red, int *clase, int s, int frag){
/*Pongo Etiqueta*/
  if(s==0)
    {  frag=frag+1; //preparo la nueva etiqueta
     *(red)=frag;
     *(clase+frag)=frag;//actualizo el vector de clase
    }
  else
    {
      *(red)=s;
    } 

return frag;
}

//5)Etiqueta falsa
       
   //Esta función  soluciona el conflicto que aparece cuando si.sa>0.
   //Busca las etiquetas verdaderas de cada valor, y de ellas se queda con la mas chica.
   //Asigna la etiqueta mas chica al elemento de la red actual.
   //Actualiza el vector de clase. 

int etiqueta_falsa(int *red, int *clase, int sa, int si){ 
//Busco las etiquetas verdaderas de sa y de si en el vector clase
while (clase[sa]<0){
    sa=-clase[sa];
}
while (clase[si]<0){
    si=-clase[si];
}

// De esas dos se queda con la mas chica y esa es la que pone en *r actual. Actualizo clase
if(sa>si){
    *red=si;
    clase[sa]=-si;
    clase[si]=si;
}
else if(sa<si){
    *red=sa;
    clase[si]=-sa;
    clase[sa]=sa;
}            
else{//son iguales
    *red=sa; //cualquiera de las dos
    clase[si]=si;
    clase[sa]=sa;
}
return 0;
}

//6)Corregir etiqueta
  
  //Esta función toma el vector clase y recorre la red reasignando
  //etiquetas cuando estas resultan ser falsas.
int corregir_etiqueta(int *red, int *clase, int n){
int i,j,s,etq;
for(i=0;i<n;i++){
    for (j=0;j<n;j++)
    {
        s=red[i*n+j];
        if (s==0)
        {
        red[i*n+j]=0;
        }
        else 
        {
        etq=clase[s];
        while (etq<0){
         etq=clase[-1*etq];
         }
        red[i*n+j]=etq;} 
    }
}
return 0;
}

//7) Percola

  //Función que me dice si percola o no    
  //Además me devuelve la etiqueta del percolante y su tamaño 

int percola (int *red, int n){
int *etiq; 
int i, k, j, m, p, etiqdelperc, tamanodelperc;
m=n;
j=1;
etiq=malloc(ceil(((m+0.0)/2)+2)*sizeof(int)); //En la primer fila no pueden las etiquetas van desde 0 1 2...hasta ceil(m/2)+1


    for (k=0;k<ceil((m+0.0)/2)+2;k++){etiq[k]=0;} //Inicio etiq

    for (k=0;k<m;k++){ //Lleno etiq

        if (*(red+k)!=0 && *(red+k)!=j){ //primer fila i=0
            etiq[*(red+k)]=*(red+k);
            j=*(red+k);} //actualizo el j con el ultimo que guarde
        else{j=j;}
     }


    j=0;
    while (j<m){
     if(*(red+(n-1)*m+j)<(ceil((m+0.0)/2)+2) && *(red+(n-1)*m+j)*etiq[*(red+(n-1)*m+j)]!=0){//recorro la ultima fila
      p=1;
      //printf("percola el %d\n",*(r+(n-1)*m+j));
      break;}
     else {
      j=j+1;
      p=0;}
    }



    if (p==1)
    {
      tamanodelperc=0;
      etiqdelperc=red[(n-1)*m+j];

    //recorro la red y cuento el tamaño:
      for (i=0;i<n;i++)
       {
          for (j=0;j<m;j++)
          { 
            if (red[i*m+j]==etiqdelperc)
            {
                tamanodelperc=tamanodelperc+1; //cada elemento que coincida con la etiqueta le sumo uno al contador tamanodelperc
            }
            else{}

          }
       }

    }
    else
    {
        //printf ("no percolo")
        tamanodelperc=0;
    }
    //printf ("Tamaño de red: %dx%d", n,n);
    //printf ("Tamaño promedio del percolante %f", tamanoprom);

free(etiq);
return tamanodelperc;
}

