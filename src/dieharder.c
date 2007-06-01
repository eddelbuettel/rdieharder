
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>

#include "dieharder.h"		/* from the front-end sources */

SEXP dieharder(SEXP genS, SEXP testS, SEXP seedS, SEXP psamplesS, SEXP verbS) {
  generator  = INTEGER_VALUE(genS);
  diehard = INTEGER_VALUE(testS);
  Seed = INTEGER_VALUE(seedS); /* (user-select) Seed, not (save switch) seed */
  psamples = INTEGER_VALUE(psamplesS);
  int verb = INTEGER_VALUE(verbS);
  unsigned int i;
  SEXP result = NULL, vec, pv, name, desc, nkps;

  if (Seed == 0) {
    seed = random_seed();
  } else {
    seed = Seed;
  }

  if (verb) {
    Rprintf("Dieharder called with gen=%d test=%d\n", generator, diehard);
    quiet = 0;
    hist_flag = 1;
  } else {
    quiet = 1; 			/* override dieharder command-line default */
    hist_flag = 0;
  }

  /*
   * Parse command line and set global variables
   */
  /*parsecl(argc,argv); */


  /*
   * Note that most of my cpu_rates (except the terminally simple/stupid) 
   * have three phases after parsecl():
   *
   * Startup: Allocate memory, initialize all derivative variables from
   * command line values.  
   */
  startup();

  /*
   * Work: Do all the work.  In a complicated cpu_rate, project_work would
   * itself be a shell for a lot of other modular routines.
   */
  work();


  /* vector of size two: [0] is scalar ks_pv, [1] is vector of pvalues */
  PROTECT(result = allocVector(VECSXP, 3)); 
  /* alloc scalar, and set it */
  PROTECT(pv = allocVector(REALSXP, 1));
  REAL(pv)[0] = testptr->ks_pvalue;
  /* vector and set it */
  PROTECT(vec = allocVector(REALSXP, testptr->psamples)); 
  for (i = 0; i < testptr->psamples; i++) {
    REAL(vec)[i] = testptr->pvalues[i];
  }
  PROTECT(name = allocVector(STRSXP, 1));
  SET_STRING_ELT(name, 0, mkChar(dtestptr->name));
  //PROTECT(desc = allocVector(STRSXP, 1));
  //SET_STRING_ELT(desc, 0, mkChar(dtestptr->description));
  //PROTECT(nkps = allocVector(REALSXP, 1));
  //REAL(nkps)[0] = (double) testptr->nkps;

  /* insert scalar and vector */
  SET_VECTOR_ELT(result, 0, pv);
  SET_VECTOR_ELT(result, 1, vec);
  SET_VECTOR_ELT(result, 2, name);
  
  //SET_VECTOR_ELT(result, 3, desc);  /* too long, and C formatted */
  //SET_VECTOR_ELT(result, 4, nkps);  /* not sure this is needed or useful */
  
  UNPROTECT(4);
  return result;
}


SEXP dieharderVectorised(SEXP genS, SEXP testS, SEXP verbS) {
  //int *genvec = INTEGER(genS);
  int ngen, i;
  diehard = INTEGER_VALUE(testS);
  int verb = INTEGER_VALUE(verbS);
  SEXP result = NULL;
  ngen = length(genS);

  if (verb) {
    Rprintf("C function dieharder called with gen=%d test=%d\n", 
	    generator, diehard);
  }

  /*
   * Parse command line and set global variables
   */
  /*parsecl(argc,argv); */

  quiet = 1; 			/* override dieharder command-line default */

  PROTECT(result = allocVector(REALSXP, ngen));

  for (i=0; i<ngen; i++) {
    generator = INTEGER( VECTOR_ELT(genS, i) )[0];


  /*
   * Note that most of my cpu_rates (except the terminally simple/stupid) 
   * have three phases after parsecl():
   *
   * Startup: Allocate memory, initialize all derivative variables from
   * command line values.  
   */
    startup();

  /*
   * Work: Do all the work.  In a complicated cpu_rate, project_work would
   * itself be a shell for a lot of other modular routines.
   */

    work();
    Rprintf("C function dieharder called with gen=%d test=%d -> %f\n", generator, diehard, testptr->ks_pvalue);
    REAL(result)[i] = testptr->ks_pvalue;
  }

  UNPROTECT(1);
  return result;
}

