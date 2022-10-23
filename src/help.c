/*
 *========================================================================
 * $Id: help.c 447 2008-09-07 14:00:25Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

#include "dieharder.h"

static Dtest user_template_dtest = {
  "Example User Test",
  "user_template",
  "\n\
#==================================================================\n\
#                  Example Dieharder Test\n\
#\n\
#                     Lagged Sum Test\n\
# This package contains many very lovely tests.  Very few of them,\n\
# however, test for lagged correlations -- the possibility that\n\
# the random number generator has a bitlevel correlation after\n\
# some period.  Diehard tests, for example, COULD NOT test for this\n\
# sort of thing with only a few million rands to sample from.\n\
# The template test is therefore very simple.  It takes a user-\n\
# specified lag (-x lag) and adds up uniform deviates sampled with\n\
# that lag.  The mean of tsamples samples summed should be\n\
# 0.5*tsamples.  The standard deviation should be sqrt(tsamples/12).\n\
# The experimental values of the sum are thus converted into a\n\
# p-value (using the erf()) and a ks-test applied to psamples of them.\n\
#==================================================================\n",
  100,
  100000,
  1,
  user_template,
  0
};

void help_user_template(void)
{

  Rprintf("%s",user_template_dtest.description);

}

void help(void)
{

 /*
  * help for EACH test must be defined in the test.c package, even if
  * it is null or says "sorry, no help available".  help should be
  * a brief test description all pretty with each line beginning with
  * "#" so it is easy to filter out of standard test results output.
  */
 if(all==YES){
   //Rprintf("%s",rgb_timing_dtest.description);
   //Rprintf("%s",rgb_persist_dtest.description);
   //Rprintf("%s",rgb_bitdist_dtest.description);
   Rprintf("%s",rgb_minimum_distance_dtest.description);
   Rprintf("%s",rgb_permutations_dtest.description);
   Rprintf("%s",rgb_lagged_sums_dtest.description);
   Rprintf("%s",diehard_birthdays_dtest.description);
   Rprintf("%s",diehard_operm5_dtest.description);
   Rprintf("%s",diehard_rank_32x32_dtest.description);
   Rprintf("%s",diehard_rank_6x8_dtest.description);
   Rprintf("%s",diehard_bitstream_dtest.description);
   Rprintf("%s",diehard_opso_dtest.description);
   Rprintf("%s",diehard_oqso_dtest.description);
   Rprintf("%s",diehard_dna_dtest.description);
   Rprintf("%s",diehard_count_1s_stream_dtest.description);
   Rprintf("%s",diehard_count_1s_byte_dtest.description);
   Rprintf("%s",diehard_parking_lot_dtest.description);
   Rprintf("%s",diehard_2dsphere_dtest.description);
   Rprintf("%s",diehard_3dsphere_dtest.description);
   Rprintf("%s",diehard_squeeze_dtest.description);
   Rprintf("%s",diehard_sums_dtest.description);
   Rprintf("%s",diehard_runs_dtest.description);
   Rprintf("%s",diehard_craps_dtest.description);
   Rprintf("%s",marsaglia_tsang_gcd_dtest.description);
   Rprintf("%s",sts_monobit_dtest.description);
   Rprintf("%s",sts_runs_dtest.description);
   Rprintf("%s",sts_serial_dtest.description);
   help_user_template();
   Exit(0);
 }

 /*
  * Switches to do each test by number
  */
 switch(diehard){
   case DIEHARD_BDAY:
     Rprintf("%s",diehard_birthdays_dtest.description);
     break;
   case DIEHARD_OPERM5:
     Rprintf("%s",diehard_operm5_dtest.description);
     break;
   case DIEHARD_RANK_32x32:
     Rprintf("%s",diehard_rank_32x32_dtest.description);
     break;
   case DIEHARD_RANK_6x8:
     Rprintf("%s",diehard_rank_6x8_dtest.description);
     break;
   case DIEHARD_BITSTREAM:
     Rprintf("%s",diehard_bitstream_dtest.description);
     break;
   case DIEHARD_OPSO:
     Rprintf("%s",diehard_opso_dtest.description);
     break;
   case DIEHARD_OQSO:
     Rprintf("%s",diehard_oqso_dtest.description);
     break;
   case DIEHARD_DNA:
     Rprintf("%s",diehard_dna_dtest.description);
     break;
   case DIEHARD_COUNT_1S_STREAM:
     Rprintf("%s",diehard_count_1s_stream_dtest.description);
     break;
   case DIEHARD_COUNT_1S_BYTE:
     Rprintf("%s",diehard_count_1s_byte_dtest.description);
     break;
   case DIEHARD_PARKING_LOT:
     Rprintf("%s",diehard_parking_lot_dtest.description);
     break;
   case DIEHARD_2DSPHERE:
     Rprintf("%s",diehard_2dsphere_dtest.description);
     break;
   case DIEHARD_3DSPHERE:
     Rprintf("%s",diehard_3dsphere_dtest.description);
     break;
   case DIEHARD_SQUEEZE:
     Rprintf("%s",diehard_squeeze_dtest.description);
     break;
   case DIEHARD_SUMS:
     Rprintf("%s",diehard_sums_dtest.description);
     break;
   case DIEHARD_RUNS:
     Rprintf("%s",diehard_runs_dtest.description);
     break;
   case DIEHARD_CRAPS:
     Rprintf("%s",diehard_craps_dtest.description);
     break;
   case MARSAGLIA_TSANG_GCD:
     Rprintf("%s",marsaglia_tsang_gcd_dtest.description);
     break;
   case MARSAGLIA_TSANG_GORILLA:
     break;
   default:
     break;
 }
 switch(rgb){
   /* case RGB_TIMING: */
   /*   Rprintf("%s",rgb_timing_dtest.description); */
   /*   break; */
   /* case RGB_PERSIST: */
   /*   Rprintf("%s",rgb_persist_dtest.description); */
   /*   break; */
   /* case RGB_BITDIST: */
   /*   Rprintf("%s",rgb_bitdist_dtest.description); */
   /*   break; */
   case RGB_MINIMUM_DISTANCE:
     Rprintf("%s",rgb_minimum_distance_dtest.description);
     break;
   case RGB_PERMUTATIONS:
     Rprintf("%s",rgb_permutations_dtest.description);
     break;
   case RGB_LAGGED_SUMS:
     Rprintf("%s",rgb_lagged_sums_dtest.description);
     break;
   case RGB_LMN:
     break;
   default:
     break;
 }
 switch(sts){
   case STS_MONOBIT:
     Rprintf("%s",sts_monobit_dtest.description);
     break;
   case STS_RUNS:
     Rprintf("%s",sts_runs_dtest.description);
     break;
   case STS_SERIAL:
     Rprintf("%s",sts_serial_dtest.description);
     break;
   default:
     break;
 }
 /*
  * Add user-defined tests here following the pattern above.  Be sure
  * to add the new tests to the enum in dieharder.h.  Also add its
  * prototypes there, following the patterns of the tests already
  * implemented.
  */
 switch(user){
   case USER_TEMPLATE:
     Rprintf("%s",user_template_dtest.description);
     break;
   default:
     break;
 }

 if(diehard == NO && rgb == NO && sts == NO && user == NO && all == NO){
   Usage();
 }
 Exit(0);

}

void Usage(void)
{

 Rprintf("\n\
dieharder version %s Copyright 2003 Robert G. Brown\n\
 \n\
Usage:\n\
  dieharder [-a] [-b bits] [-d diehard test number] [-f filename]\n\
           [-g generator number] [-h] [-i iterations] [-l] \n\
           [-n ntuple] [-p number of p samples] [-q] [-o filename] \n\
           [-N] [-r rgb test number] [-s sts test number] \n\
	   [-t number of test samples] [-u user test number] \n\
	   [-v verbose flag] [-x xvalue] [-y yvalue] [-z zvalue] \n\
\n\
  -a Runs all the tests with standard/default options to create a report\n\
  -b bits - sets the number of bits to be used in tests that act on a bit\n\
     string of variable length, e.g. the rgb bitdist test.\n\
  -d test number -  selects specific diehard test.\n\
  -f filename - two generators permit either formatted ASCII numbers\n\
     or raw binary numbers to be read in from a file for testing.\n\
     Note well that some (default) tests require 10,000,000 or more rands!\n\
     To see a sample of the (required) header for ASCII formatted input, run\n\
\n\
         dieharder -o -f example.input -t 10\n\
\n\
     and then examine the contents of example.input.\n\
     Raw binary input reads 32 bit increments of the specified data stream.\n\
  -g generator number - selects a specific generator for testing.  Using\n\
     -1 causes all known generators to be printed out to the display.\n\
  -h prints context-sensitive help -- usually Usage (this message) or a\n\
     test synopsis if entered as e.g. dieharder -d 3 -h.\n\
  -i iterations - sets iteration count for timing runs (should not be\n\
     needed).\n\
  -l list all known tests.\n\
  -n ntuple - set ntuple length for tests on short bit strings that permit\n\
     the length to be varied (e.g. rgb bitdist).\n\
  -o filename - output -t count random numbers to file.\n\
  -N force the usage of non-overlapping samples in certain diehard tests.\n\
  -p count - sets the number of p-value samples per test (default 100).\n\
  -q selects \"quiet\" operation: results only are printed on a single line\n\
     (where applicable).  This is a way of getting a very compact report.\n\
  -r test number - selects specific rgb test.\n\
  -S seed.  Any (uint) seed but (default) 0 surpresses per-test,\n\
     fully automated reseeding from /dev/random if available or the \n\
     clock if not (where the latter is NOT really desireable).\n\
  -s test number - selects specific sts test.\n\
  -t count - sets the number of random entities used in each test, where\n\
     possible.  Be warned -- some tests will take a long time with the\n\
     default value of 10000.  Read the test synopses for suggested settings\n\
     for -t or use -a first.\n\
  -u test number - selects specific user-developed test, if you've added\n\
     one or more to this tool.  dieharder provides this interface to make\n\
     it easy to add your own tests.\n\
  -v verbose flag -- controls the verbosity of the output for debugging\n\
     only.  Probably of little use to non-developers, and developers can\n\
     read the enum(s) in dieharder.h and the test sources to see which\n\
     flag values turn on output on which routines.  1 is \"all\" and will\n\
     result in a fairly detailed trace of program activity.\n\
  -x,-y,-z number - Some tests have parameters that can safely be varied\n\
     from their default value.  For example, in the diehard birthdays test,\n\
     one can vary the number of \"dates\" drawn from the \"year\" of some\n\
     length, which can also be varied.  -x 2048 -y 30 alters these two\n\
     values but should still run fine.\n\
\n\
  NOTE WELL:  The \"bogomegarates\" returned by this tool are BOGUS\n\
  and may not be even approximately correct in your context.  Also, the\n\
  quality assessment(s) for the rngs may, in fact, be completely incorrect\n\
  or misleading.  Use them at your Own Risk!  Be Warned!\n\
\n",QUOTEME(VERSION));

 Exit(0);

}

/* Cruft
 * This is a standard way of putting out a test description
void help_test(Dtest *dtest)
{

 Rprintf("%s",dtest->description);
 
}
 */
