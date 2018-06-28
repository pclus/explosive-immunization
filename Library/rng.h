#ifndef STD_H
#define STD_H
        #include <stdio.h>
        #include <stdlib.h>
        #include <time.h>
        #include <sys/time.h>
        #include <string.h>
        #include <math.h>
        #include <errno.h>
        #include "macros.h"
#endif


// GSL is not used by default
//#ifndef GSL_H
//#define GSL_H
//        #include <gsl/gsl_rng.h>
//#endif

//#define RAND (gsl_rng_uniform(rrr)) 			// random double beween 0 and 1
//#define RANDIFF (-1+2.*RAND) 				// random double between -1 and 1
//#define IRAND(n) (gsl_rng_uniform_int(rrr,n)) 	// random integer between 0 and n-1 (inclusive)

#define RAND (((double)rand())/RAND_MAX) 	// random double beween 0 and 1
#define IRAND(n) ((int) n*RAND) 		// random double between -1 and 1
#define RANDIFF (-1+2.*RAND)			// random integer between 0 and n-1 (inclusive)

//const gsl_rng_type * T;
//gsl_rng * rrr;


void initialize_rng(int seed);
