/******************************************************************************
PROGRAM NAME : Preprocessing Images for PIV 
AUTHER : Dai Nakai
DATE : 2021/3/20

Substract preprocessing image brightness from each original image brightness
at each correspond pixel.
******************************************************************************/
#include<bits/stdc++.h>
#include<sys/stat.h>
using namespace std;

const int width = 1280;
const int height = 1024;

const char* read_file_dir = "./img";
const char* read_file_header = "fc2_save_2014-08-09-141415-";
const char* ppimg_path = "./preprcs_img.bmp";
const char* write_file_dir = "./pivimg";
const char* write_file_header = "pivimg";

unsigned char image_in[height][width];
unsigned char ppimg_in[height][width];
unsigned char image_out[height][width];
unsigned char header_buf[1078];

char read_file[100];
char write_file[100];
int temp;

FILE *fp;
/*********************************main****************************************/
int main(){
    mkdir(write_file,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

    if(!(fp = fopen(ppimg_path,"rb"))){
        printf("Preprocessing image not valid\n");
        exit(1);
    }
    fread(header_buf, sizeof(unsigned char), 1078, fp);
    fread(ppimg_in, sizeof(ppimg_in), 1, fp);
    fclose(fp);

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

        //Preprocessing
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                temp = (int)image_in[i][j] - (int)ppimg_in[i][j] + 128;
                if(temp > 255){
                    image_out[i][j] = 255;
                }else{
                    image_out[i][j] = (unsigned char)temp;
                }
            }
        }

        //Image outputting
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