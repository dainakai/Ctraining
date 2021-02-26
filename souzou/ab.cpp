#include<bits/stdc++.h>
using namespace std;

int main(){
  double m[3],t[3];
  m[0] = 0.2; m[1] = 0.25; m[2] = 0.3;
  t[0] = 32.128; t[1] = 23.456; t[2] = 16.696;
  double g = 9.80;

  double a[3],b[3],v[3];

  int i,j;

  for (int k = 0; k < 3; k++)
  {
    v[k] = 0.7/t[k];
  }
  
  i=0;j=1;
  b[i] = (m[i]-m[j])/(v[i]-v[j])*g;
  a[i] = (m[j]*v[i]-m[i]*v[j])/(v[i]-v[j])*g;

  i=1;j=2;
  b[i] = (m[i]-m[j])/(v[i]-v[j])*g;
  a[i] = (m[j]*v[i]-m[i]*v[j])/(v[i]-v[j])*g;

  i=2; j=0;
  b[i] = (m[i]-m[j])/(v[i]-v[j])*g;
  a[i] = (m[j]*v[i]-m[i]*v[j])/(v[i]-v[j])*g;
  // G =av+bv^2
  
  for (int i = 0; i < 3; i++)
  {
    printf("a%d : %lf\n",i+1,a[i]);
    printf("b%d : %lf\n",i+1,b[i]);
  }
  

  


  

}