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

const int width = 128;
const int height = 128;
unsigned char header_buf[1078];
unsigned char image_out[height][width];
double re_array[height][width],im_array[height][width];
double pw[height][width];
double temp1[height],temp2[width];
double im_temp1[height], im_temp2[width];

FILE *fp;
/*********************************main****************************************/
int main(){
    //Getting the image header
    if(!(fp = fopen(inputimage,"rb"))){
        printf("No image. quitting...\n");
        exit(1);
    }
    fread(header_buf,sizeof(unsigned char),1078,fp);
    fread(image_out,sizeof(image_out),1,fp);
    fclose(fp);

    
    fp = fopen(outputimage1,"wb");
    fwrite(header_buf,sizeof(unsigned char),1078,fp);
    fwrite(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp2[j] = (double)image_out[i][j];
            im_temp2[j] = 0.0;
        }
        S_fft(temp2,im_temp2,width,1);
        for (int j = 0; j < width; j++)
        {
            re_array[i][j] = temp2[j];
            im_array[i][j] = im_temp2[j];
        }
    }

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            temp1[j] = re_array[j][i];
            im_temp1[j] = im_array[j][i];
        }
        S_fft(temp1,im_temp1,height,1);
        for (int j = 0; j < height; j++)
        {
            re_array[j][i] = temp1[j];
            im_array[j][i] = im_temp1[j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pw[i][j] = re_array[i][j]*re_array[i][j] + im_array[i][j]*im_array[i][j];
        }
    }

    double min=100000.0, max=0.0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(pw[i][j] < min){
                min = pw[i][j];
            }
            if(pw[i][j] > max){
                max = pw[i][j];
            }
        }
    }
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pw[i][j] = 255.0*(pw[i][j]-min)/(max-min);
            image_out[i][j] = (unsigned char)pw[i][j];
        }
    }
    
    fp = fopen(outputimage2,"wb");
    fwrite(header_buf,sizeof(unsigned char),1078,fp);
    fwrite(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    return 0;

    
    
    
    
    

    fp = fopen(outputimage2,"wb");
    fwrite(header_buf,sizeof(unsigned char),1078,fp);
    fwrite(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    return 0;
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
