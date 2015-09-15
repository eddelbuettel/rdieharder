\name{dieharderCallback}
\alias{dieharderCallback}
\title{Run dieharder test suite on random number generators written in R}
\description{
  \code{dieharderCallback} provides an interface to the dieharder suite
  of random number generators.  Whereas \code{dieharder} allows
  testing of the generators built in to \code{libdieharder}, this
  function evals arbitrary snippets of R code.   Note that because the R
  code is re-evaluated for each random number generated, the tests with
  \code{dieharderCallback} will usually take much longer (1000x) than the
  corresponding test of a compiled generator with \code{dieharder}.
}

\usage{
   dieharderCallback{default}(callback = "sample.int(2^16, 1) - 1",
                              bits = 16,
                              test = "rgb_bitdist",
                              seed = sample.int(2^16, 1),
                              psamples = 100,
                              tsamples = 100000,
                              ntuple = 5,
                              quiet = F,
                              verbose = F)
}

\arguments{

  \item{callback}{A snippet of R code that will be treated as a random
    number generator.  To test in advance, use
    \code{eval(parse(text=callback))}.  The range of random numbers
    generated should be include 0 and have a maximum of one less than a
    power of 2.  For example, the default callback has a range of
    [0, 2^16-1].}

  \item{bits}{The integer number of random bits used by each eval of
    \code{callback()}.  This can be set equal to or lower than the
    actual number of bits but not higher.  For example, if the
    \code{callback()} generated numbers from 0 to 255, \code{bits} can
    be at most 8.}

  \item{test}{A single dieharder test specified by number or partially
  matched string.  Use \code{dieharderTests()} to view available tests.}
  
  \item{psamples}{Integer number of probability values used for each
  test.  This is roughly the number of iterations the test performs.
  Higher numbers take longer but produce more accurate results.  See
  \code{dieharder} documentation for details.}

  \item{tsamples}{Integer number of samples used to generate each
  \code{psample}.  This is roughly the sample size with each test.
  Higher numbers take longer but produce more accurate results.  See
  \code{dieharder} documentation for details.}

  \item{ntuple}{A integer selecting the ntuple length for tests on short
    bit strings that permit varying length such as RGB bitdist.  Used by
    some but not all tests.}

  \item{seed}{An integer seed that can be specified for reproducibilty
    of results.  Passed to \code{set.seed(seed)} at the beginning of the
    test. If 0, a new random seed is generated.}

  \item{quiet}{A switch that causes the underlying \code{diehard} to
    directly print test output as it is created.  If set to false,
    \code{hist_flag} is also turned on and a histogram of generated
    pvalues is printed.}

  \item{verbose}{A switch to produce copious debugging information from
    within \code{dieharder}.  Useful only for developers.}
}

\value{
  An object of class \code{dieharder}, which inherits from the class
  \code{htest} commonly used for test statistics is returned. It
  contains the members.  See \code{dieharder} for details. 
}

\examples{
  # a good random generator has a uniform distribution across entire range
  maxRand = 255  # range will be 0:255, all numbers equally probable
  randomGenerator <- dieharderCallback(callback="as.integer(runif(1)*maxRand)", 
                                       bits=8, test="diehard_runs", seed=1)
  summary(randomGenerator)
  # a bad random generator selects some numbers more often than others
  maxRand = 246  # range 0:255, but numbers 247:255 are never chosen
  biasedGenerator <- dieharderCallback(callback="as.integer(runif(1)*maxRand)", 
                                       bits=8, test="diehard_runs", seed=1)
  summary(biasedGenerator)
}

\seealso{
  \code{\link{dieharder}}.
}

