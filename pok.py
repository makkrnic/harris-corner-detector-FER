from pylab import *
from pylab import Figure
from numpy import *
from matplotlib.patches import Ellipse

def grad(im,Ix,Iy,prozor=5):

    #koordinate oko kojih ce se gledati gradijent
    imshow(im)
    koord = ginput(1)
    x = round(koord[0][0])
    y = round(koord[0][1])
    
    #uzmi gradijente Ix i Iy unutar tog prozora
    IxF = (Ix[x-prozor:x+prozor+1,y-prozor:y+prozor+1]).flatten()
    IyF = (Iy[x-prozor:x+prozor+1,y-prozor:y+prozor+1]).flatten()

    #neka kvazi matematika...
    width = abs(max(IxF)) + abs(min(IxF))
    height = abs(max(IyF)) + abs(min(IyF))
    xS = (max(IxF) + min(IxF))/2
    yS = (max(IyF) + min(IyF))/2

    """x_segment = width/15.0
    y_segment = width/15.0
    broj_pojava = zeros((15,15),dtype='int')

    #procjeni gdje je najgusce
    for i in range(15):
        for j in range(15):
            for k in IxF:
                for m in IyF:
                    if IxF[k] >= i*x_segment and IxF[k] <= (i+1)*x_segment:
                        if IyF[m] >= j*y_segment and IyF[m] <= (j+1)*y_segment:
                            broj_pojava[i][j] += 1

    x_pom,y_pom = unravel_index(broj_pojava.argmax(), broj_pojava.shape)

    x = (x_pom*x_segment + (x_pom+1)*x_segment)/2
    y = (y_pom*y_segment + (y_pom+1)*y_segment)/2

    angle = arctan(y*1.0/x*1.0)*180/pi"""
            
    
    
    mod = sqrt(IxF**2 + IyF**2)
    pom = argmax(mod)
    angle = arctan(IyF[pom]*1.0/IxF[pom]*1.0)*180/pi
    if width < max(mod):
        width = max(mod)
    
    ells = Ellipse((xS,yS),width,height,angle)
    ells.set_alpha(0.3)


    
    fig = figure()
    ax = fig.add_subplot(111,aspect='equal')
    ax.add_artist(ells)
    ax.plot(IxF,IyF,'mo') #crtaj histogram
    gray()
    grid('on')
    show()
    return 
