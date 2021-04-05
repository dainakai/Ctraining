#include<bits/stdc++.h>
using namespace std;
void test(double *a);
int main(){
    double *array;

    array = (double *)malloc(sizeof(double)*5);
    for (int i = 0; i < 5; i++)
    {
        *(array+i) = (double)i;
        printf("%lf\n",*(array+i));
    }
    printf("\n");

    test(array+1);

    for (int i = 0; i < 5; i++)
    {
        printf("%lf\n",*(array+i));
    }
    
    
    return 0;
    
}
void test(double *a){
    a[2] = a[2]*a[2];
}

// #include <bits/stdc++.h>
// using namespace std;

// int main()
// {
// 	double *p;  //int型のポインタ変数pを宣言
// 	p = (double *)malloc(sizeof(double)*10);                 //pに配列aの先頭アドレスを代入

// 	for(int i = 0; i < 10; i++){
// 		*(p + i) = (double)i;                         //ポインタpが指すアドレス+iの中身にi を代入
// 		printf("a[%d] = %lf\n", i, *(p + i));  //結果の出力
// 	}
// 	return 0;
// }