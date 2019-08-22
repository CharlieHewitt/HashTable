/* Charlie Hewitt
AC21008: Hash Tables
Matriculation Number: 170015454 */

#include <iostream>
#include <sstream>
#include <ctime>
#include <random>
#include "HashTable.h"

using namespace std;



ulint order(ulint, ulint, default_random_engine);
ulint dLog(ulint, ulint, ulint, ulint, default_random_engine);
ulint power(ulint, ulint, ulint);
bool checkNumber(char*);


ulint order(ulint g, ulint n, default_random_engine e) 
{
	try // try for memory fail on Order
	{
		// initialise hash table
		HashTable Order = HashTable();
		uniform_int_distribution<int> d1(1,(n-2)); //get the distribution

		// do sqrt(n) iterations 
		int sqrtN = static_cast<int>(sqrt(n));

		for (int j = sqrtN; j > 0; j--)
		{
			//cout << "Order iteration: " << (sqrtN-j + 1) << endl;

			//get random number
			ulint r = d1(e);
			//cout << "generating random value: " << r <<endl;

			//compute y = g^r mod n
			ulint y = (static_cast<ulint>(power(g,r,n)));

			//cout << "computing: g^r mod n g: " << g << "^ r: " << r << " mod n: " << n << " = " << y << endl;

			// check if y in table
			if (Order.contains(y))
			{
				ulint yVal = Order.getValue(y);
				//cout << "yVal in table " << yVal << " r: " << r << endl;

				if (r > yVal ) // make sure it doesnt underflow
				{
					//cout << "returning r - yVal " << (r - yVal) << endl;
					return (r-yVal);
				}
				else if (yVal > r)
				{
					//cout << "returning yVal - r " << (yVal - r) << endl;
					return (yVal-r);
				}
				else
				{
					// continue
				}

			}
			else
			{ 
				//cout << "inserting y: " << y << " val r: " << r << endl;
				try
				{
					//cout << "insert" << endl;
					Order.insert(y,r);
				}
				catch(HashTableError)
				{
					
				}
				catch(...)
				{
					throw OUT_OF_MEMORY;
				}
			}

		}

		//cout << "returning n-1: " << n-1 << endl;
		return (n-1);
	}
	catch(HashTableError)
	{
		cerr << "Out of memory" << endl;
		throw OUT_OF_MEMORY;
	}
}

/* log a with base g mod n */
ulint dLog(ulint g, ulint a, ulint n, ulint ord, default_random_engine e)
{
	try
	{
		//initialise tables
		HashTable A = HashTable();
		HashTable B = HashTable();

		uniform_int_distribution<ulint> d1(0,(n-1));

		ulint rtN = sqrt(n);

		// sqrt N times
		for (ulint k = 0; k < rtN; k++)
		{
			//cout << "dlog iteration : " << k << endl;

			// pick a random number r (0 -> (n-1))
			ulint r = d1(e);

			// if y = a * g^r mod n is in B
			ulint y = (power(g,r,n) * a) % n;

			if (B.contains(y))
			{
				ulint valB = B.getValue(y);
				//cout << " B dlog = " << valB << " - " << r << " = " << (valB-r) << endl;

				if (r > valB)
				{
					return (valB + ord - r);
				}
				return (valB - r);
			}
			else
			{
				try
				{
					A.insert(y,r);
				}
				catch(HashTableError)
				{
			
				}
				catch(...)
				{
					throw OUT_OF_MEMORY;
				}
			}

			// generate a second random number
			r = d1(e);
			ulint gPowr = power(g,r,n);

			if (A.contains(gPowr))
			{
				ulint valA = A.getValue(gPowr);
				//cout << " A dlog = " << r << " - " << valA << " = " << (r-valA) << endl;

				if (valA > r)
				{
					return (r + ord - valA);
				}

				return (r - valA);
			}
			else
			{
				try
				{
					B.insert(gPowr,r);
				}
				catch(HashTableError)
				{
					
				}
				catch(...)
				{
					throw OUT_OF_MEMORY;
				}

			}
		}


		//cout << "woops" << endl;

		return 0;
	}
	catch(HashTableError)
	{
		cerr << "Out of memory" << endl;
		throw OUT_OF_MEMORY;
	}

}

/* a^b % n using square to multiply */
ulint power(ulint a, ulint b, ulint n)
{
	//convert h to binary
	int binaryPower[64];
	int index = -1;

	// convert b to binary
	for(int i = 0; i < 64; i++)
	{
		binaryPower[i] = 0;
	}
	while (b > 0)
	{
		index++;

		if (b % 2 == 1)
		{
			binaryPower[index] = 1;
		}
		else
		{
			binaryPower[index] = 0;
		}

		b /= 2;
	}

	// for (int i = 0; i < 64; i++)
	// {
	// 	cout << binaryPower[i];
	// }

	// cout << endl;

	ulint pow = a;
	ulint result = 1;

	int counter = 0;

	// apply square to multiply ...
	while (counter <= index)
	{

		if (binaryPower[counter] == 1)
		{
			result = (result * pow) % n;
		}

		pow = (pow * pow) % n;

		counter++;
	}

	return result;
	
}

ulint calc(ulint g, ulint a, ulint n)
{
	// define random engine
	static default_random_engine e(static_cast<unsigned int>(time(0)));
	try
	{
		// calc order, dLog and get the answer
		ulint ord = order(g,n,e);
		ulint dlog = dLog(g,a,n,ord,e);
		//cout << "order: " << ord << " dlog: " << dlog << endl;
		ulint answer = 0;

		if (ord != 0)
		{
			answer = dlog % ord;
		}

		return answer;
	}
	catch(HashTableError)
	{
		throw OUT_OF_MEMORY;
	}
}

int main(int argc, char** argv)
{
	if (argc != 4) // check number of args
	{
		cerr << "Invalid number of arguments" << endl;
		return -1;
	}

	ulint g, a, n;
	string strArray[3];
	stringstream ss;
	bool valid = true;

	// check if args are numbers
	for (int i = 1; i < 4; i++)
	{
		valid = checkNumber(argv[i]);

		if (!valid)
		{
			cerr << "invalid args" << endl;
			return -3;
		}
	}

	// convert args to ulints
	ss << argv[1];
	ss >> g;

	ss.clear();
	ss << argv[2];
	ss >> a;

	ss.clear();
	ss << argv[3];
	ss >> n;

	// order cant be 0 (div by 0)
	if (n == 0)
	{
		cout << "invalid args" << endl;
		return -3;
	}
	//cout << argv[1] << " " << argv[2] << " " << argv[3] << endl;
	

	//cout << g << " " << a << " " << n << endl;

	try
	{
		cout << calc(g,a,n) << endl;
	}
	catch(HashTableError)
	{
		// out of memory
		return -2;
	}


	return 0;

}

/* checks if given string consists of digits */
bool checkNumber(char* arg)
{
	stringstream s;
	string str;
	char digits[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	bool valid = true;
	s << arg;
	s >> str;

	for (size_t k = 0; k < str.length(); k++)
	{
		char c = str.at(k);

		if (valid == false)
		{
			return false;
		}

		valid = false;

		for (int l = 0; l < 10; l++)
		{

			if (c == digits[l])
			{
				valid = true;
			}
		}
	}

	return true;
}

