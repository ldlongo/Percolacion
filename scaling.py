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
L=['64']
#un solo tamano de red seria
#etiqueta de probas
m=40  #cantidad
a=0.2 #ext inf
b=0.8 #ext sup
step=float(b-a)/float(m)
p=[]
for i in range(0,m+1):
 p.append(a+i*step)

archivo=[]
#nombredearchivosparaleer
for i in range(0,1):
 for j in range(0,len(p)):
     archivo.append(str(L[i])+'-'+format(p[j],'.4f')+'.txt')


#creo el diccionario P
for i in range(0,len(L)): 
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

#Resumen de lo realizado hasta ahora:
#Data[i][p[j]][0/1] i: tamano de celda que   miro   i=L0,L1,...,L5.    0:s
#                   j: probabilidad    que   miro   j=p0,p1....,p40.   1:ns


#necesito tambien ns(pc)

#pcs=[0.5619,0.5792,0.5878,0.5943,0.5925,0.5925]
pcs=[0.5925]

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
#Marcadores
mark_dict = ['o', 'v', '^', '<', '>', '8', 's', 'p', '*', 'h', 'H', 'D', 'd', '*', '+','3','|','1']
#colores=['b','r','k','m','c','g','y','b','r','k','m','c','g','y','b','r']
control=np.zeros(18)
#Primer Metodo: usando ns(pc) de la iteraciones
fz=[]
z=[]
mk=-1
#grafico
plt.figure(0)
plt.xlabel('z',size=20)
plt.ylabel(r'$f(z)=\frac{ns(p)}{ns(pc)}$',size=25)
plt.semilogy()

for i in range(0,len(Data)):#recorro la lista Data
 print L[i]#para controlar por donde va
 l=float(L[i])
 if i==0: #solo me quedo con tamano de red L=64
  for j in range(0,len(p)): #recorro probabilidades p
   for k in range (0,len(Data[i][p[j]][1])):
    for m in range(0,len(Pc[L[i]][0])):
    #Busco mismos s que esten en ns(p) y en ns(pc)
     if (Data[i][p[j]][0][k]==Pc[L[i]][0][m]):
      if (Data[i][p[j]][0][k]/(l*l))>0.00007 and (Data[i][p[j]][0][k]/(l*l))<0.004 :#mequedosolo con fragmentos chicos   
       s=Pc[L[i]][0][m]
       #calculo el mk: para el plot
       for q in range(0,18):
        if int(s)==(q+1):
         mk=q+1
         control[mk]=control[mk]+1
       nsp=Data[i][p[j]][1][k] #recorro cada fila de p[j] (columna ns)
       nspc=Pc[L[i]][1][m]
       fznuevo=float(nsp)/float(nspc)
       fz.append(fznuevo) 
       sigma=(36.0/91.0)
       znuevo=pow(s,sigma)*(p[j]-pcs[i])
       z.append(znuevo) #pcs es la del tamano i
       print mk
       plt.plot(znuevo,fznuevo,marker=mark_dict[mk], markersize=5, linestyle='None',color='b',label=int(s) if control[s]==1 else '')   
plt.plot((-1.5, 1), (1, 1), 'g-')
plt.plot((0, 0), (0.001, 10), 'g-')
plt.text(-0.85,0.4, "$Tamanos\ de\ cluster$", fontsize=20, bbox=dict(facecolor='g', alpha=0.5))
plt.legend(loc="lower center", numpoints=1)
plt.xlim(-1.3,0.65)
plt.show()       

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
#plt.figure(1)
#plt.plot(z,fz,marker=mark_dict[5],linestyle='None',color='k')

#creo que el grafico de elliot es logaritmico en y
#plt.legend(loc='upper left')

'''
'''
#----------------------------------------
#sigma
#----------------------------------------

#como voy a correr para L=64 smax=L*L=64*64


#inicio s:
s=np.zeros(15)
for i in range(0,15): #es condicion del problema
 s[i]=i+1

#ahora para cada s me fijo cual es el valor de proba donde se maximiza ns
pmax=np.zeros(len(s))

for i in range(0,len(s)):
 nsmaxaux=0 #lo pongo en 0 para cada si
 pmaxaux=0
 for j in range(0,len(p)): #recorro probas
  for k in range(0,len(Data[0][p[j]][0])):
   if Data[0][p[j]][0][k]==s[i] and Data[0][p[j]][1][k]>nsmaxaux:
    nsmaxaux=Data[0][p[j]][1][k]#actualizo el nsmax
    pmaxaux=p[j] #actualizo la pmax
 pmax[i]=pmaxaux#lo guardo


#emax
emax=[]
for i in range(0,len(s)):
 emax.append((pmax[i]-pcs[0])/(pcs[0]))

sv=[]
emaxv=[]

for i in range(0,len(s)):
 sv.append((float(s[i])))
 emaxv.append((float(emax[i])))

#Fit
#Parametros del fit
parmfit=(curve_fit(func,sv,emaxv))



print "Sigma""\t""eSigma"
sigma=parmfit[0][0]
esigma=np.sqrt(parmfit[1][0][0])
#print "eSigma"
print format(parmfit[0][0],'.4f')+"\t"+format(np.sqrt(parmfit[1][0][0]),'.4f')
#print sigma
print "Tau""\t""eTau"
nu=(float(4)/float(3)) #de la table teorica para d=2
tau=2+(float(nu)*sigma)
etau=float(nu)*esigma
print format(tau,'.4f')+"\t"+format(etau,'.4f')

xfit=[]
for i in range(1,200):
 xfit.append(i*0.08)

##Valores del fit
yfit=np.zeros(len(xfit))
for i in range(0,len(xfit)):
 yfit[i]=(func(xfit[i],*parmfit[0]))


#Grafico
plt.plot(sv,emaxv,'bo',label="$L=64$")
plt.plot(xfit,yfit,'g-',label="$\epsilon_{max}(s)=A.s^{-\sigma}$")
plt.xlabel(r"$s\ tamano\ de\ cluster$", size=15)
plt.ylabel(r'$\epsilon_{max}$', size=20)
plt.text(9,-0.5, "$\sigma=0.34$""\n"r"$\tau=2.45$", fontsize=20, bbox=dict(facecolor='g', alpha=0.5))
plt.legend(loc='upper left',numpoints=1)
plt.ylim((-0.8,0))
plt.show()

'''
