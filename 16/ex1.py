import numpy as np
import cv2
import main

HEIGHT = 512
WIDTH = 512
POSI_X = 2560.0
POSI_Y = 2560.0
DX = 10.0
DIAM = 80.0
PEAK_BRIGHT = 127
WAVE_LEN = 0.6328
IMAGE1 = "object1.bmp"
IMAGE2 = "object2.bmp"
IMAGE3 = "object3.bmp"

input_hologram = cv2.imread("holography.bmp",0)
hologram = input_hologram + 0.j

trans1 = main.trans_func_gen(HEIGHT,WIDTH,DX,0.0,WAVE_LEN)
trans2 = main.trans_func_gen(HEIGHT,WIDTH,DX,10000.0,WAVE_LEN)
trans3 = main.trans_func_gen(HEIGHT,WIDTH,DX,20000.0,WAVE_LEN)

hologram_FFT = np.fft.fft2(hologram)
hologram_FFT = np.fft.fftshift(hologram_FFT)

object1 = hologram_FFT*trans1
object2 = hologram_FFT*trans2
object3 = hologram_FFT*trans3

object1 = np.fft.fftshift(object1)
object2 = np.fft.fftshift(object2)
object3 = np.fft.fftshift(object3)

object1 = np.fft.ifft2(object1)
object2 = np.fft.ifft2(object2)
object3 = np.fft.ifft2(object3)

object1 = np.abs(object1)
object2 = np.abs(object2)
object3 = np.abs(object3)

cv2.imwrite(IMAGE1,object1)
cv2.imwrite(IMAGE2,object2)
cv2.imwrite(IMAGE3,object3)
