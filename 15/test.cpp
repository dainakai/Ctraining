#include<bits/stdc++.h>
using namespace std;
const int height = 512;
const int width = 512;

int main(){
    double a[1][5];
    double *pointer;
    pointer = (double *)a;

    for (int i = 0; i < 5; i++)
    {
        *(pointer + i) = (double)i;
        printf("%lf %lf\n",a[1][i],*(pointer+i));
    }

    return 0;
    
}