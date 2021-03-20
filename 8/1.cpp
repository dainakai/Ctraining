/******************************************************************************
PROGRAM NAME : Getting an image for preprocessing of PIV
AUTHER : Dai Nakai
DATE : 2021/3/20

Compare 100 images in './img' and get an image composed of the minimum
brightness value of each pixel as './preprcs_img.bmp'.
******************************************************************************/
#include<bits/stdc++.h>
using namespace std;

const int width = 1280;
const int height = 1024;

const char* read_file_dir = "./img";
const char* read_file_header = "fc2_save_2014-08-09-141415-";
const char* write_file_path = "./preprcs_img.bmp";

unsigned char image_in[height][width];
unsigned char image_out[height][width];
unsigned char header_buf[1078];

char read_file[100];

FILE *fp;
/*********************************main****************************************/
int main(){
    
    //Initializing image_out 
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            image_out[i][j] = 255;
        }   
    }
    

    int num = 0;
    while (1)
    {
        //Image inputting
        num++;
        sprintf(read_file, "%s/%s%04d.bmp", read_file_dir, read_file_header, num-1);
        printf("inputting : %s\n", read_file);

        if(!(fp = fopen(read_file,"rb"))){
            printf("inputting : img not valid\n");
            break;
        }

        fread(header_buf, sizeof(unsigned char), 1078, fp);
        fread(image_in, sizeof(image_in), 1, fp);
        fclose(fp);

        //Getting the minimum of each pixels among 100 images
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                if(image_in[i][j] < image_out[i][j]){
                    image_out[i][j] = image_in[i][j];
                }
            }
        }
    }

    //Image outputting
    printf("outputting : %s\n",write_file_path);

    if(!(fp = fopen(write_file_path,"wb"))){
        printf("outputting : img not valid\n");
        exit(1);
    }
    fwrite(header_buf, sizeof(unsigned char), 1078, fp);
    fwrite(image_out, sizeof(image_out), 1, fp);
    fclose(fp);

    return 0;
}