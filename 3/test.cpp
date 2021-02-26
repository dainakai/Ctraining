#include<bits/stdc++.h>
using namespace std;
const char* read_file_dir = "01_plot_vec_vortex";
const char* read_file_header = "2dvec_vortex";

int main(){
  FILE *fp;
  char read_file[40];
  int i=1;
  sprintf(read_file,"./%s/%s%06d.dat",read_file_dir,read_file_header,i);
  fp = fopen(read_file,"r");
  if(fp == NULL ){
    cout << "error!" << endl;
    // exit (0);
  }
  double j;
  fscanf(fp,"%lf",&j);
  cout << j << endl;
  fclose(fp);
  return 0;
  // int i=1;
  // printf("%06d.dat\n",i);
  // return 0;

}