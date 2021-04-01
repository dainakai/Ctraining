/******************************************************************************
PROGRAM NAME : FFT with window function
AUTHER : Dai Nakai
DATE : 2021/4/1
******************************************************************************/
#include<bits/stdc++.h>
#include<sys/stat.h>
using namespace std;

void S_fft (double *ak, double *bk, int N, int ff);
void Ini_val (double *re_f, double *im_f, int N);

/*********************************main****************************************/
int main(){


    return 0;
}

void Ini_val(double *re_f, double *im_f, int N){
    for (int i = 0; i < N; i++){
        re_f[i] = sin (2.0*M_PI*1.0*(double)i/(double)N) + sin (2.0*M_PI*3.0*(double)i/(double)N) + sin (2.0*M_PI*6.0*(double)i/(double)N);
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