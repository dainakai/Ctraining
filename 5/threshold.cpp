/*************************************************
PROGRAM NAME : filtered_face.cpp
AUTHER : Yohsuke Tanaka, Dai Nakai
DATE :2021. Jan. 14.
Think a Bit , Code a Bit , Test a Bit
**************************************************/
#include<bits/stdc++.h>
using namespace std;

const int width = 540;
const int height = 360;
const int thred = 60;
const char* input_image = "5.bmp"; // name of input file
const char* output_image = "filtered_face.bmp"; // name of output file
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
  for(i =0;i< height ;i ++){
    for(j =0;j< width ;j ++){
      if( image_in [i][j]> thred ){
        image_out [i][j]=255;
      }else {
        image_out [i][j]=0;
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