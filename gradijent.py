from PIL import Image
import pylab
from pylab import *
from scipy.ndimage import filters
from numpy import *
from matplotlib.patches import Ellipse

def grad(im,sigma=1,prozor=5):
   
    Ix = zeros(im.shape)
    filters.gaussian_filter(im, (sigma,sigma), (0,1), Ix)
    Iy = zeros(im.shape)
    filters.gaussian_filter(im, (sigma,sigma), (1,0), Iy)

    #koordinate oko kojih ce se gledati gradijent
    imshow(im)
    koord = ginput(1)
    x = round(koord[0][1])
    y = round(koord[0][0])
    
    #uzmi gradijente Ix i Iy unutar tog prozora
    IxF = (Ix[x-prozor:x+prozor+1,y-prozor:y+prozor+1]).flatten()
    IyF = (Iy[x-prozor:x+prozor+1,y-prozor:y+prozor+1]).flatten()

    xS=yS=0
    IxF1=[]
    IyF1=[]

    xS=sum(IxF)/len(IxF)
    #x koordinata centra
    yS=sum(IyF)/len(IyF)   
    #y koordinata centra

    #pomicanje tocaka prema ishodistu
    for i in range(0,len(IxF)):
        IxF1.append(IxF[i]-xS)
        IyF1.append(IyF[i]-yS)

    #stvaranje matrice sa X i Y vrijednostima gradijenata
    X1=[]
    X1.append(IxF1)
    X1.append(IyF1)
    X1=array(X1)
    X1T=X1.T


    #kovarijacijska matrica
    S=(1./(len(IxF)-1))*dot(X1,X1T)

    #eigenvalues
    eva1=0.5*(S[0,0]+S[1,1]+sqrt(pow((S[0,0]-S[1,1]),2)+4*pow(S[0,1],2)))
    eva2=0.5*(S[0,0]+S[1,1]-sqrt(pow((S[0,0]-S[1,1]),2)+4*pow(S[0,1],2)))

    #eigenvectori
    if (pow((eva1-S[0,0]),2)+pow(S[0,1],2))!=0:
        eve1=S[0,1]/sqrt(pow((eva1-S[0,0]),2)+pow(S[0,1],2))
        eve2=(eva1-S[0,0])/sqrt(pow((eva1-S[0,0]),2)+pow(S[0,1],2))

    #dobivanje kuta
        tang=eve2/eve1
        kut=arctan(tang)*180./3.14

    #ako slucajno nazivnik ispadne 0
    else:
        eve1=0
        eve2=1
        kut=90  

    ells = Ellipse((xS,yS),4*sqrt(eva1),4*sqrt(eva2),kut)
    ells.set_alpha(0.3) 
  
    fig = figure()
    ax = fig.add_subplot(111,aspect='equal')
    ax.add_artist(ells)
    ax.plot(IxF,IyF,'mo') #crtaj histogram

    #crtaj eigenvectore
    pomx = 2*sqrt(eva1)*eve1
    pomy = 2*sqrt(eva1)*eve2
    ax.arrow(xS,yS,pomx,pomy,width=0.008,length_includes_head='true',color='k')
    
    pomx = 2*sqrt(eva2)*eve2
    pomy = 2*sqrt(eva2)*eve1*(-1)
    ax.arrow(xS,yS,pomx,pomy,width=0.008,length_includes_head='true',color='k')


    #crtanje koordinatnih osi
    D = 1.1*min(IyF)
    G = 1.1*max(IyF)
    if(D > -1):
        D = -1
    if(G < 1):
        G = 1
    ax.plot([0,0],[D,G],'r') # y-os
    ax.plot([0,0.02],[G,G*0.96],'r')
    ax.plot([0,-0.02],[G,G*0.96],'r')
    ax.text(0.04, G, 'Iy', style='italic',
        bbox={'facecolor':'gray', 'alpha':0, 'pad':10})
    

    D = 1.1*min(IxF)
    G = 1.1*max(IxF)
    if(D > -1):
        D = -1
    if(G < 1):
        G = 1 
    ax.plot([D,G],[0,0],'r') # x-os
    ax.plot([G,G*0.96],[0,0.02],'r')
    ax.plot([G,G*0.96],[0,-0.02],'r')
    ax.text(G, 0.04, 'Ix', style='italic',
        bbox={'facecolor':'gray', 'alpha':0, 'pad':10})
    
    ax.set_xlabel('Ix')
    ax.set_ylabel('Iy')
    ax.set_title('Histogram')


    gray()
    grid('on')
    show()

    return
