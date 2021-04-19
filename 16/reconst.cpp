/******************************************************************************
PROGRAM NAME : Digital Holography reconstructing and  verification 
AUTHER : Dai Nakai
DATE : 2021/4/17
******************************************************************************/
#include<bits/stdc++.h>
#include<sys/stat.h>
using namespace std;

const char* refimg = "./refimg.bmp"; // 2000 * 512 image
const char* input_hologram = "./twinparticleholography.bmp";
const char* output_dir = "./test_bmpimages";
const char* z_x_imagepath = "./test_z_x_image.bmp";
const char* intensity_image = "test_intensity";
const char* graph_title = "Twin Particle Intensity Distribution";

const int height = 512;
const int width = 512;
const double dx = 10.0;
const double diam = 80.0;
const double posi_x = 2560.0;
const double posi_y = 2560.0;
const double wave_length = 0.6328;
const double peak_bright = 127;
const double max_z = 20000.0;
const double min_z = 0.0;
const double step_z = 10.0;
const int num = (int)((max_z-min_z)/step_z);

unsigned char header_buf[1078];
unsigned char header_buf2[1078];
char imagename[100];
unsigned char image_in[height][width];
unsigned char image_out[height][width];
unsigned char z_x_image[width][num];
double re_hologram[height][width];
double im_hologram[height][width];
double re_trans[height][width];
double im_trans[height][width];
double re_object[height][width];
double im_object[height][width];
double particle_intensity[num];

void S_fft (double *ak, double *bk, int N, int ff);
void twoDimFFT(double re[height][width], double im[height][width], int flag);
void trans_func(double re[height][width], double im[height][width],double posi_z);

FILE *fp;
FILE *gp;
/*********************************main****************************************/
int main(){
    mkdir(output_dir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

    fp = fopen(input_hologram,"rb");
    fread(header_buf,sizeof(unsigned char),1078,fp);
    fread(image_in,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            re_hologram[i][j] = (double)image_in[i][j];
        }
    }

    twoDimFFT(re_hologram,im_hologram,1);

    for (int i = 0; i < num; i++){
        trans_func(re_trans,im_trans,i*step_z + min_z);
        for (int j = 0; j < height; j++){
            for (int k = 0; k < width; k++){
                re_object[j][k] = re_hologram[j][k]*re_trans[j][k] - im_hologram[j][k]*im_trans[j][k];
                im_object[j][k] = re_hologram[j][k]*im_trans[j][k] + im_hologram[j][k]*re_trans[j][k];
            }
        }
        twoDimFFT(re_object,im_object,-1);
        for (int j = 0; j < height; j++){
            for (int k = 0; k < width; k++){
                double tmp;
                tmp = sqrt(re_object[j][k]*re_object[j][k] + im_object[j][k]*im_object[j][k]);
                if (tmp <= 255.0){
                    image_out[j][k] = (unsigned char)(tmp);
                }else{
                    image_out[j][k] = 255;
                }

            }
        }

        particle_intensity[i] = (double)image_out[(int)(posi_y/dx)][(int)(posi_x/dx)];
        
        sprintf(imagename,"%s/%05d.bmp",output_dir,i);
        fp = fopen(imagename,"wb");
        fwrite(header_buf,sizeof(unsigned char),1078,fp);
        fwrite(image_out,sizeof(unsigned char),width*height,fp);
        fclose(fp);

        for (int j = 0; j < width; j++){
            z_x_image[j][i] = image_out[(int)(posi_y/dx)][j];
        }

        printf("%d th / %d is processed.\n",i+1,num);
    }

    fp = fopen(refimg,"rb");
    fread(header_buf2,sizeof(unsigned char),1078,fp);
    fclose(fp);

    fp = fopen(z_x_imagepath,"wb");
    fwrite(header_buf2,sizeof(unsigned char),1078,fp);
    fwrite(z_x_image,sizeof(unsigned char),width*num,fp);
    fclose(fp);

    if(!(gp = popen("gnuplot","w"))){
        printf("Gnuplot doesn't work. Software quiting...\n");
    }
    fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15'\n");
    fprintf(gp,"set output './%s.png'\n",intensity_image);
    fprintf(gp,"set key right top\n");
    fprintf(gp,"set size ratio 0.8\n");
    fprintf(gp,"set xlabel '{/Times-New-Roman:Italic=20 z} [{/Symbol m}m]'\n");
    fprintf(gp,"set xrange [0:20000]\n");
    fprintf(gp,"set yrange [0:]\n");
    fprintf(gp,"set ylabel 'Particle Intensity [-]'\n");
    fprintf(gp,"array data[%d]\n",num);
    for (int i = 0; i < num; i++){
        fprintf(gp,"data[%d] = %lf\n", i+1, particle_intensity[i]);
    }
    
    fprintf(gp,"plot data using ($1-1)*%lf:2 with lines title '%s'\n",step_z,graph_title);
    pclose(gp);

    return 0;
}

void trans_func(double re[height][width], double im[height][width],double posi_z){
    double tmp, C[3];
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){   
            C[0] = 2.0*M_PI*posi_z/wave_length;
            C[1] = wave_length*wave_length/width/width/dx/dx;
            C[2] = wave_length*wave_length/height/height/dx/dx;
            tmp = C[0]*sqrt(1.0-C[1]*(j-width/2.0)*(j-width/2.0)-C[2]*(i-height/2.0)*(i-height/2.0));
            re[i][j] = cos(tmp);
            im[i][j] = sin(tmp);
        }
    }
}

void twoDimFFT(double re[height][width], double im[height][width], int flag){
    double re_temp1[width], im_temp1[width], re_temp2[height], im_temp2[height];

    if((flag != 1) && (flag != -1)){
        printf("flag of FFT must be either 1 or -1. Software quitting... \n");
        exit(1);
    }

    if(flag == -1){
        double re_array[height][width], im_array[height][width];
        for (int i = 0; i < height/2; i++){
            for (int j = 0; j < width/2; j++){
                re_array[i][j] = re[i + height/2][j + width/2];
                im_array[i][j] = im[i + height/2][j + width/2];
                re[i + height/2][j + width/2] = re[i][j];
                im[i + height/2][j + width/2] = im[i][j];
                re[i][j] = re_array[i][j];
                im[i][j] = im_array[i][j];
            }
        }

        for (int i = height/2; i < height; i++)
        {
            for (int j = 0; j < width/2; j++)
            {
                re_array[i][j] = re[i - height/2][j + width/2];
                im_array[i][j] = im[i - height/2][j + width/2];
                re[i - height/2][j + width/2] = re[i][j];
                im[i - height/2][j + width/2] = im[i][j];
                re[i][j] = re_array[i][j];
                im[i][j] = im_array[i][j];
            }
        }
    }

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

    if(flag == 1){
        double re_array[height][width], im_array[height][width];
        for (int i = 0; i < height/2; i++){
            for (int j = 0; j < width/2; j++){
                re_array[i][j] = re[i + height/2][j + width/2];
                im_array[i][j] = im[i + height/2][j + width/2];
                re[i + height/2][j + width/2] = re[i][j];
                im[i + height/2][j + width/2] = im[i][j];
                re[i][j] = re_array[i][j];
                im[i][j] = im_array[i][j];
            }
        }

        for (int i = height/2; i < height; i++)
        {
            for (int j = 0; j < width/2; j++)
            {
                re_array[i][j] = re[i - height/2][j + width/2];
                im_array[i][j] = im[i - height/2][j + width/2];
                re[i - height/2][j + width/2] = re[i][j];
                im[i - height/2][j + width/2] = im[i][j];
                re[i][j] = re_array[i][j];
                im[i][j] = im_array[i][j];
            }
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
