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

#define N    27000//cantidad de iteraciones para L fija


//-----------------------------------------------------------------
//                           Principal
//-----------------------------------------------------------------

int main(){

int i, k, j, m, n, sumo;
int *longitudes;
float *masa;
float *pc;
float tamano;
int *red;
float sup, inf;

//Defino algunas cosas
    m=6;//cantidad de longitudes
    longitudes=malloc(m*sizeof(int)); // es la cantidad de realizaciones, o sea cantidad de pc que obtengo, y luego a prom
    masa=malloc(m*sizeof(float));//es la cantidad de veces que percolo para la probabilidad p
    pc=malloc(m*sizeof(float));
    sup=0.02;
    inf=-0.02;

//Lleno longitudes, pcritica(L) y tamano promedio
 longitudes[0]=4;
 longitudes[1]=8;
 longitudes[2]=16;
 longitudes[3]=32;
 longitudes[4]=64;
 longitudes[5]=128;

 pc[0]=0.565169;
 pc[1]=0.582233;
 pc[2]=0.590066;
 pc[3]=0.591930;
 pc[4]=0.592908;
 pc[5]=0.593546;

for (i=0;i<m;i++)
	{
		masa[i]=0;
	}


for (k=0;k<m;k++)//Recorro longitudes
 {  
   //Semilla ¿esta bien semilla aca? ¿una por longitud?
   srand(time(NULL)+k);
   n=longitudes[k];
   tamano=0; //pongo tamano en 0
   red=malloc(n*n*sizeof(int)); //creo el espacio en memoria  
   
   for (j=0;j<N;j++)//repito para una long dada N veces 
      {
       //pueblo
       llenar(red,n,pc[k]+inf);//corro para tres probas pc, pc+sup, pc+inf

       //hk   
       hoshen(red,n);   

      //percola o no percola?
       sumo=percola(red,n); //percola me devuelve el tamaño del cluster percolante y se lo pongo a sumo
       tamano=tamano+sumo;
      }
   masa[k]=(tamano)/(N); //promedio
    
 }

char filename[64];

FILE *f;   //Declara puntero a tipo FILE 
sprintf(filename, "pc-.txt" );
f=fopen(filename, "wt");
fprintf(f,"Se estudiaron %d longitudes.\n", m);
fprintf(f,"Longitud\tMasa\n");

printf ("Longitud\tMasa\n");

for (k=0;k<m;k++){
fprintf(f,"%5d\t\t%4f\n",longitudes[k],masa[k]);
printf ("%5d\t\t%4f\n",longitudes[k],masa[k]);
}

fflush(f);
fclose(f);

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

int percola (int *r, int n){
int *etiq; 
int i, k, j, m, p, etiqdelperc, tamanodelperc;
j=1;
m=n;
etiq=malloc(ceil(((m+0.0)/2)+2)*sizeof(int)); //En la primer fila no pueden las etiquetas van desde 0 1 2...hasta ceil(m/2)+1


    for (k=0;k<ceil((m+0.0)/2)+2;k++){etiq[k]=0;} //Inicio etiq

    for (k=0;k<m;k++){ //Lleno etiq

        if (*(r+k)!=0 && *(r+k)!=j){ //primer fila i=0
            etiq[*(r+k)]=*(r+k);
            j=*(r+k);} //actualizo el j con el ultimo que guarde
        else{j=j;}
     }


    j=0;
    while (j<m){
     if(*(r+(n-1)*m+j)<(ceil((m+0.0)/2)+2) && *(r+(n-1)*m+j)*etiq[*(r+(n-1)*m+j)]!=0){//recorro la ultima fila
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
      etiqdelperc=r[(n-1)*m+j];

    //recorro la red y cuento el tamaño:
      for (i=0;i<n;i++)
       {
          for (j=0;j<m;j++)
          { 
            if (r[i*m+j]==etiqdelperc)
            {
                tamanodelperc=tamanodelperc+1;
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

