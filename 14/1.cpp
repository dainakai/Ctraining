/******************************************************************************
PROGRAM NAME : two-dimensional FFT and filtering effect
AUTHER : Dai Nakai
DATE : 2021/4/6
******************************************************************************/
#include<bits/stdc++.h>
using namespace std;

const char* inputimage = "./image.bmp";
const char* outputimage1 = "./2DFFT.bmp";
const char* outputimage2 = "./highPassFiltered2DFFT.bmp";
const char* outputimage2_1 = "./highPassFiltered2DIFFT.bmp";
const char* outputimage3 = "./lowPassFiltered2DFFT.bmp";
const char* outputimage3_1 = "./lowPassFiltered2DIFFT.bmp";

void S_fft (double *ak, double *bk, int N, int ff);
void twoDimFFT(double *re, double *im, int height, int width, int flag);
void highPassFilter (double *re, double *im, int height, int width, int thre);
void lowPassFilter (double *re, double *im, int height, int width, int thre);

const int width = 1024;
const int height = 1024;
unsigned char header_buf[1078];
unsigned char *image_in;
double *re_X, *im_X;
double *re_high, *im_high, *re_low, *im_low;
double *pwr_spc;
double *highpass_spc, *lowpass_spc;
unsigned char *temp1, *temp2;
unsigned char *image_out;
unsigned char *temp_out;

FILE *fp;
/*********************************main****************************************/
int main(){

    image_in = (unsigned char*)calloc(width*height,sizeof(unsigned char));
    if(!(fp = fopen(inputimage,"rb"))){
        printf("No image. quitting...\n");
        exit(1);
    }
    fread(header_buf,sizeof(unsigned char),1078,fp);
    fread(image_in,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    re_X = (double *)calloc(width*height,sizeof(double));
    im_X = (double *)calloc(width*height,sizeof(double));
    for (int i = 0; i < width*height; i++){
        *(re_X + i) = (double)*(image_in + i);
    }

    twoDimFFT(re_X, im_X, height, width, 1);

    //2DFFT
    pwr_spc = (double *)calloc(width*height,sizeof(double));
    for (int i = 0; i < width*height; i++){
        *(pwr_spc + i) = (*(re_X + i))*(*(re_X + i))+(*(im_X + i))*(*(im_X + i));
    }

    double max = *pwr_spc, min = *pwr_spc;
    for (int i = 0; i < width*height; i++){
        if( *(pwr_spc + i) < min) min = *(pwr_spc + i);
        if( *(pwr_spc + i) > max) max = *(pwr_spc + i);
    }

    image_out = (unsigned char *)calloc(width*height, sizeof(unsigned char));
    for (int i = 0; i < width*height; i++){
        *(pwr_spc + i) = log(*(pwr_spc + i)+1);
        *(pwr_spc + i) = 255.0*(*(pwr_spc + i) - log(min+1))/(log(max+1) - log(min+1));
        *(image_out + i) = (unsigned char)(*(pwr_spc + i));
    }

    temp_out = (unsigned char*)calloc(width*height,sizeof(unsigned char));
    for (int i = 0; i < height/2; i++){
        for (int j = 0; j < width/2; j++){
            *(temp_out + j + i*width) = *(image_out + j + i*width);
            *(image_out + j +i*width) = *(image_out + j + width/2 +(i + height/2)*width);
            *(image_out + j + width/2 +(i + height/2)*width) = *(temp_out + j + i*width);
        }
    }
    for (int i = height/2; i < height; i++){
        for (int j = 0; j < width/2; j++){
            *(temp_out + j + i*width) = *(image_out + j + i*width);
            *(image_out + j + i*width) = *(image_out + j + width/2 + (i-height/2)*width);
            *(image_out + j + width/2 + (i-height/2)*width) = *(temp_out + j + i*width);
        }
    }
    
    fp = fopen(outputimage1,"wb");
    fwrite(header_buf,sizeof(unsigned char),1078,fp);
    fwrite(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    //High-pass filter
    re_high = (double *)calloc(width*height, sizeof(double));
    im_high = (double *)calloc(width*height, sizeof(double));
    memcpy(re_high, re_X, sizeof(double)*width*height);
    memcpy(im_high, im_X, sizeof(double)*width*height);

    highPassFilter (re_high, im_high, height, width, 40);

    highpass_spc = (double *)calloc(width*height,sizeof(double));
    for (int i = 0; i < width*height; i++){
        *(highpass_spc + i) = (*(re_high + i))*(*(re_high + i))+(*(im_high + i))*(*(im_high + i));
    }

    max = *highpass_spc, min = *highpass_spc;
    for (int i = 0; i < width*height; i++){
        if( *(highpass_spc + i) < min) min = *(highpass_spc + i);
        if( *(highpass_spc + i) > max) max = *(highpass_spc + i);
    }

    for (int i = 0; i < width*height; i++){
        *(highpass_spc + i) = log(*(highpass_spc + i)+1);
        *(highpass_spc + i) = 255.0*(*(highpass_spc + i) - log(min+1))/(log(max+1) - log(min+1));
        *(image_out + i) = (unsigned char)(*(highpass_spc + i));
    }

    for (int i = 0; i < height/2; i++){
        for (int j = 0; j < width/2; j++){
            *(temp_out + j + i*width) = *(image_out + j + i*width);
            *(image_out + j +i*width) = *(image_out + j + width/2 +(i + height/2)*width);
            *(image_out + j + width/2 +(i + height/2)*width) = *(temp_out + j + i*width);
        }
    }
    for (int i = height/2; i < height; i++){
        for (int j = 0; j < width/2; j++){
            *(temp_out + j + i*width) = *(image_out + j + i*width);
            *(image_out + j + i*width) = *(image_out + j + width/2 + (i-height/2)*width);
            *(image_out + j + width/2 + (i-height/2)*width) = *(temp_out + j + i*width);
        }
    }
    
    fp = fopen(outputimage2,"wb");
    fwrite(header_buf,sizeof(unsigned char),1078,fp);
    fwrite(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    twoDimFFT(re_high, im_high, height, width, -1);
    for (int i = 0; i < width*height; i++){
        *(image_out + i) = (unsigned char)*(re_high + i);
    }
    fp = fopen(outputimage2_1,"wb");
    fwrite(header_buf,sizeof(unsigned char),1078,fp);
    fwrite(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);


    //Low-pass filter
    re_low = (double *)calloc(width*height, sizeof(double));
    im_low = (double *)calloc(width*height, sizeof(double));
    memcpy(re_low, re_X, sizeof(double)*width*height);
    memcpy(im_low, im_X, sizeof(double)*width*height);

    lowPassFilter (re_low, im_low, height, width, 40);

    lowpass_spc = (double *)calloc(width*height,sizeof(double));
    for (int i = 0; i < width*height; i++){
        *(lowpass_spc + i) = (*(re_low + i))*(*(re_low + i))+(*(im_low + i))*(*(im_low + i));
    }

    max = *lowpass_spc, min = *lowpass_spc;
    for (int i = 0; i < width*height; i++){
        if( *(lowpass_spc + i) < min) min = *(lowpass_spc + i);
        if( *(lowpass_spc + i) > max) max = *(lowpass_spc + i);
    }

    for (int i = 0; i < width*height; i++){
        *(lowpass_spc + i) = log(*(lowpass_spc + i)+1);
        *(lowpass_spc + i) = 255.0*(*(lowpass_spc + i) - log(min+1))/(log(max+1) - log(min+1));
        *(image_out + i) = (unsigned char)(*(lowpass_spc + i));
    }

    for (int i = 0; i < height/2; i++){
        for (int j = 0; j < width/2; j++){
            *(temp_out + j + i*width) = *(image_out + j + i*width);
            *(image_out + j +i*width) = *(image_out + j + width/2 +(i + height/2)*width);
            *(image_out + j + width/2 +(i + height/2)*width) = *(temp_out + j + i*width);
        }
    }
    for (int i = height/2; i < height; i++){
        for (int j = 0; j < width/2; j++){
            *(temp_out + j + i*width) = *(image_out + j + i*width);
            *(image_out + j + i*width) = *(image_out + j + width/2 + (i-height/2)*width);
            *(image_out + j + width/2 + (i-height/2)*width) = *(temp_out + j + i*width);
        }
    }
    
    fp = fopen(outputimage3,"wb");
    fwrite(header_buf,sizeof(unsigned char),1078,fp);
    fwrite(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    twoDimFFT(re_low, im_low, height, width, -1);
    for (int i = 0; i < width*height; i++){
        *(image_out + i) = (unsigned char)*(re_low + i);
    }
    fp = fopen(outputimage3_1,"wb");
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

void highPassFilter (double *re, double *im, int height, int width, int thre){
    int c1,c2,c3,c4;
    bool f1,f2,f3,f4;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            c1 = i*i + j*j;
            c2 = i*i + (width-j)*(width-j);
            c3 = (height-i)*(height-i) + j*j;
            c4 = (height-i)*(height-i) + (width-j)*(width-j);

            f1 = (c1 < thre*thre);
            f2 = (c2 < thre*thre);
            f3 = (c3 < thre*thre);
            f4 = (c4 < thre*thre);

            if(f1 || f2 || f3 || f4){
                *(re + j + i*width) = 0.0;
                *(im + j + i*width) = 0.0;
            }
        }
    }
}

void lowPassFilter (double *re, double *im, int height, int width, int thre){
    int c1,c2,c3,c4;
    bool f1,f2,f3,f4;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            c1 = i*i + j*j;
            c2 = i*i + (width-j)*(width-j);
            c3 = (height-i)*(height-i) + j*j;
            c4 = (height-i)*(height-i) + (width-j)*(width-j);

            f1 = (c1 > thre*thre);
            f2 = (c2 > thre*thre);
            f3 = (c3 > thre*thre);
            f4 = (c4 > thre*thre);

            if(f1 && f2 && f3 && f4){
                *(re + j + i*width) = 0.0;
                *(im + j + i*width) = 0.0;
            }
        }
    }
}
