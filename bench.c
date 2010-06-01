#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <math.h>
#include <signal.h>
#include <X11/Xlib.h>

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

static void sigu (int signum)
{
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
   char buf[8192];
   int p[2];
   long ctr = 1;
   struct stat sbuf;

   pipe (p);

   FILE *f = fopen ("data", "r");
   fread (data, DSIZE, 1, f);
   fclose (f);

   signal (SIGURG, sigu);

   for (lp = 0; lp < 1000000; lp++) {
      s=stamp();

      //struct timespec ts; clock_gettime (CLOCK_THREAD_CPUTIME_ID, &ts);
      //printf ("%9ld\n", ts.tv_nsec);//D
      //struct rusage usage; getrusage (RUSAGE_SELF, &usage);
      //struct tms tms; times (&tms);

      //kill (0, SIGURG);
      //write (evfd, &ctr, 8);
      //read (evfd, &ctr, 8);
      //write (p[1], &buf, 1);
      //read (p[0], &buf, 4);
      //stat ("/etc/passwd", &sbuf);
      //struct timeval tv; gettimeofday (&tv, 0);
      //void *x = mmap (0, 16384, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE,-1,0);

      l = lzf_compress (data, DSIZE, data2, DSIZE*2);
      assert(l);

      j = lzf_decompress (data2, l, data3, DSIZE*2);
      assert (j == DSIZE);

      si[0]=measure(s);

      assert (!memcmp (data, data3, DSIZE));

      printf ("\r%10d (%d) ", si[0], l);
      if (si[0] < min && si[0] > 0)
        {
          printf ("\n");
          min = si[0];
        }

      fflush (stdout);

      //assert (memcmp (data, data3, DSIZE) == 0);
   }
   return 0;
}



