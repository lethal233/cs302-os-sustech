#include <stdio.h>
#include <time.h>

int main()
{
    // infinite loop for 10 seconds
    int t = 0;
    clock_t startTime = clock();
    while (clock() - startTime < 1000000000)
        ;
    printf("%lfs\n", (double)(clock() - startTime) / CLOCKS_PER_SEC);
    return 0;
}