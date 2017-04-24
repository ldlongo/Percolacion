#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#define N     128 //lado de la red simulada
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
int percola(int *red, int n);
float *mediana(float *proba, float *probaper);
float *densidad(float *proba, float *probaper, int m);



int main(){
   //Declaraciones
   int n, *red, semillas, p, i, j, m;
   float a, b, *probas, *probaper, *med, *dens;
   double paso;
   double cuentanorm;

   //Defino
   n=N;
   red=malloc(n*n*sizeof(int));
   semillas=Z; // es la cantidad de iteraciones, o sea cantidad de pc's que obtengo, y luego a promediarlas
   a=0.5; //extremo inferior
   b=0.7; //extremo superior
   m=50;//cantidad de probabilidades
   paso=(double)(b-a)/(double)(m+1.0); //paso
   probas=malloc(m*sizeof(float));  //vector que contiene a las probabilidades.
   probaper=malloc(m*sizeof(float));//vector que contiene a las probabilidades de percolar
   cuentanorm=((double)1.0)/((double)semillas);

   printf("%f",cuentanorm);

   //Lleno probas
    
   for (i=0;i<m;i++)
   {
     probas[i]=a+paso*(i+1);
     probaper[i]=0.0;
    }

//Semilla la inicio una sola vez
    srand(time(NULL));

for (i=0;i<m;i++)//recorro probas
 {  

    for (j=0;j<semillas;j++)//repito para una proba dada <semillas> veces
     {
        //pueblo
        llenar(red,n,probas[i]);

        //hk   
        hoshen(red,n);   

        //percola o no percola

        p=percola(red,n);

        if (p==1)
         {
          probaper[i] = probaper[i]+(cuentanorm);//1/semillas es la cuenta normalizada
         } 
    }
 }

med=mediana(probas, probaper);

dens=densidad(probas, probaper, m);

//Imprimo probabilidades normalizadas en consola
printf ("Tamaño de red: %d\n",n);
printf ("Se estudiaron %d probabilidades.\n", m);
printf ("Se plantaron %d semillas para cada probabilidad.\n",semillas);
printf ("   p   \n");
for (i=0;i<m;i++){
    printf ("%f\n",probas[i]);
}
printf ("probabilidad de percolar\n");
for (i=0;i<m;i++){
    printf ("%f\n",probaper[i]);
}

printf ("La mediana es %f\n", med[0]);

//Imprimo densidad
printf("funcino densidad f(p)\n");
printf ("   p   \n");
for (i=0;i<m-1;i++){
    printf ("%f\n",dens[0+i]);
}
printf ("\ndensidad f\n");
for (i=0;i<m-1;i++){
    printf ("%f\n",dens[m+i]);
}


//Imprimo en archivo de texto
char filename[64];

FILE *f;   //Declara puntero a tipo FILE 
sprintf(filename, "%d.txt", n);
f=fopen(filename, "wt");

//Imprimo probabilidades normalizadas
    fprintf (f,"p\t\t\tprobperc\n");
for (i=0;i<m;i++)
{
    fprintf (f,"%f\t\t%f\n",probas[i],probaper[i]);
}


fflush(f);
fclose(f);



free(probas);
free(probaper);
free(red);    
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

//7)Percola
  
  //Esta función toma la red y se fija si percola o no.
 // Si percola además me dice que cluster es el percolante.

int percola(int *red, int n){
int *etiq; 
int k, j, p;
j=1;
p=0;// 0/1=noperc/perc;

etiq=malloc(n*sizeof(int)); 

for (k=0;k<n;k++){etiq[k]=0;} //Inicio etiq

for (k=0;k<n;k++){ //Recorro la primer fila y Lleno etiq

    if (*(red+k)!=0 && *(red+k)!=j){ //primer fila i=0
        etiq[*(red+k)]=*(red+k);
        j=*(red+k);} //actualizo el j con el ultimo que guarde
    }


j=0;
while (j<n){ //Recorro la ultima fila
    if(*(red+(n-1)*n+j)<n && (*(red+(n-1)*n+j))*(etiq[*(red+(n-1)*n+j)])!=0){//recorro la ultima fila
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

//8)Mediana

    //Esta función recibe el vector de probabilidades proba y el de probaper
    //y devuelve el primer valor de probabilidad pc para el cual la red percola al menos la mitad de las veces
    //como tambien el valor de esa probabilidad de percolar probaper(pc)
    //salida[0]=pc  salida[1]=probaperc(pc)
float *mediana(float *proba, float *probaper){
int i;
float *salida;

i=0;
salida=malloc(2*sizeof(float));
while (probaper[i]<0.5){
i=i+1;}

salida[0]=proba[i];
salida[1]=probaper[i];

return salida;
}

//9)Densidad

   //Esta funcion toma la acumulada F(p) y calcula su derivada para devolver
   // en un vector las probas y el valor de f(p).
float *densidad(float *probas, float *probaper, int m){
int j;
float *salida;

salida=malloc(2*(m-1)*sizeof(float));//tamaño m-1 porque para la derivada 2 puntos se me transforman en 1


       for (j=0;j<(m-1);j++)
       {
                   
            salida[j]=(probas[j+1]+probas[j])/2; //tomo el punto del medio
            salida[m-1+j]=(probaper[j+1]-probaper[j])/(probas[j+1]-probas[j]);   //pendiente
       }
 
 
return salida;
}


