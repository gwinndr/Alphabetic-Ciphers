#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

using std::cout;
using std::cerr;
using std::ofstream;
using std::ifstream;
using std::string;
using std::istream;
using std::stringstream;


// This is a custom encryption scheme
//
// For encryption the system time is taken and saved to a file. That 
// system time is then used to seed the random algorithm. Then for each 
// position in the buffer, a random number between 0 and 25 is taken
// and used to shift the character
//
// For decryption, the time is read into a file and used to seed the 
// random algorithm. That is then used to figure out the shift of each 
// character in sequence, thus recreating the randomness


// Reads in the given file into a buffer
// 
// Uses a string stream because string streams
// are amazing!
string getBuffer(istream &in);

// Shifts according to whether encrypting or decrypting.
void shiftChar(int &bufChar, const bool &upper, const int &shift, const bool &encrypt);

// Gets the seed depending on whether encrypting or decrypting
// NOTE: Error code is -1
time_t getSeed(const bool &encrypt);


int main(int argc, char **argv){

	// Tests usage requirements
	if (argc != 3){
		cerr << "USAGE: [executable] [-e for encrypt OR -d for decrypt] [FILE]\n";
		return 1;
	}

	// Puts whether to encrypt or decrypt
	string arg = argv[1];
	bool encrypt;
	if (arg == "-e")
		encrypt = true;
	else if (arg == "-d")
		encrypt = false;
	else{
		cerr << "ERROR: Second argument must be -d or -e\n";
		return 1;
	}

	// Opens the given file
	string file = argv[2];
	ifstream in(file);
	if (!in){
		cerr << "ERROR: Could not open the file " << file << '\n';
		return 1;
	}


	// Gets the time variable for seeding
	// NOTE: Depends on whether encrypting or decrypting
	// NOTE: If -1, then error occurred
	time_t seed = getSeed(encrypt);
	if (seed == -1){
		cerr << "ERROR: In getSeed() function\n";
		in.close();
		return 1;
	}
	//cout << seed << '\n';
	

	// Get the buffer
	string buffer = getBuffer(in);
	//cout << buffer << '\n';

	// Now seed srand
	std::srand(seed);
	
	// Now to shift
	// Shift is done by retrieving 'random' values
	// from rand() and using them as shift variables
	// NOTE: Values range from 0-25
	// NOTE: If character in buffer invalid, skip
	int limit = buffer.size();
	for (int i=0; i<limit; ++i){
		int bufChar = buffer[i];

		// If character not valid, return
		// else determine whether character is upper or lower
		bool upper;
		if (bufChar >= 97 && bufChar <= 122)
			upper = false;
		else if (bufChar >= 65 && bufChar <= 90)
			upper = true;
		else
			continue;
		
		// Get the random value
		int shift = rand() % 26;
		//cout << shift << '\n';
		
		// Shift over depending on encryption or decryption
		// NOTE: bufChar is passed by reference and changed
		shiftChar(bufChar, upper, shift, encrypt);
		buffer[i] = (char)bufChar;
	}

	//cout << buffer << '\n';
	
	// Write result to file
	if (encrypt)
		file = "encrypt.txt";
	else
		file = "decrypt.txt";

	ofstream out(file);

	if (!out){
		cerr << "ERROR: Could not open output file!\n";
		in.close();
		return 1;
	}

	out << buffer;

	out.close();
	in.close();
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



// Gets the seed based on the system clock (if encrypt) or from file (if decrypt)
// NOTE: Error code is -1
time_t getSeed(const bool &encrypt){
	// If encrypt, use system clock
	// NOTE: Saves key to file key.txt
	// NOTE: If cannot open file, returns error code -1
	if (encrypt){
		time_t t = time(NULL);

		ofstream out("key.txt");
		if (!out)
			return -1;
		else{
			out << t;
			out.close();

			return t;
		}

	}

	// Else get from file key.txt
	// NOTE: If cannot open file, return error code -1
	else{
		ifstream in("key.txt");
		if (!in)
			return -1;
		else{
			time_t t;

			in >> t;
			in.close();

			return t;
		}
	}
}



// Shifts according to whether encrypting or decrypting.
void shiftChar(int &bufChar, const bool &upper, const int &shift, const bool &encrypt){
	// If encrypt shift left
	if (encrypt){
		bufChar -= shift;
		
		if (!upper){
			if (bufChar < 97){
				int diff = 97 - bufChar;
				bufChar = 123 - diff;
			}
		}

		else {
			if (bufChar < 65){
				int diff = 65 - bufChar;
				bufChar = 91 - diff;
			}
		}
	}

	// If decrypt shift right
	else{
		bufChar += shift;

		if (!upper){
			if (bufChar > 122){
				int diff = bufChar - 122;
				bufChar = 96 + diff;
			}
		}

		else{
			if (bufChar > 90){
				int diff = bufChar - 90;
				bufChar = 64 + diff;
			}
		}
	}
}




