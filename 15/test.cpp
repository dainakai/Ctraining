#include<bits/stdc++.h>
using namespace std;
const int height = 128;
const int width = 128;

void S_fft (double *ak, double *bk, int N, int ff);
void twoDimFFT(double re[height][width], double im[height][width], int flag);

unsigned char header_buf[1078];
unsigned char image_in[height][width];
unsigned char image_out[height][width];

double re_array[height][width];
double im_array[height][width];

FILE *fp;

int main(){
    fp = fopen("sample.bmp","rb");
    fread(header_buf,sizeof(unsigned char),1078,fp);
    fread(image_in,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            re_array[i][j] = (double)image_in[i][j];
        }
    }

    twoDimFFT(re_array,im_array,1);
    twoDimFFT(re_array,im_array,-1);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_out[i][j] = (unsigned char)re_array[i][j];
        }
        
    }
    

    fp = fopen("test1.bmp","wb");
    fwrite(header_buf,sizeof(unsigned char),1078,fp);
    fwrite(image_out,sizeof(unsigned char),height*width,fp);
    fclose(fp);
    


    return 0;
    
}

void twoDimFFT(double re[height][width], double im[height][width], int flag){
    double re_temp1[width], im_temp1[width], re_temp2[height], im_temp2[height];

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            re_temp1[j] = re[i][j];
            im_temp1[j] = im[i][j];
        }
        S_fft(re_temp1,im_temp1,width,flag);
        for (int j = 0; j < width; j++){
            re[i][j] = re_temp1[j];
            im[i][j] = im_temp1[j];
        }
    }

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            re_temp2[j] = re[j][i];
            im_temp2[j] = im[j][i];
        }
        S_fft(re_temp2,im_temp2,height,flag);
        for (int j = 0; j < height; j++)
        {
            re[j][i] = re_temp2[j];
            im[j][i] = im_temp2[j];
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
