#include<bits/stdc++.h>
using namespace std;
const int height = 512;
const int width = 512;

void S_fft (double *ak, double *bk, int N, int ff);
void twoDimFFT(double *re, double *im, int height, int width, int flag);

int main(){
    FILE *fp;
    unsigned char header_buf[1078];
    unsigned char *image_in;
    image_in =(unsigned char *)calloc(width*height,sizeof(unsigned char));
    fp = fopen("./outputimage.bmp","rb");
    fread(header_buf,sizeof(unsigned char),1078,fp);
    fread(image_in,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    double *re, *im;
    re = (double *)calloc(width*height,sizeof(double));
    im = (double *)calloc(width*height,sizeof(double));
    
    for (int i = 0; i < width*height; i++)
    {
        *(re + i) = (double)*(image_in + i);
    }

    twoDimFFT(re,im,height,width,1);
    twoDimFFT(re,im,height,width,-1);

    unsigned char *image_out;
    image_out = (unsigned char*)calloc(width*height,sizeof(unsigned char));
    for (int i = 0; i < width*height; i++)
    {
        *(image_out + i) = (unsigned char)*(re + i);
    }
    
    fp = fopen("./testoutput1.bmp","wb");
    fwrite(header_buf,sizeof(unsigned char),1078,fp);
    fwrite(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);
    

    return 0;

}


void twoDimFFT(double *re, double *im, int height, int width, int flag){
    double *re_temp1, *im_temp1, *re_temp2, *im_temp2;

    re_temp1 = (double *)calloc(width,sizeof(double));
    im_temp1 = (double *)calloc(width,sizeof(double));
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            *(re_temp1 + j) = *(re + i*width + j);
            *(im_temp1 + j) = *(im + i*width + j);
        }
        S_fft(re_temp1, im_temp1, width, flag);
        for (int j = 0; j < width; j++){
            *(re + i*width + j) = *(re_temp1 + j);
            *(im + i*width + j) = *(im_temp1 + j);
        }
    }

    re_temp2 = (double *)calloc(height,sizeof(double));
    im_temp2 = (double *)calloc(height,sizeof(double));
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            *(re_temp2 + j) = *(re + i + j*width);
            *(im_temp2 + j) = *(im + i + j*width);
        }
        S_fft(re_temp2, im_temp2, height, flag);
        for (int j = 0; j < height; j++){
            *(re + i + j*width) = *(re_temp2 + j);
            *(im + i + j*width) = *(im_temp2 + j);
        }
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
