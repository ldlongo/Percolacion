import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit
from scipy.special import zeta

#funcion para el ajuste
def func(x, a, b):
    return (b*pow(x,-a))

Data=[] #data sera una lista de diccionarios el primero es un diccionario de proba P(L1)
P={}#diccionario para un L  de etiquetas p1,p2,p3, etc...
#etiqueta de longitudes
L=['4','8','16','32','64','128']
#un solo tamano de red seria
#etiqueta de probas
m=50  #cantidad
a=0.2 #ext inf
b=0.8 #ext sup
step=(b-a)/m
p=[]
for i in range(0,m+1):
 p.append(a+i*step)

archivo=[]
#nombredearchivosparaleer
for i in range(0,len(L)):
 for j in range(0,len(p)):
     archivo.append(str(L[i])+'-'+format(p[j],'.4f')+'.txt')
 
#creo el diccionario P
for i in range(0,6): 
 for j in range(0,len(p)):
     #leo archivo
     f=open(archivo[i*len(p)+j],'r')
     lines=f.readlines()[2:]
     x=[]
     y=[]  
     for line in lines: 
      q = line.split()
      x.append(float(q[0]))
      y.append(float(q[1]))
     xv=np.array(x)
     yv=np.array(y)
     P[p[j]]=(xv,yv) #seria algo asi como el diccionario Pi
 Data.append(P)      #guardo el diccionario Pi en la pos i de la lista Data

#listo funca con esto recorro todos los datos
#resumen
#Data[i][p[j]][0/1] i: tamano de celda que   miro   i=L0,L1,...,L5.
#                   j: probabilidad    que   miro   j=p0,p1....,p50.


#necesito tambien ns(pc)

pcs=[0.5652,0.5822,0.5901,0.5919,0.5932,0.5935] #corri la de 64 con 50000 de iteraciones en cada proba y mejore el numero

#creo el diccionario Pc
Pc={}
for i in range(0,len(L)):
     #leo archivo
     f=open(str(L[i])+'-'+str(pcs[i])+'.txt','r')
     lines=f.readlines()[2:]
     x=[]
     y=[]  
     for line in lines: 
      q = line.split()
      x.append(float(q[0]))
      y.append(float(q[1]))
     xv=np.array(x)
     yv=np.array(y)
     Pc[L[i]]=(xv,yv) #Pc diccionario L[i]=L0,L1....,L5 0=s 1=ns

#---------------------------------
#f(z): scaling
#-----------------------------------
'''   
#Segundo Metodo: usando el tau y que ns(pc)=qo*s^-tau
fz2=[]
z2=[]      
tau=2.1190 #tau de L=64  del punto 2
#tau=2.1187  #tau de L=128 del punto 2
C=zeta(tau-1, 1) #zeta en tau-1
for i in range(0,len(Data)):#recorro la lista Data
 print L[i]#para controlar por donde va
 l=float(L[i])
 if i==4: #solo me quedo con tamano de red L=64
  for j in range(0,len(p)): #recorro probabilidades p
   qo=(pcs[i]/C)#zeda de riemman   
   for k in range (0,len(Data[i][p[j]][1])):
    for m in range(0,len(Pc[L[i]][0])):
    #Busco mismos s que esten en ns(p) y en ns(pc)
     if (Data[i][p[j]][0][k]==Pc[L[i]][0][m]):
      if (Pc[L[i]][0][m]/(l*l))>0.01 and (Pc[L[i]][0][m]/(l*l))<0.12:#mequedosolo con fragmentos chicos   
       s=Pc[L[i]][0][m]
       nsp=(Data[i][p[j]][1][k])/(l*l*27000) #recorro cada fila de p[j] (columna ns)
       nspc=(pow(s,-tau))#aca esta la diferencia con el primer metodo
       fz2.append(float(nsp)/float(nspc))
       sigma=(36.0/91.0)
       z2.append(pow(s,sigma)*((p[j]-pcs[i])/pcs[i])) #pcs es la del tamano i
#Nota: la forma pareceria que esta bien pero pareceria corrida mas a la izquierda.
#      en z=0 no me cruza por 1...

#Grafico

plt.figure(2)
plt.plot(z2,fz2,'bo')

plt.xlabel('z')
plt.ylabel(r'$f(z)=(\frac{ns(p)}{q_{o}s^-\tau})$')
#plt.legend(loc='upper left')
plt.show()
'''
#----------------------------------------
#sigma
#----------------------------------------

#como voy a correr para L=64 smax=L*L=64*64


#inicio s:
s=np.zeros(50)
for i in range(0,50): #es condicion del problema
 s[i]=i+1

#ahora para cada s me fijo cual es el valor de proba donde se maximiza ns
pmax=np.zeros(len(s))

for i in range(0,len(s)):
 nsmaxaux=0 #lo pongo en 0 para cada si
 pmaxaux=0
 for j in range(0,len(p)): #recorro probas
  for k in range(0,len(Data[4][p[j]][0])):
   if Data[4][p[j]][0][k]==s[i] and Data[4][p[j]][1][k]>nsmaxaux:
    nsmaxaux=Data[4][p[j]][1][k]#actualizo el nsmax
    pmaxaux=p[j] #actualizo la pmax
 pmax[i]=pmaxaux#lo guardo


#emax
emax=[]
for i in range(0,len(s)):
 emax.append((pmax[i]-pcs[4])/(pcs[4]))

sv=[]
emaxv=[]

for i in range(0,len(s)):
 sv.append((float(s[i])))
 emaxv.append((float(emax[i])))

#Fit
#Parametros del fit
parmfit=(curve_fit(func,sv,emaxv))

print parmfit
print "Sigma"
sigma=parmfit[0][0]
print sigma
print "Tau"
nu=(4/3) #de la table teorica para d=2
tau=2+nu*sigma
print tau

##Valores del fit
yfit=np.zeros(len(s))
for i in range(0,len(sv)):
 yfit[i]=(func(sv[i],*parmfit[0]))


#Grafico
plt.plot(sv,emaxv,'bo',label="L=64")
plt.plot(sv,yfit,'r-',label=r'$\epsilon_{max}(s)=A.s^{-\sigma}$')
plt.xlabel('s tamano de cluster')
plt.ylabel(r'$\epsilon_{max}$')
plt.legend(loc='upper left')
plt.show()


