#include <iostream>

using namespace std;

typedef unsigned long ulint;

ulint power(ulint a, ulint b, ulint n)
{
	//convert h to binary
	int binaryPower[64];
	int index = -1;

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

	for (int i = 0; i < 64; i++)
	{
		cout << binaryPower[i];
	}

	cout << endl;

	ulint pow = a;
	ulint result = 1;

	int counter = 0;
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

int main()
{
	cout << power(3,1233,1000000000) << endl;
}