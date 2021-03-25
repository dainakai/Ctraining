/******************************************************************************
PROGRAM NAME : One-directional DFT Software
AUTHER : Dai Nakai
DATE : 2021/3/25
******************************************************************************/
#include<bits/stdc++.h>
using namespace std;

void onedir_dft (double *p, complex<double> *z, int n){
    /* One-directional DFT function.
    *  p for input real array pointer, z for output complex array pointer
    *  size for sizeof(double), n for number of data
    */
   
   for (int l = 0; l < n; l++){
       z[l] = 0.0 + 0.0i;
   }
   
   for (int l = 0; l < n; l++){
       for (int m = 0; m < n; m++){
           z[l] += complex<double>(p[m]*cos(2*M_PI*m*l/n), -p[m]*sin(2*M_PI*m*l/n));
       }
   }
}

