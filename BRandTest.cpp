/*#include <stdio.h>
#include "BMap.h"
#include <iostream>
#include <string>
using namespace std;




#define MAX_SIZE 100
signed int keys[MAX_SIZE];
char vals[MAX_SIZE];
bool inserted[MAX_SIZE];

bool compareKeys(char* x, char* y){
	if (x == "" || y == "") return false;
	char* tmp = x;
	char* tmp2 = y;
	while (*tmp && *tmp2){
		if (*tmp++ != *tmp2++)
			return false;
	}
	return true;
}

bool testInsert(cop3530::BMap<signed int, char>& map){

	int index = rand() % MAX_SIZE;
	std::cout << "----------Insert---------" << endl;

	signed int k = keys[index];
	char v = vals[index];

	if (map.insert(k, v) >= 0){
		std::cout << k;
		std::cout << ". Inserted: " << v << endl;
		inserted[index] = true;
		map.print(std::cout);
		return true;
	}
	else {
		std::cout << k;
		std::cout << ". " << v << " Failed" << endl;
		map.print(std::cout);
		return false;
	}
}

bool testRemove(cop3530::BMap<signed int, char>& map){
	int index = rand() % MAX_SIZE;
	std::cout << "----------Remove---------" << endl;

	while (!inserted[index]){
		index = rand() % MAX_SIZE;
	}

	signed int k = keys[index];
	char v = vals[index];
	if (map.remove(k, v) >= 0){
		std::cout << k;
		std::cout << ". Removed: " << v << endl;
		map.print(std::cout);
		inserted[index] = false;
		return true;
	}
	else {
		std::cout << k;
		std::cout << ". " << v << " Failed" << endl;
		map.print(std::cout);
		return false;
	}
}

bool testSearch(cop3530::BMap<signed int, char>& map){
	int index = rand() % MAX_SIZE;

	std::cout << "----------Search---------" << endl;

	while (!inserted[index]){
		index = rand() % MAX_SIZE;
	}

	signed int k = keys[index];
	char v = vals[index];

	if (map.search(k, v) >= 0){
		std::cout << k;
		std::cout << ". Found: " << v << endl;
		map.print(std::cout);
		return true;
	}
	else {
		std::cout << k;
		std::cout << ". " << v << " Failed" << endl;
		map.print(std::cout);
		return false;
	}
}


int main(){
	int mapSize;
	int loopCount;

	/*use if KEY == std::string || char*
	char letterArray[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm'};
	for (int i = 0; i != MAX_SIZE; ++i){
	keys[i] = new char[13];
	for (int j = 0; j != 13; j++){
	int randIndex = rand() % 13;
	keys[i][j] = letterArray[randIndex];
	}
	keys[i][12] = '\0';
	vals[i] = (char)(i % 26) + 65;
	inserted[i] = false;
	}

	/*use if KEY == signed int || double  
	for (int i = 0; i != MAX_SIZE; ++i){
		keys[i] = i + 1;
		vals[i] = (char)(i % 26) + 65;
		inserted[i] = false;
	}

	std::cout << "Hash Table Tests\nPlease enter the number hash table entries:";
	cin >> mapSize;
	cop3530::BMap<signed int, char> map(mapSize);
	std::cout << "Enter the number of operations to create: ";
	cin >> loopCount;

	std::srand(4);

	int fail = 0;
	int pass = 0;


	for (int i = 0; i != loopCount; ++i){
		std::cout << endl;
		int operation = std::rand() % 6;
		char val = (i % 26) + 65;
		std::cout << "Selected operation: " << endl;
		switch (operation)
		{
		case 0: //insert
			if (map.load() == 1){
				if (testRemove(map))
					pass++;
				else fail++;
				break;
			}
			if (testInsert(map))
				pass++;
			else fail++;
			break;
		case 1: //remove
			if (map.load() == 0){
				if (testInsert(map))
					pass++;
				else fail++;
				break;
			}
			if (testRemove(map))
				pass++;
			else fail++;
			break;
		case 2: //search
			if (map.load() == 0){
				if (testInsert(map))
					pass++;
				else fail++;
				break;
			}
			if (testSearch(map))
				pass++;
			else fail++;
			break;
		default:
			if (map.load() == 1){
				if (testRemove(map))
					pass++;
				else fail++;
				break;
			}
			if (testInsert(map))
				pass++;
			else fail++;
			break;
		}

	}
	std::cout << "Total succeed: ";
	std::cout << pass << endl;

	std::cout << "Total Fail: ";
	std::cout << fail << endl;


	system("pause");

	return 0;
}*/