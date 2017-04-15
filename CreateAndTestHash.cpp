//
//  CreateAndTestHash.cpp
//  Assignment 3
//  
//  Created by Ioanis Stamos (c)
//  Modified by Nikki Jack on 10/27/15.
//
#include "QuadraticProbing.h"
#include "LinearProbing.h"
#include "DoubleProbing.h"
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;


// FILL TABLE
// Function template that parces input from database file into a hashtable.
// Can work on all three types of hashtable.
// Assumes that database file exists and is valid.
// @param string db_name is name of file to be processed
// @return a hashtable of necessary type constructed from file input.
template <typename HashTableType>
HashTableType FillTable( string db_name)
{
  HashTableType hash_table;    // a hashtable to be filled
  string line;                 // a variable to save one line
  ifstream fromFile(db_name) ; // start an input file stream

  // read file line by line, save each line in variable line
  while (getline(fromFile, line))
  { 
        hash_table.insert(line);    // insert into the hashtable
  } 
  fromFile.close(); // close input file stream
  return hash_table;    // return a filled hashtable
} // end FillTable



// PRINT STATISTICS
// Function template that displays hashtable statistics
// Can work on all three types of hashtable.
// Assumes that hash_table exists.
// @param hash_table to be processed.
// @post prints the statistics
template <typename HashTableType>
void PrintStats(const HashTableType &hash_table) 
{
  cout << "Number of elements in hashtable  (N): " << hash_table.getNumberofElements() <<endl;
  cout << "Size of hashtable                (T): " << hash_table.getTableSize() << endl;
  cout << "Load Factor:                   (N/T): " << hash_table.getLoadFactor() << endl;
  cout << "Total number of collisions       (C): " << hash_table.getTotalCollisions() << endl;
  cout << "Average number of collisions   (C/N): " << hash_table.getAverageCollisions() << endl;
} // end PrintStats



// DO QUERY
// Function template that lets query a hashtable
// Can work on all three types of hashtable.
// Assumes that hash_table exists and query_db_name file is valid.
// @param  hash_table is hashtable to be queried
// @post prints if quesries are found or not, and how many probes it took
template <typename HashTableType>
void DoQuery( const HashTableType &hash_table, string query_db_name) 
{
  string line;                      // a variable to save one line
  ifstream fromFile(query_db_name) ; // start an input file stream

  // read file line by line, save each line in variable line
  while (getline(fromFile, line))
  { 
    if (hash_table.contains(line))
    {
      cout << line << "\t\t✅ Found" ;
      cout << " with # probes: " << hash_table.probeCount(line) << endl;
    }
    else
    {
      cout << line << "\t\t❌ Not Found";
      cout << " with # probes: " << hash_table.probeCount(line) << endl;
    }
  } // end while
  fromFile.close(); // close input file stream
} // end DoQuery



// TEST FUNCTION FOR HASHTABLE
// Build a hashtable, shows stats and queries it.
// Can work on all three types of hashtable.
// @param hash_table a hashtable to be manipulated
//        words_filename a name of file for hashtable filling
//        query_filename a name of file that contains queries
//        flag is a name if hashtable type
template <typename HashTableType> 
void TestFunctionForHashTable(HashTableType &hash_table, const string &words_filename, 
                              const string &query_filename, const string &flag) 
{
  cout << "\n░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░" << endl;
  cout << "░                                                               ░" << endl ;
  cout << "░       CREATE  AND TEST HASHTABLE WITH " << flag << " PROBING   " << endl;
  cout << "░                                                               ░" << endl ;
  cout << "░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░" << endl;
  cout << "Words 📂 : " << words_filename << "   ";
  cout << "Query 📂 : " << query_filename << endl << endl;;

  hash_table = FillTable<HashTableType>(words_filename);
  cout << "Hashtable created, here are the stats " << endl; 
  cout << "¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯" << endl;
  PrintStats(hash_table);

  cout << "\n\nQuerying the hashtable to find words" << endl;
  cout << "¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯" << endl;
  DoQuery(hash_table,query_filename);
  cout << "\n░░░░░░░░░░░░░░░░░░░░ END CREATE AND TEST HASH ░░░░░░░░░░░░░░░░░░" << endl;
} // end TestFunctionForHashTable


// MAIN FUNCTION
// @param expects four strings as parameters:
//   first string program name
//   second string name of file contaning dictionary
//   third string name of file containing queries
//   fourth string a flag to determine hashtable type
int main(int argc, char **argv) 
{
  if (argc != 4) // if more or less than 4 arguments are passed,
  {             // then display error message and exit
    cout << "Usage: " << argv[0] << " <wordsfilename> <queryfilename> <flag>" << endl;
    return 0;
  }
  const string words_filename(argv[1]);
  const string query_filename(argv[2]);
  const string param_flag(argv[3]);

  if (param_flag == "linear") 
  {
    LinearHashTable<string> linear_probing_table; 
    TestFunctionForHashTable(linear_probing_table, words_filename, query_filename, param_flag); 
  } 
  else if (param_flag == "quadratic") 
  {
    QuadraticHashTable<string> quadratic_probing_table;
    TestFunctionForHashTable(quadratic_probing_table, words_filename, query_filename,param_flag);    
  } 
  else if (param_flag == "double") 
  {
    DoubleHashTable<string> double_probing_table;
    TestFunctionForHashTable(double_probing_table, words_filename, query_filename,param_flag); 
  } 
  else // there was a problem with flag
  {    // print error message
    cout << "Uknown tree type " << param_flag << " (User should provide linear, quadratic, or double)" << endl;
  }
  return 0;
} // end Main

// END OF PROGRAM
