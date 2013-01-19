import sys
from PIL import Image
from pylab import *
from scipy.ndimage import filters
from numpy import *
from matplotlib.patches import Ellipse
import harris

if __name__ == "__main__":
    if sys.argv[1] == "kutevi":
        if len(sys.argv) != 3:
           print 'Niste prilozili ispravan broj argumenata'
           exit(-1)
        im = array(Image.open(sys.argv[2]).convert('L'))
        harrisim = harris.compute_harris_response(im)
        filtered_coords = harris.get_harris_points(harrisim, 10, 0.01)
        harris.plot_harris_points(im, filtered_coords)

    if sys.argv[1] == "gradijenti":
        if (len(sys.argv) != 3) and (len(sys.argv) != 4) and (len(sys.argv) != 5):
           print 'Niste prilozili ispravan broj argumenata'
           exit(-1)
        im = array(Image.open(sys.argv[2]).convert('L'))   
        if len(sys.argv)==3:
            harris.grad(im)
        if len(sys.argv)==4:
            harris.grad(im,sys.argv[3])
        if len(sys.argv)==5:
            harris.grad(im,sys.argv[3],sys.argv[4])

    if sys.argv[1] == "uparivanje":
        if (len(sys.argv) != 4) and (len(sys.argv) != 5) and (len(sys.argv) != 6):
           print 'Niste prilozili ispravan broj argumenata'
           exit(-1)
        im1 = array(Image.open(sys.argv[2]).convert('L'))
        im2 = array(Image.open(sys.argv[3]).convert('L'))       
        if len(sys.argv)==4:
            harris.upari(im1,im2)
        if len(sys.argv)==5:
            harris.upari(im1,im2,sys.argv[4])
        if len(sys.argv)==6:
            harris.upari(im1,im2,sys.argv[4],sys.argv[5])
