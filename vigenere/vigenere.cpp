#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <algorithm>

using std::cout;
using std::cerr;
using std::ifstream;
using std::ofstream;
using std::string;
using std::istream;
using std::stringstream;


// This is the Vigenere cipher. It encrypts based on a table
// and a given keyword. The keyword maps to the proper
// substitution based on each character in the keyword


// Reads in everything from the given file to a 
// buffer. Returns that buffer.
string getBuffer(istream &in);


// Turns the buffer into cipher text according to the keyword if encrypt
// Turns the buffer into regular text according to the keyword if decrypt
void getCipherText(string &buffer, const bool &encrypt, const string &keyword);



int main(int argc, char **argv){
	// Check usage requirements
	if (argc != 4){
		cerr << "USAGE: [executable] [-d for decrypt OR -e for encrypt]" <<
			" [FILE] [keyword]\n";
		return 1;
	}

	
	// Checks whether to encrypt or decrypt
	bool encrypt;
	string arg = argv[1];
	if (arg == "-e")
		encrypt = true;
	else if (arg == "-d")
		encrypt = false;
	else{
		cerr << "ERROR: Second argument must be '-d' or 'e'\n";
		return 1;
	}

	// Starts the file stream and checks if valid
	string file = argv[2];
	ifstream in(file);
	if (!in){
		cerr << "ERROR: Could not open file specified in third argument\n";
		return 1;
	}


	// Stores the keyword
	string keyword = argv[3];
	if (keyword.size() < 2){
		cerr << "ERROR: Keyword must be at least of length 2\n";
		in.close();
	}
	//cout << keyword << '\n';
	//cout << keyword.size() << '\n';
	
	
	string buffer = getBuffer(in);
	//cout << buffer << '\n';
	
	// Make the buffer all lowercase	
	std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
	//cout << buffer << '\n';
	
	
	// Buffer turned into cipher text
	getCipherText(buffer, encrypt, keyword);
	//cout << buffer << '\n';
	
	if (encrypt)
		file = "encrypt.txt";
	else
		file = "decrypt.txt";

	ofstream out(file);
	if (!out){
		cerr << "ERROR: Coult not write cipher to " << file << '\n';
		return 1;
	}
	out << buffer;
	
	
	in.close();
	out.close();
	return 0;

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


// Gets cipher text in buffer if encrypt
// Gets regular text in buffer if decrypt
void getCipherText(string &buffer, const bool &encrypt, const string &keyword){
	// Next position in the keyword and buffer
	int keyPos = 0;
	int bufPos = 0;

	// The main encryption part
	int limit = buffer.size();
	int keySize = keyword.size();
	while (bufPos < limit){
		// Getting the buffer character
		int bufChar = buffer[bufPos];
		// If bufChar not in range, skip
		if (bufChar < 97 || bufChar > 122){
			++bufPos;
			continue;
		}

		//cout << (char)bufChar;

		// If past last character of keyword
		// reset back to first
		if (keyPos >= keySize)
			keyPos = 0;

		// Getting the current character
		// in the keyword and getting the shift
		// from the ASCII table
		int keyChar = keyword[keyPos];
		int shift = keyChar - 97;

		
		// If encrypt shift left
		if (encrypt){
			bufChar -= shift;
			if (bufChar < 97){
				int diff = 97 - bufChar;
				bufChar = 123 - diff;
			}
		}

		// If decrypt shift right
		else{
			bufChar += shift;
			if (bufChar > 122){
				int diff = bufChar - 122;
				bufChar = 96 + diff;
			}
		}

		// Put new buffer character back into buffer
		buffer[bufPos] = (char)bufChar;

		// Increment positions
		++bufPos;
		++keyPos;
	}



}




