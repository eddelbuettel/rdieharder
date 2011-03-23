# $Id$
.onLoad <- function(lib, pkg) {
    library.dynam("RDieHarder", pkg, lib )
    .dieharder.generators <<- dieharderGenerators()
    .dieharder.tests <<- dieharderTests()
}

#.First.lib <- function(lib, pkg) {
#  library.dynam("RDieHarder", pkg, lib )
#  .dieharder.generators <<- dieharderGenerators()
#}

