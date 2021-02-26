#include<bits/stdc++.h>
using namespace std;

int main (){
  double w,b,x,y,t,E,dEw,dEb;
  double n = 0.1;
  string s;

  srand(time(NULL));
  w = (double)(rand()/1000 % 100);
  b = (double)(rand()/1000 % 100);

  while(1)
  {cout << "INPUT NUMBER >> ";
  cin >> s;

  if(s == "exit"){
    exit(0);
  }else
  {
    x = s.at(0)-48;
  }
  

  t = x;
  y = w*x+b;
  E = (y-t)*(y-t);

  cout << "NUMBER >> " << x << endl;
  cout << "OUTPUT >> " << y << endl;
  cout << "ERROR >> " << E << endl;

  dEw = 2*(y-t)*x;
  dEb = 2*(y-t);

  w = w - n*dEw;
  b = b - n*dEb;}
}

//1と2だとうまくいくけど3以上だとエラーが発散する．確率ので再試行