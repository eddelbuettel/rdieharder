/*
* $Id: list_rand.c 255 2007-01-27 15:09:15Z rgb $
*
* See copyright in copyright.h and the accompanying file COPYING
* See also accompanying file STS.COPYING
*
*/

/*
 *========================================================================
 * This routine generates an output list of random numbers using the
 * selected generator.  The output goes to stdout and can be redirected
 * or piped as desired at the shell level.  The output list includes both
 * integer and uniform deviate output from the routine (one call following
 * another) and hence is NOT ideal for tests of sequential correlation.
 *========================================================================
 */

#include "dieharder.h"

void list_rand(void)
{

 int i;
 
 Rprintf("#==================================================================\n");
 Rprintf("# generator type: %s\n", gsl_rng_name(rng));
 Rprintf("# seed value: %lu, max value = %u  count = %d\n",seed, random_max, (int)tsamples);
 Rprintf("# Count\t int rand\tuniform rand\n");
 Rprintf("# ==================================================================\n");
 for(i = 1;i<=tsamples;i++) Rprintf("%d\t%lu\t %10.8f\n",i,gsl_rng_get(rng),gsl_rng_uniform(rng));

}

