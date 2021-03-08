/******************************************************************************
PROGRAM NAME : PIV Image Generating with CUDA.
AUTHER : Dai Nakai
DATE : 2021/2/22

Particle image producing software.
Images are originally gained with 1024 pixels square, then
be trimmed at their center 512 pixels square.
variable particular N, iterations rep
******************************************************************************/
#include<bits/stdc++.h>
#include<sys/stat.h>

using namespace std;

const char* inputimage1 = "./images/1024.bmp";
const char* inputimage2 = "./images/512.bmp";
const char* outputheader1 = "./original";
const char* outputheader2 = "./result";
char outputpath1[100];
char outputpath2[100];

const double avrdia = 70.0; // Average diameter
const double sddia = 20.0; // Standard deviation of diameters
const int Ndef = 1000;
int N;
const double dtdef = 5;
double dt; 
const int repdef = 2;
int rep;

const int width1 = 1024;
const int height1 = 1024;
const int width2 = 512;
const int height2 = 512;
unsigned char header_buf1 [1078];
unsigned char header_buf2 [1078];
unsigned char image_out1[height1][width1];
unsigned char image_out2[height2][width2];



FILE* fp;
/*********************************main****************************************/
int main (int argc, char *argv[]){

  if(!strcmp(argv[1],"--help")){
    printf("PIV Image Generating Software with CUDA\n\n");
    printf("Put appropriate numbers of iterations, particles, and interval in this order.\n");
    printf("You can't skip any of parameters. If you want to set them as default numbers, put '-d' as option.\n");
    exit(1);
  }else if(!strcmp(argv[1],"-d")){
    N = Ndef;
    dt = dtdef;
    rep = repdef;
    printf("parameters are set as default\n");
  }else{
    printf("%s\n",argv[1]);
  }

  srand((unsigned int)time(NULL)); // Initializing rand() with the current time

  // double a[N],b[N];
  // double u[N],v[N];
  // double radius[N]; // Each particle diameter
  // int particle_x[rep][N],particle_y[rep][N];
  // double Z; // Standard Normal Distribution
  // double bright[rep][height1][width1];

  // int maxb[N];
  

  return 0;
}