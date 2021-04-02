/******************************************************************************
PROGRAM NAME : FFT with window function
AUTHER : Dai Nakai
DATE : 2021/4/1
******************************************************************************/
#include<bits/stdc++.h>
#include<sys/stat.h>
using namespace std;

void S_fft (double *ak, double *bk, int N, int ff);
void Ini_val (double *re_f, double *im_f, int N);
void rec_window(double *re_f, double *im_f, int N);
void ham_window(double *re_f, double *im_f, int N);
void han_window(double *re_f, double *im_f, int N);
void elementWiseProduct (double *a, double *b, double *c, int N);
void ini_with0 (double *a, int N);
void outputFreqImageWithGnuplot (double *re, double *im, int N, double t_length, const char *string);
void outputWaveImageWithGnuplot (double *re, int N, double t_length, const char *string);

const int N = 512;
const double t_length =1.0; // Measuring time
const size_t size = N * sizeof(double);
/*********************************main****************************************/
int main(){

    //Define original signal and window function arrays
    double *re_signal, *im_signal, *re_rec, *im_rec, *re_ham, *im_ham, *re_han, *im_han;

    re_signal = (double *)malloc(size);
    im_signal = (double *)malloc(size);
    re_rec = (double *)malloc(size);
    im_rec = (double *)malloc(size);
    re_ham = (double *)malloc(size);
    im_ham = (double *)malloc(size);
    re_han = (double *)malloc(size);
    im_han = (double *)malloc(size);

    Ini_val(re_signal,im_signal,N);
    rec_window(re_rec,im_rec,N);
    ham_window(re_ham,im_ham,N);
    han_window(re_han,im_han,N);

    //FFT of each array
    double *re_FFTsig, *im_FFTsig, *re_FFTrec, *im_FFTrec, *re_FFTham, *im_FFTham, *re_FFThan, *im_FFThan;

    re_FFTsig = (double *)malloc(size);
    im_FFTsig = (double *)malloc(size);
    re_FFTrec = (double *)malloc(size);
    im_FFTrec = (double *)malloc(size);
    re_FFTham = (double *)malloc(size);
    im_FFTham = (double *)malloc(size);
    re_FFThan = (double *)malloc(size);
    im_FFThan = (double *)malloc(size);

    memcpy(re_FFTsig,re_signal,size);
    memcpy(im_FFTsig,im_signal,size);
    memcpy(re_FFTrec,re_rec,size);
    memcpy(im_FFTrec,im_rec,size);
    memcpy(re_FFTham,re_ham,size);
    memcpy(im_FFTham,im_ham,size);
    memcpy(re_FFThan,re_han,size);
    memcpy(im_FFThan,im_han,size);

    S_fft(re_FFTsig, im_FFTsig, N, 1); //Corresponds to the signal with rectangular window applied
    S_fft(re_FFTrec, im_FFTrec, N, 1);
    S_fft(re_FFTham, im_FFTham, N, 1);
    S_fft(re_FFThan, im_FFThan, N, 1);

    //Calculation of signals with each window applied (Hamming and Hanning)
    double *re_sigwHam, *im_sigwHam, *re_sigwHan, *im_sigwHan;

    re_sigwHam = (double *)malloc(size);
    im_sigwHam = (double *)malloc(size);
    re_sigwHan = (double *)malloc(size);
    im_sigwHan = (double *)malloc(size);

    elementWiseProduct(re_ham, re_signal, re_sigwHam, N);
    elementWiseProduct(re_han, re_signal, re_sigwHan, N);

    ini_with0(im_sigwHam, N);
    ini_with0(im_sigwHan, N);

    double *re_FFTsigwHam, *im_FFTsigwHam, *re_FFTsigwHan, *im_FFTsigwHan;

    re_FFTsigwHam = (double *)malloc(size);
    im_FFTsigwHam = (double *)malloc(size);
    re_FFTsigwHan = (double *)malloc(size);
    im_FFTsigwHan = (double *)malloc(size);

    memcpy(re_FFTsigwHam,re_sigwHam,size);
    memcpy(im_FFTsigwHam,im_sigwHam,size);
    memcpy(re_FFTsigwHan,re_sigwHan,size);
    memcpy(im_FFTsigwHan,im_sigwHan,size);

    S_fft(re_FFTsigwHam, im_FFTsigwHam,N,1); //Hamming window applied
    S_fft(re_FFTsigwHan, im_FFTsigwHan,N,1); //Hanning window applied

    //Outputting
    outputWaveImageWithGnuplot(re_signal,N,t_length,"originalSignalWave");
    outputWaveImageWithGnuplot(re_ham,N,t_length,"HammingWindow");
    outputWaveImageWithGnuplot(re_han,N,t_length,"HanningWindow");
    outputWaveImageWithGnuplot(re_sigwHam,N,t_length,"HammingWindowAppliedSignal");
    outputWaveImageWithGnuplot(re_sigwHan,N,t_length,"HanningWindowAppliedSignal");

    outputFreqImageWithGnuplot(re_FFTsig, im_FFTsig, N, t_length,"FFTofOriginalSignal");
    outputFreqImageWithGnuplot(re_FFTrec, im_FFTrec, N, t_length,"FFTofRectangularWindow");
    outputFreqImageWithGnuplot(re_FFTham, im_FFTham, N, t_length,"FFTofHammingWindow");
    outputFreqImageWithGnuplot(re_FFThan, im_FFThan, N, t_length,"FFTofHanningWindow");
    outputFreqImageWithGnuplot(re_FFTsigwHam,im_FFTsigwHam,N,t_length,"FFTofHammingWindowAppliedSignal");
    outputFreqImageWithGnuplot(re_FFTsigwHan,im_FFTsigwHan,N,t_length,"FFTofHanningWindowAppliedSignal");

    return 0;
}

void Ini_val(double *re_f, double *im_f, int N){
    for (int i = 0; i < N; i++){
        re_f[i] = sin (2.0*M_PI*1.0*(double)i/(double)N) + sin (2.0*M_PI*3.0*(double)i/(double)N) + sin (2.0*M_PI*6.0*(double)i/(double)N);
        im_f[i] = 0.0;
    }
}

void rec_window(double *re_f, double *im_f, int N){
    for (int i = 0; i < N; i++){
        re_f[i] = 1.0;
        im_f[i] = 0.0;
    }
}

void ham_window(double *re_f, double *im_f, int N){
    for (int i = 0; i < N; i++){
        re_f[i] = 0.54 - 0.46*cos(2*M_PI*i/N);
        im_f[i] = 0.0;
    }
}

void han_window(double *re_f, double *im_f, int N){
    for (int i = 0; i < N; i++){
        re_f[i] = 0.50 - 0.50*cos(2*M_PI*i/N);
        im_f[i] = 0.0;
    }
}

void elementWiseProduct(double *a, double *b, double *c, int N){
    for (int i = 0; i < N; i++){
        c[i] = a[i] * b[i];
    }
}

void ini_with0(double *a, int N){
    for (int i = 0; i < N; i++){
        a[i] = 0.0;
    }
    
}

void S_fft(double *ak, double *bk, int N, int ff){
    int i,j,k,k1,num,nhalf,phi,phi0,rot[N];
    double s,sc,c,a0,b0,tmp;

    for (i = 0; i < N; i++){
        rot[i] = 0;
    }

    nhalf = N/2;
    num = N/2;
    sc = 2.0*M_PI/(double)N;

    while(num>=1){
        for (j = 0; j < N; j+=(2*num)){
            phi = rot[j]/2;
            phi0 = phi + nhalf;
            c = cos(sc*(double)phi);
            s = sin(sc*(double)(phi*ff));

            for(k=j; k<j+num; k++){
                k1 = k+num;
                a0 = ak[k1]*c - bk[k1]*s;
                b0 = ak[k1]*s + bk[k1]*c;
                ak[k1] = ak[k]-a0;
                bk[k1] = bk[k]-b0;
                ak[k] +=a0;
                bk[k] += b0;
                rot[k] = phi;
                rot[k1] = phi0;
            }
        }
        num /= 2;
    }

    if(ff<0){
        for ( i = 0; i < N; i++){
            ak[i] /= (double)N;
            bk[i] /= (double)N;
        }
    }

    for(i=0; i<N-1; i++){
        if((j=rot[i])>i){
            tmp=ak[i]; ak[i] = ak[j]; ak[j]=tmp;
            tmp=bk[i]; bk[i] = bk[j]; bk[j]=tmp;
        }
    }
}

void outputFreqImageWithGnuplot (double *re, double *im, int N, double t_length, const char *string){
    double *pw, *fq;
    pw = (double *)malloc(size);
    fq = (double *)malloc(size);

    for (int i = 0; i < N; i++){
        pw[i] = re[i]*re[i] + im[i]*im[i];
    }
    
    
    FILE *gp;
    if(!(gp = popen("gnuplot","w"))){
        printf("Gnuplot doesn't work. Software quiting...\n");
    }

    fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15'\n");
    fprintf(gp,"set output './%s.png'\n",string);
    fprintf(gp,"set key right top\n");
    fprintf(gp,"set size ratio 0.8\n");
    fprintf(gp,"set xlabel '{/Times-New-Roman:Italic=20 f} [Hz]'\n");
    fprintf(gp,"set xrange [0:10]\n");
    fprintf(gp,"set ylabel 'Power spectrum [-]'\n");
    fprintf(gp,"array data[%d]\n",N);
    for (int i = 0; i < N; i++)
    {
        fprintf(gp,"data[%d] = %lf\n", i+1, pw[i]);
    }
    
    fprintf(gp,"plot data using ($1-1)/%lf:2 with lines title '%s'\n",t_length,string);
    pclose(gp);
}

void outputWaveImageWithGnuplot (double *re, int N, double t_length, const char *string){
    FILE *gp;
    if(!(gp = popen("gnuplot","w"))){
        printf("Gnuplot doesn't work. Software quiting...\n");
    }

    fprintf(gp,"set terminal pngcairo enhanced font 'Times New Roman,15'\n");
    fprintf(gp,"set output './%s.png'\n",string);
    fprintf(gp,"set key right top\n");
    fprintf(gp,"set size ratio 0.8\n");
    fprintf(gp,"set xlabel '{/Times-New-Roman:Italic=20 t} [sec]'\n");
    fprintf(gp,"set ylabel 'Signal [-]'\n");
    fprintf(gp,"array data[%d]\n",N);
    for (int i = 0; i < N; i++)
    {
        fprintf(gp,"data[%d] = %lf\n", i+1, re[i]);
    }
    fprintf(gp,"plot data using ($1-1)*%lf/%d:2 with lines title '%s'\n",t_length,N,string);
    pclose(gp);
}