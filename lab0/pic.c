#include<stdio.h>
#include<math.h>

int main(){
    int n;
    scanf("%d",&n);
    int a = (n+1)/2;
    for (size_t i = 1; i <= a; i++)
    {
        /* code */
        for (size_t j = 0; j < a-i; j++)
        {
            /* code */
            putchar(' ');
        }
        for (size_t j = 0; j < 2*i-1; j++)
        {
            /* code */
            putchar('*');
        }
        for (size_t j = 0; j < a-i; j++)
        {
            /* code */
            putchar(' ');
        }
        putchar('\n');      
    }
    for (size_t i = a -1 ; i >=1; i--)
    {
        /* code */
        for (size_t j = 0; j < a-i; j++)
        {
            /* code */
            putchar(' ');
        }
        for (size_t j = 0; j < 2*i-1; j++)
        {
            /* code */
            putchar('*');
        }
        for (size_t j = 0; j < a-i; j++)
        {
            /* code */
            putchar(' ');
        }       
        putchar('\n');
    }
    
    return 0;
}