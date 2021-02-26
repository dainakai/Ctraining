#include<bits/stdc++.h>
using namespace std;

int main (){
  double rho = 998.2 ; //kg/m^3
  double r = 0.016 ; // m
  double zeta = 87.8; //gear ratio
  double m = 0.25 ; // kg
  double alpha = 0.40819;
  double beta = -0.00454;
  double g = 9.80;
  double l = 0.7; //m
  double a = 0.014142;
  double b;
  int n;

  cout << "LENGTH b : " ;
  cin >> b ;
  cout << endl;

  b /=1000.0;

  cout << "TIMES of TRIAL : ";
  cin >> n;
  cout << endl;

  double time[n];
  for (int i = 0; i < n; i++)
  {
    printf("TIME %d :",i+1);
    scanf("%lf",&time[i]);
    printf("\n");
  }
  
  double velo[n];
  for (int i = 0; i < n; i++)
  {
    velo[i] = 0;
    velo[i] = l/time[i];
  }
  
  double cdvalue[n];
  for (int i = 0; i < n; i++)
  {
    cdvalue[i] = 4*r*r*r*(m*g*velo[i]-alpha*g*velo[i]-beta*g)/(rho*a*a*a*a*b*zeta*zeta*zeta*velo[i]*velo[i]*velo[i]);
  }
  
  for (int i = 0; i < n; i++)
  {
    printf("Cd %d : %lf\n",i+1,cdvalue[i]);
  }
  
  double avr;
  for (int i = 0; i < n; i++)
  {
    avr += cdvalue[i];
  }
  avr /= n;

  printf("Cd Avr. : %lf\n",avr);
  

  FILE* fp;

  fp = fopen("data.dat","w");
  for (int i = 0; i < n; i++)
  {
    fprintf(fp,"%lf\n",cdvalue[i]);
  }
  fprintf(fp,"%lf\n",avr);
  fclose(fp);

  return 0;

}