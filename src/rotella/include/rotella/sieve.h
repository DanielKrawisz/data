// from http://numberccruncher.blogspot.com/2015/06/i-have-included-some-code-which-uses.html
// from https://www.cs.cmu.edu/afs/cs/user/mjs/ftp/thesis-program/2005/rotella.pdf 

/************************************************
 * sieve.h
 *
 *  Created on: 22 Jun 2015
 *      Author:
 *************************************************/

#ifndef SIEVE_H_
#define SIEVE_H_

#include <gmp.h>
#include <gmpxx.h>

class sieve {

    private:
        mpz_t table;
        unsigned int size;

    public:
        sieve(); /* constructor */
        int isPrime (mpz_class r);
        ~sieve(); /* destructor */
};



#endif /* SIEVE_H_ */
