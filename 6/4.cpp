/******************************************************************************
PROGRAM NAME : PIV Image Generating (trial = 1)
AUTHER : Dai Nakai
DATE : 2021/1/22
******************************************************************************/
#include<bits/stdc++.h>
using namespace std;

const char* inputimage = "image.bmp";
const char* outputimage = "result4.bmp";

const int width = 1024;
const int height = 1024;
unsigned char header_buf[1078];
unsigned char image_out[height][width];

const double avrdia = 70.0; // Average diameter
const double sddia = 20.0; // Standard deviation of diameters
const int N = 1000;

double a[N],b[N];
double radius[N]; // Each particle diameter
int particle_x[N],particle_y[N]; // Particle placement
double Z; // Standard Normal Distribution
double bright[height][width]; // Brightness of particles
int maxb[N]; // Random peak brightness
FILE * fp;
/*********************************main****************************************/
int main (){
  srand((unsigned int)time(NULL)); // Initializing rand() with the current time

    for (int i = 0; i < N; i++){ // Two independent random arrays
    a[i]=(double)rand()/RAND_MAX;
    b[i]=(double)rand()/RAND_MAX;

    Z = sqrt(-2.0*log(a[i]))*cos((double)2.0*M_PI*b[i]);
    radius[i] = sddia*Z+avrdia;

    particle_x[i] = rand() % 1024 ;
    particle_y[i] = rand() % 1024 ;
    maxb[i]= (rand() % 128)+128;
  }
  

  for (int j = 0; j < height; j++){
    for (int k = 0; k < width; k++){
      bright[j][k]=0.0;
    }
  }
  
  for (int i = 0; i < N; i++){
    for (int j = 0; j < height; j++){
      for (int k = 0; k < width; k++){
        bright[j][k] += maxb[i]*exp((double) -1.0*((k-particle_x[i])*(k-particle_x[i])+(j-particle_y[i])*(j-particle_y[i]))/(2.0*radius[i]*radius[i]/100.0) );
      }
    }
  }

  double min=255.0,max=0.0;
  for (int j = 0; j < height; j++){
    for (int k = 0; k < width; k++){
      if(bright[j][k] < min){
        min = bright[j][k];
      }
      if(bright[j][k]>max){
        max = bright[j][k];
      }
    }
  }

  for (int j = 0; j < height; j++){
    for (int k = 0; k < width; k++){
      image_out[j][k] = (bright[j][k]-min)/(max-min)*255;
    }
  }

  // Read image data
  fp = fopen(inputimage, "rb");
  fread(header_buf, sizeof(unsigned char), 1078, fp);// Read Header
  fclose (fp);

  fp = fopen(outputimage, "wb");
  fwrite(header_buf, sizeof(unsigned char), 1078, fp);// Write Header
  fwrite(image_out, sizeof(image_out), 1, fp);// Write 8bit image intensity
  fclose (fp);

  return 0;
}