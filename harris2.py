#!/usr/bin/env python
# -*- coding: utf-8 -*-

from PIL import Image
from pylab import *
from scipy.ndimage import filters

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

    #    figure()
    #    gray()
    #    imshow(Wxx)
    #    figure()
    #    gray()
    #    imshow(Wxy)
    #    figure()
    #    gray()
    #    imshow(Wyy)
    
    # determinant and trace
    Wdet = Wxx*Wyy - Wxy**2
    Wtr = Wxx + Wyy

    #    figure()
    #    gray()
    #    imshow(Wdet/Wtr)

    return Wdet / Wtr

def get_harris_points(harrisim,min_dist=10,threshold=0.1):
    """ Return corners from a Harris response image
    min_dist is the minimum number of pixels separating
    corners and image boundary. """

    # find top corner candidates above a threshold
    corner_threshold = harrisim.max() * threshold
    # postavlja vrijednosti veće od corner_threshold na 1, ostale na 0
    harrisim_t = (harrisim > corner_threshold) * 1

    # get coordinates of candidates
    coords = array(harrisim_t.nonzero()).T

    # ...and their values
    candidate_values = [harrisim[c[0],c[1]] for c in coords]

    # sort candidates
    index = argsort(candidate_values)

    # store allowed point locations in array
    allowed_locations = zeros(harrisim.shape)
    # postavlja 1 na sve lokacije udaljene barem min_dist od ruba matrice
    allowed_locations[min_dist:-min_dist,min_dist:-min_dist] = 1

    # select the best points taking min_distance into account
    filtered_coords = []
    for i in index:
        if allowed_locations[coords[i,0],coords[i,1]] == 1:
            filtered_coords.append(coords[i])
            # za svaku novu pronađenu točku, postavlja kvadrat stranice 2*min_dist oko točke na 0
            allowed_locations[(coords[i,0]-min_dist):(coords[i,0]+min_dist),
                              (coords[i,1]-min_dist):(coords[i,1]+min_dist)] = 0
    return filtered_coords

def plot_harris_points(image,filtered_coords):
    """ Plots corners found in image. """

    figure()
    gray()
    imshow(image)
    plot([p[1] for p in filtered_coords],[p[0] for p in filtered_coords],'ro')
    axis('off')
    show()

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print 'You didn\'t supply a picture! Exiting...'
        exit(-1)
    im = array(Image.open(sys.argv[1]).convert('L'))
    harrisim = compute_harris_response(im)
    filtered_coords = get_harris_points(harrisim,10, 0.01)
    plot_harris_points(im, filtered_coords)
