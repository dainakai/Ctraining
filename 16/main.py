import numpy as np
import cv2

def trans_func_gen (height, width, dx, posi_z,wave_len):
    z = np.full([height, width], 0.+0.j)

    for i in range(height):
        for k in range(width):
            z[i][k] = np.exp(2.0j*np.pi*posi_z/wave_len*np.sqrt(1.0-((k - width/2)*wave_len/width/dx)**2 - ((i - height/2)*wave_len/height/dx)**2))

    return z

def object_plane_gen (height,width,dx,posi_x,posi_y,diam):
    z = np.full([height,width],0.+0.j)
    for i in range(height):
        for k in range(width):
            if (i*dx - posi_y)**2 + (k*dx - posi_x)**2 > (diam/2)**2 :
                z[i][k] = 1.0+0.j

    return z 

def main():
    HEIGHT = 512
    WIDTH = 512
    POSI_X = 2560.0
    POSI_Y = 2560.0
    DX = 10.0
    DIAM = 80.0
    PEAK_BRIGHT = 127
    WAVE_LEN = 0.6328

    object = object_plane_gen(HEIGHT,WIDTH,DX,POSI_X,POSI_Y,DIAM)

    cv2.imwrite("objectplane.bmp",PEAK_BRIGHT*np.abs(object))

    trans1 = trans_func_gen(HEIGHT,WIDTH,DX,10000.0,WAVE_LEN) # z = 10 mm transfer function

    object_FFT = np.fft.fft2(object)
    object_FFT = np.fft.fftshift(object_FFT)

    hologram = object_FFT*trans1

    hologram = np.fft.fftshift(hologram)
    hologram = np.fft.ifft2(hologram)

    pw = np.abs(hologram)
    cv2.imwrite("holography.bmp",PEAK_BRIGHT*pw)

if __name__ == "__main__": 
    main()