/******************************************************************************
PROGRAM NAME : FDM analysis of Taylor-Green vortex 
AUTHER : Dai Nakai
DATE : 2021 Jan. 7th
Think a Bit , Code a Bit , Test a Bit
******************************************************************************/
#include <bits/stdc++.h>
using namespace std;

const char * output_data_file = "result4.dat"; // name of output file
const int x_grid = 31;// number of grids in x direction
const int y_grid = 31;// number of grids in y direction
const int grid_space = 10;// grid width
const double PI = 4.0* atan (1.0);// Calc. Pi
float v[x_grid][y_grid]; //v vector array
float u[x_grid][y_grid]; //u vector array
float omega[x_grid][y_grid]; // vorticity omega_z
float sub[x_grid][y_grid]; //tolerance of FDM omega and exact omega
float rms=0;//RMS of sub
FILE * fp ;

/* output settings */
const char* xxlabel = "{/Times-New-Roman:Italic=20 x} [pixel]";
const char* yylabel = "{/Times-New-Roman:Italic=20 y} [pixel]";
const char* cb_label = "{/Symbol:Italic=20 w}_{/Times-New-Roman:Italic=20 z} [/sec]";//color bar range min
const char* gray_label = "{/Symbol:Italic=20 D}{/Symbol:Italic=20 w}_{/Times-New-Roman:Italic=20 z} [/sec]";//color bar range min
const double v_r = 1.0;//magnified ratio for vector length
const int x_min = 0;//x range min
const int x_max = 30;//x range max
const int y_min = 0;//y range min
const int y_max = 30;//y range max
const int cb_min = -2;//color bar range min
const int cb_max = 2;//color bar range max
const float gray_min = 0.0;//gray color bar range min 
const float gray_max = 0.1;//gray color bar range max
FILE *gp;	//gnuplot
/********************************* MAIN *********************************/
int main (){
  int i,j;
	double diffvx,diffuy;
  // preparing for output file
  fp = fopen ( output_data_file ,"w");

	/* Taylor-Green Vortex Field */
  // Calc. 2D velocity vector
  for(i =0;i< x_grid ;i++){
    for(j=0;j< y_grid ;j++){
      u[i][j]= cos (2.0* PI/ (x_grid-1) *i)* sin (2.0* PI/ (y_grid-1) *j);
      v[i][j]= -1.0 * sin (2.0* PI/ (x_grid-1) *i)*cos (2.0* PI/ (y_grid-1) *j);
    }
  }

	//Calc. vorticity value 
	for (int i = 0; i < x_grid; i++){
		for (int j = 0; j < y_grid; j++){
			if(i==0){ //Forward difference
				diffvx = (-3.0*v[0][j]+4.0*v[1][j]-v[2][j])/(4.0* PI/ x_grid);
			}else if(i == x_grid-1){ //Backward difference
				diffvx = (3.0*v[x_grid-1][j]-4.0*v[x_grid-2][j]+v[x_grid-3][j])/(4.0* PI/ x_grid);
			}else{ //Central difference
				diffvx = (v[i+1][j]-v[i-1][j])/(4.0* PI/ x_grid);
			}

			if(j==0){ //Forward difference
				diffuy = (-3.0*u[i][0]+4.0*u[i][1]-u[i][2])/(4.0* PI/ y_grid);
			}else if(j==y_grid-1){ //Backward difference
				diffuy = (3.0*u[i][y_grid-1]-4.0*u[i][y_grid-2]+u[i][y_grid-3])/(4.0* PI/ y_grid);
			}else{ //Central difference
				diffuy = (u[i][j+1]-u[i][j-1])/(4.0* PI/ y_grid);
			}

			omega[i][j] = diffvx - diffuy; //definition of vorticity
			sub[i][j] = fabs(-2.0* cos (2.0* PI/ (x_grid-1) *i)*cos (2.0* PI/ (y_grid-1) *j) - omega[i][j]);
			rms += sub[i][j]*sub[i][j];

			fprintf ( fp ,"%d \t %d \t %.3 lf \t %.3 lf \t %.3 lf \t %.3 lf \n",
      i ,j ,omega[i][j],u[i][j],v[i][j],sub[i][j]);
      printf ("%d \t %d \t %.3 lf \t %.3 lf \t %.3 lf \t %.3 lf\n",i ,j ,omega[i][j],u[i][j],v[i][j],sub[i][j]);
		}
		fprintf(fp,"\n");
    printf("\n");
	}
	
	rms /= x_grid*y_grid;
	rms = sqrt(rms);

	fprintf(fp,"RMS Value : %.3 lf\n",rms);
	printf("RMS Value : %.3 lf\n",rms);

  fclose (fp);
	

  if ((gp = popen("gnuplot", "w")) == NULL) {
	printf("gnuplot is not here!\n");
	exit(0);
	}

  //PNG image
	fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15' \n");
	fprintf(gp,"set output 'FDM.png'\n");
	fprintf(gp,"set multiplot\n");
	fprintf(gp,"unset key\n");
	fprintf(gp,"set size ratio -1\n");

	fprintf(gp,"set lmargin screen 0.15\n");
	fprintf(gp,"set rmargin screen 0.85\n");
	fprintf(gp,"set tmargin screen 0.85\n");
	fprintf(gp,"set bmargin screen 0.15\n");

	fprintf(gp,"set xrange [%d:%d]\n",x_min,x_max);
	fprintf(gp,"set xlabel '%s'offset 0.0,0.5\n",xxlabel);
	fprintf(gp,"set yrange [%d:%d]\n",y_min,y_max);
	fprintf(gp,"set ylabel '%s'offset 0.5,0.0\n",yylabel);

	fprintf(gp,"set cblabel '%s'offset 0.0,0.0\n",cb_label);
	fprintf(gp,"set cbrange['%d':'%d']\n",cb_min,cb_max);
	fprintf(gp,"set colorbox vertical user origin 0.8, 0.2 size 0.025,0.6\n");
	fprintf(gp,"set palette rgbformulae 22,13,-31\n");

	fprintf(gp,"set pm3d map\n");
	fprintf(gp,"splot '%s'  using 2:1:3 with pm3d, '%s' using 2:1:($1*0.0):(%lf*$5):(%lf*$4):($1*0.0) with vectors head filled lt 2 lc 'black' \n",output_data_file,output_data_file,i,v_r,v_r);

	fprintf(gp,"unset multiplot\n");
	fprintf(gp,"set output 'TOLERANCE.png'\n");
	fprintf(gp,"set cblabel '%s'offset 0.0,0.0\n",gray_label);
	fprintf(gp,"set palette gray negative\n");
	fprintf(gp,"set cbrange['%lf':'%lf']\n",gray_min,gray_max);
	fprintf(gp,"splot '%s' using 2:1:6 with pm3d\n",output_data_file);

 	fflush(gp); //Clean up Data

	fprintf(gp, "exit\n"); // Quit gnuplot
	pclose(gp);
	return 0;
}