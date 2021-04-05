/******************************************************************************
PROGRAM NAME : two-dimensional FFT
AUTHER : Dai Nakai
DATE : 2021/4/5
******************************************************************************/
#include<bits/stdc++.h>
using namespace std;

const char* inputimage = "./sample.bmp";
const char* outputimage1 = "./original.bmp";
const char* outputimage2 = "./2DFFT.bmp";

void S_fft (double *ak, double *bk, int N, int ff);
void Two_dim_FFT (double *re_array, double *im_array, int height, int width);

const int width = 128;
const int height = 128;
unsigned char header_buf[1078];
double *re_image_calc;
double *im_image_calc;
double *pwr_spc;
unsigned char *temp;
unsigned char *image_out;

FILE *fp;
/*********************************main****************************************/
int main(){
    image_out = (unsigned char *)malloc(sizeof(unsigned char)*height*width);
    //Getting the image header
    if(!(fp = fopen(inputimage,"rb"))){
        printf("No image. quitting...\n");
        exit(1);
    }
    fread(header_buf,sizeof(unsigned char),1078,fp);
    fread(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    //Generating the original image
    // image_out = (unsigned char *)malloc(sizeof(unsigned char)*height*width);
    // for (int i = 0; i < height; i++){
    //     for (int j = 0; j < width; j++){
    //         *(image_out + i*height + j) = (unsigned char)(127.5*sin(2*M_PI*10.0*(j)/width)+127.5);
    //         // *(image_out + i*height + j) = 255 * ((i+j)%2);
    //     }
    // }
    
    fp = fopen(outputimage1,"wb");
    fwrite(header_buf,sizeof(unsigned char),1078,fp);
    fwrite(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    re_image_calc = (double *)malloc(sizeof(double)*width*height);
    im_image_calc = (double *)malloc(sizeof(double)*width*height);
    for (int i = 0; i < width*height; i++){
        *(re_image_calc + i) = (double)*(image_out + i);
        *(im_image_calc + i) = 0.0;
    }
    
    Two_dim_FFT(re_image_calc, im_image_calc, height, width);

    pwr_spc = (double *)malloc(sizeof(double)*width*height);
    double min,max; min = 1000000.0;  max = 0.0;
    for (int i = 0; i < width*height; i++){
        *(pwr_spc + i) = (*(re_image_calc + i))*(*(re_image_calc + i)) + (*(im_image_calc + i))*(*(im_image_calc + i));
        if(*(pwr_spc+i) < min){ min = *(pwr_spc + i); }
        if(*(pwr_spc+i) > max){ max = *(pwr_spc + i); }
        // printf("%lf\n",*(pwr_spc + i));
    }
    for (int i = 0; i < width*height; i++){
        // *(pwr_spc + i) = 255.0*((*(pwr_spc + i) - min)/(max-min));
        *(image_out + i) = (unsigned char)(255.0*((*(pwr_spc + i) - min)/(max-min)));
    }

    // temp = (unsigned char *)malloc(sizeof(unsigned char)*width*height/4);
    // for (int i = 0; i < height/2; i++){
    //     for (int j = 0; j < width/2; j++){
    //         *(temp + j +i*width) = *(image_out + j + i*width);
    //         *(image_out + j + i*width) = *(image_out + j+width/2 + (i + height/2)*width);
    //         *(image_out + j+width/2 + (i + height/2)*width) = *(temp + j +i*width);
    //     }
    // }
    // for (int i = 0; i < height/2; i++){
    //     for (int j = width/2; j < width; j++){
    //         *(temp + j +i*width) = *(image_out + j + i*width);
    //         *(image_out + j + i*width) = *(image_out + j-width/2 + (i + height/2)*width);
    //         *(image_out + j-width/2 + (i + height/2)*width) = *(temp + j +i*width);
    //     }
    // }
    

    fp = fopen(outputimage2,"wb");
    fwrite(header_buf,sizeof(unsigned char),1078,fp);
    fwrite(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    return 0;
}


void Two_dim_FFT (double *re_array, double *im_array, int height, int width){
    double *re_temp, *im_temp;
    re_temp = (double *)malloc(sizeof(double)*height);
    im_temp = (double *)malloc(sizeof(double)*height);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            *(re_temp + j) = *(re_array+i*width + j);
            *(im_temp + j) = 0.0;
        }
        S_fft(re_temp, im_temp, width, 1);
        for (int j = 0; j < width; j++)
        {
            *(re_array + i*width + j) = *(re_temp + j);
            *(im_array + i*width + j) = *(im_temp + j);
        }
    }
    
    // for (int i = 0; i < height; i++){
    //     S_fft((re_array + i*width), (im_array + i*width), width, 1);
    // }


    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            *(re_temp + j) = *(re_array + i + j*width);
            *(im_temp + j) = 0.0;
        }
        S_fft(re_temp, im_temp, height, 1);
        for (int j = 0; j < height; j++){
            *(re_array + i + j*height) = *(re_temp + j);
            *(im_array + i + j*height) = *(im_temp + j);
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
