#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
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

int eventfd(unsigned int,int);

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
   int evfd = eventfd (0, 0);
   long ctr = 1;
   struct stat sbuf;

   pipe (p);

   FILE *f = fopen ("data", "r");
   fread (data, DSIZE, 1, f);
   fclose (f);

   signal (SIGURG, sigu);

   for (lp = 0; lp < 100000; lp++) {
      s=stamp();

      //snprintf (buf, 64, "<1.%llx>", (unsigned long long)0xa234567812ULL);
      //getpgrp();
      //kill (0, SIGURG);
      //write (evfd, &ctr, 8);
      //read (evfd, &ctr, 8);
      //write (p[1], &buf, 1);
      //read (p[0], &buf, 4);
      //stat ("/etc/passwd", &sbuf);
      free(malloc(8*1024*1024));

      si[0]=measure(s);

      //j = lzf_decompress (data2, l, data3, DSIZE*2);

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



