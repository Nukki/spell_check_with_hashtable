//
//  DoubleProbing.h
//  Assignment 3
//  
//  Created by Mark Allen Weiss (c)
//  Modified by Nikki Jack on 10/27/15.
//
#ifndef DOUBLE_PROBING_H
#define DOUBLE_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>
#include <fstream>
#include "QuadraticProbing.h"
using namespace std;


// DoubleProbing Hash table class
//
// Derived from HashTable in "QuadraticProbing.h"
// Basic functionality is the same as in HashTable, but it uses double hash probing.
// Thus, the difference lies in the implementation of 
// internal functions to find position and count the number of probes. 
// Also a new function hash2() is added to properly implement double hash probing.
//
// Sample usage: DoubleHashTable<string> passwords;
//               passwords.insert("pa$$w0rd");
// 
template <typename HashedObj> 
class DoubleHashTable : public QuadraticHashTable<HashedObj>
{
 public:

    explicit DoubleHashTable() : QuadraticHashTable<HashedObj>() {  }
      
 private:

    // calculates probes by making the offset 
    // equal to the result of additional hashing
    int findPos( const HashedObj & x ) const
    {
        int offset = hash2(x);
        int currentPos = this->myhash( x );

        while( this->array[ currentPos ].info != this->EMPTY &&
               this->array[ currentPos ].element != x)
        { 
            currentPos += offset;  // Compute ith probe
            if( currentPos >= this->array.size( ) )
                currentPos -= this->array.size( );
        }
        return currentPos;
    }

    
    // calculates probes by making the offset 
    // equal to the result of additional hashing
    int getNumberOfProbes(const HashedObj & x) const
    {
        int numProbes = 0;
        int offset = hash2(x);
        int currentPos = this->myhash( x );

        while( this->array[ currentPos ].info != this->EMPTY &&
               this->array[ currentPos ].element != x )
        { 
            currentPos += offset;  // Compute ith probe
            if( currentPos >= this->array.size( ) )
                currentPos -= this->array.size( );
            numProbes++;
        }
        return numProbes;
    }

     size_t hash2(const HashedObj & x) const
    {
        static hash<HashedObj> hf; 
        return (73 - ( hf(x) % 73) ); 
    }
};



#endif
