/******************************************************************************
PROGRAM NAME : FFT Software (N=1024)
AUTHER : Dai Nakai
DATE : 2021/3/29
******************************************************************************/
#include<bits/stdc++.h>
#include<sys/stat.h>
using namespace std;

void S_fft (double *ak, double *bk, int N, int ff);
void Ini_val (double *re_f, double *im_f, int N, int A, int B);

const char* output_data_dir = "./data";
const char* output_img_dir = "./img";

const int A = 20; //Frequency parameter 1
const int B = 5; // 2
const int N = 1024; // 2^m
const double t_length =1.0; // Measuring time

FILE *fp;
FILE *gp;
/*********************************main****************************************/
int main () {
    mkdir(output_data_dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    mkdir(output_img_dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

    double *re_f, *im_f;
    re_f = (double *)malloc(sizeof(double)*N);
    im_f = (double *)malloc(sizeof(double)*N);
    
    Ini_val(re_f, im_f, N, A, B);

    fp = fopen("./data/Ex1-wave.dat","w");
    for (int i = 0; i < N; i++){
        fprintf(fp,"%f %f \n",t_length*(double)i/(double)N, re_f[i]);
    }
    fclose(fp); 

    S_fft(re_f, im_f, N, -1);

    double pw, fq;
    fp = fopen("./data/Ex1-FFT.dat","w");
    for (int i = 0; i < (N/2); i++){
        pw = sqrt(re_f[i]*re_f[i] + im_f[i]*im_f[i]);
        fq = (double)i/t_length;
        fprintf(fp,"%f %f \n",fq, pw);
    }
    fclose(fp);

    free(re_f);
    free(im_f);

    gp = popen ("gnuplot","w");
    fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15'\n");
	fprintf(gp,"set output './img/Ex1-wave.png'\n");
	fprintf(gp,"unset key\n");
	fprintf(gp,"set size ratio 0.8\n");
    fprintf(gp,"set xlabel '{/Times-New-Roman:Italic=20 t} [sec]'\n");
    fprintf(gp,"plot './data/Ex1-wave.dat' using 1:2 with lines\n");
    pclose(gp);

    gp = popen ("gnuplot","w");
    fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15'\n");
	fprintf(gp,"set output './img/Ex1-FFT.png'\n");
	fprintf(gp,"unset key\n");
	fprintf(gp,"set size ratio 1\n");
    fprintf(gp,"set xlabel '{/Times-New-Roman:Italic=20 f} [Hz]'\n");
    fprintf(gp,"set xrange [0:40]\n");
    fprintf(gp,"plot './data/Ex1-FFT.dat' using 1:2 with lines\n");
    pclose(gp);

    return 0;
}

void Ini_val(double *re_f, double *im_f, int N, int pA, int pB){
    for (int i = 0; i < N; i++){
        re_f[i] = sin ((double)pA*2.0*M_PI*(double)i/(double)N)*cos((double)pB*2.0*M_PI*(double)i/(double)N);
        im_f[i] = 0.0;
    }
}

void S_fft(double *ak, double *bk, int N, int ff){
    int i,j,k,k1,num,nhalf,phi,phi0,rot[N];
    double s,sc,c,a0,b0,tmp;

    for (i = 0; i < N; i++){
        rot[i] = 0;
    }

    nhalf = N/2;
    num = N/2;
    sc = 2.0*M_PI/(double)N;

    while(num>=1){
        for (j = 0; j < N; j+=(2*num)){
            phi = rot[j]/2;
            phi0 = phi + nhalf;
            c = cos(sc*(double)phi);
            s = sin(sc*(double)(phi*ff));

            for(k=j; k<j+num; k++){
                k1 = k+num;
                a0 = ak[k1]*c - bk[k1]*s;
                b0 = ak[k1]*s + bk[k1]*c;
                ak[k1] = ak[k]-a0;
                bk[k1] = bk[k]-b0;
                ak[k] +=a0;
                bk[k] += b0;
                rot[k] = phi;
                rot[k1] = phi0;
            }
        }
        num /= 2;
    }

    if(ff<0){
        for ( i = 0; i < N; i++){
            ak[i] /= (double)N;
            bk[i] /= (double)N;
        }
    }

    for(i=0; i<N-1; i++){
        if((j=rot[i])>i){
            tmp=ak[i]; ak[i] = ak[j]; ak[j]=tmp;
            tmp=bk[i]; bk[i] = bk[j]; bk[j]=tmp;
        }
    }
}