#include<bits/stdc++.h>
using namespace std;

const char* path = "data.dat";
const int number = 1000;
FILE* fp;


int main(){
  double data[number];
  for (int i = 0; i < number; i++)
  {
    double l,t;
    l = i;
    t = number;
    data[i] = cos (2*l*M_PI/t);
  }
  
  fp = fopen(path,"w");
  
  for (int i = 0; i < number; i++)
  {
    fprintf(fp,"%d %f\n",i,data[i]);
  }
  fclose(fp);
}