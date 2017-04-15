//
//  LinearProbing.h
//  Assignment 3
//  
//  Created by Mark Allen Weiss (c)
//  Modified by Nikki Jack on 10/27/15.
//
#ifndef LINEAR_PROBING_H
#define LINEAR_PROBING_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include "QuadraticProbing.h"
using namespace std;


// LinearProbing Hash table class
//
// Derived from HashTable in "QuadraticProbing.h"
// Basic functionality is the same as in HashTable,, but it uses linear probing.
// Thus, the difference lies in the implementation of 
// internal functions to find position and count the number of probes. 
//
// Sample usage: LinearHashTable<string> passwords;
//               passwords.insert("pa$$w0rd");
// 

template <typename HashedObj>
class LinearHashTable: public QuadraticHashTable<HashedObj>
{
  public:

    explicit LinearHashTable() : QuadraticHashTable<HashedObj>() {  }

  private:

    // calculates probes by increasing the position by 1
    int findPos( const HashedObj & x ) const
    {
        int currentPos = this->myhash( x );

        while( this->array[ currentPos ].info != this->EMPTY &&
               this->array[ currentPos ].element != x )
        {
            currentPos ++;  // Compute ith probe
            if( currentPos >= this->array.size( ) )
                currentPos -= this->array.size( );
        }
        return currentPos;
    }

    // calculates probes by increasing the position by 1
    int getNumberOfProbes(const HashedObj & x) const
    {
        int numProbes = 0;
        int currentPos = this->myhash( x );

        while( this->array[ currentPos ].info != this->EMPTY &&
               this->array[ currentPos ].element != x  )
        {
            currentPos ++;  // Compute ith probe
            if( currentPos >= this->array.size( ) )
                currentPos -= this->array.size( );
            numProbes++;
        }
        return numProbes;
    }

};


#endif