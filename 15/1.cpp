/******************************************************************************
PROGRAM NAME : Digital holography generating
AUTHER : Dai Nakai
DATE : 2021/4/12
******************************************************************************/
#include<bits/stdc++.h>
using namespace std;

void S_fft (double *ak, double *bk, int N, int ff);
void twoDimFFT(double *re, double *im, int height, int width, int flag);

const char* refimg = "./512.bmp";
const char* image = "./outputimage.bmp";
const char* hologram = "./outputholography.bmp";
char* header_buf[1078];

const int height = 512;
const int width = 512;
const double dx = 10.0;
const double diam = 80.0;
const double posi_x = 2560.0;
const double posi_y = 2560.0;
const double posi_z = 10000.0;
const double wave_length = 0.6328;

unsigned char* image_out;
double *re_object_plane;
double *im_object_plane;
double *re_trans_func;
double *im_trans_func;
double *re_hologram_plane;
double *im_hologram_plane;
double *wave_strength;

FILE *fp;
/*********************************main****************************************/
int main () {
    if(!(fp = fopen(refimg,"rb"))){
        printf("No reference image. quitting...\n");
        exit(1);
    }
    fread(header_buf,sizeof(unsigned char),1078,fp);
    fclose(fp);

    re_object_plane = (double *)calloc(width*height,sizeof(double));
    im_object_plane = (double *)calloc(width*height,sizeof(double));
    re_trans_func = (double *)calloc(width*height,sizeof(double));
    im_trans_func = (double *)calloc(width*height,sizeof(double));
    re_hologram_plane = (double *)calloc(width*height,sizeof(double));
    im_hologram_plane = (double *)calloc(width*height,sizeof(double));
    image_out = (unsigned char *)calloc(height*width,sizeof(unsigned char));

    double temp;
    double const1,const2,const3;
    const1 = 1.0/wave_length/wave_length;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if( ((double)j*dx - posi_x)*((double)j*dx - posi_x) + ((double)i*dx - posi_y)*((double)i*dx - posi_y) > diam*diam/4.0){
                *(re_object_plane + j + i*width) = 255.0;
            }
            *(image_out + j + i*width) = (unsigned char)*(re_object_plane + j + i*width);

            const2 = ((double)i - height/2.0)*((double)i - height/2.0)/height/height/dx/dx;
            const3 = ((double)j - width/2.0)*((double)j - width/2.0)/width/width/dx/dx;
            temp = 2.0*M_PI*posi_z*sqrt(const1-const2-const3);
            *(re_trans_func + j + i*width) = cos(temp);
            *(im_trans_func + j + i*width) = sin(temp);
        }
    }

    fp = fopen(image,"wb");
    fwrite(header_buf,sizeof(unsigned char),1078,fp);
    fwrite(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);
    
    twoDimFFT(re_object_plane,im_object_plane,height,width,1);

    for (int i = 0; i < width*height; i++){
        *(re_hologram_plane + i) = *(re_object_plane + i) * *(re_trans_func + i) - *(im_object_plane + i) * *(im_trans_func + i);
        *(im_hologram_plane + i) = *(re_object_plane + i) * *(im_trans_func + i) + *(im_object_plane + i) * *(re_trans_func + i);
    }

    twoDimFFT(re_hologram_plane,im_hologram_plane,height,width,-1);

    wave_strength = (double *)calloc(height*width,sizeof(double));
    for (int i = 0; i < width*height; i++){
        *(wave_strength + i) = *(re_hologram_plane + i) * *(re_hologram_plane + i) + *(im_hologram_plane + i)* *(im_hologram_plane + i) + 1.0 + 2.0 * *(re_hologram_plane + i);
    }

    // double min = *wave_strength, max = *wave_strength;
    // for (int i = 0; i < width*height; i++){
    //     if(*(wave_strength + i) > max) max = *(wave_strength + i);
    //     if(*(wave_strength + i) < min) min = *(wave_strength + i);
    // }
    // for (int i = 0; i < width*height; i++){
    //     *(wave_strength + i) = 255.0*(*(wave_strength + i) - min)/(max-min);
    //     *(image_out + i) = (unsigned char)*(wave_strength + i);
    // }

    for (int i = 0; i < width*height; i++)
    {
        *(wave_strength + i) = sqrt(*(wave_strength + i));
        *(image_out + i) = (unsigned char)*(wave_strength + i);
    }
    


    fp = fopen(hologram,"wb");
    fwrite(header_buf,sizeof(unsigned char),1078,fp);
    fwrite(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    free(image_out);
    free(re_object_plane);
    free(im_object_plane);
    free(re_trans_func);
    free(im_trans_func);
    free(re_hologram_plane);
    free(im_hologram_plane);
    free(wave_strength);

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

    free(re_temp1);
    free(im_temp1);
    free(re_temp2);
    free(im_temp2);
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
