#include<bits/stdc++.h>
using namespace std;

int main (){
  double eta = 0.01;
  int n;
  double y[10];
  double x[10];
  double E[10];
  double w[10][10];
  double dE[10][10];
  double b[10];

  srand(time(NULL));
  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++){
      w[i][j] = (double)rand()/1000.0;
      w[i][j] -= (int)w[i][j];
    }
    b[i] = (double)rand()/1000.0;
    b[i] -= (int)b[i];
  }

  for (int i = 0; i < 10; i++){
    y[i] = 0.0;
  }
  



  for(int c=0; c < 10000; c++){
  n = c % 10;

  for (int i = 0; i < 10; i++){
    x[i] = 0.0;
  }
  
  switch (n){
  case 0:
    x[0] = 1.0;
    break;

  case 1:
    x[1] = 1.0;
    break;

  case 2:
    x[2] = 1.0;
    break;

  case 3:
    x[3] = 1.0;
    break;

  case 4:
    x[4] = 1.0;
    break;

  case 5:
    x[5] = 1.0;
    break;

  case 6:
    x[6] = 1.0;
    break;

  case 7:
    x[7] = 1.0;
    break;

  case 8:
    x[8] = 1.0;
    break;

  case 9:
    x[9] = 1.0;
    break;
  
  default:
    break;
  }

  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++){
      y[i] += w[i][j]*x[j];
    }
    y[i] += b[i];
    y[i] = tanh(y[i]);
  }

  for (int i = 0; i < 10; i++){
    E[i] = (y[i]-x[i])*(y[i]-x[i]);
    for (int j = 0; j < 10; j++){
      dE[i][j] = 2.0*(y[i]-x[i])*x[j];
    }
  }
  
  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++){
      w[i][j] -= eta* dE[i][j];
    }
    b[i] -= eta * 2.0*(y[i]-x[i]);
  }
  
  for (int i = 0; i < 10; i++)
  {
    printf("%5.3lf ",y[i]);
  }
  printf("\n");
  
  }

}
//いい感じ．線形変換にはかならず活性化関数を組み合わせる．