#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define TRIALS 1000000L
#define FAST_TRIALS 1L

void crcInit(void);
char crcFast(char* message, long nBytes);

int main(int argc, char*argv[])
{
  unsigned char retval = 0;
  long i = 0;
  struct timeval tf, ti;
  unsigned long timems = 0;

  if (argc != 2)
  {
	  printf("\n");
      printf("Insufficient number of arguments provided.\n");
	  printf("Example: ./hw3 message\n");
	  printf("(Note, if your message has spaces in it, you may need to surround it in \"quotes\"!)\n");
      printf("\n");
	  printf("\n");
	  printf("Exiting.\n\n");
	  
	  return 1;
  }

  gettimeofday(&ti, NULL);
  
  for (i = 0; i < TRIALS; i++)
  {
      crcInit();
      retval = crcFast(argv[1], strlen(argv[1]));
  }
  
  gettimeofday(&tf, NULL);

  timems=(tf.tv_sec * 1000 + tf.tv_usec / 1000) - (ti.tv_sec * 1000 + tf.tv_usec / 1000);
  
  printf("\n");
  printf("Results\n");
  printf("=======\n\n");
  printf("CRC: %X\n", retval);
  printf("Iterations run: %lu.  Total execution time: %lu ms.  Time per iteration: %lu us\n\n", i, timems, (1000 * timems) / TRIALS);
  
  return 0;
}

