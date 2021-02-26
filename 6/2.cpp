/******************************************************************************
PROGRAM NAME : Random particle generating (variable N)
AUTHER : Dai Nakai
DATE : 2021/1/21
******************************************************************************/
#include<bits/stdc++.h>
using namespace std;

const char* dataoutput = "data2.dat";
const char* pngimage = "data2.png";

const double avrdia = 70.0; // Average diameter
const double sddia = 20.0; // Standard deviation of diameters

// Number of particles
const int minN = 10;
const int maxN = 10000;
const int intervalN = 10;
int N;

double a[maxN],b[maxN];
double particle[maxN]; // Each particle diameter
double Z; // Standard Normal Distribution
double avr[maxN/intervalN];
double v[maxN/intervalN];

FILE * fp;
FILE * gp;
/*********************************main****************************************/
int main (){
  srand((unsigned int)time(NULL)); // Initializing rand() with the current time

  fp = fopen(dataoutput,"w");
  for (int i = 0; i < maxN/intervalN; i++)
  {
    N = minN+i*intervalN;

    for (int j = 0; j < N; j++){ // Two independent random arrays
    a[j]=(double)rand()/RAND_MAX;
    b[j]=(double)rand()/RAND_MAX;
    }

    for (int j = 0; j < N; j++){ // Box-Mullar's method
    Z = sqrt(-2.0*log(a[j]))*cos((double)2.0*M_PI*b[j]);
    particle[j] = sddia*Z+avrdia;
    }

    avr[i]=0;
    for (int j = 0; j < N; j++){
    avr[i] += particle[j];
    }
    avr[i] /= N;
    
    v[i]=0;
    for (int j = 0; j < N; j++){
    v[i] += (particle[j]-avr[i])*(particle[j]-avr[i]);
    }
    v[i] /= N;

    fprintf(fp,"%d %lf %lf\n",N,avr[i],v[i]);
  }
  
  fclose(fp);

  /***********image outputting***********/
  if ((gp = popen("gnuplot", "w")) == NULL) {
	printf("gnuplot is not here!\n");
	exit(0);
	}
  fprintf(gp,"set terminal pngcairo size 1920 , 1440 color font 'Times-New-Roman,35'\n");
  fprintf(gp,"set output '%s'\n",pngimage);
  fprintf(gp,"set ytics nomirror\n");
  fprintf(gp,"set logscale x\n");
  fprintf(gp,"set y2tics nomirror\n");
  fprintf(gp,"set ylabel 'Average'\n");
  fprintf(gp,"set y2label 'Variance'\n");
  fprintf(gp,"plot '%s' using 1:2 with lines axis x1y1 title 'Average', '%s' using 1:3 with lines axis x1y2 title 'Variance'\n",dataoutput,dataoutput);
  pclose(gp);

  return 0;
}