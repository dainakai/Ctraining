/******************************************************************************
PROGRAM NAME : TRAINING 1-3
AUTHER : DAI NAKAI
DATE : DEC. 10TH 2020
Think a Bit , Code a Bit , Test a Bit
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include<math.h> //sqrtの呼び出し
const char* input_data_file = "input.dat"; // name of input file
const char* output_data_file = "result3.dat"; // name of output file
const int data_long = 11; // length of data
double data_value [ data_long ]; // Array for data
FILE* input_file ;// pointer of inputfile
FILE* output_file ;// pointer of outputifle
/********************************* MAIN *********************************/
int main (){
  int i;
  double value;
  double avr=0,sd=0,d=0;

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

  // Preparing for output file
  output_file = fopen ( output_data_file ,"w");
  // Write output file
  for(i =0;i< data_long ;i ++){
    avr += data_value[i];
  }
  avr /= data_long;
  
  for (int i = 0; i < data_long; i++)
  {
    d += (data_value[i]-avr)*(data_value[i]-avr)/data_long; //分散の計算
  }

  sd = sqrt(d); //分散のスクエア（標準偏差）
  
  fprintf ( output_file ,"%.3lf\n",sd);
  fclose ( output_file );

  printf("\nDATA S.D. : %.3lf\n",sd);
  printf("DATA S.D. WAS SAVED\n");
  return (0);
}