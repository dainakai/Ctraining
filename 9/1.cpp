/******************************************************************************
PROGRAM NAME : One-directional DFT Software
AUTHER : Dai Nakai
DATE : 2021/3/25
******************************************************************************/
#include<bits/stdc++.h>
using namespace std;

const char* inputdata = "./time-sin.dat";
const char* outputdata = "./output.dat";
const char* outputimg = "./output.png";

const double t_s = 0.01;
const int N = 512;

double idx[N];
double re_data[N];
double im_data[N];
double re_dft[N];
double im_dft[N];
double re_inv_dft[N];
double im_inv_dft[N];
double amp_spc[N];
double phs_spc[N];

const int y_min = 0;
const int y_max = 100;
const char* xlabel = "{/Times-New-Roman:Italic=20 f} [Hz]";
const char* ylabel = "{/Times-New-Roman:Italic=20 I^2} [V^2]";

void onedir_dft (double *re_p, double *im_p, double *re_z, double *im_z, int n);
void onedir_inv_dft (double *re_z, double *im_z, double *re_p, double *im_p, int n);
double comp_norm (double re_z, double im_z);
double comp_phase (double re_z, double im_z);

FILE *fp;
FILE *gp;
/*********************************main****************************************/
int main(){
    fp = fopen(inputdata,"r");
    for (int i = 0; i < N; i++)
    {
        fscanf(fp,"%lf %lf\n", &idx[i], &re_data[i]);
        im_data[i] = 0.0;
    }
    fclose(fp);

    onedir_dft(re_data, im_data, re_dft, im_dft, N);
    onedir_inv_dft(re_dft, im_dft, re_inv_dft, im_inv_dft, N);

    fp = fopen(outputdata, "w");
    for (int i = 0; i < N; i++)
    {
        fprintf(fp,"%lf\t%lf\n", idx[i], re_inv_dft[i]);
    }
    fclose(fp);

    for (int i = 0; i < N; i++)
    {
        amp_spc[i] = comp_norm(re_dft[i], im_dft[i]);
        phs_spc[i] = comp_phase(re_dft[i], im_dft[i]);
    }

    if ((gp = popen("gnuplot", "w")) == NULL) {
	    printf("gnuplot is not here!\n");
	    exit(0);
	}

    fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15'\n");
	fprintf(gp,"set output '%s'\n",outputimg);
	fprintf(gp,"unset key\n");

    fprintf(gp,"array data[%d]\n",N);

    for (int i = 0; i < N; i++)
    {
        fprintf(gp,"data[%d] = %lf\n", i+1, amp_spc[i]);
    }

    fprintf(gp,"set yrange[%d:%d]\n", y_min, y_max);
	fprintf(gp,"set xlabel '%s'offset 0.0,0.5\n",xlabel);
	fprintf(gp,"set ylabel '%s'offset 0.5,0.0\n",ylabel);

    fprintf(gp,"plot data using (($1-1)/(%lf*%d)):2 with lines lc -1 lw 2\n",t_s,N);
    
    pclose(gp);


    return 0;
}

void onedir_dft (double *re_p, double *im_p, double *re_z, double *im_z, int n){
   for (int i = 0; i < n; i++){
       re_z[i] = 0.0;
       im_z[i] = 0.0;
   }
   
   for (int i = 0; i < n; i++){
       for (int j = 0; j < n; j++){
           re_z[i] += re_p[j]*cos(2.0*M_PI*i*j/n)+im_p[j]*sin(2.0*M_PI*i*j/n);
           im_z[i] += im_p[j]*cos(2.0*M_PI*i*j/n)-re_p[j]*sin(2.0*M_PI*i*j/n);
       }
   }
}

void onedir_inv_dft (double *re_z, double *im_z, double *re_p, double *im_p, int n){
    for (int i = 0; i < n; i++){
        re_p[i] = 0.0;
        im_p[i] = 0.0;
    }

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            re_p[i] += (re_z[j]*cos(2.0*M_PI*i*j/n)-im_z[j]*sin(2.0*M_PI*i*j/n))/N;
            im_p[i] += (im_z[j]*cos(2.0*M_PI*i*j/n)+re_z[j]*sin(2.0*M_PI*i*j/n))/N;
        }
    }
}

double comp_norm (double re_z, double im_z){
    double norm;
    norm = sqrt(re_z*re_z+im_z*im_z);
    return norm;
}

double comp_phase (double re_z, double im_z){
    double phase;
    phase = atan(im_z/re_z);
    return phase;
}

