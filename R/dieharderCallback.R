dieharderCallback <- function(callback = "sample.int(2^16, 1) - 1",
                              bits = 16,
                              test = "rgb_bitdist",
                              seed = sample.int(2^16, 1),
                              psamples = 100,
                              tsamples = 100000,
                              ntuple = 5,
                              quiet = F,
                              verbose = F) {

    tests = .dieharder.tests
    if (is.character(test)) {    # convert test from name to number
        found <- charmatch(test, as.character(tests$names))
        if (is.na(found)) {
            warning("No test named '", test, "' was found")
            return(NULL)
        }
        if (found == 0) {
            warning("Test name '", test, "' is ambiguous")
            return(NULL)
        }
        testnum <- tests$id[found]
    } else {
        testnum = test
    }

    val <- .Call("dieharderCallback",
                 as.character(callback),
                 as.integer(bits),
                 as.integer(testnum),
                 as.integer(seed),
                 as.integer(psamples),
                 as.integer(tsamples),
                 as.integer(ntuple),
                 as.integer(quiet),
                 as.integer(verbose),
                 PACKAGE="RDieHarder")

    obj <- list(p.value=val[[1]][1],
                data=val[[2]],      ## not used by htest methods
                method=val[[3]],
                data.name=paste0("dieharderCallback(callback='", callback,
                    "', test='", as.character(tests[tests$id == testnum,"names"]), "'",
                    ", bits=", bits, ", seed=", as.integer(seed),
                    ", psamples=", as.integer(psamples), ", tsamples=", as.integer(tsamples),
                    ", ntuple=", as.integer(ntuple), ")"),
                nvalues=val[[4]],
                p.values=val[[1]]
                )
    class(obj) <- c("dieharder", "htest")
    return(obj)
}
