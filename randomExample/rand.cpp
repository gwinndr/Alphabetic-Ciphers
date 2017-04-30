#include <iostream>
#include <cstdlib>

using namespace std;

int main(){
	int seed;
	
	cout << "Seed: ";
	cin >> seed;

	srand(seed);

	for (int i=0; i<10; ++i)
		cout << rand() % 1000<< '\n';
}
