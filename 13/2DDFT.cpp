#include<bits/stdc++.h>
using namespace std;

const char* inputimage = "./sample64.bmp";
const char* outputimage1 = "./original.bmp";
const char* outputimage2 = "./2DFFT.bmp";

const int width = 64;
const int height = 64;
unsigned char header_buf[1078];
unsigned char image_out[height][width];
double realimage[height][width];
double re_array[height][width],im_array[height][width];
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
            realimage[i][j] = (double)image_out[i][j]/255.0;
        }
        
    }
    

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double temp1=0.0;
            double temp2=0.0;
            
            for (int k = 0; k < height; k++)
            {
                for (int l = 0; l < width; l++)
                {
                    temp1 += (double)realimage[k][l]*cos(2.0*M_PI*(i*k/height+j*l/width));
                    temp2 += (double)realimage[k][l]*sin(-2.0*M_PI*(i*k/height+j*l/width));
                }
            }

            re_array[i][j] = temp1;
            im_array[i][j] = temp2;
        }
    }
    



    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pw[i][j] = re_array[i][j]*re_array[i][j] + im_array[i][j]*im_array[i][j];
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
    printf("%lf %lf\n",max, min);
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pw[i][j] = 255.0*((pw[i][j]-min)/(max-min));
            image_out[i][j] = (unsigned char)pw[i][j];
        }
    }
    // for (int i = 0; i < width; i += 4)
    // {
    //     printf("%05lf %05lf %05lf %05lf\n",pw[75][i],pw[75][i+1],pw[75][i+2],pw[75][i+3]);
    // }
    
    fp = fopen(outputimage2,"wb");
    fwrite(header_buf,sizeof(unsigned char),1078,fp);
    fwrite(image_out,sizeof(unsigned char),width*height,fp);
    fclose(fp);

    return 0;


}



