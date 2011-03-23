
stopifnot(require(RDieHarder, quiet=TRUE))

if (Sys.getenv("RUN_EXPENSIVE_TEST") == "YES") {
    ## RGB tests 3, 5 and 6 at ntuple=5
    #for (num in c(3,5,6)) {
    #    print( dieharder(test=200+num, seed=1, ntuple=5)$p.value )
    #}

    ## Dieharder tests 1 to 17
    for (num in seq(1,17)) {
        print( dieharder(test=num, seed=1, ntuple=5)$p.value )
    }

    ## STS tests 1 to 2 (3 is a sequence)
    for (num in seq(0,1)) {
        print( dieharder(test=100+num, seed=1, ntuple=5)$p.value )
    }
}
