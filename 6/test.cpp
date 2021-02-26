#include <bits/stdc++.h>
 
using namespace std;
 
int main() {
  FILE* fp;

  fp = fopen("test.dat","wb");
  
  unsigned char a[5][4];
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      a[i][j]=i*j;
    }
  }

  fwrite(a,sizeof(a),1,fp);
  fclose(fp);
  
}