//
//  SpellCheck.cpp
//  Assignment 3
//  
//  Created by Ioanis Stamos (c)
//  Modified by Nikki Jack on 10/27/15.
//
#include <iostream>
#include "DoubleProbing.h"
#include <string>
#include <vector>
#include <fstream>
#include <cctype>

using namespace std;



// BUILD DICTIONARY
// Function template that parces input from database file into a hashtable.
// Can work on all three types of hashtable.
// Assumes that database file exists and is valid.
// @param string db_name is name of file to be processed
// @return a hashtable of necessary type constructed from file input.
template <typename HashTableType>
HashTableType BuildDictionary(const string & db_name) 
{
  HashTableType hash_table;    // a hashtable to be filled
  string line;                 // a variable to save one line
  ifstream fromFile(db_name) ; // start an input file stream

  // read file line by line, save each line in variable line
  while (getline(fromFile, line))
  { 
        hash_table.insert(line);    // insert a line into hashmap
  } 
  fromFile.close(); // close input file stream
  return hash_table;    // return a filled hashtable
} // end BuildDictionary


// IS NUMBER
// Processes one word. Helper for GetWords(..)
// Function determines if the word is a number.
// @param string a_word is a word to be checked
// @return true if aword is a number, false otherwise
bool IsNumber(const string & a_word)
{
	return (isdigit(a_word[0]));
} // end isNumber


// CLEAN WORD
// Processes one word. Helper for GetWords(..)
// Function cleans the word from punctuation at its end or beginning
// @param string a_word is a word to be cleaned. 
// @return a word that starts and ends with a letter
string  CleanWord(const string & a_word)
{   // if there's punctuation or carriage return at the end of word
	if (ispunct(a_word.back()) || a_word.back() == '\r')
	{   // see if word without last letter works
		return CleanWord(a_word.substr(0,a_word.length()-1));
	}
	// if there's punctuation at the beginning of word
	if (ispunct(a_word.front()))
	{   // see if word without first letter works
		return CleanWord(a_word.substr(1,a_word.length()));
	}
	return a_word; // this word works
} // end CleanWord


// LOWER CASE
// Processes one word. Helper for GetWords(..)
// Function puts all characters of the word to lower case. 
// @param string a_word to transform
// @return a lowercase word
string  LowerCase(const string & a_word)
{  
	string wrd = a_word;
	for (unsigned int i = 0; i < wrd.length();i++ )
	{
		wrd[i] = tolower(wrd[i]);
	}
	return wrd;
} // end LowerCase



// GET WORDS
// Function that reads text from document and saves it as a
// collection of separate words. 
// Assumes that document file exists and is valid.
// @param string file_name is name of file to be processed
// @return a vector of all words from file
vector<string> GetWords( string file_name)
{
  vector<string> words;        // a big vector to save all separated words
  string word;                 // a variable to save one line
  ifstream fromFile(file_name) ; // start an input file stream

  while (!fromFile.eof())    // while its not end of file
  {                          // read work by word
	while (fromFile >> word) // save each word in a variable
	{ // clean word from puctuation and lowercase it
  		word = LowerCase(CleanWord(word));
        if(!IsNumber(word) && word!= "") //if it's not numeric or epty
  			words.push_back(word);       // insert the word into words vector
  	}
  } // end outer while

  fromFile.close(); // close input file stream
  return words;    // return a vector of all words from file_name
} // end GetWords


// TRY ADD LETTER
// One of the ways to modify the string. Helper for GetSuggestions(..)
// This function processes one misspelled word to find those
// modifications of this word that would be correct by adding 
// all possible characters at all possible positions in the word.
// Returns valid modifications, i.e those that are found in the dictionary.
// Assumes that dictionary hashtable is NOT empty.
// @param wrongWord a misspelled word 
//        dict is a hashtable containing the dictionary.
// @return a vector of wrongWord correction options
vector<string> TryAddLetter(const string & wrongWord,const DoubleHashTable<string> & dict ) 
{
	vector<string> suggestions; // a vector of suggested words

	// try inserting at every position "i" including at the end.
	for(unsigned int i = 0; i < wrongWord.length()+1; i++ )
	{   
		// turn the characters into their ASCII values
		// try inserting each letter from a to z
		for( int j = 'a'; j <= 'z' ;j++ ) 
		{
			string newWord = "";      // a modified word to be filled
			char aChar = j;           // turn the ASCII value back to character
			newWord += wrongWord.substr(0,i);    // first half
			newWord += aChar;        // insert current character in position i
		   	newWord += wrongWord.substr(i, wrongWord.back()); // seconf half
			if( dict.contains(newWord) ) // if the modified word is in dictionary
			{							 // add it to suggestions vector
				suggestions.push_back(newWord);
			}
		} // end inner for loop
	} // end outer for loop
	return suggestions;
} // end TryAddLetter


// TRY REMOVE LETTER
// One of the ways to modify the string. Helper for GetSuggestions(..)
// This function processes one misspelled word to find those
// modifications of this word that would be correct by removing each
// character one at a time. 
// Returns valid modifications, i.e those that are found in the dictionary.
// Assumes that dictionary hashtable is NOT empty.
// @param wrongWord a misspelled word 
//        dict is a hashtable containing the dictionary.
// @return a vector of wrongWord correction options
vector<string> TryRemoveLetter(const string & wrongWord,const DoubleHashTable<string> & dict )
{
	vector<string> suggestions; // a vector of suggested words
	string newWord;             // a variable for modified word

	// compose a word without character number i
	for(unsigned int i = 0; i < wrongWord.length(); i++ )
	{  
		newWord = wrongWord.substr(0,i) + // add first part before char number i
				  wrongWord.substr(i+1, wrongWord.back()); // add second part
				                                           // after char number i
		if( dict.contains(newWord) ) // if the modified word is in dictionary
		{                            // add it to suggestions vector
			suggestions.push_back(newWord);
		}
	}
	return suggestions;
} // end TryRemoveLetter



// TRY SWAP LETTERS
// One of the ways to modify the string. Helper for GetSuggestions(..)
// Function that processes one misspelled word to find those
// modifications of this word that would be correct. It keeps swapping two 
// adjacent letters at all positions of the word.
// Returns valid modifications, i.e those that are found in the dictionary.
// Assumes that dictionary hashtable is NOT empty.
// @param wrongWord a misspelled word 
//        dict is a hashtable containing the dictionary.
// @return a vector of wrongWord correction options
vector<string> TrySwapLetters(const string & wrongWord,const DoubleHashTable<string> & dict )
{
	vector<string> suggestions; // a vector of suggested words
	string newWord;             // a variable for modified word

	// at each position starting  1, swap char with the previous char
	for(unsigned int i = 1; i < wrongWord.length(); i++ )
	{
		newWord = wrongWord;    // copy a word for modification
		std::swap(newWord[i-1], newWord[i]); // swap letters next to each other
		if( dict.contains(newWord) ) // if the modified word is in dictionary
		{                            // add it to suggestions vector
			suggestions.push_back(newWord);
		}
	} 
	return suggestions;
} // end TrySwapLetters


// TRY SPLIT
// One of the ways to modify the string. Helper for GetSuggestions(..)
// Function that processes one misspelled string to find those
// modifications that would be correct. 
// It's aimed to split those strings that contain two words connected 
// by punctuation, e.g. this--was, more]than.
// Returns two valid words separated by space, both or one word is found in dictionary.
// Assumes that dictionary hashtable is NOT empty.
// @param wrongWord a misspelled word 
//        dict is a hashtable containing the dictionary.
// @return a vector of wrongWord correction, which will have one item
vector<string> TrySplit(const string & wrongWord,const DoubleHashTable<string> & dict)
{
	vector<string> suggestions; // a vector of suggested words
	string newWord;             // a variable for modified word
	vector<string> splits;      // a vector to hole separated words

	unsigned int counter = 0;  // counter to control outer while
	unsigned int counter2 ;    // counter to control inner while
	while (counter < wrongWord.length())
	{
		counter2 = counter;
		string word;   // a variable to save a separated word

		// keep constructing the word until run into punctuation
		// or end of original word. when it stops the word is separated
		while(!ispunct(wrongWord[counter2]) && (counter2 < wrongWord.length()) )
		{   
			word += wrongWord[counter2]; // add a chacater to word
			counter2 ++;	
		} 
		if( dict.contains(word)) // if the word is valid, i.e. found in dictionary 
		{                        // add it to vector of separated words
			splits.push_back(word);
		}
		counter = ++counter2;
	} // end outer while

	for (auto wrd : splits)  // construct a modified word by
	{                        // concatenating valid words and a space
		newWord += wrd;
		newWord += " ";
	} // end for loop

	if (newWord != "")  // if the modified word is not empty
		suggestions.push_back(newWord); // add it to suggestions

	return suggestions;
} // end TrySplit



// GET SUGGESTIONS
// Helper for CheckSpelling(..)
// Function that processes one misspelled word to find all
// modifications of this word that would be correct. 
// Returns  all valid modifications, i.e those 
// that are found in the dictionary.
// Assumes that dictionary hashtable is NOT empty.
// @param wrongWord a misspelled word 
//        dict is a hashtable containing the dictionary.
// @return string containing all wrongWord correction options
string GetSuggestions(const string & wrongWord, const DoubleHashTable<string> & dict )
{
	string result; // variable to save all suggestions of correct word

	vector<string> rm = TryRemoveLetter(wrongWord, dict);
  	vector<string> sw = TrySwapLetters(wrongWord, dict);
  	vector<string> ad = TryAddLetter(wrongWord, dict);
  	vector<string> sp = TrySplit(wrongWord, dict);

  	if (rm.empty() && sw.empty() && ad.empty() && sp.empty())
  	{  // a shrugging guy to show that no suggestions were found
  		return " ¯\\_(ツ)_/¯"; 
  	}
  	else
  	{   // add to result suggestions from addition, if any
  		for (auto w : ad)
  		{
  			result += w;
  			result += " ";
  		}
  		// add to result suggestions from removal, if any
  		for (auto wd : rm)
  		{
  			result += wd;
  			result += " ";
  		}
  		// add to result suggestions from letter swap, if any
  		for (auto wrd : sw)
  		{
  			result += wrd;
  			result += " ";
  		}
  		// add to result suggestions from word splitting, if any
  		for (auto wrd : sp)
  		{ 
  			result += wrd;
  			result += " ";
  		}
  	}
  	return result;
} // end GetSuggestions



// CHECK SPELLING
// Function that processes the vector of words to find misspellings
// by comparing it to a dictionary. Prints misspelled words
// along with options to correct them.
// Assumes that word vector and dictionary hashtable are NOT empty.
// @param words is a vector to be checked 
//        dict is a hashtable containign the dictionary.
// @post the suggestions to correct misspelled words are printed out.
void CheckSpelling(const vector<string> & words, const DoubleHashTable<string> & dict )
{
	for (auto wrd: words) // go through every word in text
  	{
  		if( !dict.contains(wrd))   // if the word is Not in dictionary
  		{
  			cout << endl << "Misspelled: " << wrd <<endl;   // print the word
  			cout << "Correction: ";
  			cout << GetSuggestions(wrd,dict); // print suggections of word correction
  			cout << endl;
  		}
  	}
} // end CheckSpelling


// TEST FUNCTION
// Function reads a dictionary and a document and
// checks the document's spelling with the help of dictionary
// @param document name of document file
//        dictionary name of file containing dictionary
void TestFunction(const string & document, const string & dictionary)
{
  cout << "\n░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░" << endl;
  cout << "░                                                               ░" << endl ;
  cout << "░                    RUNNING       SPELLCHECK                   ░" << endl;
  cout << "░                                                               ░" << endl ;
  cout << "░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░" << endl;
  cout << "Document 📂 : " << document << ",  Dictionary 📂 : " << dictionary << endl;

  cout << "\n\nHere are misspelled words and their possible corrections" << endl;
  cout << "¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯" << endl;

  DoubleHashTable<string> theDictionary = BuildDictionary<DoubleHashTable<string>>(dictionary);
  vector<string> textToCheck = GetWords(document);
  CheckSpelling(textToCheck, theDictionary);

  cout << "\n░░░░░░░░░░░░░░░░░░░░░░  END OF SPELLCHECK  ░░░░░░░░░░░░░░░░░░░░░░" << endl;
} // end TestFunction



// main function
// @param expects four strings as parameters:
//   first string program name
//   second string name of document file
//   third string name of file containing dictionary
int main(int argc, char **argv) 
{
  if (argc != 3) // if more or less than 3 arguments are passed,
  {				 // then display error message and exit
    cout << "Usage: " << argv[0] << " <documentfile> <dictionaryfile>" << endl;
    return 0;
  }
  string document_filename(argv[1]);   // save document name
  string dictionary_filename(argv[2]); // save dictionary file name
  TestFunction(document_filename,dictionary_filename);
  return 0;
} // end Main

// END OF PROGRAM
