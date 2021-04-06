#include<bits/stdc++.h>
using namespace std;

const char* inputimage = "./sample.bmp";
const char* outputimage1 = "./original.bmp";
const char* outputimage2 = "./2DFFT.bmp";

const int width = 128;
const int height = 128;
unsigned char header_buf[1078];
unsigned char image_out[height][width];
double realimage[height][width];
double re_array[height][width],im_array[height][width];
double re_X[height][width],im_X[height][width];
double pw[height][width];


FILE *fp;

int main(){
    //Getting the image header
    if(!(fp = fopen(inputimage,"rb"))){
        printf("No image. quitting...\n");
        exit(1);
    }
    fread(header_buf,sizeof(unsigned char),1078,fp);
    fread(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    
    fp = fopen(outputimage1,"wb");
    fwrite(header_buf,sizeof(unsigned char),1078,fp);
    fwrite(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    for (int i = 0; i < width; i += 4)
    {
        printf("%03d %03d %03d %03d\n",image_out[25][i],image_out[25][i+1],image_out[25][i+2],image_out[25][i+3]);
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            realimage[i][j] = (double)image_out[i][j];
        }
        
    }
    

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < height; k++)
            {
                re_array[i][j] += realimage[k][j]*cos(2.0*M_PI*((double)i*k/height));
                im_array[i][j] += realimage[k][j]*(-1.0)*sin(2.0*M_PI*((double)i*k/height));
            }
                printf("re_array[%d][%d] : %lf\n",i,j,im_array[i][j]);
        }
    }


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < width; k++)
            {
                re_X[i][j] += re_array[i][k]*cos(2.0*M_PI*((double)j*k/width)) + im_array[i][k]*sin(2.0*M_PI*((double)j*k/width));
                im_X[i][j] += re_array[i][k]*sin(-2.0*M_PI*((double)j*k/width)) + im_array[i][k]*cos(2.0*M_PI*((double)j*k/width));
            }
            
        }
        
    }
    
    



    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pw[i][j] = re_X[i][j]*re_X[i][j] + im_X[i][j]*im_X[i][j];
        }
    }

    for (int i = 0; i < width; i += 4)
    {
        printf("%05lf %05lf %05lf %05lf\n",pw[25][i],pw[25][i+1],pw[25][i+2],pw[25][i+3]);
    }
    
    

    double min=pw[0][0], max=pw[0][0];
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
    printf("max : %lf, min : %lf\n",max, min);
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // pw[i][j] = 255.0*log(99999999999.0*(pw[i][j]-min)/(max-min)+1.0)/log(100000000000);
            pw [i][j] = log(pw[i][j]);
            pw[i][j] = 255.0*(pw[i][j] - log(min))/(log(max)-log(min));
            printf("%lf\n",pw[i][j]);
            image_out[i][j] = (unsigned char)pw[i][j];
        }
    }
    unsigned char temp[height][width];
    for (int i = 0; i < height/2; i++)
    {
        for (int j = 0; j < width/2; j++)
        {
            temp[i][j] = image_out[i+height/2][j+width/2];
            image_out[i+height/2][j+width/2] = image_out[i][j];
            image_out[i][j] = temp[i][j];
        }
    }

    for (int i = height/2; i < height; i++)
    {
        for (int j = 0; j < width/2; j++)
        {
            temp[i][j] = image_out[i-height/2][j+width/2];
            image_out[i-height/2][j+width/2] = image_out[i][j];
            image_out[i][j] = temp[i][j];
        }
        
    }
    
    fp = fopen(outputimage2,"wb");
    fwrite(header_buf,sizeof(unsigned char),1078,fp);
    fwrite(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    return 0;


}



