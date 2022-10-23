/*
 *========================================================================
 * $Id: startup.c 442 2008-09-06 18:37:53Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 * This routine does all the required initialization and startup,
 * including memory allocation and prefilling of vectors.  It is
 * COMPLETELY outside the timing loops.
 *========================================================================
 */

#include "dieharder.h"
static int firstcall=1;

void startup(void)
{

 int i,j;

 /*
  * Large chunks of the following should only be done on the first call
  * to dieharder startup().  Maybe all of it, we'll see.  Either way,
  * the firstcall flag can be used to ensure that only the parts that can
  * safely be repeated or are required to be repeated are repeated.
  */

 if(firstcall == 1){
   /* printf("This is the first call to startup(), I hope.\n"); */
   /*
    * The very first thing we do is see if any simple help options
    * were requested, as we exit immediately if they were and can
    * save all sorts of work.
    */
   if(list == YES) {
     list_tests();
     Exit(0);
   }

   /*
    * This fills the global *dh_rng_types defined in libdieharder with all
    * the rngs known directly to libdieharder including known hardware
    * generators and file input "generators".  This routine also sets the
    * counts of each "kind" of generator into global/shared variables.  This
    * command must be run (by all UIs, not just the dieharder CLI) BEFORE
    * adding any UI generators, and BEFORE selecting a generator or input
    * stream to test.
    */
   dieharder_rng_types();

   /*
    * We new have to work a bit harder to determine how many
    * generators we have of the different types because there are
    * different ranges for different sources of generator.
    *
    * We start with the basic GSL generators, which start at offset 0.
    */
   i = 0;
   while(types[i] != NULL){
     i++;
   }
   dh_num_gsl_rngs = i;
   MYDEBUG(D_STARTUP){
     Rprintf("# startup:  Found %u GSL rngs.\n",dh_num_gsl_rngs);
   }

   /*
    * Next come the dieharder generators, which start at offset 200.
    */
   i = 200;
   j = 0;
   while(types[i] != NULL){
     i++;
     j++;
   }
   dh_num_dieharder_rngs = j;
   MYDEBUG(D_STARTUP){
     Rprintf("# startup:  Found %u dieharder rngs.\n",dh_num_dieharder_rngs);
   }

   /*
    * Next come the R generators, which start at offset 400.
    */
   i = 400;
   j = 0;
   while(types[i] != NULL){
     i++;
     j++;
   }
   dh_num_R_rngs = j;
   MYDEBUG(D_STARTUP){
     Rprintf("# startup:  Found %u R rngs.\n",dh_num_R_rngs);
   }

   /*
    * Next come the hardware generators, which start at offset 500.
    */
   i = 500;
   j = 0;
   while(types[i] != NULL){
     i++;
     j++;
   }
   dh_num_hardware_rngs = j;
   MYDEBUG(D_STARTUP){
     Rprintf("# startup:  Found %u hardware rngs.\n",dh_num_hardware_rngs);
   }

   /*
    * Finally, any generators added by the user at the interface level.
    * That is, if you are hacking dieharder to add your own rng, add it
    * below and it should "just appear" in the dieharder list of available
    * generators and be immediately useful.
    */
   i = 600;
   j = 0;
   types[i] = gsl_rng_empty_random;
   i++;
   j++;
   dh_num_user_rngs = j;
   MYDEBUG(D_STARTUP){
     Rprintf("# startup:  Found %u user interface generators.\n",dh_num_user_rngs);
   }

   dh_num_rngs = dh_num_gsl_rngs + dh_num_dieharder_rngs + dh_num_R_rngs +
       dh_num_hardware_rngs + dh_num_user_rngs;

 }

 /*
  * In principle, in Rdh, one could change to an illegal generator
  * in a new call.  This is probably not a good way to manage it, but
  * it will do for now.
  */
 //if (generator == -1){
if (generator == 0){
   list_rngs();
   Exit(0);
 }

 if(generator == 0 || generator >= MAXRNGS){
   REprintf("Error:  rng %d does not exist!\n",generator);
   list_rngs();
   Exit(0);
 }

 if(types[generator] == 0){
   REprintf("Error:  rng %d does not exist!\n",generator);
   list_rngs();
   Exit(0);
 }

 /*
  * We need a sanity check for file input.  File input is permitted
  * iff we have a file name, if the output flag is not set, AND if
  * generator is either file_input or file_input_raw.  Otherwise
  * IF generator is a file input type (primary condition) we punt
  * with a list of types and a honk.
  */
 if(strncmp("file_input",types[generator]->name,10) == 0){
   if(fromfile != 1){
     REprintf("Error: generator %s uses file input but no file has been loaded",types[generator]->name);
     list_rngs();
     Exit(0);
   }
   if(output_file){
     REprintf("Error: generator %s uses file input but output flag set.",types[generator]->name);
     Usage();
     Exit(0);
   }
 }

 /*
  * If we get here, in principle the generator is valid and the right
  * inputs are defined to run it (in the case of file_input). We therefore
  * initialize the selected gsl rng using (if possible) random_seed()
  * seeds from /dev/random.  If this is a second or third pass through
  * startup, as long as gsl_rng_free(rng) has run, we SHOULD be clear
  * to re-allocate it or a new one now.
  */
 if(verbose == D_STARTUP || verbose == D_SEED || verbose == D_ALL){
   Rprintf("# startup(): Creating and seeding generator %s\n",types[generator]->name);
 }
 rng = gsl_rng_alloc(types[generator]);
 if(Seed == 0){
   seed = random_seed();
   if(verbose == D_STARTUP || verbose == D_SEED || verbose == D_ALL){
     Rprintf("# startup(): Generating random seed %lu\n",seed);
   }
 } else {
   seed = Seed;
   if(verbose == D_STARTUP || verbose == D_SEED || verbose == D_ALL){
     Rprintf("# startup(): Setting random seed %lu by hand.\n",seed);
   }
 }
 gsl_rng_set(rng,seed);
 /* It is indeed the same on looped calls, so this is really cruft.
 Rprintf("Just for grins, the first rand returned by the generator is %lu\n",
  gsl_rng_get(rng));
  */

 /*
  * Simultaneously count the number of significant bits in the rng
  * AND create a mask.  Note that several routines in bits WILL NOT
  * WORK unless this is all set correctly, which actually complicates
  * things because I stupidly didn't make this data into components
  * of the rng object (difficult to do given that the latter is actually
  * already defined in the GSL, admittedly).
  */
 random_max = gsl_rng_max(rng);
 rmax = random_max;
 rmax_bits = 0;
 rmax_mask = 0;
 while(rmax){
   rmax >>= 1;
   rmax_mask = rmax_mask << 1;
   rmax_mask++;
   rmax_bits++;
 }

 if(output_file){
   output_rnds();
   /* We'll fix it so we don't have to exit here later, maybe. */
   Exit(0);
 }

 /*
  * TESTING PLAYGROUND.  bits.c code MUST be carefully validated as it is
  * quite complex and crucial to happy testing.  We cannot test this until
  * AFTER rng is set up.
  testbits();
  Exit(0);
  *
  */


 /*
  * Probably cruft, but in any event the right solution is to malloc the
  * memory inside the test itself and free it at the end.
  */
 ks_pvalue = 0;
 ks_pvalue2 = 0;
 kspi = 0;

 firstcall = 0;

}
