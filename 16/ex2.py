import numpy as np
import cv2
import main
import matplotlib.pyplot as plt
import os

os.makedirs("images",exist_ok=True)

HEIGHT = 512
WIDTH = 512
POSI_X = 2560.0
POSI_Y = 2560.0
DX = 10.0
DIAM = 80.0
PEAK_BRIGHT = 127
WAVE_LEN = 0.6328
MIN_Z = 0.0
MAX_Z = 20000.0
STEP_Z = 10.0
N = int((MAX_Z-MIN_Z)/STEP_Z)

input_hologram = cv2.imread("holography.bmp",0)
hologram = input_hologram + 0.j

particle_intensity = np.zeros(N)
z_x_image = np.zeros([WIDTH,N])

posi_z = np.arange(MIN_Z,MAX_Z,STEP_Z)
for i in range(N):
    trans = main.trans_func_gen(HEIGHT,WIDTH,DX,i*STEP_Z+MIN_Z,WAVE_LEN)
    hologram_FFT = np.fft.fft2(hologram)
    hologram_FFT = np.fft.fftshift(hologram_FFT)
    object = hologram_FFT*trans
    object = np.fft.fftshift(object)
    object = np.fft.ifft2(object)
    particle_intensity[i] = np.abs(object[int(POSI_Y/DX)][int(POSI_X/DX)])
    object = np.abs(object)
    cv2.imwrite("./images/image"+str(i)+".bmp",object)
    z_x_image[:,i] = object[int(POSI_X/DX),:]
    print(i,"th /",N," is processed.")

cv2.imwrite("z_x_image.png",z_x_image)

num = np.arange(N)
fig = plt.figure()
plt.plot(num,particle_intensity,label="Particle Light Intensity")
plt.legend()
fig.savefig("plot.png")

