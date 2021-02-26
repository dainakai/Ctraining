/******************************************************************************
PROGRAM NAME : PIV Image Generating (variable trials, with OpenMP)
AUTHER : Dai Nakai
DATE : 2021/1/29
******************************************************************************/
#include<bits/stdc++.h>
#include<sys/stat.h>
#include<omp.h>
using namespace std;

const char* inputimage = "image.bmp";
const char* outputheader = "./result";
char outputpath[100];

const double avrdia = 70.0; // Average diameter
const double sddia = 20.0; // Standard deviation of diameters
const int N = 1000;
const double dt = 30;
const int trials = 2;

const int width = 1024;
const int height = 1024;
unsigned char header_buf [1078];
unsigned char image_out[height][width];

double a[N],b[N];
double u[N],v[N];
double radius[N]; // Each particle diameter
int particle_x[trials][N],particle_y[trials][N];
double Z; // Standard Normal Distribution
double bright[trials][height][width];

int maxb[N];

FILE* fp;
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

    particle_x[0][i] = rand() % 1024 ;
    particle_y[0][i] = rand() % 1024 ;
    maxb[i]= (rand() % 128)+128;
  }
   
  for (int i = 1; i < trials; i++){
    for (int j = 0; j < N; j++){
      u[j] = cos (2.0* M_PI/ (width-1) *particle_x[i-1][j])* sin (2.0* M_PI/ (height-1) *particle_y[i-1][j]);
      v[j] = -1.0* sin(2.0* M_PI/ (width-1) *particle_x[i-1][j])* cos (2.0* M_PI/ (height-1) *particle_y[i-1][j]);

      particle_x[i][j] = particle_x[i-1][j] + u[j]*dt;
      particle_y[i][j] = particle_y[i-1][j] + v[j]*dt;
    }
  }
  
  double max[trials],min[trials];

  #pragma omp parallel for
  for(int c=0; c < trials ; c++){

    for (int j = 0; j < height; j++){
      for (int k = 0; k < width; k++){
        bright[c][j][k]=0.0;
      }
    }
  
    for (int i = 0; i < N; i++){
      for (int j = 0; j < height; j++){
        for (int k = 0; k < width; k++){
          bright[c][j][k] += maxb[i]*exp((double) -1.0*((k-particle_x[c][i])*(k-particle_x[c][i])+(j-particle_y[c][i])*(j-particle_y[c][i]))/(2.0*radius[i]*radius[i]/100.0) );
        }
      }
    }

    max[c]=0; min[c]=255.5;
    for (int j = 0; j < height; j++){
      for (int k = 0; k < width; k++){
        if(bright[c][j][k] < min[c]){
          min[c] = bright[c][j][k];
        }
        if(bright[c][j][k]>max[c]){
          max[c] = bright[c][j][k];
        }
      }
    }

    for (int j = 0; j < height; j++){
      for (int k = 0; k < width; k++){
        bright[c][j][k] = (bright[c][j][k]-min[c])/(max[c]-min[c])*255;
      }
    }

  }
  
  #pragma omp critical
  for (int c = 0; c < trials; c++){
    sprintf(outputpath,"%s/%05d.bmp",outputheader,c);
    fp = fopen(outputpath, "wb");
    fwrite(header_buf, sizeof(unsigned char), 1078, fp);// Write Header
    for (int j = 0; j < height; j++){
      for (int k = 0; k < width; k++){
        image_out[j][k] = bright[c][j][k];
      }
    }
    fwrite(image_out, sizeof(image_out), 1, fp);// Write 8bit image intensity
    fclose (fp);
  }

  return 0;
}