#!/usr/bin/env python2

from pylab import *
from numpy import *
from PIL import Image
from scipy.ndimage import filters
import sys


def compute_harris_response(im,sigma=3):
    """ Compute the Harris corner detector response function 
        for each pixel in a graylevel image. """
    
    # derivatives
    imx = zeros(im.shape)
    filters.gaussian_filter(im, (sigma,sigma), (0,1), imx)
    imy = zeros(im.shape)
    filters.gaussian_filter(im, (sigma,sigma), (1,0), imy)
    
    # compute components of the Harris matrix
    Wxx = filters.gaussian_filter(imx*imx,sigma)
    Wxy = filters.gaussian_filter(imx*imy,sigma)
    Wyy = filters.gaussian_filter(imy*imy,sigma)
    
    # determinant and trace
    Wdet = Wxx*Wyy - Wxy**2
    Wtr = Wxx + Wyy
    
    return Wdet/Wtr,imx,imy
   
    
def get_harris_points(harrisim,min_dist=10,threshold=0.1):
    """ Return corners from a Harris response image
        min_dist is the minimum number of pixels separating 
        corners and image boundary. """
    
    # find top corner candidates above a threshold
    corner_threshold = harrisim.max() * threshold
    harrisim_t = (harrisim > corner_threshold) * 1
    
    # get coordinates of candidates
    coords = array(harrisim_t.nonzero()).T
    
    # ...and their values
    candidate_values = [harrisim[c[0],c[1]] for c in coords]
    
    # sort candidates
    index = argsort(candidate_values)
    
    # store allowed point locations in array
    allowed_locations = zeros(harrisim.shape)
    allowed_locations[min_dist:-min_dist,min_dist:-min_dist] = 1
    
    # select the best points taking min_distance into account
    filtered_coords = []
    for i in index:
        if allowed_locations[coords[i,0],coords[i,1]] == 1:
            filtered_coords.append(coords[i])
            allowed_locations[(coords[i,0]-min_dist):(coords[i,0]+min_dist), 
                        (coords[i,1]-min_dist):(coords[i,1]+min_dist)] = 0
    
    return filtered_coords
    
    
def plot_harris_points(image,filtered_coords):
    """ Plots corners found in image. """
    
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
    """ Show a figure with lines joining the accepted matches 
        input: im1,im2 (images as arrays), locs1,locs2 (feature locations), 
        matchscores (as output from 'match()'), 
        show_below (if images should be shown below matches). """
    
    im3 = appendimages(im1,im2)

    im3 = vstack((im3,im3)) #prikazi i originalnu sliku bez spojnih linija
    
    imshow(im3)
    
    cols1 = im1.shape[1]
    for i,m in enumerate(matchscores):
        if m>0:
            plot([locs1[i][1],locs2[m][1]+cols1],[locs1[i][0],locs2[m][0]],'c')
    axis('off')

def upari(im1,im2,sigma,wid):

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

if __name__ == '__main__':
  im = array(Image.open(sys.argv[1]).convert('L'))
  (harrisim,_,_) = compute_harris_response(im)
  
  
  dbgfile = open('harrisResponseDebugPython', 'w+')
  for r in harrisim:
    for e in r:
      #print e
      dbgfile.write (str(e) + '\n')
  
  dbgfile.close()


  filtered = get_harris_points (harrisim, 6)
  plot_harris_points (im, filtered)
