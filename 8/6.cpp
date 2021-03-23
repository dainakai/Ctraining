/******************************************************************************
PROGRAM NAME : Culclation of mean velo and vort and visuzalizaiton
AUTHER : Dai Nakai
DATE : 2021/3/22
******************************************************************************/
#include<bits/stdc++.h>
using namespace std;

const char* read_file_dir = "./pivvector";
const char* read_file_header = "vecfield";
const char* write_file_dat = "./6output.dat";
const char* write_file_img = "./6output.png";
char read_file[100];

const int imgnum = 99;
const int width = 1280;
const int height = 1024;
const int M = 32; // x horizontal length of Interrogation window
const int N = 32; // y vertical length of Interrogation window
const int Sx = 16; // half of horizontal extension of I window
const int Sy = 16; // half of vertical extension of I window
const int Cdimx = 2*(width-2*Sx)/M - 1;
const int Cdimy = 2*(height-2*Sy)/N - 1;

const char* xxlabel = "{/Times-New-Roman:Italic=20 x} [pixel]";
const char* yylabel = "{/Times-New-Roman:Italic=20 y} [pixel]";
const char* cb_label = "{/Symbol:Italic=20 w}_{/Times-New-Roman:Italic=20 z} [sec]";//color bar range min
const double v_r = 20.0;//magnified ratio for vector length
const int x_min = 0;//x range min
const int x_max = 1280;//x range max
const int y_min = 0;//y range min
const int y_max = 1024;//y range max
const int cb_min = -1;//color bar range min
const int cb_max = 1;//color bar range max

double vx[imgnum][Cdimy][Cdimx];
double vy[imgnum][Cdimy][Cdimx];
double w[imgnum][Cdimy][Cdimx];

double vxm[Cdimy][Cdimx];
double vym[Cdimy][Cdimx];
double wm[Cdimy][Cdimx];

int temp;


FILE *fp;
FILE *gp;
/*********************************main****************************************/
int main(){
    for (int num = 0; num < imgnum; num++)
    {
        sprintf(read_file,"%s/%s%04d.dat",read_file_dir,read_file_header,num);
        fp = fopen(read_file,"r");
        for (int i = 0; i < Cdimy; i++){
            for (int j = 0; j < Cdimx; j++){
                fscanf(fp,"%d %d %lf %lf %lf\n",&temp,&temp,&vx[num][i][j],&vy[num][i][j],&w[num][i][j]);
            }
            fscanf(fp,"\n");
        }
        fclose(fp);
    }

    for (int i = 0; i < Cdimy; i++){
        for (int j = 0; j < Cdimx; j++){
            vxm[i][j] = 0.0;
            vym[i][j] = 0.0;
            wm[i][j] = 0.0;
        }
    }
    

    for (int i = 0; i < Cdimy; i++){
        for (int j = 0; j < Cdimx; j++){
            for (int num = 0; num < imgnum; num++){
                vxm[i][j] += vx[num][i][j];
                vxm[i][j] += vx[num][i][j];
                wm[i][j] += w[num][i][j];
            }
        }
    }
    
    for (int i = 0; i < Cdimy; i++){
        for (int j = 0; j < Cdimx; j++){
            vxm[i][j] /= Cdimx*Cdimy;
            vym[i][j] /= Cdimx*Cdimy;
            wm[i][j] /= Cdimx*Cdimy;
        }
    }

    fp = fopen(write_file_dat,"w");
    for (int i = 0; i < Cdimy; i++){
        for (int j = 0; j < Cdimx; j++){
            fprintf(fp, "%d %d %lf %lf %lf\n", j, i, vxm[i][j], vym[i][j], wm[i][j]);
        }
        fprintf(fp,"\n");
    }
    fclose(fp);

    if ((gp = popen("gnuplot", "w")) == NULL) {
	    printf("gnuplot is not here!\n");
	    exit(0);
    }

    fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15' \n");
	fprintf(gp,"set output '%s'\n",write_file_img);
	fprintf(gp,"set multiplot\n");
	fprintf(gp,"unset key\n");
	
	fprintf(gp,"set size ratio 0.8\n");

	fprintf(gp,"set lmargin screen 0.1\n");
	fprintf(gp,"set rmargin screen 0.85\n");
	fprintf(gp,"set tmargin screen 0.85\n");
	fprintf(gp,"set bmargin screen 0.15\n");

	fprintf(gp,"set xrange [%d:%d]\n",x_min,x_max);
	fprintf(gp,"set xlabel '%s'offset 0.0,0.5\n",xxlabel);
	fprintf(gp,"set yrange [%d:%d]\n",y_min,y_max);
	fprintf(gp,"set ylabel '%s'offset 0.5,0.0\n",yylabel);


	fprintf(gp,"set cblabel '%s'offset 0.0,0.0\n",cb_label);
	fprintf(gp,"set cbrange['%d':'%d']\n",cb_min,cb_max);
	fprintf(gp,"set colorbox vertical user origin 0.83, 0.2 size 0.025,0.6\n");
	fprintf(gp,"set palette rgbformulae 22,13,-31\n");

	fprintf(gp,"set pm3d map\n");
	fprintf(gp,"set pm3d interpolate 16,16\n");
	fprintf(gp,"splot '%s' using ($1*16+32):($2*16+32):($5*10) with pm3d, '%s' using ($1*16+32):($2*16+32):($1*0.0):(%lf*$3/(sqrt($3*$3+$4*$4))):(%lf*$4/(sqrt($3*$3+$4*$4))):($1*0.0) with vectors head filled lt 2 lc 'black' \n",write_file_dat,write_file_dat,v_r,v_r);

 	fflush(gp); //Clean up Data

	fprintf(gp, "exit\n"); // Quit gnuplot
	pclose(gp);

    return 0;
}