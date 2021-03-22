/******************************************************************************
PROGRAM NAME : Histogram equalization Software (1280*1024 pixels) 
AUTHER : Dai Nakai
DATE : 2021/3/22

Histogram equalization procesing on preprocessed images for PIV.
******************************************************************************/
#include<bits/stdc++.h>
#include<sys/stat.h>
using namespace std;

const int width = 1280;
const int height = 1024;
const int intbit = 256;

const char* read_file_dir = "./retouchedimg";
const char* read_file_header = "retouchedimg";
const char* write_file_dir = "./pivimg";
const char* write_file_header = "pivimg";

unsigned char image_in[height][width];
unsigned char image_out[height][width];
unsigned char header_buf[1078];
unsigned int h[intbit];

char read_file[100];
char write_file[100];
float temp[height][width];

FILE *fp;
/*********************************main****************************************/
int main(){
    mkdir(write_file_dir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

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

        //Processing
        for (int i = 0; i < intbit; i++){
            h[i] = 0;
        }

        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                h[image_in[i][j]]++;
                temp[i][j] = 0;
            }
        }

        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                for (int k = 0; k < image_in[i][j]+1; k++){
                    temp[i][j] += h[k];
                }
            }
        }

        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                image_out[i][j] = (unsigned char)(temp[i][j]*intbit/(height*width)+0.5);
            }
        }

        // Image outputting
        sprintf(write_file, "%s/%s%04d.bmp", write_file_dir, write_file_header, num-1);
        printf("outputting : %s\n", write_file);

        if(!(fp = fopen(write_file,"wb"))){
            printf("outputting : img not valid\n");
            break;
        }

        fwrite(header_buf, sizeof(unsigned char), 1078 , fp);
        fwrite(image_out, sizeof(image_out), 1, fp);
        fclose(fp);

    }

    return 0;
}