/******************************************************************************
PROGRAM NAME : PIV Image Generating (trials = 2)
AUTHER : Dai Nakai
DATE : 2021/1/28
******************************************************************************/
#include<bits/stdc++.h>
#include<sys/stat.h>
using namespace std;

const char* inputimage = "image.bmp";
const char* outputheader = "./result";
char outputpath[100];

const int width = 1024;
const int height = 1024;
unsigned char header_buf [1078];
unsigned char image_out [height][width];

const double avrdia = 70.0; // Average diameter
const double sddia = 20.0; // Standard deviation of diameters
const int N = 1000;
const double dt = 10;
const int trials = 2;

double a[N],b[N];
double u[N],v[N];
double radius[N]; // Each particle diameter
int particle_x[N],particle_y[N];
double Z; // Standard Normal Distribution
double bright[height][width];
int maxb[N];

FILE * fp;
/*********************************main****************************************/
int main (){
  srand((unsigned int)time(NULL)); // Initializing rand() with the current time
  mkdir(outputheader,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
  
  // Read image data
  fp = fopen(inputimage, "rb");
  fread(header_buf, sizeof(unsigned char), 1078, fp);// Read Header
  fclose (fp);


  for (int i = 0; i < N; i++){ // Two independent random arrays
    a[i]=(double)rand()/RAND_MAX;
    b[i]=(double)rand()/RAND_MAX;

    Z = sqrt(-2.0*log(a[i]))*cos((double)2.0*M_PI*b[i]);
    radius[i] = sddia*Z+avrdia;

    particle_x[i] = rand() % 1024 ;
    particle_y[i] = rand() % 1024 ;
    maxb[i]= (rand() % 128)+128;
  }
   

  for(int c=0; c < trials ; c++){

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

    sprintf(outputpath,"%s/%05d.bmp",outputheader,c);
    fp = fopen(outputpath, "wb");
    fwrite(header_buf, sizeof(unsigned char), 1078, fp);// Write Header
    fwrite(image_out, sizeof(image_out), 1, fp);// Write 8bit image intensity
    fclose (fp);
    
    // Preparing for the next times
    for (int i = 0; i < N; i++){
      u[i] = cos (2.0* M_PI/ (width-1) *particle_x[i])* sin (2.0* M_PI/ (height-1) *particle_y[i]);
      v[i] = -1.0* sin(2.0* M_PI/ (width-1) *particle_x[i])* cos (2.0* M_PI/ (height-1) *particle_y[i]);
    }

    for (int i = 0; i < N; i++){
      particle_x[i] += u[i]*dt;
      particle_y[i] += v[i]*dt;
    }

  }
  return 0;
}