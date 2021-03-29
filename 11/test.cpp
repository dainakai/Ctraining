#include<bits/stdc++.h>
using namespace std;

#define PI 4.0*atan(1.0)

FILE *fp;

int main(){
    int i,N=512;

    double dt = 0.01;
    double sc, pw, fq, x[1024], y[1024];

    void S_fft(double* , double *y, int, int);

    sc = 2.0*PI*dt;
    for ( i = 0; i < N; i++)
    {
        x[i] = sin(sc*(double)i)+0.1*sin(sc*(double)(120*i));
        y[i] = 0;
    }
    S_fft(x,y,N,1);

    for ( i = 0; i < (N/2); i++)
    {
        printf(" i = %d %f %f \n",i,x[i],y[i]);
    }

    fp = fopen("fft.dat","w");

    for ( i = 0; i < (N/2); i++)
    {
        pw = sqrt(x[i]*x[i] + y[i]*y[i]);
        fq = (double)i/(dt*(double)N);
        fprintf(fp,"%f %f \n",fq, pw);
    }
    fclose(fp);
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