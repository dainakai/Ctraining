/******************************************************************************
PROGRAM NAME:sq-file-input.cpp
AUTHER: Yohsuke TANAKA, Dai NAKAI
DATE: 2020 12/23
A PROGRAM FOR IMPORTING CONSECTIVE DATA 235.CSV
******************************************************************************/
#include<bits/stdc++.h>
using namespace std;

// PATH SETTINGS
const char* read_file1 = "235.CSV"; 
const char* read_file2 = "C_lab_ono.dat"; 
const char* read_file3 = "C_v_p_1k_ono.dat"; 
const char* write_file = "resultSPL.dat"; 
FILE *fp;

const int CH = 8; //INPORTING CHANNELS
const int data_long = 60000;//DATA LENGTH OF NR500
const int skip_lines = 58; //SKIPPING LINES
double wave_value[data_long][CH];//EACH DATUM BY TIME AND CH
char date[100];
const double P0 = 0.000020;

//CALC. INTEGRATED VALUE WITH SIMPSONS RULE
const double dt = 0.000020;
double integ[CH];
double synpinteg(double data[data_long][CH],int l){
	double integ=0;
	for (int i = 0; i < data_long; i++)
	{
		if (i==0 || i == data_long-1)
		{
			integ += data[i][l];
		}else {
			if(i/2 -i/2.0 == 0.0)
		{
			integ += 2*data[i][l];
		}else
		{
			integ += 4*data[i][l];
		}
		}
	}
	integ *= dt/3;
	return integ;
}

/*********************************   MAIN   *********************************/
int main(){
int i,tim;
double ch1,ch2,ch3,ch4,ch5,ch6,ch7,ch8;

fp=fopen(read_file1,"r");if(fp==NULL){printf("I can't open infile1!\n");exit(0);}

//SKIPPING LINES IN 235.CSV
char buff[100];
for (int i = 0; i < skip_lines; i++){
	fgets(buff,sizeof(buff),fp);
}

for(i=0;i<data_long;i++){
	fscanf(fp,"%[^,], %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf",date, &tim, &ch1, &ch2, &ch3, &ch4, &ch5, &ch6, &ch7, &ch8);//SCAN EACH DATUM
	wave_value[i][0]=ch1;wave_value[i][1]=ch2;
	wave_value[i][2]=ch3;wave_value[i][3]=ch4;
	wave_value[i][4]=ch5;wave_value[i][5]=ch6;
	wave_value[i][6]=ch7;wave_value[i][7]=ch8;
}
fclose(fp);

//SQUARE VALUE FOR RMS
for (int i = 0; i < data_long; i++){
	for (int j = 0; j < CH; j++){
		wave_value[i][j] *= wave_value[i][j];
	}
}

//CALC. INTEG. SQUARE RMS
for (int i = 0; i < CH; i++){
	integ[i] = synpinteg(wave_value,i);
}
for (int i = 0; i < CH; i++){
	integ[i] /= dt * (data_long-1);
}

//IMPORTING CALIBRATION DATA FILES
double C_lab_ono[CH],C_v_p_1k_ono[CH];

fp = fopen(read_file2,"r");if(fp==NULL){printf("I can't open infile2!\n");exit(0);}
for (int i = 0; i < CH; i++){
	double temp;
	fscanf(fp,"%lf",&temp);
	C_lab_ono[i] = temp;
}
fclose(fp);

fp = fopen(read_file3,"r");if(fp==NULL){printf("I can't open infile3!\n");exit(0);}
double temp;
fscanf(fp,"%lf",&temp);
for (int i = 0; i < CH; i++){
	C_v_p_1k_ono[i]=temp;
}
fclose(fp);


// CALC. SPL
double SPL[CH];
for (int i = 0; i < CH; i++){
	SPL[i] = 10.0*log10(C_v_p_1k_ono[i]*C_lab_ono[i]*integ[i]/P0/P0);
}

//DATA OUTPUT (resultSPL.dat AND output.plt)
fp = fopen(write_file,"w");
fprintf(fp,"0 %lf\n 45 %lf\n 90 %lf\n 135 %lf\n 180 %lf\n 225 %lf\n 270 %lf\n 315 %lf\n 360 %lf\n",SPL[2],SPL[1],SPL[0],SPL[7],SPL[6],SPL[5],SPL[4],SPL[3],SPL[2]);
fclose(fp);

system("gnuplot output.plt"); //_popen() DOESN'T WORK ON MAC

return 0;
}