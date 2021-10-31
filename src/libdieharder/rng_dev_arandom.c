/*
 * dev_arandom
 * 
 * See copyright in copyright.h and the accompanying file COPYING
 *
 */

#include <dieharder/libdieharder.h>

static unsigned long int dev_arandom_get (void *vstate);
static double dev_arandom_get_double (void *vstate);
static void dev_arandom_set (void *vstate, unsigned long int s);

typedef struct
  {
    FILE *fp;
  }
dev_arandom_state_t;

static unsigned long int
dev_arandom_get (void *vstate)
{
  dev_arandom_state_t *state = (dev_arandom_state_t *) vstate;
  unsigned long int j;

  if(state->fp != NULL) {
    int rc = fread(&j,sizeof(j),1,state->fp);
    if (rc == 0) REprintf( "Error: Nothing written.\n");
    return j;
  } else {
    Rf_error("Error: /dev/arandom not open.  Exiting.\n");
    //exit(0);
  }

}

static double
dev_arandom_get_double (void *vstate)
{
  return dev_arandom_get (vstate) / (double) UINT_MAX;
}

static void
dev_arandom_set (void *vstate, unsigned long int s)
{
  dev_arandom_state_t *state = (dev_arandom_state_t *) vstate;

 if ((state->fp = fopen("/dev/arandom","r")) == NULL) {
   Rf_error("Error: Cannot open /dev/arandom, exiting.\n"
            "/dev/arandom may only be available on BSD systems.\n");
   //exit(0);
 }

 return;

}

static const gsl_rng_type dev_arandom_type =
{"/dev/arandom",		/* name */
 UINT_MAX,			/* RAND_MAX */
 0,				/* RAND_MIN */
 sizeof (dev_arandom_state_t),
 &dev_arandom_set,
 &dev_arandom_get,
 &dev_arandom_get_double};

const gsl_rng_type *gsl_rng_dev_arandom = &dev_arandom_type;
