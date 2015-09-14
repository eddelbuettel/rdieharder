
/*
 * RDieHarder interface to DieHarder
 * Copyright (C) 2006 - 2011 Dirk Eddelbuettel
 * GPL'ed
 *
 * Based on dieharder.c from DieHarder, and interfacing DieHarder
 * DieHarder is Copyright 2002 - 2008 (C) Robert G. Brown and GPL'ed
 *
 */

#ifdef RDIEHARDER
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dieharder.h"		/* from the front-end sources */

void choose_rng();
void run_all_tests();
void run_test();

SEXP dieharder(SEXP genS, SEXP testS, SEXP seedS, SEXP psamplesS, SEXP verbS, SEXP infileS, SEXP ntupleS) {

    /* In the RDieHarder/R/zzz.R startup code, dieharderGenerators()
     * has already called dieharder_rng_types(), and dieharderTests()
     * has already called dieharder_rng_tests().  The results are
     * stored in RDieHarder:::.dieharderGenerators and
     * RDieHarder:::.dieharderTests, as well as C static variables
     * used by libdieharder.  Since user defined generators and tests
     * may have been added, we do not call these routines again. 
     */

    int verb;
    char *inputfile;

    char *argv[] = { "dieharder" };	/* Setup argv to allow call of parsecl() to let dieharder set globals */
    optind = 0;
    parsecl(1, argv);			/* also covers part of setup_globals() */

    generator  = INTEGER_VALUE(genS);	/* 'our' parameters from R; used below by choose_rng() and run_test() */
    dtest_num = INTEGER_VALUE(testS);

    Seed = (unsigned long int) INTEGER_VALUE(seedS); /* (user-select) Seed, not (save switch) seed */
    psamples = INTEGER_VALUE(psamplesS);
    verb = INTEGER_VALUE(verbS);
    inputfile = (char*) CHARACTER_VALUE(infileS);
    ntuple = INTEGER_VALUE(ntupleS);

    result = NULL;

    if (strcmp(inputfile, "") != 0) {
	strncpy(filename, inputfile, 128);
	fromfile = 1;			/* flag this as file input */
    }
 
   if (Seed == 0) {
    	seed = random_seed();
    } else {
    	seed = (unsigned long int) Seed;
    }

    if (verb) {
	Rprintf("Dieharder called with gen=%d test=%d seed=%lu\n", generator, dtest_num, seed);
	quiet = 0;
	hist_flag = 1;
    } else {
	quiet = 1; 			/* override dieharder command-line default */
	hist_flag = 0;
    }

    
    /*
     * Pick a rng, establish a seed based on how things were initialized
     * in parsecl() or elsewhere.  Note that choose_rng() times the selected
     * rng as a matter of course now.
     */
    choose_rng();

    /*
     * At this point, a valid rng should be selected, allocated, and
     * provisionally seeded.  It -a(ll) is set (CLI only) run all the
     * available tests on the selected rng, reseeding at the beginning of
     * each test if Seed is nonzero.  Otherwise, run the single selected
     * test (which may still return a vector of pvalues) on the single
     * selected rng.  The CLI then goes on to exit; an interactive UI would
     * presumably loop back to permit the user to run another test on the
     * selected rng or select a new rng (and run more tests on it) until the
     * user elects to exit.
     *
     * It is the UI's responsibility to ensure that run_test() is not called
     * without choosing a valid rng first!
     */
    /* if(all){ */
    /* 	run_all_tests(); */
    /* } else { */
	run_test();
    /* } */

    /*
     * This ends the core loop for a non-CLI interactive UI.  GUIs will
     * typically exit directly from the event loop.  Tool UIs may well fall
     * through, and the CLI simply proceeds sequentially to exit.  It isn't
     * strictly necessary to execute an exit() command at the end, but it
     * does make the code a bit clearer (and let's one choose an exit code,
     * if that might ever matter.  Exit code 0 clearly means "completed
     * normally".
     */
    if (rng != NULL) {
	gsl_rng_free(rng);
	rng = NULL;
    }
    reset_bit_buffers();

    return result;    			/* And then bring our results back to R */
}


SEXP dieharderGenerators(void) {
    SEXP result, gens, genid;
    unsigned int i,j;

    dh_num_gsl_rngs = dh_num_dieharder_rngs = dh_num_R_rngs = dh_num_hardware_rngs = dh_num_user_rngs = 0;

    dieharder_rng_types (); /* fills dh_rng_types[] -- from libdieharder/dieharder_rng_types.c */

    /*
     * We new have to work a bit harder to determine how many
     * generators we have of the different types because there are
     * different ranges for different sources of generator.
     *
     * We start with the basic GSL generators, which start at offset 0.
     */
    i = 0;
    while(dh_rng_types[i] != NULL){
	i++;
	j++;
    }
    dh_num_gsl_rngs = i;

    /*
     * Next come the dieharder generators, which start at offset 200.
     */
    i = 200;
    j = 0;
    while(dh_rng_types[i] != NULL){
	i++;
	j++;
    }
    dh_num_dieharder_rngs = j;

    /*
     * Next come the R generators, which start at offset 400.
     */
    i = 400;
    j = 0;
    while(dh_rng_types[i] != NULL){
	i++;
	j++;
    }
    dh_num_R_rngs = j;

    /*
     * Next come the hardware generators, which start at offset 500.
     */
    i = 500;
    j = 0;
    while(dh_rng_types[i] != NULL){
	i++;
	j++;
    }
    dh_num_hardware_rngs = j;

    /*
     * Finally, any generators added by the user at the interface level.
     * That is, if you are hacking dieharder to add your own rng, add it
     * below and it should "just appear" in the dieharder list of available
     * generators and be immediately useful.
     */
    i = 600;
    j = 0;
    dh_rng_types[i] = gsl_rng_empty_random;  
    i++;
    j++;
    dh_num_user_rngs = j;

    /* /\* */
    /*  * Now add my own types and count THEM. */
    /*  *\/ */
    /* add_ui_rngs(); */
    /* while(types[i] != NULL){ */
    /* 	i++; */
    /* } */

    dh_num_rngs = dh_num_gsl_rngs + dh_num_dieharder_rngs + dh_num_R_rngs +
	dh_num_hardware_rngs + dh_num_user_rngs;

    /* REprintf("RNGs: %d = %d GSL + %d DieHarder + %d R + %d Hardware + %d User \n",  */
    /* 	     dh_num_rngs, dh_num_gsl_rngs, dh_num_dieharder_rngs, dh_num_R_rngs, dh_num_hardware_rngs, dh_num_user_rngs); */

    /* vector of size two: [0] is string with description, [1] is int id number */
    PROTECT(result = allocVector(VECSXP, 2)); 
    PROTECT(gens = allocVector(STRSXP, dh_num_rngs)); 
    PROTECT(genid = allocVector(INTSXP, dh_num_rngs)); 

    j = 0;
    for (i = 0; i < dh_num_gsl_rngs; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(dh_rng_types[i]->name));
	INTEGER(genid)[j++] = i;
    }
    for (i = 200; i < 200 + dh_num_dieharder_rngs; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(dh_rng_types[i]->name));
	INTEGER(genid)[j++] = i;
    }
    for (i = 400; i < 400 + dh_num_R_rngs; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(dh_rng_types[i]->name));
	INTEGER(genid)[j++] = i;
    }
    for (i = 500; i < 500 + dh_num_hardware_rngs; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(dh_rng_types[i]->name));
	INTEGER(genid)[j++] = i;
    }
    for (i = 600; i < 600 + dh_num_user_rngs; i++) { 
     	SET_STRING_ELT(gens,  j,   mkChar(dh_rng_types[i]->name)); 
     	INTEGER(genid)[j++] = i; 
    } 
    SET_VECTOR_ELT(result, 0, gens);
    SET_VECTOR_ELT(result, 1, genid);
  
    UNPROTECT(3);
    return result;
}

SEXP dieharderTests(void) {
    SEXP result, gens, genid;
    unsigned int i,j;

    // just in case: reset everything
    dh_num_diehard_tests = dh_num_sts_tests = dh_num_other_tests = dh_num_user_tests = 0;

    dieharder_test_types (); /* fills dh_test_types[] -- from libdieharder/dieharder_test_types.c */

    dh_num_tests = dh_num_diehard_tests + dh_num_sts_tests + dh_num_other_tests + dh_num_user_tests;
    //REprintf("Tests: %d = %d DieHarder + %d STS + %d Other + %d User \n", dh_num_tests, dh_num_diehard_tests, dh_num_sts_tests, dh_num_other_tests, dh_num_user_tests); 

    /* vector of size two: [0] is string with description, [1] is int id number */
    PROTECT(result = allocVector(VECSXP, 2)); 
    PROTECT(gens = allocVector(STRSXP, dh_num_tests)); 
    PROTECT(genid = allocVector(INTSXP, dh_num_tests)); 

    j = 0;
    for (i = 0; i < dh_num_diehard_tests; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(dh_test_types[i]->sname));
	INTEGER(genid)[j++] = i;
    }
    for (i = 100; i < 100 + dh_num_sts_tests; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(dh_test_types[i]->sname));
	INTEGER(genid)[j++] = i;
    }
    for (i = 200; i < 200 + dh_num_other_tests; i++) {
	SET_STRING_ELT(gens,  j,   mkChar(dh_test_types[i]->sname));
	INTEGER(genid)[j++] = i;
    }
    for (i = 600; i < 600 + dh_num_user_tests; i++) { 
     	SET_STRING_ELT(gens,  j,   mkChar(dh_test_types[i]->sname)); 
     	INTEGER(genid)[j++] = i; 
    } 
    SET_VECTOR_ELT(result, 0, gens);
    SET_VECTOR_ELT(result, 1, genid);
  
    UNPROTECT(3);
    return result;
}


void save_values_for_R(Dtest *dtest,Test **test) {
    unsigned int i;
    SEXP vec, pv, name, nkps;

    Test **rdh_testptr = NULL;
    Dtest *rdh_dtestptr = NULL;
    
    if (rdh_dtestptr == NULL) {
	rdh_dtestptr = dtest;
	/* we use R_alloc as R will free this upon return; see R Extensions manual */
	rdh_testptr = (Test **) R_alloc((size_t) dtest->nkps, sizeof(Test *));
	for(i=0; i<dtest->nkps; i++) {
	    rdh_testptr[i] = (Test *) R_alloc(1, sizeof(Test));
	    memcpy(rdh_testptr[i], test[i], sizeof(Test));
	}
    }

    /* create vector of size four: [0] is vector (!!) ks_pv, [1] is pvalues vec, [2] name, [3] nkps */
    PROTECT(result = allocVector(VECSXP, 4)); 

    /* alloc vector and scalars, and set it */
    PROTECT(pv = allocVector(REALSXP, rdh_dtestptr->nkps));
    PROTECT(name = allocVector(STRSXP, 1));
    PROTECT(nkps = allocVector(INTSXP, 1));

    if (rdh_testptr != NULL && rdh_dtestptr != NULL) {
	for (i=0; i<rdh_dtestptr->nkps; i++) { 		/* there can be nkps p-values per test */
	    REAL(pv)[i] = rdh_testptr[i]->ks_pvalue;
	}
	PROTECT(vec = allocVector(REALSXP, rdh_testptr[0]->psamples)); /* alloc vector and set it */
	for (i = 0; i < rdh_testptr[0]->psamples; i++) {
	    REAL(vec)[i] = rdh_testptr[0]->pvalues[i];
	}
	SET_STRING_ELT(name, 0, mkChar(rdh_dtestptr->name));
	INTEGER(nkps)[0] = rdh_dtestptr->nkps; 		/* nb of Kuiper KS p-values in pv vector */
    } else {
	PROTECT(vec = allocVector(REALSXP, 1)); 
	REAL(pv)[0] = R_NaN;
	REAL(vec)[0] = R_NaN;
	SET_STRING_ELT(name, 0, mkChar(""));
	INTEGER(nkps)[0] = R_NaN;
    }

    /* insert vectors and scalars into res vector */
    SET_VECTOR_ELT(result, 0, pv);
    SET_VECTOR_ELT(result, 1, vec);
    SET_VECTOR_ELT(result, 2, name);
    SET_VECTOR_ELT(result, 3, nkps);
  
    UNPROTECT(5);
}
#endif   /* RDIEHARDER */

