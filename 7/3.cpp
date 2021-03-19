/******************************************************************************
PROGRAM NAME : PIV Error Analysis
AUTHER : Dai Nakai
DATE : 2021/3/19

PIV Verification on each variable particles N.
for 512by512 image, 32by32 interrogation, 64by64 search, 50% overlapped only.
******************************************************************************/
#include<bits/stdc++.h>
using namespace std;

struct data {
  int no;
  const char *path;
};

const char* outputpath = "./errordata.dat";
const char* outputimage = "./errorgraph.png";
const int v = 10;
const int idxx = 29;
const int idxy = 29;
const double thr = 5.0;

double vx[v+1][idxy][idxx];
double vy[v+1][idxy][idxx];
double err_abs[v][idxy][idxx];
int err_count[v];

FILE *fp;
FILE *gp;
/*********************************main*****************************************/
int main(){

  struct data datapath [v]=
  {
    {1,"./data1000.dat"},
    {2,"./data2000.dat"},
    {3,"./data3000.dat"},
    {4,"./data4000.dat"},
    {5,"./data5000.dat"},
    {6,"./data6000.dat"},
    {7,"./data7000.dat"},
    {8,"./data8000.dat"},
    {9,"./data9000.dat"},
    {10,"./data10000.dat"}
  };

  for (int i = 0; i < v; i++)
  {
    fp = fopen(datapath[i].path,"r");
    int x,y;
    for (int j = 0; j < idxx*idxy; j++){
      fscanf(fp,"%d %d",&x,&y);
      fscanf(fp,"%lf %lf\n",&vx[i][y][x],&vy[i][y][x]);
    }
    fclose(fp);
  }

  for (int k = 0; k < idxy; k++){
    for (int l = 0; l < idxx; l++){
      vx[10][k][l] = cos(2.0*M_PI/1024.0*(double)(512*32*16*l))*sin(2.0*M_PI/1024.0*(double)(512*32*16*k));
      vy[10][k][l] = -1.0*sin(2.0*M_PI/1024.0*(double)(512*32*16*l))*cos(2.0*M_PI/1024.0*(double)(512*32*16*k));
    }
  }

  for (int i = 0; i < v; i++){
    for (int k = 0; k < idxy; k++){
      for (int l = 0; l < idxx; l++){
        err_abs[i][k][l] = sqrt((vx[i][k][l]-vx[10][k][l])*(vx[i][k][l]-vx[10][k][l])+(vy[i][k][l]-vy[10][k][l])*(vy[i][k][l]-vy[10][k][l]));
      }
    }
  }
  
  for (int i = 0; i < v; i++){
    err_count[i] = 0;

    for (int k = 0; k < idxy; k++){
      for (int l = 0; l < idxx; l++){
        if(err_abs[i][k][l] > thr){
          err_count[i]++;
        }
      }
    }
  }
  
  fp = fopen(outputpath,"w");
  for (int i = 0; i < v; i++)
  {
    fprintf(fp,"%d %lf\n",(i+1)*1000,err_count[i]/64.0);
  }
  fclose(fp);
  
  if ((gp = popen("gnuplot", "w")) == NULL) {
	printf("gnuplot is not here!\n");
	exit(0);
	}

  fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15' \n");
	fprintf(gp,"set output '%s'\n",outputimage);
  fprintf(gp,"unset key\n");
	
  fprintf(gp,"set xrange[0:11000]\n");
	fprintf(gp,"set xlabel 'Number of particles'\n");
	fprintf(gp,"set ylabel 'Error count per 64 pixels square'\n");
  fprintf(gp,"plot '%s' using 1:2 with linespoints\n",outputpath);
  pclose(gp);
  
  return 0;
}