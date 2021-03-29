/******************************************************************************
PROGRAM NAME : FFT Verification
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
const char* output_header1 = "wave";
const char* output_header2 = "FFT";
const char* output_time = "timedat";
char file_path_dat[100];
char file_path_img[100];

const int A = 20; //Frequency parameter 1
const int B = 5; // 2
const double t_length =1.0; // Measuring time

double pw, fq;

const int num_p = 10;
int N[num_p];

double cpu_time[2*num_p];

FILE *fp;
FILE *gp;
/*********************************main****************************************/
int main(){
    mkdir(output_data_dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    mkdir(output_img_dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

    N[0] = 1<<6; //N[0] = 64
    for (int i = 1; i < num_p; i++){
        N[i] = N[i-1]<<1;
    }
    
    for (int i = 0; i < num_p; i++){
        double *re_f, *im_f;
        re_f = (double *)malloc(sizeof(double)*N[i]);
        im_f = (double *)malloc(sizeof(double)*N[i]);
        Ini_val(re_f, im_f, N[i], A, B);

        sprintf(file_path_dat, "%s/%s%05d.dat",output_data_dir,output_header1,N[i]);
        fp = fopen(file_path_dat,"w");
        for (int j = 0; j < N[i]; j++){
            fprintf(fp,"%lf %lf\n",t_length*(double)j/(double)N[i],re_f[j]);
        }
        fclose(fp);

        sprintf(file_path_img, "%s/%s%05d.png",output_img_dir,output_header1,N[i]);
        gp = popen("gnuplot","w");
        fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15'\n");
	    fprintf(gp,"set output '%s'\n",file_path_img);
	    fprintf(gp,"set key right top\n");
	    fprintf(gp,"set size ratio 0.8\n");
        fprintf(gp,"set xlabel '{/Times-New-Roman:Italic=20 t} [sec]'\n");
        fprintf(gp,"plot '%s' using 1:2 with lines title 'N = %d'\n",file_path_dat,N[i]);
        pclose(gp);

        cpu_time[2*i] = clock()/1000.0;

        S_fft(re_f, im_f, N[i], 1);

        cpu_time[2*i+1] = clock()/1000.0;

        sprintf(file_path_dat, "%s/%s%05d.dat",output_data_dir,output_header2,N[i]);
        fp = fopen(file_path_dat,"w");
        for (int j = 0; j < (N[i]/2); j++){
            pw = sqrt(re_f[j]*re_f[j] + im_f[j]*im_f[j]);
            fq = (double)j/t_length;
            fprintf(fp,"%f %f \n",fq, pw);
        }
        fclose(fp);

        sprintf(file_path_img, "%s/%s%05d.png",output_img_dir,output_header2,N[i]);
        gp = popen ("gnuplot","w");
        fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15'\n");
	    fprintf(gp,"set output '%s'\n",file_path_img);
	    fprintf(gp,"set key right top\n");
	    fprintf(gp,"set size ratio 0.8\n");
        fprintf(gp,"set xlabel '{/Times-New-Roman:Italic=20 f} [Hz]'\n");
        fprintf(gp,"set xrange [0:40]\n");
        fprintf(gp,"plot '%s' using 1:2 with lines title 'N = %d'\n",file_path_dat,N[i]);
        pclose(gp);

        free(re_f);
        free(im_f);
    }
    
    sprintf(file_path_dat,"%s/%s.dat",output_data_dir,output_time);
    fp = fopen(file_path_dat,"w");
    for (int i = 0; i < num_p; i++){
        fprintf(fp,"%d %lf\n",N[i],cpu_time[2*i+1]-cpu_time[2*i]);
    }
    fclose(fp);

    sprintf(file_path_img,"%s/%s.png",output_img_dir,output_time);
    gp = popen("gnuplot","w");
    fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15'\n");
	fprintf(gp,"set output '%s'\n",file_path_img);
	fprintf(gp,"unset key\n");
    fprintf(gp,"set size ratio 0.8\n");
    fprintf(gp,"set xlabel 'Number of discrete value {/Times-New-Roman:Italic=20 N} [-]'\n");
    // fprintf(gp,"set xrange [0:40]\n");
    fprintf(gp,"plot '%s' using 1:2 with linespoints\n",file_path_dat);
    pclose(gp);

    return 0;
}

void Ini_val(double *re_f, double *im_f, int N, int A, int B){
    for (int i = 0; i < N; i++){
        re_f[i] = sin ((double)A*2.0*M_PI*(double)i/(double)N)*cos((double)B*2.0*M_PI*(double)i/(double)N);
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