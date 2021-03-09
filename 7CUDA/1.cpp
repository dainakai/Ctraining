/***************************************************************************************************:
 * PROGRAM NAME : PIV Image Generating Software 
 * AUTHER : Dai Nakai
 * DATE : March 9, 2021
 * 
 * This software is made to generate PIV particle masks on Taylor-Green vortex.
 * Images are originally gained with 1024 pixels square, then trimmed at their center 512 pixels. 
 * Variables : N for particle number, t for number of images
 * 
 * 
 * **************************************************************************************************/
#include<bits/stdc++.h>
#include<sys/stat.h>
#include<omp.h>
using namespace std;

const char* imageheader1 = "./header/1024.bmp";
const char* imageheader2 = "./header/512.bmp";
const char* outputheader1 = "./original";
const char* outputheader2 = "./images";

const double avrdia = 70.0; // Particle mean diameter
const double sddia = 20.0; // Standard deviation of diameters
const int N = 1000;
const double dt = 0.01;
const int t = 2;

const int width1 = 1024;
const int height1 = 1024;
const int width2 = 512;
const int height2 = 512;

unsigned char header_buf1 [1078];
unsigned char header_buf2 [1078];
unsigned char image_out1[height1][width1];
unsigned char image_out2[height2][width2];

double u[N],v[N];
double Z[N];
double radius[N]; // Each particle radius
double particle_x[t][N],particle_y[t][N];
double Z; // Standard Normal Distribution
double bright[t][height1][width1];

int maxb[N];

FILE* fp;
/******************************************main*************************************************/
int main(int argc, char **argv){
  // Initialize rand() with current time
  srand((unsigned int)time(NULL));

  // Output image directories
  mkdir(outputheader1,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
  mkdir(outputheader2,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
  
  // Read image headers
  fp = fopen(imageheader1, "rb");
  fread(header_buf1, sizeof(unsigned char), 1078, fp);// Read Header1
  fclose (fp);
  fp = fopen(imageheader2, "rb");
  fread(header_buf2, sizeof(unsigned char), 1078, fp);// Read Header2
  fclose (fp);

  for (int i = 0; i < N; i++)
  {
    Z[i] = sqrt(-2.0*log((double)(rand()/RAND_MAX)))*cos(2.0*M_PI*((double)(rand()/RAND_MAX)));
    radius[i] = sddia*Z[i] + 0.5*avrdia;

    particle_x[0][i] = rand() % 1024;
    particle_y[0][i] = rand() % 1024;
    maxb[i] = (rand() % 128) + 128;
  }
  

}