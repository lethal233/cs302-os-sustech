#include<stdio.h>
#include<math.h>

int main(){
    int a;
    double avg = 0;
    for (size_t i = 0; i < 20; i++)
    {
        /* code */
        scanf("%d",&a);
        avg += a;
    }

    printf("average is %.4f\n",avg/20);
    
    return 0;
}