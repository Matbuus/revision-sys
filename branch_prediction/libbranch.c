#include <stdlib.h>
#include "libbranch.h"

int random_value(int n, int is_random) {
  int res = rand();
  if(is_random)
    res = res % 2;
  else
    res = n % 2;
  return res;
}

int f1(int n) {
  int res = n;
  res = res/2;
}

int f2(int n) {
  int res = n*2;
  res = res+7;
}
