#include<bits/stdc++.h>
using namespace std;

int main(){
    // int a[3][2] = {1,2,3,4,5,6};
    // int *a_p;
    // a_p = (int*)a;
    
    // for (int i = 0; i < 6; i++)
    // {
    //     printf("%d : %d\n", i, *(a_p+i));
    // }
    // return 0;

    // int a[3][2] = {1,2,3,4,5,6};
    // int *a_p;
    // a_p = (int*)a;
    
    // for (int i = 0; i < 6; i++)
    // {
    //     printf("%d : %d\n", i, a_p[i]);
    // }
    // return 0;

    int a[3][2] = {1,2,3,4,5,6};
    
    for (int i = 0; i < 6; i++)
    {
        printf("%d : %d\n", i, *(a+i));
    }
    return 0;
}