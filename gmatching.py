import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

#funcion para el ajuste
def func(x, a, b):
    return (a*x)+b

data={} #diccionario de datos
#probas
archivo=['6.txt','128.txt']
key=['6','128']

for i in range(0,len(key)):
 f=open(archivo[i],'r')
 lines=f.readlines()[2:]
 x=[]
 y=[]
  
 for line in lines: 
      p = line.split()
      x.append(float(p[0]))
      y.append(float(p[1]))
 xv=np.array(x)
 yv=np.array(y)
 data[key[i]]=[xv,yv]
 f.close()
'''
#--------------------------------------------------------------------------
#gamma matching
#Metodo 1: interseccion de pendientes
#Se grafican gammas vs p alrededor de pc para pc- y pc+ y se busca donde
#se intersectan ambas curvas
#-------------------------------------------------------------------------- 

pc=[0.587771, 0.5925]
gamasup={}
gamainf={}
for k in range(0,len(key)):
 aux1=[]
 aux2=[]   
 for i in range(0,len(data[key[k]][0])-1):
    if data[key[k]][0][i]>pc[k]:
     #defino
     pinf=data[key[k]][0][i]
     psup=data[key[k]][0][i+1]
     m2inf=data[key[k]][1][i]
     m2sup=data[key[k]][1][i+1]
     #ademas sabemos que la derivada tiene que ser positiva me quedo con las que son posit
     if (float(m2sup-m2inf)/float(psup-pinf))<0:
      aux1.append(data[key[k]][0][i]-pc[k])
      aux2.append(float(m2sup-m2inf)/float(psup-pinf))
 gamasup[key[k]]=(aux1,aux2)
 

for k in range(0,len(key)):
 aux1=[]
 aux2=[]   
 for i in range(0,len(data[key[k]][0])-1):
    if data[key[k]][0][i]<pc[k]:
     #defino
     pinf=data[key[k]][0][i]
     psup=data[key[k]][0][i+1]
     m2inf=data[key[k]][1][i]
     m2sup=data[key[k]][1][i+1]
     #ademas sabemos que la derivada tiene que ser positiva me quedo con las que son posit
     if (float(m2sup-m2inf)/float(psup-pinf))>0:
      aux1.append(-(data[key[k]][0][i]-pc[k]))
      aux2.append(float(m2sup-m2inf)/float(psup-pinf))
 gamainf[key[k]]=(aux1,aux2)


#---------------------------------------------------------------
 
#grafico:
 #key[0] para 6 y key[1] para 128
L=0#0 o 1 si quiero L=6 o L=128
plt.figure(0)
#grafico en dos colores:
for i in range(0,len(data[key[L]][0])):
 if   data[key[L]][0][i]<pc[1]:             
  plt.plot(data[key[L]][0][i],data[key[L]][1][i],'ro')
 else:              
  plt.plot(data[key[L]][0][i],data[key[L]][1][i],'bo')
               
plt.xlabel(r'$p$',size=15)
plt.ylabel(r'$M_{2}(p)$',size=15)               
plt.figure(1)
plt.plot(gamainf[key[L]][0],gamainf[key[L]][1],'ro',label=r'$\gamma-$')
plt.plot(gamasup[key[L]][0],gamasup[key[L]][1],'bo',label=r'$\gamma+$')
plt.legend()
plt.xlabel(r'$|p-p_{c}|$')
plt.ylabel(r'$\gamma$'" gamma")

plt.show()

'''
#--------------------------------------------------------------------------
#gamma matching
#Metodo de pendientes paralelas
#Se grafican log(M2) VS log(p-pc) para p>pc y p<pc. Luego se buscan
#ajustes cuyas pendientes a lo largo de esas curvas que sean paralelas.
#-------------------------------------------------------------------------- 
#L 6: con n=4(cantidad de puntos del fit) y margen=0.001
      #encontro un gamma =-2.13
#L 128: con n=4  (cantidad de puntos del fit) y margen=0.001
      #encontro un gamma =-2.64

pc=[0.587771, 0.5925]

logfitxmas=[]
logfitymas=[]
L=1#0 o 1 si quiero L=6 o L=128

# y me contruyo dos graficos uno en pc+ y otro pc- ambos en log log
for i in range(0,len(data[key[L]][0])):
 if data[key[L]][0][i]>pc[L]:
  logfitxmas.append(np.log(data[key[L]][0][i]-pc[L]))
  logfitymas.append(np.log(data[key[L]][1][i]))

logfitxmen=[]
logfitymen=[]  
for i in range(0,len(data[key[L]][0])):  
 if data[key[L]][0][i]<pc[L]:
  logfitxmen.append(np.log(pc[L]-data[key[L]][0][i]))
  logfitymen.append(np.log(data[key[L]][1][i]))

#me muevo sobre las curvas (xmen,ymen) y (xmas,ymas), y elijo realizar ajustes tomando n puntos
n=4#numero de puntos que tomo para fit
gammamenos=[]#tiene las pendientes a lo largo de la curva gamma menos
ordenadamenos=[]
for i in range(0,len(logfitxmen)-n):
 xdata=[]
 ydata=[]
 for j in range(0,n-1):
  xdata.append(logfitxmen[i+j])
  ydata.append(logfitymen[i+j])
 parmfit=(curve_fit(func,xdata,ydata))
 gammamenos.append(parmfit[0][0])
 ordenadamenos.append(parmfit[0][1])

#me muevo sobre las curvas (xmen,ymen) y (xmas,ymas), y elijo realizar ajustes tomando n puntos
n=4#numero de puntos que tomo para fit
gammamas=[]#tiene las pendientes a lo largo de la curva gamma menos
ordenadamas=[]
for i in range(0,len(logfitxmas)-n):
 xdata=[]
 ydata=[]
 parmfit=[]
 for j in range(0,n-1):
  xdata.append(logfitxmas[i+j])
  ydata.append(logfitymas[i+j])
 parmfit=(curve_fit(func,xdata,ydata))
 #print parmfit[0][0]
 gammamas.append(parmfit[0][0]) 
 ordenadamas.append(parmfit[0][1])

#busco ahora gammas coincidentes en mas menos 0.01
#comparo todos con todos:
margen=0.001
matchmenos=[]#aca guardo las dos pendientes que coinciden y sus ordenadas
matchmas=[]
for i in range(0,len(gammamas)):
 for j in range(0,len(gammamenos)):
  # me fijo cual es menor
  if (abs(gammamas[i]-gammamenos[j])<margen):
    print "Encontro el siguiente match"  
    print (gammamas[i],gammamenos[j])
    matchmenos=[gammamenos[j],ordenadamenos[j]]
    matchmas=[gammamas[i],ordenadamas[i]]    


#Grafico 1#gammamas
plt.figure(0)
for i in range(0,len(gammamas)):#numero de fits que tengo
 xfit=np.zeros(len(logfitxmas))
 yfit=np.zeros(len(logfitxmas))
 for j in range(0,len(logfitxmas)):
  parmfit=[gammamas[i],ordenadamas[i]]
  xfit[j]=logfitxmas[j]
  yfit[j]=(func(logfitxmas[j],*parmfit))
 plt.plot(xfit,yfit,'b-')
plt.plot(logfitxmas,logfitymas,'bo',label=r'$\gamma+$')
plt.xlabel(r'$log |p-p_{c}|$')
plt.ylabel("log M2")
plt.legend()

#Grafico 2#gammamenos
plt.figure(1)
for i in range(0,len(gammamenos)):#numero de fits que tengo
 xfit=np.zeros(len(logfitxmen))
 yfit=np.zeros(len(logfitxmen))
 for j in range(0,len(logfitxmen)):
  parmfit=[gammamenos[i],ordenadamenos[i]]
  xfit[j]=logfitxmen[j]
  yfit[j]=(func(logfitxmen[j],*parmfit))
 plt.plot(xfit,yfit,'r-')
plt.plot(logfitxmen,logfitymen,'ro',label=r'$\gamma-$')
plt.xlabel(r'$log |p-p_{c}|$')
plt.ylabel("log M2")
plt.legend()



#Grafico 3:
plt.figure(2)
#gammamenos
xfit=np.zeros(len(logfitxmen))
yfit=np.zeros(len(logfitxmen))
parmfit=[]
for j in range(0,len(logfitxmen)):
  parmfit=[matchmenos[0],matchmenos[1]]
  xfit[j]=logfitxmen[j]
  yfit[j]=(func(logfitxmen[j],*parmfit))
plt.plot(xfit,yfit,'r-')
plt.plot(logfitxmen,logfitymen,'ro',label=r'$\gamma-$')
plt.xlabel(r'$log |p-p_{c}|$',size=15)
plt.ylabel(r'$log M2$',size=15)
plt.legend()

#gammamas
xfit=np.zeros(len(logfitxmas))
yfit=np.zeros(len(logfitxmas))
parmfit=[]
for j in range(0,len(logfitxmas)):
  parmfit=[matchmas[0],matchmas[1]]
  xfit[j]=logfitxmas[j]
  yfit[j]=(func(logfitxmas[j],*parmfit))
plt.plot(xfit,yfit,'b-')
plt.plot(logfitxmas,logfitymas,'bo',label=r'$\gamma+$')

plt.ylim(-5,12)
plt.xlim(-5,0)
plt.legend()
plt.xlabel(r'$log |p-p_{c}|$')
plt.ylabel(r'$log\ M_{2}$')
plt.text(-2.8,10.5, "$L=128$", fontsize=25, bbox=dict(facecolor='w', alpha=0.5))
plt.text(-2.5,4.5, "$\gamma_{-}=-2.643$", fontsize=15, bbox=dict(facecolor='r', alpha=0.5))
plt.text(-4.4,2, "$\gamma_{+}=-2.644$", fontsize=15, bbox=dict(facecolor='b', alpha=0.5))
plt.legend()
plt.show()
'''

