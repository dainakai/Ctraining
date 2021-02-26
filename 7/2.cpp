/******************************************************************************
PROGRAM NAME : PIV on Taylor-Green vortex
AUTHER : Dai Nakai
DATE : 2021/2/18

Read two images which are adjacent in time each other.
Former called interrogation, latter called search window.
Calculate cross-correlation and vectors which are 50% overlapped each other.

CAUTION! : Image pixel arrays [y][x] are arranged so that the origin [0][0] is
the upper-left corner and y-axis is inversed. 
******************************************************************************/
#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

const char* inputimage1 = "./result/00000.bmp";
const char* inputimage2 = "./result/00001.bmp";
unsigned char header_buf [1078];

const int L=512; // Length of inputting image square
unsigned char image1[L][L]; // brightness value arrays of former image
unsigned char image2[L][L]; // same as above but latter image

const int M = 32; // x horizontal length of Interrogation window
const int N = 32; // y vertical length of Interrogation window
const int x_min = -16; // Search window expansion for Interrogation window
const int x_max = 16;
const int y_min = -16;
const int y_max = 16;
const int shift = 16; // Correlation window shift

double C[(L-N)/N][(L-M)/M][N][M]; // Correlation window on each Interrogation
double vx[L][L]; // x-axis velocity
double vy[L][L]; // y-axis velocity (notice CAUTION!)

const int grid_s = 1; // grid spacing shift

// Temporary data (needed when using OpenMP)
double numer[(L-N)/N][(L-M)/M][N][M]; // Numerator of each C[N][M]
double denom1[(L-N)/N][(L-M)/M][N][M]; // Denominator of rach C[N][M]
double denom2[(L-N)/N][(L-M)/M]; // Denominator of rach C[N][M]
double avrI[(L-N)/N][(L-M)/M];
double avrS[(L-N)/N][(L-M)/M];


FILE* fp;
/*********************************main*****************************************/
int main(void){

  // Copying image brightness data
  fp = fopen(inputimage1, "rb");
  fread(header_buf, sizeof(unsigned char), 1078, fp); // Read Header
  fread(image1, sizeof(unsigned char), L*L, fp);
  fclose (fp);

  fp = fopen(inputimage2, "rb");
  fread(header_buf, sizeof(unsigned char), 1078, fp); // Read Header
  fread(image2, sizeof(unsigned char), L*L, fp);
  fclose (fp);

  // Window calculating one by one
  for (int i = 0; i < (L-N)/N; i++){ // Start point x
    for (int j = 0; j < (L-M)/M; j++){ // Start point y
      
      // Calculate avr. values I,S[N][M]
      avrI[i][j] = 0;
      avrS[i][j] = 0;

      for (int k = 0; k < N; k++){
        for (int l = 0; l < M; l++){
          avrI[i][j] += image1[i*N+k][j*M+l];
          avrS[i][j] += image2[i*N+k][j*M+l];
        }
      }

      avrI[i][j] /= N*M;
      avrS[i][j] /= N*M;


      // Calculate numer, denom, and Crr
      for (int k = 0; k < N; k++){
        for (int l = 0; l < M; l++){
          numer[i][j][k][l] = 0;
          denom1[i][j][k][l] = 0;
          denom2[i][j] = 0;
        }
      }
      

      for (int k = 0; k < N; k++){
        for (int l = 0; l < M; l++){
          for (int n = 0; n < N; n++){
            for (int m = 0; m < M; m++){
              numer[i][j][k][l] += (image1[i*N+n][j*M+m]-avrS[i][j])*(image2[i*N+n][j*M+m]-avrI[i][j]);
            }
          }
        }
      }
      
      for (int k = 0; k < N; k++){
        for (int l = 0; l < M; l++){
          for (int n = 0; n < N; n++){
            for (int m = 0; m < M; m++){
              denom1[i][j][k][l] += (image1[i*N+n][j*M+m]-avrS[i][j])*(image1[i*N+n][j*M+m]-avrS[i][j]);
              denom2[i][j] += (image2[i*N+n][j*M+m]-avrI[i][j])*(image2[i*N+n][j*M+m]-avrI[i][j]);
            }
          }
        }
      }
      

    }
  }
  










  return 0;
}