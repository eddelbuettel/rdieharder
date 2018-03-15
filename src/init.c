#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* FIXME: 
   Check these declarations against the C/Fortran source code.
*/

/* .Call calls */
extern SEXP dieharder(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP dieharderGenerators();
extern SEXP dieharderTests();

static const R_CallMethodDef CallEntries[] = {
    {"dieharder",           (DL_FUNC) &dieharder,           7},
    {"dieharderGenerators", (DL_FUNC) &dieharderGenerators, 0},
    {"dieharderTests",      (DL_FUNC) &dieharderTests,      0},
    {NULL, NULL, 0}
};

void R_init_RDieHarder(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
