/******************************************************************************
PROGRAM NAME : PIV on Taylor-Green vortex
AUTHER : Dai Nakai
DATE : 2021/2/18

Read two images which are adjacent in time each other.
Former is called interrogation, latter is called search window.
Calculate cross-correlation and vectors which are 50% overlapped each other.

CAUTION!
* Image pixel arrays [y][x] are arranged so that the origin [0][0] is on
*the upper-left corner and y-axis is inversed. 
* Variable parameters should be power of two so that arrays which are 
*dependent on them can be computed. 
******************************************************************************/
#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

const char* inputimage1 = "./result/00000.bmp";
const char* inputimage2 = "./result/00001.bmp";
unsigned char header_buf [1078];

const char* outputdata = "data.dat";
const char* outputpiv = "result.png";
const char* xxlabel = "{/Times-New-Roman:Italic=20 x} [pixel]";
const char* yylabel = "{/Times-New-Roman:Italic=20 y} [pixel]";
const char* cb_label = "{/Symbol:Italic=20 w}_{/Times-New-Roman:Italic=20 z} [sec]";//color bar range min

const int L=512; // Length of inputting image square
unsigned char image1[L][L]; // brightness value arrays of former image
unsigned char image2[L][L]; // same as above but latter image

/************************PARAMETERS*****************************/
const int M = 32; // x horizontal length of Interrogation window
const int N = 32; // y vertical length of Interrogation window
const int Sx = 16; // half of horizontal extension of I winodw
const int Sy = 16; // half of vertical extension of I window
/***********************PARAMETERS END**************************/

const int x_min = -Sx; // moving range of I window which consists Search window
const int x_max = Sx;
const int y_min = -Sy;
const int y_max = Sy;
const int shift = 16; // Correlation window shift
const int Cdimx = 2*(L-2*Sx)/M - 1; // horizontal dimension of correlation windows
const int Cdimy = 2*(L-2*Sy)/N - 1; // vertical dimension of correlation windows


double C[Cdimy][Cdimx][N][M]; // Correlation window on each Interrogation
double vx[Cdimy][Cdimx]; // x-axis velocity
double vy[Cdimy][Cdimx]; // y-axis velocity

// Temporary data
double numer[Cdimy][Cdimx][N][M]; // Numerator of each C window
double denom1[Cdimy][Cdimx][N][M]; // Denominator of each C window
double denom2[Cdimy][Cdimx]; // Denominator of each C window
double avrI[Cdimy][Cdimx];
double avrS[Cdimy][Cdimx];

FILE* fp;
FILE* gp;
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
  #pragma omp parallel for
  for (int i = 0; i < Cdimy; i++){ // Start point x
    for (int j = 0; j < Cdimx; j++){ // Start point y
      
      // Calculate avr. values of I
      avrI[i][j] = 0;
      avrS[i][j] = 0;

      for (int k = 0; k < N; k++){
        for (int l = 0; l < M; l++){
          avrI[i][j] += image1[Sy+i*N/2+k][Sx+j*M/2+l];
        }
      }

      for (int k = y_min ; k < N + y_max; k++){
        for (int l = x_min ; l < M + x_max; l++){
          avrS[i][j] += image2[Sy+i*N/2+k][Sx+i*M/2+l];
        }
      }
      
      avrI[i][j] /= N*M;
      avrS[i][j] /= (N + y_max - y_min)*(M + x_max - x_min);

    }
  }

  #pragma omp parallel for
  for (int i = 0; i < Cdimy; i++){
    for (int j = 0; j < Cdimx; j++){

      // Calculate numer, denom, and Crr
      for (int k = 0; k < N; k++){
        for (int l = 0; l < M; l++){
          numer[i][j][k][l] = 0;
          denom1[i][j][k][l] = 0;
          denom2[i][j] = 0;
        }
      }
      
////  changed changed tested image2[Sy + i*N/2 + k]
      for (int CIdxy = 0; CIdxy < N; CIdxy++){
        for (int CIdxx = 0; CIdxx < M; CIdxx++){
          for (int k = 0; k < N; k++){
            for (int l = 0; l < M; l++){
              numer[i][j][CIdxy][CIdxx] += (image1[Sy+i*N/2+k][Sx+j*M/2+l]-avrI[i][j])*(image2[ i*N/2+k + CIdxy][ j*M/2+l + CIdxx]-avrS[i][j]);
            }
          }
        }
      }
      
      for (int CIdxy = 0; CIdxy < N; CIdxy++){
        for (int CIdxx = 0; CIdxx < M; CIdxx++){
          for (int k = 0; k < N; k++){
            for (int l = 0; l < M; l++){
              denom1[i][j][CIdxy][CIdxx] += (image2[i*N/2+k + CIdxy][j*M/2+l + CIdxx]-avrS[i][j])*(image2[i*N/2+k + CIdxy][j*M/2+l + CIdxx]-avrS[i][j]);
            }
          }
        }
      }

      for (int k = 0; k < N; k++){
        for (int l = 0; l < M; l++){
          denom2[i][j] = (image1[Sy+i*N/2+k][Sx+j*M/2+l]-avrI[i][j])*(image1[Sy+i*N/2+k][Sx+j*M/2+l]-avrI[i][j]);
        }
      }
      
    }
  }

  #pragma omp parallel for
  for (int i = 0; i < Cdimy; i++){
    for (int j = 0; j < Cdimx; j++){
      for (int CIdxy = 0; CIdxy < N; CIdxy++){
        for (int CIdxx = 0; CIdxx < M; CIdxx++){
          C[i][j][CIdxy][CIdxx] = numer[i][j][CIdxy][CIdxx]/sqrt(denom1[i][j][CIdxy][CIdxx]*denom2[i][j]);
        }
      }
    }
  }

  double min[Cdimy][Cdimx];
  double max[Cdimy][Cdimx];
  for (int i = 0; i < Cdimy; i++){
    for (int j = 0; j < Cdimx; j++){
      max[i][j] = 0.0;

      for (int k = 0; k < N; k++){
        for (int l = 0; l < M; l++){
          if(max[i][j] < C[i][j][k][l]){
            max[i][j] = C[i][j][k][l];
            vx[i][j] = (double)l - (double)M/2.0;
            vy[i][j] = (double)N/2.0 - (double)k;
          }
        }
      }
    }
  }

  // for (int i = 0; i < Cdimy; i++)
  // {
  //   for (int j = 0; j < Cdimx; j++)
  //   {
  //     if(vx[i][j]*vx[i][j]+vy[i][j]*vy[i][j] > 10){
  //       vx[i][j] = 0.0;
  //       vy[i][j] = 0.0;
  //     }
  //   }
    
  // }
  

  fp = fopen(outputdata,"w");
  for (int i = 0; i < Cdimy; i++){
    for (int j = 0; j < Cdimx; j++){
      fprintf(fp, "%d %d %lf %lf\n", j, i, vx[i][j], vy[i][j]);
    }
  }

  fclose(fp);
  
  if ((gp = popen("gnuplot", "w")) == NULL) {
	printf("gnuplot is not here!\n");
	exit(0);
	}

  //PNG image
	fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15' \n");
	fprintf(gp,"set output '%s'\n",outputpiv);
	
	fprintf(gp,"set size ratio 1\n");

  // fprintf(gp,"set yrange reverse\n");

	fprintf(gp,"set xlabel '%s'offset 0.0,0.5\n",xxlabel);
	fprintf(gp,"set ylabel '%s'offset 0.5,0.0\n",yylabel);

	fprintf(gp,"plot '%s' using 1:2:($3*0.5):($4*(-0.5))  w vector ti ''\n",outputdata);

 	fflush(gp); //Clean up Data

	fprintf(gp, "exit\n"); // Quit gnuplot
	pclose(gp);

  
  return 0;
}