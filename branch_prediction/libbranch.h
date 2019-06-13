#ifndef LIBBRANCH_H
#define LIBBRANCH_H

/* compute a more or less random value
 * return a random value if is_random is set
 * or an easy-to-guess value if is_random is set to zero
 */
int random_value(int n, int is_random);

/* compute something */
int f1(int n);

/* compute something else */
int f2(int n);

#endif
