#include <stdio.h>
#include <time.h>
#define CACHE_MIN (1024*4096)       /* smallest array to test */
#define CACHE_MAX (1024*262144)  /* largest array to test */
long x[CACHE_MAX];              /* array to stride through */
#define SAMPLE 25

int main(void)
{
  long register i, index, stride, limit, temp=0;
  long steps, tsteps, csize;
  double sec;
  clock_t start, end;
  for ( csize=CACHE_MIN ; csize<=CACHE_MAX ; csize*=2 ) {
    for ( stride=1 ; stride<=csize/2 ; stride*=2 ) {
      sec=0;
      limit = csize-stride+1;
      steps=0;
      do {
        start = clock();
        for ( i=SAMPLE*stride ; i!=0 ; i-- )
          for ( index=0 ; index<limit ; index+=stride )
            x[index] = x[index] + 1;
        steps ++;
        end = clock();
        sec += ((double)(end-start)/CLOCKS_PER_SEC);
      } while (sec < 1.0);
      tsteps = 0;
      do {
        start = clock();
        for ( i=SAMPLE*stride ; i!=0 ; i-- )
          for ( index=0 ; index<limit ; index+=stride )
            temp = temp + index;
        tsteps ++;
        end = clock();
        sec -= ((double)(end-start)/CLOCKS_PER_SEC);
      } while (tsteps<steps);
      printf("%d,%d,%0.2f\n",
              csize*sizeof(long), stride*sizeof(long),
              (double) sec*1e9/(steps*SAMPLE*stride*((limit-1)/stride+1)));
    }
  }
  printf("Done\n");
  return 0;
}
