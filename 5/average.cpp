/*************************************************
PROGRAM NAME : average_face.cpp
AUTHER : Yohsuke Tanaka, Dai Nakai
DATE :2021. Jan. 14.
Think a Bit , Code a Bit , Test a Bit
**************************************************/
#include<bits/stdc++.h>
using namespace std;

const int width = 540;
const int height = 360;
const char* input_image = "5.bmp"; // name of input file
const char* output_image = "average_face.bmp"; // name of output file
unsigned char header_buf [1078];
unsigned char image_in [height][width];
unsigned char image_out [height][width];
FILE* infile;
FILE* outfile;
/******************** MAIN **************************/
int main ()
{
  int i,j;

  // Read image data
  infile = fopen(input_image, "rb");
  if(infile == NULL){ printf ("No such an input_image ! \n"); exit (0);}
  fread(header_buf, sizeof(unsigned char), 1078, infile);// Read Header
  fread(image_in, sizeof(image_in), 1, infile);// Read 8bit image intensity
  fclose (infile);

  // Filtering
  double lb,b,rb,l,r,lt,t,rt; //Left, Right, Top, Bottom
  for(i =0;i< height ;i++){
    for(j =0;j< width ;j++){
      
      if(i==0 && j==0){
        lt = 0; t = image_in[i+1][j]; rt = image_in[i+1][j+1];
        l = 0; r = image_in[i][j+1];
        lb = 0; b = 0; rb = 0;
      }
      if(i==0 && j==width-1){
        lt = image_in[i+1][j-1]; t = image_in[i+1][j]; rt = 0;
        l = image_in[i][j-1]; r = 0;
        lb = 0; b = 0; rb = 0;
      }
      if(i==0 && j!=0 && j!=width-1){
        lt = image_in[i+1][j-1]; t = image_in[i+1][j]; rt = image_in[i+1][j+1];
        l = image_in[i][j-1]; r = image_in[i][j+1];
        lb = 0; b = 0; rb = 0;;
      }

      if(i==height-1 && j==0){
        lt = 0; t = 0; rt = 0;
        l = 0; r = image_in[i][j+1];
        lb = 0; b = image_in[i-1][j]; rb = image_in[i-1][j+1];
      }
      if(i==height-1 && j==width-1){
        lt = 0; t = 0; rt = 0;
        l = image_in[i][j-1]; r = 0;
        lb = image_in[i-1][j-1]; b = image_in[i-1][j]; rb = 0;
      }
      if(i==height-1 && j!=0 && j!=width-1){
        lt = 0; t = 0; rt = 0;
        l = image_in[i][j-1]; r = image_in[i][j+1];
        lb = image_in[i-1][j-1]; b = image_in[i-1][j]; rb = image_in[i-1][j+1];
      }

      if(i!=0 && i!=height-1 && j==0){
        lt = 0; t = image_in[i+1][j]; rt = image_in[i+1][j+1];
        l = 0; r = image_in[i][j+1];
        lb = 0; b = image_in[i-1][j]; rb = image_in[i-1][j+1];
      }
      if(i!=0 && i!=height-1 && j==width-1){
        lt = image_in[i+1][j-1]; t = image_in[i+1][j]; rt = 0;
        l = image_in[i][j-1]; r = 0;
        lb = image_in[i-1][j-1]; b = image_in[i-1][j]; rb = 0;
      }
      if(i!=0 && i!=height-1 && j!=0 && j!=width-1){
        lt = image_in[i+1][j-1]; t = image_in[i+1][j]; rt = image_in[i+1][j+1];
        l = image_in[i][j-1]; r = image_in[i][j+1];
        lb = image_in[i-1][j-1]; b = image_in[i-1][j]; rb = image_in[i-1][j+1];
      }

      image_out[i][j] = (lt + t + rt + l + image_in[i][j] + r + lb + b + rb)/9;
    }
  }

  // Write filtered image data
  outfile = fopen(output_image, "wb");
  fwrite(header_buf, sizeof(unsigned char), 1078, outfile);// Write Header
  fwrite(image_out, sizeof(image_out), 1, outfile);// Write 8bit image intensity
  fclose (outfile);

  return 0;
}