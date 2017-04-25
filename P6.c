#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>


#define Z  27000 //iteraciones para cada proba.
#define N     128 //tamaño de la red

//-------------------------------------------------------------------
//                     Declaracion de funciones
//-------------------------------------------------------------------
void llenar(int *red, int n, float prob); 
void imprimir(int *red, int n);
int hoshen (int *red, int n);
int actualizar(int *red, int *clase, int s, int frag);
int etiqueta_falsa(int *red, int *clase, int sa, int si);
int corregir_etiqueta(int *red, int *clase, int n);
int percola(int *red, int n);
void tamanos (int *tamano, int *cuentatamano, int *red, int *clase, int n, int etiqperc);
float momentok(int*tamano, int *cuentatamano, int k, int n, int numiter);

//-----------------------------------------------------------------
//                           Principal
//-----------------------------------------------------------------
int main(){
	//Declaraciones
    int n, *red, i, j, m, z;
    int *clase;
    int *tamano;
    int *cuentatamano;
    float *probas, a, b, paso;
    float *momentdos;
    int numiter;
    int etiqperc;
   
    
    //Defino
    m=40;
    a=0.5;
    b=0.65;
    paso=(float)(b-a)/(float)(m);
    probas=malloc((m+1)*sizeof(float));
    numiter=Z;
   

 //inicializo probas
 for (i=0;i<(m+1);i++){probas[i]=a+(i*paso);}  

  n=N;
  red=malloc(n*n*sizeof(int));
  clase=malloc(n*n*sizeof(int));
  tamano=malloc(n*n*sizeof(int));
  cuentatamano=malloc(n*n*sizeof(int));
  momentdos=malloc((m+1)*sizeof(float));

  //Semilla
    srand(time(NULL)); //una semilla sola

  for  (z=0;z<(m+1);z++)//recorro probas
   {
    

    //Incializo tamano y cuentatamano cada vez que paso a una nueva proba
    for (i=0;i<n*n;i++)
      { 
       tamano[i]=i;     
       cuentatamano[i]=0;
      }
    for (j=0;j<Z;j++)//repito para una proba dada Z veces 
     {
      //pueblo
      llenar(red,n,probas[z]);

      //hk   
      hoshen(red,n);

      //cluster percolante
      etiqperc=percola(red,n);   

      //tamaños
      tamanos(tamano, cuentatamano, red, clase, n, etiqperc);

     }
   
    momentdos[z]=momentok(tamano, cuentatamano, 2, n, numiter); //calculo el momento de orden 2.   
   }

 printf("proba\t\tmomentdos\n");
 for (z=0;z<(m+1);z++)
  {
   printf("%f\t\t%f\n",probas[z],momentdos[z]);
  } 

  //Imprimo archivo de texto cuando termino con esa probabilidad, nomre del archivo tiene la L y proba
     char filename[64];

     FILE *f;                   
     sprintf(filename, "%d.txt", n);
     f=fopen(filename,"wt");

     fprintf(f,"Proba\t\tMomentdos\n",Z);
     fprintf(f,"Se plantaron %d semillas:\n",Z);
     for (z=0;z<(m+1);z++)
      {
       fprintf(f,"%f\t\t%f\n",probas[z],momentdos[z]);
      } 
     
  
      fflush(f);
      fclose(f);

free(red);
free(tamano);
free(cuentatamano);
free(clase);
free(probas);
free(momentdos);
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

//7) Tamanos
   
  //Esta función recorre la red y me dice para cada etiqueta(etiq) cual es el tamaño del cluster(cuentaetiq)
  //y para cada tamaño de cluster(tamano) que cantidad de clusters hay (cuentatamano)    

void tamanos (int *tamano, int *cuentatamano, int *red, int *clase, int n, int etiqperc){

int i, j, k, l;
int *etiq, *cuentaetiq;
int aux;

etiq=malloc(n*n*sizeof(int));

//guardo en etiq que etiquetas hay en la red(las que en clase son mayores a 0, osea, etiquetas verdaderas)
   for (l=0;l<n*n;l++)
     {
      if (clase[l]>0)
       {   
        etiq[l]=clase[l];
       }
     }

//creo array cuentasetiq y lo inicializo en 0
cuentaetiq=malloc(n*n*sizeof(int));
for (k=0;k<n*n;k++){      
cuentaetiq[k]=0;
}

    
//recorro red
    for (i=0;i<n;i++){
       for (j=0;j<n;j++){
          if (red[i*n+j]>0 && red[i*n+j]!=etiqperc) //la ultima condicion del if es para excluir de la cuenta al cluster percolante si lo hubiese
          {
            aux=red[i*n+j];
            cuentaetiq[aux]=cuentaetiq[aux]+1;
          }
      }
    } 

//recorro cuentaetiq
   for (l=0;l<n*n;l++)
     {
      if (cuentaetiq[l]>0)
       {   
        aux=cuentaetiq[l];
        cuentatamano[aux]=cuentatamano[aux]+1;
       }
     }

     free(etiq);
     free(cuentaetiq);
}
  
 //8) Momentok
   
  //Esta funcion recibe tamano s y cuenta tamano s y un entero k.
  // Devuelve el momento de orden k
 float momentok(int*tamano, int *cuentatamano, int k, int n, int numiter){
	int i;
	float momentdos;
	float ns;
	float s;
 
    momentdos=0;

    for (i=0;i<(n*n);i++)
     {
      if (cuentatamano[i]!=0)
       {	
        ns=((float)cuentatamano[i])/((float)(n*n*numiter));	
        s=(float)tamano[i];
	    momentdos=momentdos+(ns*(s*s)); //normalizo cuenta para obtener ns
	   } 
     }

return momentdos;
}

//9)Percola
  
  //Esta función toma la red y se fija si percola o no.
 // Si percola además me dice que cluster es el percolante.

int percola(int *red, int n){
int *etiq; 
int k, j, p;
j=1;
p=0;// 0/1=noperc/perc;
int clusterperc;

clusterperc=0;
etiq=malloc((ceil((n+0.0)/2)+2)*sizeof(int)); //En la primer fila las etiquetas van desde 0 1 2...hasta ceil(m/2)+1

for (k=0;k<(ceil((n+0.0)/2)+2);k++){etiq[k]=0;} //Inicio etiq

for (k=0;k<n;k++){ //Lleno etiq

    if (*(red+k)!=0 && *(red+k)!=j){ //primer fila i=0
        etiq[*(red+k)]=*(red+k);
        j=*(red+k);} //actualizo el j con el ultimo que guarde
    }


j=0;
while (j<n){
    if(*(red+(n-1)*n+j)<(ceil((n+0.0)/2)+2) && (*(red+(n-1)*n+j))*(etiq[*(red+(n-1)*n+j)])!=0){//recorro la ultima fila
    p=1;
    //printf("Si percolo: %d\n",*(r+(n-1)*n+j));
    clusterperc=*(red+(n-1)*n+j);
     break;}
    else {
      j=j+1;
      p=0;}
    }  

//if (p==0){printf("No Percolo\n");}    

free(etiq);
return clusterperc; //0 sino perc y la etiq del perc cuando percola
}
