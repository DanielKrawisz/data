// from http://numberccruncher.blogspot.com/2015/06/i-have-included-some-code-which-uses.html
// from https://www.cs.cmu.edu/afs/cs/user/mjs/ftp/thesis-program/2005/rotella.pdf 

/***************************************************
* aks.cpp:


 *
 *  Created on: 23 Jun 2015
 *      Author: Rotella
 ***********************************************/

#include <iostream>
#include <stdlib.h>
#include "aks.h"
using namespace std;



mpz_pX::mpz_pX() {
    degree = 0;
    coef = (mpz_class**)calloc(1,sizeof(mpz_class*));
    coef[0] = new mpz_class(0);
}

mpz_pX::mpz_pX(unsigned int initial_length) {
    degree = initial_length;
    coef = (mpz_class **)calloc(1,sizeof(mpz_class*)*(initial_length+1));
    unsigned int i;
    for(i=0; i<=degree; i++) {
        coef[i] = new mpz_class(0);
    }
}

mpz_pX::mpz_pX(const mpz_pX& o) {
    degree = o.getDegree();
    coef = (mpz_class**)calloc(1,sizeof(mpz_class*)*(degree+1));
    unsigned int i;
    for(i=0; i<=degree; i++) {
        coef[i] = new mpz_class(o.getCoef(i));
    }
}

inline mpz_class mpz_pX::getCoef(unsigned int i) const {
    static mpz_class zero(0);
    if(i > degree)
        return zero;
    return *(coef[i]);
}


int mpz_pX::isEqual(mpz_pX o) {
    if(o.getDegree() != degree)
        return 0;
    unsigned int i;
    for(i=0; i<=degree; i++)
        if(o.getCoef(i) != *coef[i])
            return 0;
  return 1;
}


void mpz_pX::setCoef(mpz_class new_coef, unsigned int i) {

if(i < 0)
    fprintf(stderr, "coef is less than 0\n");
 if(i > degree) {
        unsigned int j;
        coef = (mpz_class **)realloc(coef, sizeof(mpz_class*)*(i+1));
        for(j=degree+1; j<i; j++)
            coef[j] = new mpz_class(0);
        coef[i] = new mpz_class(new_coef);
        degree = i;
    }
    else {
        delete coef[i];
        coef[i] = new mpz_class(new_coef);
    }
}


void mpz_pX::compact() {
    unsigned int i;
    static mpz_class zero = 0;
    for(i=degree; i>0; i--) {
        if (*(coef[i]) != zero)
            break;
        delete coef[i];
    }
    if(degree != i) {
       coef = (mpz_class **)realloc(coef, (sizeof(mpz_class*)*(degree+1),sizeof(mpz_class*)*(i+1)) );
       degree = i;
    }

}


void mpz_pX::clear() {
    unsigned int i;
    for(i=0; i<=degree; i++) {
        delete coef[i];
    }
    coef = (mpz_class **)realloc(coef, sizeof(mpz_class*));
    degree = 0;
    coef[0] = new mpz_class(0);
}

mpz_pX::~mpz_pX() {
    unsigned int i;
    for(i=0; i<=degree; i++) {
        delete coef[i];
    }
    free(coef);
}



void mpz_pX_mod_mult(mpz_pX& rop, const mpz_pX _x,
const mpz_pX _y, mpz_class mod,
unsigned int polymod) {
    mpz_pX x = _x;
    mpz_pX y = _y;
    rop.clear();
    unsigned int xdeg = x.getDegree();
    unsigned int ydeg = y.getDegree();
    unsigned int maxdeg = xdeg < ydeg ? ydeg : xdeg;
    unsigned int k;
    for(k=0; k<polymod; k++) {
        mpz_class sum = 0;
        unsigned int i;
        for(i=0; i<=k; i++) {
            sum += x.getCoef(i)*(y.getCoef(k-i)+
                    y.getCoef(k+polymod-i));
        }
        for(i=k+1; i<=k+polymod; i++) {
            sum += x.getCoef(i)*y.getCoef(k+polymod-i);
        }
        rop.setCoef(sum % mod, k);
        if(k>maxdeg && sum==0)
            break;
    }
    rop.compact();
}


void mpz_pX_mod_power(mpz_pX &rop, const mpz_pX& x,
mpz_class power, mpz_class mult_mod,
unsigned int poly_mod)
{
    rop.clear();
    rop.setCoef(1,0);
    unsigned int i = mpz_sizeinbase(power.get_mpz_t(),2);
    for( ; i >=0; i--) {
       mpz_pX_mod_mult(rop, rop, rop, mult_mod, poly_mod);
       if(mpz_tstbit(power.get_mpz_t(),i)) {
            mpz_pX_mod_mult(rop, rop, x, mult_mod, poly_mod);
      }
       if(i==0)
           break;
    }
    rop.compact();
}
