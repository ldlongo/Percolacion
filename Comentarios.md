# Percolacion
---------------------------
       Problema 1
---------------------------

a)
-Se corrieron redes de tamaño 4, 16, 32, 64, 128.
-Para cada tamaño de red, se realizaron 27000 iteraciones, y en cada una se obtuvo una probabilidad critica.
-Con esos valores de pc se obtuvo el valor promedio y la desviacion standar:


Red       Promedio <pc>(L)       Desviacion Standar

4          0.565169               0.103374       
16         0.590066               0.044969
32         0.591930               0.027446
64         0.592908               0.017542
128        0.593546               0.010196


b)
-Se corrieron redes de tamaño 4, 16, 32, 64, 128.
-Para cada tamaño de red, se utilizaron 100 probabilidades entre 0 y 1.
-Para cada probabilidad se realizaron 27000 iteraciones en las cuales se calculo la probabilidad de que el cluster percolase.
-De esta manera se obtuvieron puntos F(p) y se graficaron en un mismo grafico.
-Se creo la funcion mediana la cual determina el valor de pc como el primer valor de probabilidad que hace que el cluster percole mas de la mitad de las veces. 
-Se creo la funcion densidad que deriva la distribucion acumulada F(p) y obtiene la densidad de probabilidad f(p).


Red       Mediana       
4          0.574 
8          0.584                   
16         0.594               
32         0.594              
64         0.594                
128        0.594                 

c) Dos graficos:
   En un grafico se tomaron los valores de dispersion y L del punto 1a) y se 
realizo un ajuste log-log de los datos. La pendiente (-1/nu)=-0.672 permitio
calcular el exponente critico nu=1.487.
   En otro se grafico pc(L) en funcion de la disp. Se realizo un ajuste
porque no daba lineal y entonces se obtuvo un valor de ordenada de 
pc(inf)=0.59327


d) Para cada tamaño de red se realizo una busqueda de probabilidades
por encima y por debajo de pc entre 0.55 y 0.65. Se grafico logns vs logs. 
Ademas para cada probabilidad se realizo un ajuste lineal
para valores de logs<4 y se calculo el chicuadrado de cada ajuste. Luego
se grafico y se observo que hay una tendencia a minimizarse en el pc.

---------------------------
       Problema 2
---------------------------

Para cada tamaño de red L=[4,8,16,32,64,128] se calculo 
la Fuerza del cluster P y se grafico en funcion de la probabilidad p.
Se tomaron 100 probabilidades entre 0 y 1 en cada caso, y en cada probabilidad
se realizaron 27000 iteraciones.
Luego se utilizaron las probabilidades criticas del punto 1a) para
graficar log(P)vslog(p-pc) alrededor de pc +-0.3. 
Se ajustaron mediante una recta y de ellas se obtuvo el coeficiente Beta.
Con Beta y usando el valor de sigma teorico=36/91 se obtuvieron valores
para Tau:

Red	Betas	Taus
4	0.4594	2.1484
8	0.3848	2.1243
16	0.3713	2.1199
32	0.3703	2.1196
64	0.3685	2.1190
128	0.3677	2.1187

---------------------------
       Problema 3
---------------------------

En este problema se calculo la masa del cluster percolante M vs el tamaño
de la red L=[4,8,16,32,64,128]. Esto fue realizado para 3 valores de probabilidad:
pc(L), pc+(L), y pc-(L), donde pc+=pc(L)+0.02 y pc-=pc(L)-0.02.
Se graficaron log(M)vslog(L) para cada una de estas probabilidades.
Se realizaron ajustes lineales para las curvas de pc y pc+, obteniendose los siguientes valores:

proba	Pendiente
pc	1.87164223999 (Dimension Fractal)
pc+	2.09834816949 (d)
