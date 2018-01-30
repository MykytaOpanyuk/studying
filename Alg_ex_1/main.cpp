//============================================================================
// Name        : perfect_hasing_ex_1.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <list>
#include <cmath>
#include <ctime>

using namespace std;

const int check_collision_value = -1; //use it to check collision after hashing
long long p;//prime number for hashing function

typedef struct for_hashing {
	long long a,b;
	int m;
} hashing;

void write_numbers_in_file ()
{
	bool check = true;
	int count = 0;
	long long value;
	fstream file("numbers.bin", ios_base::binary | ios_base::app);
	while (check) {
		cout << "Write a positive number (negative to stop):" << endl;
		cin >> value;
		if (value < 0)
			check = false;
		else {
			file << value << " ";
			count++;
		}
		if ((count % 5) == 0)
			file << endl;
	}

}

int read_numbers_from_file (vector <long long>& data)
{
	int value, n = 0;
	fstream file("numbers.bin", ios_base::binary | ios_base::in);
	while (file >> value) {
		data.push_back(value);
		n++;
	}
	return n;
}

long long max_number(vector<long long> array)
{
	int max_data = 0;
	for (int i = 0; i < (int)array.size(); i++) {
		if (max_data < array[i])
			max_data = array[i];
	}
	return max_data;
}

bool is_prime(long long value)
{
	if ((value == 0) || (value == 1))
		return false;

	for (long long i = 2; i <= (long long)sqrt(value); i++)
		if (value % i == 0)
			return false;

	return true;
}

long long successor_prime(long long value)
{
	if (value == 0)
		value++;
	value++;
	while (!is_prime(value))
		value++;

	return value;
}

unsigned long long hash(hashing *table, int key)
{
	return (((table->a*key + table->b) % p) % table->m);
}

class secondary_table
{
public:
	hashing *new_table;
	vector<int> cells;
	secondary_table() {}
	void Initialize_variables(int size)
	{
		if (size == 1) {
			new_table->m = size;
			new_table->a = 0;
			new_table->b = 0;
		}
		else {
			new_table->m = size;
			new_table->a = rand() % p;
			new_table->b = rand() % (p - 1) + 1;
		}
		for (int i = 0; i < size; i++)
			cells[i] = check_collision_value;
	}
void Construct(vector<int> input)
{
	int size;
	unsigned long long hash_key;
	if (input.empty()) {
		size = 0;
		return;
	}
	size = input.size() * input.size();//to a have a low chance of collision
	bool check = false;
	cells.resize(size);
	new_table = new hashing;
	// while there is no collisions in table
	while (check == false) {
		Initialize_variables(size);

	    for(unsigned int i = 0; i < input.size(); i++) {
	    	hash_key = hash(new_table, input[i]);
	    	// if collision then try construct new hash table
	    	if (cells[hash_key] != check_collision_value)
	    		break;
	    	cells[hash_key] = input[i];
	    }

	    check = true;
	}
}

};

class main_table
{
    hashing *basic_table;
    vector<vector<int> > elements_in_cells;
    vector<secondary_table> sec_tables;

public:
    main_table(int size)
    {
        basic_table = new hashing;
        basic_table->m = size;
        basic_table->a = rand() % p;
        basic_table->b = rand() % (p - 1) + 1;

        sec_tables.resize(size);
    }

    void Initialize(vector<long long>& numbers) //type of data for hashing is numbers
    {
        //_tableSize = numbers.size();
    	//_buckets.resize(numbers.size());
    	elements_in_cells.resize(basic_table->m);
        for (unsigned int i = 0; i < numbers.size(); i++) {
            unsigned long long hash_key = hash(basic_table, numbers[i]);
            elements_in_cells[hash_key].push_back(numbers[i]);
        }
        for (unsigned int i = 0; i < numbers.size(); i++) {
        	sec_tables[i].Construct(elements_in_cells[i]);

        }
    }
    bool is_contains(int number)
    {
        unsigned int hash_key = hash(basic_table, number);

        if (sec_tables[hash_key].new_table->m == 0)
    		return false;

        unsigned long long hash_sec_key = hash(sec_tables[hash_key].new_table, number);

    	if (sec_tables[hash_key].cells[hash_sec_key] == number)
    		return true;
    	return false;
    }
};


int main() {
	int n, number_for_search, choice;//number of elements
	vector<long long> numbers;

	srand (time(0));

	do {
		cout << "1)Add new numbers to file." << endl;
		cout << "2)Use file for hashing." << endl;
		cin >> choice;
		if (choice == 1)
			write_numbers_in_file();
		if (choice == 2)
			n = read_numbers_from_file(numbers);
	}
	while (choice != 2);
	p = successor_prime(max_number(numbers));
	main_table T(n);

	cout << "Hashing numbers..." << endl;

	cout << numbers[0] << " ";
	T.Initialize(numbers);

	cout << "All numbers have a hash key." << endl;

	cin >> number_for_search;
	if (T.is_contains(number_for_search))
		cout << "This number is in table." << endl;
	else
		cout << "No, there isn't number like this." << endl;
	return 0;
}
