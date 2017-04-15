//
//  QuadraticProbing.h
//  Assignment 3
//  
//  Created by Mark Allen Weiss (c)
//  Modified by Nikki Jack on 10/27/15.
//
#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>
#include "QuadraticProbing.h"
using namespace std;

int nextPrime( int n );

//
// QuadraticProbing Hash table class
// Hashes the value of the object to get its position in the table.
// Uses quadratic probing to find next available position in case of collision.
// Displays hashtable size, number of elements in the table and how many
// collision happenned during insertions.
//
// Sample usage: QuadraticHashTable<string> passwords;
//               passwords.insert("pa$$w0rd");
//               passwords.getAverageCollisions();
//
template <typename HashedObj>
class QuadraticHashTable
{
  public:

    // Constructor
    explicit QuadraticHashTable( int size = 101 ) : array( nextPrime( size ) )
      { makeEmpty( ); }

    // CONTAINS
    // Checks is the hashtable contains an object
    // @param x is an object to checked
    // @return true if successful, false otherwise
    bool contains( const HashedObj & x ) const
    {
        return isActive( findPos( x ) );
    }

    // PROBE COUNT
    // Counts the number of probes it takes 
    // to insert or to find an object
    // @param x is an object to look for
    // @return number of probes
    int probeCount(const HashedObj & x) const
    {
        return getNumberOfProbes(x);
    }

    // GET NUMBER OF ELEMENTS
    // @return current number of elements in the hashtable
    int getNumberofElements() const
    {
        return currentSize;
    }

    // GET TABLE SIZE
    // @return size of hashtable
    int getTableSize() const
    {
        return array.size();
    }

    // GET LOAD FACTOR
    // @return load factor ratio
    double getLoadFactor() const
    {
        double result = currentSize/double(array.size()) ;
        return result;
    }

    // GET TOTAL COLLISIONS
    // @return total numer of collisions while building a hashtable
    int getTotalCollisions() const
    {
        return totalCollisions;
    }

    // GET AVERAGE COLLSIONS
    // @return average number of collisions per object
    double getAverageCollisions() const
    {
        return totalCollisions / double(currentSize);
    }

    // MAKE EMPTY
    // @post a hashtable is initialized or cleared
    void makeEmpty( )
    {
        currentSize = 0;
        totalCollisions = 0;
        for( auto & entry : array )
            entry.info = EMPTY;
    }

    // INSERT
    // inserts the object into the hashtable by lvalue
    // @param x is an object to be inserted
    // @return true if successful, false otherwise
    bool insert( const HashedObj & x )
    {
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;

        totalCollisions += getNumberOfProbes(x);
        array[ currentPos ].element = x;
        array[ currentPos ].info = ACTIVE;
        if( ++currentSize > array.size( ) / 2 )
            rehash( );
        return true;
    }
    
    // INSERT
    // inserts the object into the hashtable by rvalue
    // @param x is an object to be inserted
    // @return true if successful, false otherwise
    bool insert( HashedObj && x )
    {
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;
        
        totalCollisions += getNumberOfProbes(x);
        array[ currentPos ] = std::move( x );
        array[ currentPos ].info = ACTIVE;
        if( ++currentSize > array.size( ) / 2 )
            rehash( );
        return true;
    }

    // REMOVE
    // removes an object from hashtable
    // @param x is an object to be removed
    // @return true if successful, false otherwise
    bool remove( const HashedObj & x )
    {
        int currentPos = findPos( x );
        if( !isActive( currentPos ) )
            return false;

        array[ currentPos ].info = DELETED;
        return true;
    }

    // flags to show the status of a certain cell in a table
    enum EntryType { ACTIVE, EMPTY, DELETED };

  protected:

    // one cell of hashtable
    // a struct to hold and object and its status
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry( const HashedObj & e = HashedObj{ }, EntryType i = EMPTY )
          : element{ e }, info{ i } { }
        
        HashEntry( HashedObj && e, EntryType i = EMPTY )
          : element{ std::move( e ) }, info{ i } { }
    };
    
    vector<HashEntry> array;  // container for hashtable
    int currentSize;          // number of elements in hashtable
    int totalCollisions;      // collsions counter

    // IS ACTIVE
    // Checks a cell in hashtable at certain position is active
    // @param currentPos is position in the table to check
    // @return true if is active, false otherwise
    bool isActive( int currentPos ) const
      { return array[ currentPos ].info == ACTIVE; }

    // FIND POS
    // Collsions handler
    // Finds a position where to insert object x
    // or gets its location if it's been inserted before
    // @param object x is looking for its position
    // @return the position for object x in a hashtable
    virtual int findPos( const HashedObj & x ) const
    {
        int offset = 1;
        int currentPos = myhash( x );

        while( array[ currentPos ].info != EMPTY &&
               array[ currentPos ].element != x )
        { 
            currentPos += offset;  // Compute ith probe
            offset += 2;
            if( currentPos >= array.size( ) )
                currentPos -= array.size( );
        }
        return currentPos;
    }

    // GET NUMBER OF PROBES
    // Counts how many probes it takes to find a position for an object
    // @param x is an object looking for its position
    // @return number of probes 
    virtual int getNumberOfProbes(const HashedObj & x) const
    {
        int numProbes = 0;
        int offset = 1;
        int currentPos = myhash( x );

        while( array[ currentPos ].info != EMPTY &&
               array[ currentPos ].element != x )
        { 
            currentPos += offset;  // Compute ith probe
            offset += 2;
            if( currentPos >= array.size( ) )
                currentPos -= array.size( );
            numProbes++;
        }
        return numProbes;
    }

    // REHASH
    // @post Table size is increased
    void rehash( )
    {
        vector<HashEntry> oldArray = array;

            // Create new double-sized, empty table
        array.resize( nextPrime( 2 * oldArray.size( ) ) );
        for( auto & entry : array )
            entry.info = EMPTY;

            // Copy table over
        currentSize = 0;
        for( auto & entry : oldArray )
            if( entry.info == ACTIVE )
                insert( std::move( entry.element ) );
    }

    // MY HASH
    // The first attempt to find position in hashtable for an object.
    // @param x is an object looking for position
    // @return the suggested position in hashtable for x
    size_t myhash( const HashedObj & x ) const
    {
        static hash<HashedObj> hf;
        return hf( x ) % array.size( );
    }
};

#endif
