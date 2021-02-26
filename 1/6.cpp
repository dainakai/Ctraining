/******************************************************************************
PROGRAM NAME : TRAINING 1-6
AUTHER : DAI NAKAI
DATE : DEC. 11TH 2020
Think a Bit , Code a Bit , Test a Bit
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
const char* input_data_file = "input.dat"; // name of input file
const char* output_data_file1 = "result6-1.dat"; // name of output files
const char* output_data_file2 = "result6-2.dat";
const char* output_data_file3 = "result6-3.dat";
const char* output_data_file4 = "result6-4.dat";
const int data_long = 11; // length of data
double data_value [ data_long ]; // Array for data
FILE* input_file ;// pointer of inputfile
FILE* output_file ;// pointer of outputifle
/********************************* MAIN *********************************/
int main (){
  int i;
  double value;
  double sum=0,avr,d,sd,med,temp;
  printf (" Read inputdata \n \n");

  // Preparing for Input file
  input_file = fopen (input_data_file ,"r");
  if( input_file == NULL )
  {
    printf ("I can ’t open infile !\n");
    exit (0);
  }
  // Read input file
  for(i = 0;i< data_long ;i++){
    fscanf ( input_file ,"%lf",& value );
    data_value [i]= value ;
    printf (" data_value [%d]= %.3 lf \n",i, data_value [i]);
  }
  fclose ( input_file );

  //DATA SORTING
  for (int i = 0; i < data_long; i++)
  {
    for (int j = 0; j < i; j++)
    {
      if(data_value[i]<data_value[j]){
        temp = data_value[i];
        data_value[i]=data_value[j];
        data_value[j]=temp;
      }
    }
  }
  med = data_value[5];

  //CALC SUM, AVR, SD
  for (int i = 0; i < data_long; i++)
  {
    sum += data_value[i];
  }
  avr = sum / data_long;
  for (int i = 0; i < data_long; i++)
  {
    d += (data_value[i]-avr)*(data_value[i]-avr)/data_long;
  }
  sd = sqrt(d);


  printf (" \n Write outputdata \n\n");

  // SUM OUTPUT
  output_file = fopen ( output_data_file1 ,"w");
  fprintf ( output_file ,"%.3lf\n",sum);
  fclose ( output_file );

  // AVR OUTPUT
  output_file = fopen ( output_data_file2 ,"w");
  fprintf ( output_file ,"%.3lf\n",avr);
  fclose ( output_file );

  // SD OUTPUT
  output_file = fopen ( output_data_file3 ,"w");
  fprintf ( output_file ,"%.3lf\n",sd);
  fclose ( output_file );

  // MED OUTPUT
  output_file = fopen ( output_data_file4 ,"w");
  fprintf ( output_file ,"%.3lf\n",med);
  fclose ( output_file );

  printf("sum :%.3lf\n",sum);
  printf("avr :%.3lf\n",avr);
  printf("sd :%.3lf\n",sd);
  printf("med :%.3lf\n",med);
  printf("VALUES WERE SAVED\n");
  return (0);
}