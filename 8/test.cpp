#include<bits/stdc++.h>
using namespace std;

FILE *fp;
int main(){
    fp = fopen("./test.dat","r");
    int a[6];
    for (int i = 0; i < 6; i++)
    {
        fscanf(fp,"%d",&a[i]);
    }
    
    
    cout << a[0] << endl;
    cout << a[1] << endl;
    cout << a[2] << endl;
    cout << a[3] << endl;
    cout << a[4] << endl;
    cout << a[5] << endl;
    return 0;
}