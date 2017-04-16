#include <iostream>
#include <cstdlib>
#include <fstream>
#include <map>
#include <sstream>

using std::ifstream;
using std::ofstream;
using std::cout;
using std::cerr;
using std::string;
using std::map;
using std::istream;
using std::stringstream;

#define SHIFT 3


// This is a basic Caesar (shift) cipher
//
// Inspired by Julius Caesar, this encryption algorithm shifts
// the alphabet by 3 characters


// Encodes the proper mapping of characters based on
// whether encrypting or decrypting
void getMap(map<char, char> &cipherText, const bool &encrypt);

// Reads in everything from the given file to a 
// buffer. Returns that buffer.
string getBuffer(istream &in);

// Encrypts or decrypts the buffer and returns the corresponding new buffer
//
// NOTE: Leaves original untouched
string mapBuffer(map<char, char> &cipherText, const string &buffer);


int main(int argc, char **argv){

	// Check usage requirements
	if (argc != 3){
		cerr << "USAGE: [executable] [-d for decrypt -e for encrypt] [FILE]\n";
		return 1;
	}
	
	// Check that given file can be opened with an ifstream
	ifstream file(argv[2]);
	if(!file){
		cerr << "ERROR: Could not open file " << argv[2] << '\n';
		return 1;
	}

	// Check that the second argument is -d and -e
	bool encrypt;
	// Need string for comparisons
	string param = argv[1];
	// If -e, encrypt
	if (param == "-e")
		encrypt = true;
	
	// If -d, decrypt
	else if (param == "-d")
		encrypt = false;

	// If neither, throw error
	else{
		cerr << "ERROR: Second argument must be -e (encrypt) or -d (decrypt)\n";
		file.close();
		return 1;
	}

	// Getting our mapping
	map<char, char> cipherText;
	getMap(cipherText, encrypt);

	//for (auto it = cipherText.begin(); it != cipherText.end(); ++it)
		//cout << it->first << ':' << it->second << '\n';
	
	// Read the file into the buffer
	string buffer = getBuffer(file);	
	//cout << buffer << '\n';
	
	// Maps the buffer according to the cipher
	string newBuffer = mapBuffer(cipherText, buffer);
	//cout << newBuffer << '\n';
	
	string outputFile;
	if (encrypt)
		outputFile = "encrypted.txt";
	else
		outputFile = "decrypted.txt";

	ofstream out(outputFile);

	if (out)
		out << newBuffer;
	else{
		cerr << "ERROR: Could not open output file " << outputFile << '\n';
		return 1;
	}
		

	file.close();
	return 0;
	
}


// A mapping is created with the given map that maps
// characters to their corresponding text depending
// on whether we are encrypting or decrypting
//
// We use the ASCII chart to add the shift to the 
// character. If we run past the last upper or 
// lower case decimal number, we reset back to 
// a or A
void getMap(map<char, char> &cipherText, const bool &encrypt){

	// Get the upper and lower case alphabet
	string lower = "abcdefghijklmnopqrstuvwxyz";
	string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	// Map upper case
	int limit = upper.size();
	for (int i=0; i<limit; ++i){
		int real = upper[i];
		int cipher = real + SHIFT;

		if (cipher > 90){
			int difference = cipher - 90;
			cipher = 64 + difference;
		}
		
		// Whether or not we are encrypting
		// changes the mapping
		if (encrypt)
			cipherText[real] = cipher;
		else
			cipherText[cipher] = real;
	}
		
	// Map lower case
	limit = lower.size();
	for (int i=0; i<limit; ++i){
		int real = lower[i];
		int cipher = real + SHIFT;

		if (cipher > 122){
			int difference = cipher - 122;
			cipher = 96 + difference;
		}

		if (encrypt)
			cipherText[real] = cipher;
		else
			cipherText[cipher] = real;
	}
}


// Reads in the given file into a buffer
// 
// Uses a string stream because string streams
// are amazing!
string getBuffer(istream &in){
	stringstream buffer;
	buffer << in.rdbuf();
	return buffer.str();
}


// Maps the buffer to it's new buffer character by character
// 
// This implements both the encryption and decryption
string mapBuffer(map<char, char> &cipherText, const string &buffer){
	// Create the newBuffer
	string newBuffer;

	// Gets the appropriate limits and implements the caesar cipher
	auto endItr = cipherText.end();
	int limit = buffer.size();
	for (int i=0; i < limit; ++i){
		char key = buffer[i];

		auto itr = cipherText.find(key);
		if (itr  != endItr)
			newBuffer += itr->second;

		else
			newBuffer += key;
	}

	return newBuffer;
}

