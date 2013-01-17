from PIL import Image
from pylab import *
from scipy.ndimage import filters
from numpy import *
from matplotlib.patches import Ellipse


def compute_harris_response(im,sigma=3):
    """ Izracunaj odziv Harrisove funkcije za detekciju kuteva. """
    
    # derivati
    imx = zeros(im.shape)
    filters.gaussian_filter(im, (sigma,sigma), (0,1), imx)
    imy = zeros(im.shape)
    filters.gaussian_filter(im, (sigma,sigma), (1,0), imy)
    
    # izracunaj komponente Harrisove matrice
    Wxx = filters.gaussian_filter(imx*imx,sigma)
    Wxy = filters.gaussian_filter(imx*imy,sigma)
    Wyy = filters.gaussian_filter(imy*imy,sigma)
    
    # determinanta i trag matrice
    Wdet = Wxx*Wyy - Wxy**2
    Wtr = Wxx + Wyy
    
    return Wdet/Wtr,imx,imy
   
    
def get_harris_points(harrisim,min_dist=10,threshold=0.1):
    """ Odaberi tocke koje ce biti proglasene kutevima."""
    
    # izbaci one tocke cija vrijednost funkcije nije iznad granicne
    corner_threshold = harrisim.max() * threshold
    harrisim_t = (harrisim > corner_threshold) * 1
    
    # koordinate kandidate za kuteve
    coords = array(harrisim_t.nonzero()).T
    
    # njihove vrijednosti
    candidate_values = [harrisim[c[0],c[1]] for c in coords]
    
    index = argsort(candidate_values)
    
    # pomocna matrica koja oznacuje mjesta na kojima je dozvoljen kut
    allowed_locations = zeros(harrisim.shape)
    allowed_locations[min_dist:-min_dist,min_dist:-min_dist] = 1
    
    # izaberi kuteve uzimajuci u obzir minimalnu udaljenost izmedju 2 kuta
    filtered_coords = []
    for i in index:
        if allowed_locations[coords[i,0],coords[i,1]] == 1:
            filtered_coords.append(coords[i])
            allowed_locations[(coords[i,0]-min_dist):(coords[i,0]+min_dist), 
                        (coords[i,1]-min_dist):(coords[i,1]+min_dist)] = 0
    
    return filtered_coords
    
    
def plot_harris_points(image,filtered_coords):
    """ Oznaci kuteve na slici. """
    
    figure()
    gray()
    imshow(image)
    plot([p[1] for p in filtered_coords],
                [p[0] for p in filtered_coords],'*')
    axis('off')
    show()
    

def get_descriptors(image,filtered_coords,wid=5):
    """Za svaku tocku u kojoj je kut vrati vrijednosti piksela oko te tocke 
       u kvadratnom prozoru sirine 2*wid+1"""
    
    desc = []
    for coords in filtered_coords:
        patch = image[coords[0]-wid:coords[0]+wid+1,
                            coords[1]-wid:coords[1]+wid+1].flatten()
        desc.append(patch)
    # za svaku tocku vraca se vrijednost okolnih piksela u 1D polju
    return desc


def match(desc1,desc2,threshold=0.5):
    """Koristeci normaliziranu kriznu korelaciju usporedi opisnike
       kuteva prve slike s opisnicima kuteva druge slike i pronadji
       poklapanja"""
    
    n = len(desc1[0])
    
    # pair-wise distances
    d = -ones((len(desc1),len(desc2))) #negacija zbog kasnijeg sortiranja
    for i in range(len(desc1)):
        for j in range(len(desc2)):
            d1 = (desc1[i] - mean(desc1[i])) / std(desc1[i])
            d2 = (desc2[j] - mean(desc2[j])) / std(desc2[j])
            ncc_value = sum(d1 * d2) / (n-1) 
            if ncc_value > threshold:
                d[i,j] = ncc_value
            
    ndx = argsort(-d) #na prvo mjesto u retku dolazi mjesto najveceg poklapanja
    matchscores = ndx[:,0] #filtriramo samo prvi stupac 
                           #jer su tamo najvece vrijednosti
    return matchscores


def match_twosided(desc1,desc2,threshold=0.5):
    """ Dvostrano simetricno poklapanje"""
    
    matches_12 = match(desc1,desc2,threshold)
    matches_21 = match(desc2,desc1,threshold)
    
    ndx_12 = where(matches_12 >= 0)[0] #valjane koordinate
    
    # ukloni poklapanja kuteva ukoliko nisu simetricna
    for n in ndx_12:
        if matches_21[matches_12[n]] != n:
            matches_12[n] = -1
    
    return matches_12


def appendimages(im1,im2):
    """ Vrati novu sliku sastavljenu od dvije ulazne spojene bocno """
    
    # posto ih spajamo bocno slike trebaju imati jednak broj redaka
    # ukoliko jedna ima manje redaka od druge nju nadopunjujemo s redcima nula
    rows1 = im1.shape[0]    
    rows2 = im2.shape[0]
    
    if rows1 < rows2:
        im1 = concatenate((im1,zeros((rows2-rows1,im1.shape[1]))),axis=0)
    elif rows1 > rows2:
        im2 = concatenate((im2,zeros((rows1-rows2,im2.shape[1]))),axis=0)
    # ukoliko imaju jednak broj redaka, samo se spajaju
    
    return concatenate((im1,im2), axis=1)
    
    
def plot_matches(im1,im2,locs1,locs2,matchscores):
    """ Povezi tocke koje su uparene. """
    
    im3 = appendimages(im1,im2)

    im3 = vstack((im3,im3)) #prikazi i originalnu sliku bez spojnih linija
    
    imshow(im3)
    
    cols1 = im1.shape[1]
    for i,m in enumerate(matchscores):
        if m>0:
            plot([locs1[i][1],locs2[m][1]+cols1],[locs1[i][0],locs2[m][0]],'c')
    axis('off')

def upari(im1,im2,sigma=1,wid=5):

    harrisim = compute_harris_response(im1,sigma)
    filtered_coords1 = get_harris_points(harrisim[0],wid+1)
    d1 = get_descriptors(im1,filtered_coords1,wid)
    harrisim = compute_harris_response(im2,sigma)
    filtered_coords2 = get_harris_points(harrisim[0],wid+1)
    d2 = get_descriptors(im2,filtered_coords2,wid)
    matches = match_twosided(d1,d2)
    figure()
    gray()
    plot_matches(im1,im2,filtered_coords1,filtered_coords2,matches)
    show()

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
    XY=[]
    XY.append(IxF1)
    XY.append(IyF1)
    XY=array(XY)
    XYT=XY.T


    #kovarijacijska matrica
    S=(1./(len(IxF)-1))*dot(XY,XYT)

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
        poz=IxF[0]
        pog=0
        for i in range(0,len(IxF)):
            if poz!=IxF[i]:
                pog=1
                break
        if pog==0:
            eve1=0
            eve2=1
            kut=90
            
        else:
            eve1=1
            eve2=0
            kut=0    
 
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
