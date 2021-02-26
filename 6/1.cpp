/******************************************************************************
PROGRAM NAME : Random particle generating (N=1000)
AUTHER : Dai Nakai
DATE : 2021/1/20
******************************************************************************/
#include<bits/stdc++.h>
using namespace std;

const char* dataoutput = "data1.dat";
const char* pngimage = "data1.png";

const double avrdia = 70.0; // Average diameter
const double sddia = 20.0; // Standard deviation of diameters
const int N = 1000; // Number of particles
const int h = 10; // histogram interval

double a[N],b[N];
double particle[N]; // Each particle diameter

double Z; // Standard Normal Distribution
double avr;
double v;

FILE * fp;
FILE * gp;
/*********************************main****************************************/
int main (){
  srand((unsigned int)time(NULL)); // Initializing rand() with the current time
  
  for (int i = 0; i < N; i++){ // Two independent random arrays
    a[i]=(double)rand()/RAND_MAX;
    b[i]=(double)rand()/RAND_MAX;
  }
  
  for (int i = 0; i < N; i++){ // Box-Mullar's method
    Z = sqrt(-2.0*log(a[i]))*cos((double)2.0*M_PI*b[i]);
    particle[i] = sddia*Z+avrdia;
  }

  avr=0;
  for (int i = 0; i < N; i++){
    avr += particle[i];
  }
  avr /= N;

  v=0;
  for (int i = 0; i < N; i++){
    v += (particle[i]-avr)*(particle[i]-avr);
  }
  v /= N;

  cout << "AVERAGE : " << avr << endl;
  cout << "VARIANCE : " << v << endl;
  
  double count[N/h];
  for (int i = 0; i < 100; i++)
  {
    count[i]=0;
  }
  
  for (int i = 0; i < N; i++){
    int a;
    a = particle[i]/10;
    count[a]++;
  }
  
  fp = fopen(dataoutput,"w");
  int i = 0;
  while(count[i]!=0 || i==0){
    fprintf(fp,"%d5 %lf\n",i,count[i]);
    i++;
  }
  fclose(fp);

  /***********histogram outputting***********/
  if ((gp = popen("gnuplot", "w")) == NULL) {
	printf("gnuplot is not here!\n");
	exit(0);
	}
  fprintf(gp,"set terminal pngcairo size 1920 , 1440 color font 'Times-New-Roman,35'\n");
  fprintf(gp,"set output '%s'\n",pngimage);
  fprintf(gp,"plot '%s' using 1:2 with boxes\n",dataoutput);
  pclose(gp);

  return 0;
}