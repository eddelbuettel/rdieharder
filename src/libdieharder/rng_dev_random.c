/*
 * dev_random
 * 
 * See copyright in copyright.h and the accompanying file COPYING
 *
 */

#include <dieharder/libdieharder.h>

static unsigned long int dev_random_get (void *vstate);
static double dev_random_get_double (void *vstate);
static void dev_random_set (void *vstate, unsigned long int s);

typedef struct
  {
    FILE *fp;
  }
dev_random_state_t;

static unsigned long int
dev_random_get (void *vstate)
{
  dev_random_state_t *state = (dev_random_state_t *) vstate;
  unsigned long int j;

  if(state->fp != NULL) {
    int rc = fread(&j,sizeof(j),1,state->fp);
    if (rc == 0) REprintf( "Error: Nothing written.\n");
    return j;
  } else {
    Rf_error("Error: /dev/random not open.  Exiting.\n");
    //exit(0);
  }

}

static double
dev_random_get_double (void *vstate)
{
  return dev_random_get (vstate) / (double) UINT_MAX;
}

static void
dev_random_set (void *vstate, unsigned long int s)
{
  dev_random_state_t *state = (dev_random_state_t *) vstate;

 if ((state->fp = fopen("/dev/random","r")) == NULL) {
   Rf_error("Error: Cannot open /dev/random, exiting.\n"
            "/dev/random may only be available on Linux systems.\n");
   //exit(0);
 }

 return;

}

static const gsl_rng_type dev_random_type =
{"/dev/random",			/* name */
 UINT_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (dev_random_state_t),
 &dev_random_set,
 &dev_random_get,
 &dev_random_get_double};

const gsl_rng_type *gsl_rng_dev_random = &dev_random_type;
