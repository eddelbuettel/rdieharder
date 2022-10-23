/*
 * sts_monobit test header.
 */

#pragma once

/*
 * function prototype
 */
int sts_monobit(Test **test,int irun);

static Dtest sts_monobit_dtest __attribute__((unused)) = {
  "STS Monobit Test",
  "sts_monobit",
  "\
#==================================================================\n\
#                     STS Monobit Test\n\
# Very simple.  Counts the 1 bits in a long string of random uints.\n\
# Compares to expected number, generates a p-value directly from\n\
# erfc().  Very effective at revealing overtly weak generators;\n\
# Not so good at determining where stronger ones eventually fail.\n\
#==================================================================\n",
  100,
  100000,
  1,
  sts_monobit,
  0
};

