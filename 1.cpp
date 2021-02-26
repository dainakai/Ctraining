#include<bits/stdc++.h>
using namespace std;

int main(){
  int n,temp;
  cin >> n;
  vector<int> v(n);
  for (int i = 0; i < n; i++)
  {
    cin >> v.at(i);
  }

  for (int  i = 0; i < n; i++)
  {
    for (int j = 0; j < i; j++)
    {
      if(v.at(i) < v.at(j))
      {
        temp = v.at(j);
        v.at(j) = v.at(i);
        v.at(i) = temp;
      }
    }
  }
  int t = n;
  for (int  i = 0; i < n-1; i++)
  {
    if(v.at(i) == v.at(i+1))
    {
      t--;
    }
  }
  cout << t << endl;
}