/******************************************************************************
PROGRAM NAME : Preprocessing Image for PIV 
AUTHER : Dai Nakai
DATE : 2021/3/20

Comparing 100 images in './img' and get an image composed of the minimum
brightness value of each pixel.
******************************************************************************/
#include<bits/stdc++.h>
#include<sys/stat.h>
#include<omp.h>
using namespace std;

const int width = 1280;
const int height = 1024;

const char* read_file_dir = "./img";
const char* read_file_header = "fc2_save_2014-08-09-141415-";
const char* write_file_dir = "./pivimg";
const char* write_file_header = "pivimg";

unsigned char image_in[height][width];
unsigned char image_out[height][width];
unsigned char header_buf[1078];

char read_file[100];
char write_file[100];

FILE *fp;
/*********************************main****************************************/