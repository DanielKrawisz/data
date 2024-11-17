// from http://numberccruncher.blogspot.com/2015/06/i-have-included-some-code-which-uses.html
// from https://www.cs.cmu.edu/afs/cs/user/mjs/ftp/thesis-program/2005/rotella.pdf 

/******************************************************
 * akslib.cpp
 *
 *  Created on: 23 Jun 2015
 *      Author: Rotella
 ******************************************************/

#include "rotella/aks.h"
#include "rotella/sieve.h"
#include <iostream>

int aks (mpz_class n) {

    if (mpz_perfect_power_p (n.get_mpz_t ())) {
        return 0;
    }
    
    sieve s;
    mpz_class r = 2;
    mpz_class logn = mpz_sizeinbase (n.get_mpz_t (),2);
    mpz_class limit = logn * logn;
    limit *= 4;

    /* Witness search */

    while(r<n) {
        if(mpz_divisible_p(n.get_mpz_t(), r.get_mpz_t())) {
            return 0;
        }

        int failed = 0;
        if(s.isPrime(r)) {
            mpz_class i = 1;
            for( ; i<=limit; i++) {
                mpz_class res = 0;
                mpz_powm (res.get_mpz_t(), n.get_mpz_t(),
                    i.get_mpz_t (), r.get_mpz_t());
                if (res == 1){
                    failed = 1;
                    break;
                }
            }

        if(!failed) break;
    
        }

        r++;

    }

    if (r == n) {
        return 1;
    }


    /* Polynomial check */

    unsigned int a;
    mpz_class sqrtr;

    //actually the floor, add one later to get the ceil

    mpz_sqrt(sqrtr.get_mpz_t(), r.get_mpz_t());
    mpz_class polylimit = 2 * (sqrtr+1) * logn;
    unsigned int intr = mpz_get_ui(r.get_mpz_t());

    for (a=1; a <=polylimit; a++) {
        mpz_class final_size = n % r;
        mpz_pX compare(mpz_get_ui(final_size.get_mpz_t()));
        compare.setCoef(1, mpz_get_ui(final_size.get_mpz_t()));
        compare.setCoef(a, 0);
        mpz_pX res(intr);
        mpz_pX base(1);
        base.setCoef (a,0);
        base.setCoef(1,1);
        mpz_pX_mod_power(res, base, n, n, intr);
        if (!res.isEqual(compare)) {
            return 0;
        }
    }

    return 1;

}

