// from http://numberccruncher.blogspot.com/2015/06/i-have-included-some-code-which-uses.html
// from https://www.cs.cmu.edu/afs/cs/user/mjs/ftp/thesis-program/2005/rotella.pdf 

/*********************************************************
 * aks.h
 *
 *  Created on: 22 Jun 2015
 *      Author: Rotella
 *********************************************************/

#ifndef AKS_H_
#define AKS_H_

#include <iostream>
#include <ostream>
#include <math.h>
#include <gmp.h>
#include <gmpxx.h>
using namespace std;

class mpz_pX {
    private:
        mpz_class **coef; /* Array of coefficients */
        unsigned int degree;
    public:
        mpz_pX(); /* constructors */
        mpz_pX(unsigned int initial_length);
        mpz_pX(const mpz_pX&);
 
    friend ostream& operator<<(ostream&, const mpz_pX&);
    mpz_pX& operator=(const mpz_pX&);
    void setCoef(mpz_class new_coef, unsigned int i);
    inline mpz_class getCoef(unsigned int i) const;
    inline unsigned int getDegree() const {return degree;};
    int isEqual(mpz_pX p);
    void clear();
    void compact();
 
    ~mpz_pX();  /* destructor */

};

void mpz_pX_mod_mult(mpz_pX& rop, const mpz_pX x,
const mpz_pX y, mpz_class mod,
unsigned int polymod);

void mpz_pX_mod_power(mpz_pX &rop, const mpz_pX& x,
mpz_class power, mpz_class mult_mod,
unsigned int polymod);

#endif /* AKS_H_ */
