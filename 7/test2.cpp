#include<bits/stdc++.h>
using namespace std;
int main(){
  int a[6];
  a[0]=4;
  a[1]=3;
  a[2]=6;
  a[3]=1;
  a[4]=2;
  a[5]=7;

  int max = 0;
  for (int i = 0; i < 6; i++)
  {
    if(max < a[i]){
      max = a[i];
      cout << max << endl;
    }
  }
  return 0;
}