/*******************************************************************************
 * main.cpp
 * language C++
 *  Created on: 22 Jun 2015
 *      This simple program is able to test very large numbers for primality.
 *      It uses the GMP library to handle the very large numbers and uses
 *      AKS routine to test for primality. It is still very slow.
 *      In a sense it is a curosity. The routine has since been
 *      optimised further by Pomerance [not included here]
 *
 *      The Akslib library uses the code published in the article
 *      "An Efficient Implementation of the AKS
 *      Polynomial-Time Primality Proving Algorithm"
 *       by Chris Rotella May 6, 2005
 *******************************************************************************/

#include <iostream>
#include <gmp.h>
#include <gmpxx.h>
#include "rotella/akslib.h"

using namespace std;

int main () {

    // Declare GMP class
    mpz_class x;

    cout << " Input number to test for primality " << endl;

    cin >> x;   // Store value in object x

    if (aks (x) == 1)
        cout << x << " is a prime" << endl;
    else
        cout << x << " is composite " << endl;

}

