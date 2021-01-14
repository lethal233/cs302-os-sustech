#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int a[100];

int compare(const void *,const void *);

int main(){
    int n = 0;
    int b;
    while(scanf("%d",&b)!=EOF){
        a[n++]=b;
    }
    qsort(a,n,sizeof(int),compare);
    for (size_t i = 0; i < n; i++)
    {
        printf("%d ",a[i]);
        /* code */
    }
    
    return 0;
}

int compare(const void *a, const void * b){
    return (*(int *)a-*(int *)b);
}