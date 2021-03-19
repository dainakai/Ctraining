/******************************************************************************
PROGRAM NAME : PIV Image Generating Software
AUTHER : Dai Nakai
DATE : 2021/2/22

Particle image producing software.
Images are originally gained with 1024 pixels square, then
trimmed at their center 512 pixels square.
variables: particular N, iterations rep
******************************************************************************/
#include<bits/stdc++.h>
#include<sys/stat.h>
#include<omp.h>
using namespace std;

const char* inputimage1 = "./images/1024.bmp";
const char* inputimage2 = "./images/512.bmp";
const char* outputheader1 = "./original10000";
const char* outputheader2 = "./result10000";
char outputpath1[100];
char outputpath2[100];

const double avrdia = 70.0; // Average diameter
const double sddia = 20.0; // Standard deviation of diameters
const int N = 10000;
const double dt = 4; 
const int rep = 2;

const int width1 = 1024;
const int height1 = 1024;
const int width2 = 512;
const int height2 = 512;
unsigned char header_buf1 [1078];
unsigned char header_buf2 [1078];
unsigned char image_out1[height1][width1];
unsigned char image_out2[height2][width2];

double a[N],b[N];
double u[N],v[N];
double radius[N]; // Each particle diameter
double particle_x[rep][N],particle_y[rep][N];
double Z[N]; // Standard Normal Distribution
double bright[rep][height1][width1];

int maxb[N];

FILE* fp;
/*********************************main****************************************/
int main (int argc, char *argv[]){
  srand((unsigned int)time(NULL)); // Initializing rand() with the current time

  // Output image directory
  mkdir(outputheader1,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
  mkdir(outputheader2,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
  
  // Read image data
  fp = fopen(inputimage1, "rb");
  fread(header_buf1, sizeof(unsigned char), 1078, fp);// Read Header1
  fclose (fp);
  fp = fopen(inputimage2, "rb");
  fread(header_buf2, sizeof(unsigned char), 1078, fp);// Read Header2
  fclose (fp);

  #pragma omp parallel for
  for (int i = 0; i < N; i++){ // Two independent random arrays
    a[i]=(double)rand()/RAND_MAX;
    b[i]=(double)rand()/RAND_MAX;

    Z[i] = sqrt(-2.0*log(a[i]))*cos((double)2.0*M_PI*b[i]);
    radius[i] = sddia*Z[i]+0.50*avrdia;

    particle_x[0][i] = rand() % 1024 ;
    particle_y[0][i] = rand() % 1024 ;
    maxb[i]= (rand() % 128)+128;
  }

  #pragma omp critical 
  for (int i = 1; i < rep; i++){
    for (int j = 0; j < N; j++){
      u[j] = cos (2.0* M_PI/ (width1-1.0) *particle_x[i-1][j])* sin (2.0* M_PI/ (height1-1.0) *particle_y[i-1][j]);
      v[j] = -1.0* sin(2.0* M_PI/ (width1-1.0) *particle_x[i-1][j])* cos (2.0* M_PI/ (height1-1.0) *particle_y[i-1][j]);

      particle_x[i][j] = particle_x[i-1][j] - u[j]*dt;
      particle_y[i][j] = particle_y[i-1][j] - v[j]*dt;
    }
  }
  
  double max[rep],min[rep];

  #pragma omp parallel for
  for(int c=0; c < rep ; c++){

    for (int j = 0; j < height1; j++){
      for (int k = 0; k < width1; k++){
        bright[c][j][k]=0.0;
      }
    }
  
    for (int i = 0; i < N; i++){
      for (int j = 0; j < height1; j++){
        for (int k = 0; k < width1; k++){
          bright[c][j][k] += maxb[i]*exp(-1.0*((particle_x[c][i]-k)*(particle_x[c][i]-k)+(particle_y[c][i]-j)*(particle_y[c][i]-j))/(2.0*radius[i]*radius[i]/100) );
        }
      }
    }

    max[c]=0.0; min[c]=255.5;
    for (int j = 0; j < height1; j++){
      for (int k = 0; k < width1; k++){
        if(bright[c][j][k] < min[c]){
          min[c] = bright[c][j][k];
        }
        if(bright[c][j][k]>max[c]){
          max[c] = bright[c][j][k];
        }
      }
    }

    for (int j = 0; j < height1; j++){
      for (int k = 0; k < width1; k++){
        bright[c][j][k] = (bright[c][j][k]-min[c])/(max[c]-min[c])*255.0;
      }
    }

  }
  
  #pragma omp critical
  for (int c = 0; c < rep; c++){
    sprintf(outputpath1,"%s/%05d.bmp",outputheader1,c);
    fp = fopen(outputpath1, "wb");
    fwrite(header_buf1, sizeof(unsigned char), 1078, fp);// Write Header
    for (int j = 0; j < height1; j++){
      for (int k = 0; k < width1; k++){
        image_out1[j][k] = bright[c][j][k];
      }
    }
    fwrite(image_out1, sizeof(image_out1), 1, fp);// Write 8bit image intensity
    fclose (fp);

    sprintf(outputpath2,"%s/%05d.bmp",outputheader2,c);
    fp = fopen(outputpath2, "wb");
    fwrite(header_buf2, sizeof(unsigned char), 1078, fp);// Write Header
    for (int j = 0; j < height2; j++){
      for (int k = 0; k < width2; k++){
        image_out2[j][k] = bright[c][j+255][k+255];
      }
    }
    fwrite(image_out2, sizeof(image_out2), 1, fp);// Write 8bit image intensity
    fclose (fp);
  }

  return 0;
}