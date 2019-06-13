#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#if HAVE_PAPI
#include <papi.h>
#endif
#include "libbranch.h"

#define GET_TIME(t) gettimeofday(&(t), NULL)
#define TIME_DIFF(t1, t2) (((t2).tv_sec - (t1).tv_sec)*1e6 + ((t2).tv_usec - (t1).tv_usec))

#define nb_iter 100000000
#define NB_EVENTS 3
int main(int argc, char**argv) {
  /* initialize srand with a fixed seed to improve the reproductibility of the experiment */
  srand(0);

  int is_random = 0;
  int i;
  int result = 0;
  struct timeval t1, t2;

  if(argc<2) {
    fprintf(stderr, "Usage: %s <0|1>\n", argv[0]);
    fprintf(stderr, "\t0 -> let the branch predictor guess correctly\n");
    fprintf(stderr, "\t1 -> mess with the branch predictor\n");
    return EXIT_FAILURE;
  }

  is_random = atoi(argv[1]);

  if(is_random) {
    printf("is random is set\n");
  } else {
    printf("is random is not set\n");
  }

  GET_TIME(t1);
  /* Start counting events */

#if HAVE_PAPI
  int Events[NB_EVENTS] = { PAPI_TOT_INS, PAPI_BR_CN, PAPI_BR_MSP };
  long_long values[NB_EVENTS];
  if (PAPI_start_counters(Events, NB_EVENTS) != PAPI_OK)
    abort();
#endif	/* HAVE_PAPI */

  for(i=0; i<nb_iter; i++) {
    int val = random_value(i, is_random);
    if(val % 2) {
      result += f1(val);
    } else {
      result = f2(i + val);
    }
  }
  /* Stop counting events */
#if HAVE_PAPI
  if (PAPI_stop_counters(values, NB_EVENTS) != PAPI_OK)
    abort();
#endif	/* HAVE_PAPI */

  GET_TIME(t2);
  double duration = TIME_DIFF(t1, t2)/1000;
  printf("%d iterations in %lf ms\n", nb_iter, duration);
  printf("result=%d\n", result);

#if HAVE_PAPI
  /* nombre of instructions issued */
  long long nb_instructions = values[0];
  /* nombre of conditional branches */
  long long nb_branches = values[1];
  /* nombre of mispredicted conditional branches */
  long long nb_mp_branches = values[2];

  double mispredict_rate = 1000.*nb_mp_branches/nb_iter;

  printf("instructions:          %lld\n", nb_instructions);
  printf("branches:              %lld\n", nb_branches);
  printf("mispredicted branches: %lld\n", nb_mp_branches);

  printf("mispredicted branch per 1000 iter: %lf\n", mispredict_rate);
#endif	/* HAVE_PAPI */
  return EXIT_SUCCESS;
}
