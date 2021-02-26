/*************************************************
PROGRAM NAME : laplacian_face.cpp
AUTHER : Yohsuke Tanaka, Dai Nakai
DATE :2021. Jan. 14.
Think a Bit , Code a Bit , Test a Bit
**************************************************/
#include<bits/stdc++.h>
using namespace std;

const int width = 540;
const int height = 360;
const char* input_image = "5.bmp"; // name of input file
const char* output_image = "laplacian_face.bmp"; // name of output file
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
  char l,r,u,d;
  for(i =0;i< height ;i++){
    for(j =0;j< width ;j++){
      if(i==0){
        l = 0;
      }else if(i==height-1){
        r = 0;
      }else{
        l = image_in[i-1][j];
        r = image_in[i+1][j];
      }

      if(j==0){
        d = 0;
      }else if(j==width-1){
        u = 0;
      }else{
        d = image_in[i][j-1];
        u = image_in[i][j+1];
      }
      
      image_out[i][j] = l + r + u + d - 4*image_in[i][j]+128;
      if(image_out[i][j] < 0){
        image_out[i][j] = 0;
      }
      if(image_out[i][j] > 255){
        image_out[i][j] = 255;
      }
    }
  }

  // Write filtered image data
  outfile = fopen(output_image, "wb");
  fwrite(header_buf, sizeof(unsigned char), 1078, outfile);// Write Header
  fwrite(image_out, sizeof(image_out), 1, outfile);// Write 8bit image intensity
  fclose (outfile);

  return 0;
}