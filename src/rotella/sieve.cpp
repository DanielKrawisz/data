// from http://numberccruncher.blogspot.com/2015/06/i-have-included-some-code-which-uses.html
// from https://www.cs.cmu.edu/afs/cs/user/mjs/ftp/thesis-program/2005/rotella.pdf 

/*********************************************************
 * sieve.cpp
 *
 *  Created on: 22 Jun 2015
 *      Author:
 *********************************************************/


#include <gmp.h>
#include <gmpxx.h>
#include "sieve.h"

sieve::sieve() {
    mpz_init(table);
    size = 2;
}

int sieve::isPrime(mpz_class r) {

    unsigned int rul = mpz_get_ui(r.get_mpz_t());
    if(size >= rul) { /* just a lookup */
        return !mpz_tstbit(table,rul);
    }
    else
    {
        unsigned int oldsize = size;
        size *= 2;
        unsigned int i;
        for(i=2; i<=size; i++) {
            if(!mpz_tstbit(table,i)) {
                unsigned int j;
                for(j=i*2; j<=size; j+=i) {
                    mpz_setbit(table,j);
                }
            }
        }
        return !mpz_tstbit(table,rul);
    }
}
sieve::~sieve() {
    mpz_clear(table);
}
