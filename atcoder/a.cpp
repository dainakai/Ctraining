#include<bits/stdc++.h>
using namespace std;

int main (){
  string s;
  string t="";
  string t1="dream",t2="erase", t3="er";

  cin >> s ;

  int i=0;
  while(i < s.size())
  {
    if(s.at(i)=='d'){
      t += t1;
      i += 4;
      continue;
    }
    if(s.at(i)=='e'){
      if(s.at(i+2)=='a'){
        t += t2;
        i += 4;
        continue;
      }else{
        t += t3;
        i += 1;
        continue;
      }
    }
  }
  
  if(s == t){
  cout << "YES" << endl;
  }else
  {
    cout << "NO" << endl;
  }
}