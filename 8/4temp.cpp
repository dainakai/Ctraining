//DATA OUTPUT VERIFICATION
#include<bits/stdc++.h>
using namespace std;

const char* read_file_dir = "./pivvector";
const char* read_file_header = "vecfield";
char read_file[100];

const int imgnum = 99;
const int width = 1280;
const int height = 1024;
const int M = 32; // x horizontal length of Interrogation window
const int N = 32; // y vertical length of Interrogation window
const int Sx = 16; // half of horizontal extension of I window
const int Sy = 16; // half of vertical extension of I window
const int Cdimx = 2*(width-2*Sx)/M - 1;
const int Cdimy = 2*(height-2*Sy)/N - 1;

double vx[imgnum][Cdimy][Cdimx];
double vy[imgnum][Cdimy][Cdimx];
double w[imgnum][Cdimy][Cdimx];

int temp;

FILE* fp;

int main(){
    for (int num = 0; num < imgnum; num++)
    {
        sprintf(read_file,"%s/%s%04d.dat",read_file_dir,read_file_header,num);
        fp = fopen(read_file,"r");
        for (int i = 0; i < Cdimy; i++){
            for (int j = 0; j < Cdimx; j++){
                fscanf(fp,"%d %d %lf %lf %lf\n",&temp,&temp,&vx[num][i][j],&vy[num][i][j],&w[num][i][j]);
            }
        }
        fclose(fp);

        fp = fopen(read_file,"w");
        for (int i = 0; i < Cdimy; i++)
        {
            for (int j = 0; j < Cdimx; j++)
            {
                fprintf(fp,"%d %d %lf %lf %lf\n",j,i,vx[num][i][j], vy[num][i][j],w[num][i][j]);
            }
            fprintf(fp,"\n");
        }
        fclose(fp);
    }
    return 0;
}