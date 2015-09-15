#include "dieharder.h"	    
#include <R_ext/Parse.h>

#define CSTRING(string) CHAR(STRING_ELT(string, 0))
typedef unsigned long int uli_t;

static SEXP global_parsed_callback = NULL;

static void callback_set_seed(void *unused, uli_t seed) {
    // call R's set.seed using equivalent of parse(text="set.seed(seed)")
    SEXP call = PROTECT(lang2(install("set.seed"), ScalarInteger(seed)));
    int evalError;
    SEXP result = R_tryEval(call, R_GlobalEnv, &evalError);
    UNPROTECT(1);
    if (evalError) error("Failed to eval 'set.seed(%d)'", seed);
    if (! quiet) Rprintf("\nSet seed to %u\n", seed);
    return;
}

static uli_t callback_get_int(void *unused) {
    if (global_parsed_callback == NULL) return 0;
    int evalError;
    SEXP random_sexp = R_tryEval(global_parsed_callback, R_GlobalEnv, &evalError);
    if (evalError) {
        error("Failed to eval stored call");
        return 0;
    }
    uli_t random_int = INTEGER_VALUE(random_sexp);
    if (verbose) Rprintf("%u ", random_int);
    return random_int;
}

static void initialize_globals(SEXP bits_sexp, SEXP seed_sexp, SEXP ntuple_sexp, 
			       SEXP quiet_sexp, SEXP verbose_sexp) {

    // FUTURE: add pmultiplier to user supplied?
    quiet = INTEGER_VALUE(quiet_sexp);
    if (! quiet) hist_flag = 1; // print histogram unless quiet
    verbose = INTEGER_VALUE(verbose_sexp);
    ntuple = INTEGER_VALUE(ntuple_sexp);

    uli_t bits = INTEGER_VALUE(bits_sexp);
    uli_t max = (1LL << bits) - 1;

    gsl_rng_type callback_rng = {
	"callback",         // arbitrary name
	max,                // rand max (pow2 - 1)
	0,  	            // rand min (must be 0) 
	0,                  // no storage space
	&callback_set_seed, // set seed 
	&callback_get_int,  // random int
	NULL                // no double
    };

    gsl_rng_default_seed = INTEGER_VALUE(seed_sexp); 
    rng = gsl_rng_alloc(&callback_rng); // uses gsl_rng_default_seed

    rmax_mask = max;
    rmax_bits = bits;

    overlap = 1;       /* use overlapping samples */
    multiply_p = 1;    /* use default number of psamples */
    gnumbs[0] = 13;    /* mt19937 as a "good" generator */

    Xtrategy = 1;          /* 0 means "no strategy, off", the default */
    Xweak = 0.005;         /* Point where generator is flagged as "weak" */
    Xfail = 0.000001;      /* Point where generator unambiguously fails dieharder */
    Xstep = 100;           /* Number of pvalues to add when iterating in TTD/RA */
    Xoff = 100000;         /* This is quite stressful and may run a long time */
}

static SEXP parse_and_test_callback(SEXP callback) {
    ParseStatus parseStatus;
    SEXP parsedVector = R_ParseVector(callback, 1, &parseStatus, R_NilValue);
    if (parseStatus != PARSE_OK) {
        error("Failed to parse \'%s\' as string", CSTRING(callback));
	return NULL;
    }
    
    SEXP parsedCallback = VECTOR_ELT(parsedVector, 0);

    int evalError;
    SEXP result = R_tryEval(parsedCallback, R_GlobalEnv, &evalError);
    if (evalError) {
        error("Failed to eval parsed callback '%s'", CSTRING(callback));
	return NULL;
    }

    if (TYPEOF(result) != INTSXP && TYPEOF(result) != REALSXP) {
        error("Result from eval parsed callback '%s' is not integer or real", CSTRING(callback));
	return NULL;
    }
	
    if (length(result) != 1) {
        error("Result from eval parsed callback '%s' is not length 1", CSTRING(callback));
	return NULL;
    }

    if (INTEGER_VALUE(result) == NA_INTEGER)  {
	// this might be a rare random error but safest to force retry
	error("Result from eval parsed callback '%s' was NA", CSTRING(callback));
        return NULL;
    }
    
    if (verbose) Rprintf("Test random was: %u\n", INTEGER_VALUE(result));
    return parsedCallback;
}

SEXP dieharderCallback(SEXP callback_sexp, SEXP bits_sexp, SEXP testnum_sexp, 
		       SEXP seed_sexp, SEXP psamples_sexp, SEXP tsamples_sexp,
		       SEXP ntuple_sexp, SEXP quiet_sexp, SEXP verbose_sexp) {

    initialize_globals(bits_sexp, seed_sexp, ntuple_sexp,
		       quiet_sexp, verbose_sexp);

    Dtest *test_type = dh_test_types[INTEGER_VALUE(testnum_sexp)];
    Test **test_results = create_test(test_type, INTEGER_VALUE(tsamples_sexp), 
				      INTEGER_VALUE(psamples_sexp));

    global_parsed_callback = parse_and_test_callback(callback_sexp);
    if (global_parsed_callback) {
	PROTECT(global_parsed_callback);
	std_test(test_type, test_results);
	UNPROTECT(1);
    } else {
	result = NULL;
    }
    global_parsed_callback = NULL;

    output(test_type, test_results);
    save_values_for_R(test_type, test_results);
    destroy_test(test_type, test_results);
    gsl_rng_free(rng);
    rng = NULL;
    reset_bit_buffers();

    return result;  	
}


