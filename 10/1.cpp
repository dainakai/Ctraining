/******************************************************************************
PROGRAM NAME : One-dimensional DFT and Band-pass filter Software
AUTHER : Dai Nakai
DATE : 2021/3/26
******************************************************************************/
#include<bits/stdc++.h>
#include<sys/stat.h>
using namespace std;

const char* inputdata = "./noise-signal.dat";
const char* outputdata = "./filtered-data.png";
const char* outputdir = "./img";
const char* outputimg0 = "./img/img0.png";
const char* outputimg1 = "./img/img1.png";
const char* outputimg2 = "./img/img2.png";
const char* outputimg3 = "./img/img3.png";

const double dt = 20*0.000001; // 20 usec interval
double f_0;

int N;
const int buf_size = 256;
char buf[buf_size];

const int y_min = 0;
const int y_max1 = 500;
const int y_max2 = 500;
const double y_min3 = -0.05;
const double y_max3 = 0.05;
const int x_min1 = 0;
const int x_max1 = 50000;
const int x_min2 = 220;
const int x_max2 = 260;
const int x_min3 = 0;
const int x_max3 = 65536;
const char* xlabel = "{/Times-New-Roman:Italic=20 f} [Hz]";
const char* xlabel3 = "{/Times-New-Roman:Italic=20 t} [{/Symbol=13 \\155}sec]";
const char* ylabel = "{/Times-New-Roman:Italic=20 I} [V]";

void onedim_dft (double *re_p, double *im_p, double *re_z, double *im_z, int n);
void onedim_inv_dft (double *re_z, double *im_z, double *re_p, double *im_p, int n);
double comp_norm (double re_z, double im_z);

FILE *fp;
FILE *gp;
/*********************************main****************************************/
int main(){
    mkdir(outputdir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

    // Data counting
    fp = fopen(inputdata,"r");
    while(fgets(buf, buf_size, fp)){
        N++;
    }
    fclose(fp);
    printf("number of input data : %d\n",N);

    double re_data[N];
    double im_data[N];
    double re_dft[N];
    double im_dft[N];
    double re_inv_dft[N];
    double im_inv_dft[N];
    double amp_spc[N];
    f_0 = 1.0/(dt*N);

    fp = fopen(inputdata, "r");
    for (int i = 0; i < N; i++){
        fscanf(fp,"%lf\n", &re_data[i]);
    }

    //Original time series data
    if ((gp = popen("gnuplot", "w")) == NULL) {
	    printf("gnuplot is not here!\n");
	    exit(0);
	}

    fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15'\n");
	fprintf(gp,"set output '%s'\n",outputimg0);
	fprintf(gp,"unset key\n");

    fprintf(gp,"array data[%d]\n",N);

    for (int i = 0; i < N; i++)
    {
        fprintf(gp,"data[%d] = %lf\n", i+1, re_data[i]);
    }

    fprintf(gp,"set xrange[%d:%d]\n", x_min3, x_max3);
    fprintf(gp,"set yrange[%lf:%lf]\n", y_min3, y_max3);
	fprintf(gp,"set xlabel '%s'offset 0.0,0.5\n",xlabel3);
	fprintf(gp,"set ylabel '%s'offset 0.5,0.0\n",ylabel);

    fprintf(gp,"plot data using (($1-1)*%lf):2 with lines lc -1 lw 2\n",dt*1000000,N);
    
    pclose(gp);

    //Problem 1
    onedim_dft(re_data, im_data, re_dft, im_dft, N);
    for (int i = 0; i < N; i++){
        im_data[i] = 0.0;
    }
    for (int i = 0; i < N; i++){
        amp_spc[i] = comp_norm(re_dft[i], im_dft[i]);
    }
    
    if ((gp = popen("gnuplot", "w")) == NULL) {
	    printf("gnuplot is not here!\n");
	    exit(0);
	}

    fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15'\n");
	fprintf(gp,"set output '%s'\n",outputimg1);
	fprintf(gp,"unset key\n");

    fprintf(gp,"array data[%d]\n",N);

    for (int i = 0; i < N; i++)
    {
        fprintf(gp,"data[%d] = %lf\n", i+1, amp_spc[i]);
    }

    fprintf(gp,"set xrange[%d:%d]\n", x_min1, x_max1);
    fprintf(gp,"set yrange[%d:%d]\n", y_min, y_max1);
	fprintf(gp,"set xlabel '%s'offset 0.0,0.5\n",xlabel);
	fprintf(gp,"set ylabel '%s'offset 0.5,0.0\n",ylabel);

    fprintf(gp,"plot data using (($1-1)*%lf):2 with lines lc -1 lw 2\n",f_0,N);
    
    pclose(gp);

    //Problem 2
    for (int i = 0; i < N; i++){
        if((i*f_0 < 230) || (i*f_0 > 250)){
            re_dft[i] = 0.0;
            im_dft[i] = 0.0;
            amp_spc[i] = 0.0;
        }
    }

    if ((gp = popen("gnuplot", "w")) == NULL) {
	    printf("gnuplot is not here!\n");
	    exit(0);
	}

    fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15'\n");
	fprintf(gp,"set output '%s'\n",outputimg2);
	fprintf(gp,"unset key\n");

    fprintf(gp,"array data[%d]\n",N);

    for (int i = 0; i < N; i++)
    {
        fprintf(gp,"data[%d] = %lf\n", i+1, amp_spc[i]);
    }

    fprintf(gp,"set xrange[%d:%d]\n", x_min2, x_max2);
    fprintf(gp,"set yrange[%d:%d]\n", y_min, y_max2);
	fprintf(gp,"set xlabel '%s'offset 0.0,0.5\n",xlabel);
	fprintf(gp,"set ylabel '%s'offset 0.5,0.0\n",ylabel);

    fprintf(gp,"plot data using (($1-1)*%lf):2 with lines lc -1 lw 2\n",f_0,N);
    
    pclose(gp);

    //Problem 3
    onedim_inv_dft(re_dft, im_dft, re_inv_dft, im_inv_dft, N);

    if ((gp = popen("gnuplot", "w")) == NULL) {
	    printf("gnuplot is not here!\n");
	    exit(0);
	}

    fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15'\n");
	fprintf(gp,"set output '%s'\n",outputimg3);
	fprintf(gp,"unset key\n");

    fprintf(gp,"array data[%d]\n",N);

    for (int i = 0; i < N; i++)
    {
        fprintf(gp,"data[%d] = %lf\n", i+1, re_inv_dft[i]);
    }

    fprintf(gp,"set xrange[%d:%d]\n", x_min3, x_max3);
    fprintf(gp,"set yrange[%lf:%lf]\n", y_min3, y_max3);
	fprintf(gp,"set xlabel '%s'offset 0.0,0.5\n",xlabel3);
	fprintf(gp,"set ylabel '%s'offset 0.5,0.0\n",ylabel);

    fprintf(gp,"plot data using (($1-1)*%lf):2 with lines lc -1 lw 2\n",dt*1000000,N);
    
    pclose(gp);


    return 0;
}

void onedim_dft (double *re_p, double *im_p, double *re_z, double *im_z, int n){
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

void onedim_inv_dft (double *re_z, double *im_z, double *re_p, double *im_p, int n){
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