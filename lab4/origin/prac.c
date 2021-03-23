#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// int a []= {90,81,78,95,79,72,85};
struct prac
{
    int *a;
    int id;
};



void printAvg(struct prac* p){
    int sum = 0;
    for (int i =0 ;i< 7;++i){
        sum += p->a[i];
    }
    printf("Average is %d, thread #%d\n",sum/7,p->id);
}

void printMax(struct prac* p){
    int max = 0x80000000;
    for (int i = 0;i<7;++i){
        max = max > p->a[i]?max:p->a[i];
    }
    printf("Max is %d, thread #%d\n",max,p->id);
}

void printMin(struct prac* p){
    int min = 0x7fffffff;
    for (int i = 0;i<7;++i){
        min = min < p->a[i]?min:p->a[i];
    }
    printf("Min is %d, thread #%d\n",min,p->id);
}
int main (){
    int a []= {90,81,78,95,79,72,85};
    pthread_t threads[3];
    struct prac arg[3];
    void (*p[3]) (struct prac) = {&printAvg,&printMax,&printMin};
    for (int i = 0;i<3;++i){
        printf("In main: creating thread %d\n", i);
		arg[i] = (struct prac){&a, i};
		pthread_create(&threads[i], NULL, p[i], &arg[i]);
    }
    for(int t=0;t<3;t++){
		pthread_join(threads[t], NULL);
	}
    printf("\nall threads finish.\n");
	/* Last thing that main() should do */
	pthread_exit(NULL);
    return 0;
}