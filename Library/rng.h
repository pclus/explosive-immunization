#ifndef GSL_H
#define GSL_H
        #include <gsl/gsl_rng.h>
#endif


#define RAND (gsl_rng_uniform(rrr)) 		// random double beween 0 and 1
#define RANDIFF (-1+2.*RAND) 			// random double between -1 and 1
#define IRAND(n) (gsl_rng_uniform_int(rrr,n)) 	// random integer between 0 and n-1 (inclusive)

const gsl_rng_type * T;
gsl_rng * rrr;


void initialize_rng(int seed);
