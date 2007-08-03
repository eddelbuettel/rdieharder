# $Id$
.onLoad <- function(lib, pkg) {
  library.dynam("RDieHarder", pkg, lib )
  .dieharder.generators <<- dieharderGenerators()
}

#.First.lib <- function(lib, pkg) {
#  library.dynam("RDieHarder", pkg, lib )
#  .dieharder.generators <<- dieharderGenerators()
#}

