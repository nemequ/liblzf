#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "lzf.h"
//#include "fastlz.c"

typedef unsigned long tval;
typedef unsigned long long stamp64;

extern inline tval stamp(void)
{
	tval tsc;
	asm volatile("rdtsc" : "=a" (tsc) : : "edx");
	return tsc;
}

extern inline tval measure(tval t)
{
	tval tsc;
	asm volatile("rdtsc" : "=a" (tsc) : : "edx");
	if (tsc>t)
		return tsc-t;
	else
		return t-tsc;
}

#define DSIZE 2821120

unsigned char data[DSIZE], data2[DSIZE*2], data3[DSIZE*2];

int main(void)
{
   tval s;
   tval si[1000];
   int i, l, j;
   int min = 1<<30;
   int lp;

   FILE *f = fopen ("data", "r");
   fread (data, DSIZE, 1, f);
   fclose (f);
   
   for (lp = 0; lp < 1000; lp++) {
      l = lzf_compress (data, DSIZE, data2, DSIZE*2);
      s=stamp();
      //l = fastlz_compress_level (1, data, DSIZE, data2);
      j = lzf_decompress (data2, l, data3, DSIZE*2);
      si[0]=measure(s);

      printf ("\r%10d (%d) ", si[0], l);
      if (si[0] < min && si[0] > 0)
        {
          printf ("\n");
          min = si[0];
        }

      fflush (stdout);

      assert (memcmp (data, data3, DSIZE) == 0);
   }
   return 0;
}



