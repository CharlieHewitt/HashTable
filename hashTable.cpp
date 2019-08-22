#include "HashTable.h"

using namespace std;

int main()
{
	try{

		HashTable t;
		for(int i = 0; i<10000000;i++)
		{
			cout << "loop: " << i << endl;
			t.insert(i, 0);
		}
	//t.insert(4,7);
	// t.insert(4,8);
	//t.getValue(7);
			cout << t.load() << endl;
	}
	catch(HashTableError)
	{
		cout << " woops" << endl;
	}




}