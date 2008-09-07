/*
 *========================================================================
 * $Id: rdieharder.c 255 2007-01-27 15:09:15Z db $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

#ifdef RDIEHARDER
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>

#include "dieharder.h"		/* from the front-end sources */

SEXP dieharder(SEXP genS, SEXP testS, SEXP seedS, SEXP psamplesS, SEXP verbS, SEXP infileS, SEXP ntupleS) {

    int verb, testarg, nprotect=0;
    unsigned int i;
    SEXP result = NULL, vec, pv, name, desc, nkps;
    char *inputfile;

    /*
     * Parse command line and set global variables
     */
    char *argv[] = { "dieharder" };
    optind = 0;
    parsecl(1, argv); 

    generator  = INTEGER_VALUE(genS);
    testarg = INTEGER_VALUE(testS);
    diehard = rgb = sts = user = 0;
    if (testarg < 100) {
	diehard = testarg;
    } else if (testarg < 200) {
	rgb = testarg - 100;
    } else if (testarg < 300) {
	sts = testarg - 200;
    } else {
	user = testarg - 300;
    }
    Seed = INTEGER_VALUE(seedS); /* (user-select) Seed, not (save switch) seed */
    psamples = INTEGER_VALUE(psamplesS);
    verb = INTEGER_VALUE(verbS);
    inputfile = (char*) CHARACTER_VALUE(infileS);
    ntuple = INTEGER_VALUE(ntupleS);

    if (strcmp(inputfile, "") != 0) {
	strncpy(filename, inputfile, 128);
	fromfile = 1;		/* flag this as file input */
	if (verb) Rprintf("Dieharder using input file %s generator %d fromfile %d seed %d\n", filename, generator, fromfile, Seed);
    }

    if (Seed == 0) {
	seed = random_seed();
    } else {
	seed = (unsigned long int) Seed;
    }

    if (verb) {
	Rprintf("Dieharder called with gen=%d test=%d seed=%lu\n", 
		generator, diehard, seed);
	quiet = 0;
	hist_flag = 1;
	/*verbose = D_ALL;*/
    } else {
	quiet = 1; 			/* override dieharder command-line default */
	hist_flag = 0;
    }

    /*
     * Note that most of my cpu_rates (except the terminally simple/stupid) 
     * have three phases after parsecl():
     *
     * Startup: Allocate memory, initialize all derivative variables from
     * command line values.  
     */
    if (verb) Rprintf("Dieharder before startup\n");
    startup();
    if (verb) Rprintf("Dieharder after startup\n");

    /*
     * Work: Do all the work.  In a complicated cpu_rate, project_work would
     * itself be a shell for a lot of other modular routines.
     */
    if (verb) Rprintf("Dieharder before work\n");
    work();
    if (verb) Rprintf("Dieharder after work\n");

    gsl_rng_free(rng);
    reset_bit_buffers();


    /* vector of size three: [0] is scalar ks_pv, [1] is pvalues vec, [2] name */
    PROTECT(result = allocVector(VECSXP, 3)); 

    /* alloc scalar, and set it */
    PROTECT(pv = allocVector(REALSXP, 1));
    PROTECT(name = allocVector(STRSXP, 1));
    if (rdh_testptr != NULL) {
	REAL(pv)[0] = rdh_testptr->ks_pvalue;
	/* alloc vector and set it */
	PROTECT(vec = allocVector(REALSXP, rdh_testptr->psamples)); 
	nprotect++;
	for (i = 0; i < rdh_testptr->psamples; i++) {
	    REAL(vec)[i] = rdh_testptr->pvalues[i];
	}
	SET_STRING_ELT(name, 0, mkChar(rdh_dtestptr->name));
    } else {
	PROTECT(vec = allocVector(REALSXP, 1)); 
	REAL(pv)[0] = R_NaN;
	REAL(vec)[0] = R_NaN;
	SET_STRING_ELT(name, 0, mkChar(""));
    }
    //PROTECT(desc = allocVector(STRSXP, 1));
    //SET_STRING_ELT(desc, 0, mkChar(rdh_dtestptr->description));
    //PROTECT(nkps = allocVector(REALSXP, 1));
    //REAL(nkps)[0] = (double) rdh_testptr->nkps;

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
	Rprintf("C function dieharder called with gen=%d test=%d -> %f\n", generator, diehard, rdh_testptr->ks_pvalue);
	REAL(result)[i] = rdh_testptr->ks_pvalue;
    }

    UNPROTECT(1);
    return result;
}

SEXP dieharderGenerators(void) {
    SEXP result, gens, genid;
    unsigned int i,j;

    /* from startup.c */
    /*
     * We new have to work a bit harder to determine how many
     * generators we have of the different types because there are
     * different ranges for different sources of generator.
     *
     * We start with the basic GSL generators, which start at offset 0.
     */
    types = dieharder_rng_types_setup ();
    i = 0;
    while(types[i] != NULL){
	i++;
	j++;
    }
    num_gsl_rngs = i;

    /*
     * Next come the dieharder generators, which start at offset 200.
     */
    i = 200;
    j = 0;
    while(types[i] != NULL){
	i++;
	j++;
    }
    num_dieharder_rngs = j;

    /*
     * Next come the R generators, which start at offset 400.
     */
    i = 400;
    j = 0;
    while(types[i] != NULL){
	i++;
	j++;
    }
    num_R_rngs = j;

    /*
     * Next come the hardware generators, which start at offset 500.
     */
    i = 500;
    j = 0;
    while(types[i] != NULL){
	i++;
	j++;
    }
    num_hardware_rngs = j;

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
    num_ui_rngs = j;

    /* /\* */
    /*  * Now add my own types and count THEM. */
    /*  *\/ */
    /* add_ui_rngs(); */
    /* while(types[i] != NULL){ */
    /* 	i++; */
    /* } */

    num_rngs = num_gsl_rngs + num_dieharder_rngs + num_R_rngs +
	num_hardware_rngs + num_ui_rngs;

    /* vector of size onetwo: [0] is scalar ks_pv, [1] is vector of pvalues */
    PROTECT(result = allocVector(VECSXP, 2)); 
    PROTECT(gens = allocVector(STRSXP, num_rngs)); 
    PROTECT(genid = allocVector(INTSXP, num_rngs)); 

    j = 0;
    for (i = 0; i < num_gsl_rngs; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(types[i]->name));
	INTEGER(genid)[j++] = i;
    }
    for (i = 200; i < 200 + num_dieharder_rngs; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(types[i]->name));
	INTEGER(genid)[j++] = i;
    }
    for (i = 400; i < 400 + num_R_rngs; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(types[i]->name));
	INTEGER(genid)[j++] = i;
    }
    for (i = 500; i < 500 + num_hardware_rngs; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(types[i]->name));
	INTEGER(genid)[j++] = i;
    }
    for (i = 600; i < 600 + num_ui_rngs; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(types[i]->name));
	INTEGER(genid)[j++] = i;
    }
    SET_VECTOR_ELT(result, 0, gens);
    SET_VECTOR_ELT(result, 1, genid);
  
    UNPROTECT(3);
    return result;
}
#endif   /* RDIEHARDER */
