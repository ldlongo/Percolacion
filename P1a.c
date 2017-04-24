#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#define N    128 //lado de la red simulada
#define Z  27000 //iteraciones para cada proba.


//-------------------------------------------------------------------
//                     Declaracion de funciones
//-------------------------------------------------------------------
void llenar(int *red, int n, float prob); 
void imprimir(int *red, int n);
int hoshen (int *red, int n);
int actualizar(int *red, int *clase, int s, int frag);
int etiqueta_falsa(int *red, int *clase, int sa, int si);
int corregir_etiqueta(int *red, int *clase, int n);
int percola(int *r, int n);
float *promydisp(float *a, int n);


int main(){
   //Declaraciones
   int n, *red, j, prec, semillas, z, p; 
   float prob, prom, disp;
   float *pc;

   //Defino
   n=N;
   red=malloc(n*n*sizeof(float));
   prec=20; // es la precisión, medida en cantidad de pasos que hago el metodo de biyeccion.
   semillas=Z; // es la cantidad de iteraciones, o sea cantidad de pc's que obtengo, y luego a promediarlas
   pc=malloc(semillas*sizeof(float)); //array que tiene las probas criticas

for(j=0;j<semillas;j++)

{

  prob=0.5;  //vuelvo a empezar con la proba en 0.5
  prec=4;     //vuelvo a ponerlo en 4

  //Semilla
  srand(time(NULL)+j);

  for(z=0;z<prec;z++)

    {

    //pueblo

    llenar(red,n,prob);
    
    //hk   

    hoshen(red,n);   

    //percola o no percola?

    p=percola(red,n);


    //nueva proba 
    if (p==1){
       //printf("percolo: %d , con proba %f\n",p,prob);
       prob = prob-(1.0/prec);}
    else{
       //printf("no percolo\n");
       prob = prob+(1.0/prec);} 
    
    prec=prec*2;//incremento prec

    }

  pc[j]=prob;//guardo el ultimo valor de proba de la iteracion

}

//promedio y dispersion
prom=promydisp(pc,semillas)[0];
disp=promydisp(pc,semillas)[1];

//Imprimo pc's , promedio y dispersion:

printf("Tamaño de red: %d\n",n);
printf("Se plantaron %d semillas:\n",semillas);
printf("pc:\n");

for (j=0;j<semillas;j++)
 {
   printf("pc=%f\n", pc[j]);
 }
printf("promedio %f\n",prom);
printf("dispersion %f\n",disp);


//Imprimo en archivo de texto
char filename[64];

FILE *f;                   /* Declara puntero a tipo FILE */
   sprintf(filename, "%d.txt", n);
   f=fopen(filename,"wt");

fprintf(f,"Tamaño de red: %d",n);
fprintf(f,"Se plantaron %d semillas:\n",semillas);
fprintf(f,"pc:\n");

for (j=0;j<semillas;j++){
 fprintf(f,"%3f\n",pc[j]);
}

fprintf(f,"promedio %f\n",prom);
fprintf(f,"dispersion %f\n",disp);

fflush(f);
fclose(f);


free(red);
free(pc);
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

//7)Percola
  
  //Esta función toma la red y se fija si percola o no.
 // Si percola además me dice que cluster es el percolante.

int percola(int *r, int n){
int *etiq; 
int k, j, p;
j=1;
p=0;// 0/1=noperc/perc;

etiq=malloc((ceil((n+0.0)/2)+2)*sizeof(int)); //En la primer fila las etiquetas van desde 0 1 2...hasta ceil(m/2)+1

for (k=0;k<(ceil((n+0.0)/2)+2);k++){etiq[k]=0;} //Inicio etiq

for (k=0;k<n;k++){ //Lleno etiq

    if (*(r+k)!=0 && *(r+k)!=j){ //primer fila i=0
        etiq[*(r+k)]=*(r+k);
        j=*(r+k);} //actualizo el j con el ultimo que guarde
    }


j=0;
while (j<n){
    if(*(r+(n-1)*n+j)<(ceil((n+0.0)/2)+2) && (*(r+(n-1)*n+j))*(etiq[*(r+(n-1)*n+j)])!=0){//recorro la ultima fila
    p=1;
    //printf("Si percolo: %d\n",*(r+(n-1)*n+j));
     break;}
    else {
      j=j+1;
      p=0;}
    }  

//if (p==0){printf("No Percolo\n");}    

free(etiq);
return p;
}

//8)proydis
  
  //Esta funcion toma un array a y su tamaño n
  //y devuelve el promedio y la desviacion standar.

float *promydisp(float *a, int n){
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
    desv=pow(a[i]-(prom/n),2)+desv;}

    salida[0]=prom/n;
    salida[1]=sqrt(desv/n);
    return salida;
}