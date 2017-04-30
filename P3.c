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
float *promydisp(int *a, int n);


#define N    27000//cantidad de iteraciones para L fija


//-----------------------------------------------------------------
//                           Principal
//-----------------------------------------------------------------

int main(){

int i, k, j, m, n, t;
int *longitudes;
float *masa, *errormasa;
float *pc;
int *tamano;
int *red;
float sup, inf;

//Semilla ¿esta bien semilla aca? ¿una por longitud?
   srand(time(NULL));

//Defino algunas cosas
    m=8;//cantidad de longitudes
    longitudes=malloc(m*sizeof(int));
    masa=malloc(m*sizeof(float));
    errormasa=malloc(m*sizeof(float));
    pc=malloc(m*sizeof(float));
    sup=0.02;
    inf=-0.03;

//Lleno longitudes, pcritica(L) y tamano promedio
 longitudes[0]=4;
 longitudes[1]=8;
 longitudes[2]=16;
 longitudes[3]=32;
 longitudes[4]=64;
 longitudes[5]=128;
 longitudes[6]=256;
 longitudes[7]=512;

 pc[0]=0.561902;
 pc[1]=0.579216;
 pc[2]=0.587771;
 pc[3]=0.594311;
 pc[4]=0.592488;
 pc[5]=0.592529;
 pc[6]=0.592529;
 pc[7]=0.592529;

for (i=0;i<m;i++)
	{
		masa[i]=0;
	}


for (k=0;k<m;k++)//Recorro longitudes
 {  
   t=0; 
   n=longitudes[k];
   red=malloc(n*n*sizeof(int)); //creo el espacio en memoria  
   tamano=malloc(N*sizeof(int)); //aca voy a guardar los tamaños para las N iteraciones luego promedio y dispersion
   for (j=0;j<N;j++){tamano[j]=0;} //pongo tamanos en cero

   for (j=0;j<N;j++)//repito para una long dada N veces 
      {
       //pueblo
       llenar(red,n,pc[k]+inf);//corro para tres probas pc, pc+sup, pc+inf //ver arriba inf

       //hk   
       hoshen(red,n);   

      //percola o no percola?
       //sumo=percola(red,n); //percola me devuelve el tamaño del cluster percolante y se lo guardo a sumo
       //tamano=tamano+sumo;
       if (percola(red,n)!=0) //tiene en cuent que si no percolo no lo sume a tamaño.
       {
        tamano[t]=percola(red,n);
        t=t+1;//t es cuantos lugares de tamanos lleno
       }
        
      }
   masa[k]=promydisp(tamano,t)[0]; //promedio
   errormasa[k]=promydisp(tamano,t)[1];//error en la masa
    
 }

char filename[64];

FILE *f;   //Declara puntero a tipo FILE 
sprintf(filename, "pc--.txt" );
f=fopen(filename, "wt");
fprintf(f,"Se estudiaron %d longitudes.\n", m);
fprintf(f,"Longitud\tMasa\t\t\tErrormasa\n");

printf ("Longitud\tMasa\t\t\tErrormasa\n");

for (k=0;k<m;k++){
fprintf(f,"%5d\t\t%4f\t\t%5f\n",longitudes[k],masa[k],errormasa[k]);
printf ("%5d\t\t%4f\t\t%5f\n",longitudes[k],masa[k],errormasa[k]);
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
     test = (float)rand()/(float)RAND_MAX; 
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

//8)proydis
  
  //Esta funcion toma un array a y su tamaño n
  //y devuelve el promedio y la desviacion standar.

float *promydisp(int *a, int n){
    int i;
    float prom;
    float desv;
    float *salida; //salida[0]=promedio salida[1]=dispersion
 
    salida=malloc(2*sizeof(float));
    prom=0;
    desv=0;

    for (i=0;i<n;i=i+1){
    prom=prom+a[i];    
    }
     
    for (i=0;i<n;i=i+1){ 
    desv=desv+pow(a[i]-((float)prom/(float)n),2);}

    salida[0]=((float)prom/(float)n);
    salida[1]=sqrt((float)desv/(float)n);
    return salida;
}