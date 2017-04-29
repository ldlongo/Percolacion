                     # Percolacion
---------------------------------------------------------------
 Problema 1
----------------------------------------------------------------
a)
-Se corrieron redes de tamaño 4, 8, 16, 32, 64, 128.
-Para cada tamaño de red, se realizaron 27000 iteraciones, y en cada una se obtuvo una probabilidad critica.
-Con esos valores de pc se obtuvo el valor promedio y la desviacion standar:


Red       Promedio <pc>(L)       Desviacion Standar

4          0.561902               0.103012 
8          0.579216               0.070261      
16         0.587771               0.044733
32         0.594311               0.027819
64         0.592488               0.016824
128        0.592529               0.009722


b)

-Se corrieron redes de tamaño 4, 16, 32, 64, 128.
-Para cada tamaño de red, se utilizaron 100 probabilidades entre 0 y 1.
-Para cada probabilidad se realizaron 27000 iteraciones en las cuales se calculo la probabilidad de que el cluster percolase.
-De esta manera se obtuvieron puntos F(p) y se graficaron en un mismo grafico.
-Se creo la funcion mediana la cual determina el valor de pc como el primer valor de probabilidad que hace que el cluster percole mas de la mitad de las veces. 
-Para utilizar la funcion mediana se realizo un barrido más fino entre probabilidades de 0.5 y 0.7 y entre 0.58 y 0.60.

Red       Mediana(pc)     
4          0.5706 
8          0.5863                   
16         0.5902              
32         0.5933             
64         0.5929              
128        0.5926                  

c) En un grafico se tomaron los valores de dispersion y L del punto 1a) y se 
realizo un ajuste log-log de los datos. La pendiente (-1/nu) permitio
calcular el exponente critico nu:

Pendiente
-0.683
Exp nu sim.
1.465
Exp nu teorico
1.33

   En otro se grafico pc(L) en funcion de la disp. Se realizo un ajuste
porque no daba lineal y entonces se obtuvo un valor de ordenada de 
pc(inf)=0.5937


d) Para cada tamaño de red se realizo una busqueda de probabilidades
por encima y por debajo de pc entre 0.55 y 0.65. Se grafico logns vs logs. 
Ademas para cada probabilidad se realizo un ajuste lineal
para valores de logs<4 y se calculo el chicuadrado de cada ajuste. Luego
se grafico y se observo que hay una tendencia a minimizarse en el pc.

d) Para cada tamaño de red se realizo una busqueda de probabilidades
por encima y por debajo de pc entre 0.55 y 0.65. Se grafico logns vs logs. 
Ademas para cada probabilidad se realizo un ajuste lineal
y se calculo el chicuadrado de cada ajuste. El punto de chicuadrado
minimo fue elegido como valor de tau.

L      X^2(Chi)     Tau
4       0.609      -0.92 
8       0.606      -1.27 
16      0.598      -0.94
32      0.592      -1.61 
64      0.584      -1.68
128     0.591      -1.73

----------------------------------------------------------------------
  Problema 2
----------------------------------------------------------------------

Para cada tamaño de red L=[4,8,16,32,64,128] se calculo 
la Fuerza del cluster P y se grafico en funcion de la probabilidad p.
Se tomaron 100 probabilidades entre 0 y 1 en cada caso, y en cada probabilidad
se realizaron 27000 iteraciones.
Luego se utilizaron las probabilidades criticas del punto 1a) para
graficar log(P)vslog(p-pc) alrededor de pc +-0.3. 
Se ajustaron mediante una recta y de ellas se obtuvo el coeficiente Beta.
Con Beta y usando el valor de sigma teorico=36/91 se obtuvieron valores
para Tau:

Red	Betas	eBeta	Taus	eTau
4	0.5123	0.0054	2.1654	0.0017
8	0.3998	0.0028	2.1291	0.0009
16	0.3460	0.0031	2.1117	0.0010
32	0.3333	0.0040	2.1076	0.0013
64	0.3326	0.0043	2.1074	0.0014
128	0.3315	0.0044	2.1071	0.0014

-----------------------------------------------------------------------
 Problema 3
-----------------------------------------------------------------------

En este problema se calculo la masa del cluster percolante M vs el tamaño
de la red L=[4,8,16,32,64,128]. Esto fue realizado para 3 valores de probabilidad:
pc(L), pc+(L), y pc-(L), donde pc+=pc(L)+0.02 y pc-=pc(L)-0.02.
Se graficaron log(M)vslog(L) para cada una de estas probabilidades.
Se realizaron ajustes lineales para las curvas de pc y pc+, obteniendose los siguientes valores:

proba	Pendiente
pc	1.84716702675 (Dimension Fractal)
pc+	2.09376548734

-----------------------------------------------------------------------
 Problema 4 y 5
-----------------------------------------------------------------------

EL codigo lo que hace es tomar un vector de longitudes L[4 8 16 32 64 128]
Para cada longitud lo que hace es poblar la red con 50 probabilidades entre 
0.2 y 0.8 y para cada probabilidad realiza 27000 iteraciones. Cada probabilidad
devuelve una tabla de s vs ns, impresa cno el nombre L-p.txt donde
L es el tamaño de la red y p la probabilidad.

En python: scaling.py
Se realizo luego un analisis de los datos para valores de s entre 
 0.01<s/so<0.12 con so=(L^2). Se observo que los datos colapsaban
a una curva que es la funcion f(z). Me dio un poco mas corrida a la izquierda.
de lo que se esperaba que era que en z=0, f(0)=1.

Con estos datos luego se tuvieron en cuenta fragmentos con 1<s<15 y se busco
el valor de pmax, la proba para la que se hace maximo ns.
 
Se levanto la curva de Emax=zmax*s^(-Sigma) donde Emax=(pmax-pc(L)/(Pc(L)), 
y pmax la probabilidad donde se maximiza el tamaño de cluster s.

Se realizo un ajuste lineal en vez de uno logaritmico, mediante la funcion.
f(s)=b*s^(-a)
El parametro sigma resulto:
Sigma	eSigma
0.3421	0.0134
Con este valor y haciendo uso del nu teorico=4/3 se calculo el Tau.
Tau	eTau
2.4561	0.0179

---------------------------------------------------------------------
  Problema 6
----------------------------------------------------------------------
El programa en C lo que hace es calcular para un cierto tamaño de red L, 
el momento de segundo orden M2. Para ello se realizo un barrido en probabilidades p
y para cada una de ellas se obtuvo una tabla de s vs ns. Con ella se utilizo la funcion
momentok, la cual devuelve el momento de orden k de la distribucion.

Para procesar los datos se utilizo un programa en python llamado gmatching.py
Lo que hacemos alli es realizar para cda tamaño de red dos metodos para encontrar
el valor del exponente gamma:
------------------------------------
Primer metodo
------------------------------------
1- Se grafico M2(p)
2- Se calcularon los valores de gamma como las pendientes alrededor de pc
para valores de p>pc y p<pc. OBteniendose Gamma+ y Gamma-
3-Se graficaron Gamma+(p) y Gamma-(p) y se busco la iterseccion de las dos curvas.
 
-----------------------------------
Segundo metodo
----------------------------------
1- Grafico de logM2(p) vs log |p-pc| para p>pc y p<pc, obteniendose dos curvas.
2- A continuacion se recorrieron ambas curvas relizando el siguiente procedimiento de ajuste:
   a-Nos paramos en un punto y consideramos los 4 puntos siguientes.
   b-Realizamos un ajuste lineal.
   c-Nos movemos al siguiente punto de la curva.
3-Una vez que terminamos de recorrer ambas curvas vamos a tener una lista
  de pendientes de los ajustes. Se procede entonces a buscar aquellas que coinciden
  con un margen de 0.001.

Los valores de gamma encontrados fueron:
L=6     gamma=2.13
L=128   gamma=2.64
