/* Charlie Hewitt
AC21008: Hash Tables
Matriculation Number: 170015454 */

#ifndef MY_HASH_TABLE
#define MY_HASH_TABLE

#include "HashNode.h"
#include <vector>
#include <list>
#include <iostream>


using namespace std;

enum HashTableError { OUT_OF_MEMORY, KEY_NOT_FOUND, DUPLICATE_KEY }; // extend if necessary

typedef unsigned long ulint;

class HashTable {
  typedef vector <list<HashNode> > Table;
  Table *table; // size of table is stored in the Table data structure
  size_t num;   // number of entries in the HashTable;

public:
  HashTable();       // constructor, initializes table of size 11;
  HashTable(size_t); // constructor, requires size of table as arg
  ~HashTable();      // deconstructor

  size_t size(); // returns size of the hash table (number of buckets)
  size_t hash_function(ulint);  // the table's hash function
  ulint getValue(ulint);    // find and return data associated with key

  void insert(ulint,ulint); // insert data associated with key into table
  void erase(ulint);        // remove key and associated data from table

  void rehash(size_t); // sets a new size for the hash table, rehashes the hash table 

  float load(); // returns load factor of table (num/size * 100)
  bool contains(ulint); // checks whether table contains given key
};

  /* Initialises a hash table of size 11 */ 
  HashTable::HashTable()
  {
    try
    {
      this -> table = new Table(11, list<HashNode>()); // new vector<list<HashNode>>(11, list<HashNode>());;
      this -> num = 0;
    }
    catch(...) // if bad alloc thrown, throw out of memory
    {
      cerr << "Out of memory" << endl;
      throw OUT_OF_MEMORY;
    }
  }

  /* Initialises a hash table of given size t */
  HashTable::HashTable(size_t t)
  {
    try
    {
      this -> table = new Table(t, list<HashNode>());
      this -> num = 0;
    }
    catch(...) // if bad alloc thrown, throw out of memory
    {
      cerr << "Out of memory" << endl;
      throw OUT_OF_MEMORY;
    }

    // if (table == nullptr)
    // {
    //   cout << "out of mem" << endl;
    //   throw OUT_OF_MEMORY;
    // }
  }

  /* Destructor for HashTable */
  HashTable::~HashTable()
  {
    delete this -> table;
  }

  /* Hash function, hashes k to h = k mod tablesize (uniform distribution if size is prime) */
  size_t HashTable::hash_function(ulint k)
  {
    ulint h = k % size(); 
    return h;
  }

  /* Returns the size of the table */
  size_t HashTable::size()
  {
    size_t s = (*table).size();
    return s;
  }

  /* Inserts key, value into the table (and increments num) */
  void HashTable::insert(ulint key, ulint value)
  {
    if (contains(key)) // if key already in table, throw duplicate key expection
    {
      throw DUPLICATE_KEY;
    }

    if (load() > .9) // if load > .9, rehash the table to be double the size (+1 to get a slightly better distribution)
    {
      try
      {
        rehash(2* size() + 1);
      }
      catch(HashTableError)
      {
        throw OUT_OF_MEMORY;
      }

    }
    
    // create a node with key, value to add to the table
    HashNode temp;
    temp.assign(key,value);

    // hash the key to find a spot in the table
    size_t h = hash_function(key);

   // cout << "hash: " << h << endl;

    (*table).at(h).push_back(temp);

    // cout << "inserting key: " << key << " value: " << value << endl;

    // increment number of entries in table
    num++; 
  }

  /* Returns the value of the key in the table, or throws KEY_NOT_FOUND */
  ulint HashTable::getValue(ulint key)
  {
    size_t h = hash_function(key);

    for (HashNode e : (*table).at(h))
    {
    //  cout << "iterating through list" << endl;
      if (e.getKey() == key)
      {
     //   cout << "Found key: " << e.getKey() << " value: " << e.getValue() << endl;
        return e.getValue();
      }
    }

    throw KEY_NOT_FOUND;
  }

  /* If key is in the table, erase it and deincrement number of entries in table, Else throw KEY_NOT_FOUND */
  void HashTable::erase(ulint key)
  {
    // check if key in table
    if (!contains(key))
    {
      throw KEY_NOT_FOUND;
    }

    size_t h = hash_function(key);

    // the bucket the key is stored in
    list<HashNode>* chain = &(*table).at(h);

    // iterate through the chain until found, then erase node and num--
    list<HashNode>::iterator it;
    for (it = (*chain).begin(); it != (*chain).end(); ++it)
    {
        //cout << "looping through chain" << endl;
      if ((*it).getKey() == key)
      {
          //cout << "removing key: " << (*it).getKey() << " value: " << (*it).getValue() << endl;
        it = (*chain).erase(it);
          //cout << " erased" << endl;
        num--;
        break;
      }
    }
    // cout << "for loop finished " << endl;
  }


  /* Rehashes the current hashtable to the new, given size and inserts all the elements from the old table */
  void HashTable::rehash(size_t newSize)
  {

    // store old table
    Table* oldTable = table;

    try // create new table of new size
    {
      table = new Table(newSize, list<HashNode>());
    }
    catch(...){throw OUT_OF_MEMORY;};

    // new table is currently empty
    num = 0;

    // iterate through old table and insert each (key, value) into the new one
    Table::iterator tableIt;
    list<HashNode>::iterator listIt;
    int i = -1;

    for (tableIt = (*oldTable).begin(); tableIt!= (*oldTable).end(); ++tableIt) //for every list in the table
    {
      i++;
      list<HashNode>* listPtr = &((*oldTable).at(i)); // current bucket ( could be done by *tableIt!!!)

      for(listIt = (*listPtr).begin(); listIt != (*listPtr).end(); ++listIt) // for every node, get key value and insert it
      {
        ulint key = (*listIt).getKey();
        ulint value = (*listIt).getValue();
        //cout << "Rehashing key: " << key << endl;

        try // not necessary
        {
          insert(key, value);
        }
        catch(HashTableError)
        {
          cerr << "Error duplicate key" << endl;
        }

      }
    }

    // clean up
    delete oldTable;

  }

  /* returns the load factor of table (num of entries/ num of buckets) */
  float HashTable::load()
  {
    return (static_cast<float>(num)/static_cast<float>(size()));
  }

  /* checks if the table contains a node with the key */
  bool HashTable::contains(ulint key)
  {
    size_t h = hash_function(key);

    list<HashNode>::iterator listIt;

    for (listIt = (*table).at(h).begin(); listIt != (*table).at(h).end(); ++listIt) // the list the key should be in
    {
      if (listIt->getKey() == key)
      {
        return true;
      }
    }

    return false;
  }

#endif
