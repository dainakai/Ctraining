/******************************************************************************
PROGRAM NAME : PIV on flow around cylinder
AUTHER : Dai Nakai
DATE : 2021/3/22
******************************************************************************/
#include<bits/stdc++.h>
#include<sys/stat.h>
#include<omp.h>
using namespace std;

const int width = 1280;
const int height = 1024;

const char* read_file_dir = "./pivimg";
const char* read_file_header = "pivimg";
const char* write_file_dir = "./pivvector";
const char* write_file_header = "vecfield";

unsigned char image1[height][width];
unsigned char image2[height][width];
unsigned char header_buf[1078];

char read_file1[100];
char read_file2[100];
char write_file[100];

/************************PARAMETERS*****************************/
const int M = 32; // x horizontal length of Interrogation window
const int N = 32; // y vertical length of Interrogation window
const int Sx = 16; // half of horizontal extension of I window
const int Sy = 16; // half of vertical extension of I window
/***********************PARAMETERS END**************************/

const int x_min = -Sx; // moving range of I window which consists Search window
const int x_max = Sx;
const int y_min = -Sy;
const int y_max = Sy;
const int Cdimx = 2*(width-2*Sx)/M - 1; // horizontal dimension of correlation windows
const int Cdimy = 2*(height-2*Sy)/N - 1; // vertical dimension of correlation windows


double C[Cdimy][Cdimx][N][M]; // Correlation window on each Interrogation
double vx[Cdimy][Cdimx]; // x-axis velocity
double vy[Cdimy][Cdimx]; // y-axis velocity

// Temporary data
double numer[Cdimy][Cdimx][N][M]; // Numerator of each C window
double denom1[Cdimy][Cdimx][N][M]; // Denominator of each C window
double denom2[Cdimy][Cdimx]; // Denominator of each C window
double avrI[Cdimy][Cdimx];
double avrS[Cdimy][Cdimx];
double maxt[Cdimy][Cdimx];

int num = 0;

FILE *fp1;
FILE *fp2;
/*********************************main****************************************/
int main(){
    mkdir(write_file_dir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

    while(1)
    {
        //Image inputting
        num++;
        sprintf(read_file1, "%s/%s%04d.bmp", read_file_dir, read_file_header, num-1);
        sprintf(read_file2, "%s/%s%04d.bmp", read_file_dir, read_file_header, num);
        printf("inputting : %s and %s\n", read_file1, read_file2);

        if(!(fp1 = fopen(read_file1,"rb")) || !(fp2 = fopen(read_file2,"rb"))){
            printf("inputting : img %s or %s is not valid\n", read_file1, read_file2);
            break;
        }

        fread(header_buf, sizeof(unsigned char), 1078, fp1);
        fread(image1, sizeof(image1), 1, fp1);
        fclose(fp1);

        fread(header_buf, sizeof(unsigned char), 1078, fp2);
        fread(image2, sizeof(image2), 1, fp2);
        fclose(fp2);



        //Processing
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
      
                for (int CIdxy = 0; CIdxy < N; CIdxy++){
                    for (int CIdxx = 0; CIdxx < M; CIdxx++){
                        for (int k = 0; k < N; k++){
                            for (int l = 0; l < M; l++){
                                numer[i][j][CIdxy][CIdxx] += (image1[Sy+i*N/2+k][Sx+j*M/2+l]-avrI[i][j])*(image2[i*N/2+k + CIdxy][j*M/2+l + CIdxx]-avrS[i][j]);
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
                        denom2[i][j] += (image1[Sy+i*N/2+k][Sx+j*M/2+l]-avrI[i][j])*(image1[Sy+i*N/2+k][Sx+j*M/2+l]-avrI[i][j]);
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


        for (int i = 0; i < Cdimy; i++){
            for (int j = 0; j < Cdimx; j++){
                maxt[i][j] = -10000.0;

                for (int k = 0; k < N; k++){
                    for (int l = 0; l < M; l++){
                        if(maxt[i][j] < C[i][j][k][l]){
                            maxt[i][j] = C[i][j][k][l];
                            vx[i][j] = (double)l - (double)M/2.0;
                            vy[i][j] = (double)k - (double)N/2.0;
                        }
                    }
                }
            }
        }

        //Data outputting
        sprintf(write_file, "%s/%s%04d.dat", write_file_dir, write_file_header, num-1);
        fp1 = fopen(write_file,"w");

        for (int i = 0; i < Cdimy; i++){
            for (int j = 0; j < Cdimx; j++){
                fprintf(fp1, "%d %d %lf %lf\n", j, i, vx[i][j], vy[i][j]);
            }
        }

    }

    return 0;
}