#ifndef _BMap_H_
#define _BMap_H_
#include <math.h>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <stdio.h>

namespace cop3530 {
	template<typename KEY, typename VALUE>
	class BMap {
	//---------------------------------------------------------------------
	//Private members and functions
	//---------------------------------------------------------------------
	private:

		template<typename KEYTYPE, typename VALTYPE>
		struct item;

		template<typename VALTYPE>
		struct item <signed int, VALTYPE>{
			signed int key;
			VALTYPE val;
			bool isOccupied;
			item<signed int, VALTYPE>* next;

			//Standard constructor, used to insert actual items
			item(signed int k, VALTYPE v, bool occupied){
				key = k;
				val = v;
				isOccupied = occupied;
				next = 0;
			}

			//Default constructor, used to create nullItems
			item(){
				key = -1;
				val = 0;
				isOccupied = false;
				next = 0;
			}

			//Comparison operators
			bool compareKeys(signed int k){
				return key == k;
			}
			bool operator==(const item& rhs) const { return (key == rhs.key && val == rhs.val && isOccupied == rhs.isOccupied && next == rhs.next); }
			bool operator!=(const item& rhs) const { return (key != rhs.key && val != rhs.val && isOccupied != rhs.isOccupied && next != rhs.next); }
		};

		template<typename VALTYPE>
		struct item < double, VALTYPE > {
			double key;
			VALTYPE val;
			bool isOccupied;
			item<double, VALTYPE>* next;

			//Standard constructor, used to insert actual items
			item(double k, VALTYPE v, bool occupied){
				key = k;
				val = v;
				isOccupied = occupied;
				next = 0;
			}

			//Default constructor, used to create nullItems
			item(){
				key = -1.0;
				val = 0;
				isOccupied = false;
				next = 0;
			}

			//Comparison operators
			bool compareKeys(double k){
				return key == k;
			}
			bool operator==(const item& rhs) const { return (key == rhs.key && val == rhs.val && isOccupied == rhs.isOccupied && next == rhs.next); }
			bool operator!=(const item& rhs) const { return (key != rhs.key && val != rhs.val && isOccupied != rhs.isOccupied && next != rhs.next); }
		};

		template<typename VALTYPE>
		struct item < char*, VALTYPE > {
			char* key;
			VALTYPE val;
			bool isOccupied;
			item<char*, VALTYPE>* next;

			//Standard constructor, used to insert actual items
			item(char* k, VALTYPE v, bool occupied){
				key = k;
				val = v;
				isOccupied = occupied;
				next = 0;
			}

			//Default constructor, used to create nullItems
			item(){
				key = "0";
				val = 0;
				isOccupied = false;
				next = 0;
			}

			//Comparison operators
			bool compareKeys(char* k){
				if (key == "" || k == "") return false;
				char* tmp = key;
				char* tmp2 = k;
				while (*tmp && *tmp2){
					if (*tmp++ != *tmp2++)
						return false;
				}

				return true;
			}
			bool operator==(const item& rhs) const { return (compareKeys(rhs.key) && val == rhs.val && isOccupied == rhs.isOccupied && next == rhs.next); }
			bool operator!=(const item& rhs) const { return (!compareKeys(rhs.key) && val != rhs.val && isOccupied != rhs.isOccupied && next != rhs.next); }
		};

		template<typename VALTYPE>
		struct item < std::string, VALTYPE > {
			std::string key;
			VALTYPE val;
			bool isOccupied;
			item<std::string, VALTYPE>* next;

			//Standard constructor, used to insert actual items
			item(std::string k, VALTYPE v, bool occupied){
				key = k;
				val = v;
				isOccupied = occupied;
				next = 0;
			}

			//Default constructor, used to create nullItems
			item(){
				key = "";
				val = 0;
				isOccupied = false;
				next = 0;
			}

			//Comparison operators
			bool compareKeys(std::string k){
				if (key == "" || k == "") return false;
				int i, j;
				for (i = j = 0; i != key.size() && j != k.size(); ++i, ++j){
					if (key[i] != k[j])
						return false;
				}
				return true;
			}
			bool operator==(const item& rhs) const { return (compareKeys(rhs.key) && val == rhs.val && isOccupied == rhs.isOccupied && next == rhs.next); }
			bool operator!=(const item& rhs) const { return (!compareKeys(rhs.key) && val != rhs.val && isOccupied != rhs.isOccupied && next != rhs.next); }
		};

		template<typename KEYTYPE, int DUMMY>
		class hash;

		template<int DUMMY>
		class hash < signed int, DUMMY > {
		private:
			BMap& m_Parent;
		public:
			hash(BMap& parent) : m_Parent(parent){}
			size_t operator()(signed int key){
				double fractionalPart = (key * m_Parent.A) - floor(key * m_Parent.A);
				size_t index = (int)floor(fractionalPart * m_Parent.m_Capacity);

				return index;
			}
		};

		template<int DUMMY>
		class hash < double, DUMMY > {
		private:
			BMap& m_Parent;
		public:
			hash(BMap& parent) : m_Parent(parent){}
			size_t operator()(double key){
				double fractionalPart = (key * m_Parent.A) - floor(key * m_Parent.A);
				size_t index = (int)floor(fractionalPart * m_Parent.m_Capacity);

				return index;
			}
		};

		template<int DUMMY>
		class hash < char*, DUMMY > {
		private:
			BMap& m_Parent;
		public:
			hash(BMap& parent) : m_Parent(parent){}
			size_t operator()(char* key){
				unsigned int index = 0;
				char* tmp = key;
				while (*tmp){
					if (index > 1000000){
						index /= *tmp;
					}
					index ^= (index << 5) + *tmp++;
				}

				size_t result = index;
				return result;
			}
		};

		template<int DUMMY>
		class hash < std::string, DUMMY > {
		private:
			BMap& m_Parent;
		public:
			hash(BMap& parent) : m_Parent(parent){}
			size_t operator()(std::string key){
				int index = 1;
				for (size_t j = 0; j != key.size(); ++j)
					index ^= (index << 5) + key[j];

				size_t result = index;
				return result;
			}
		};

		bool isPrime(int x){
			if (x <= 3)
				return x > 1;
			else if (x % 2 == 0 || x % 3 == 0)
				return false;
			else {
				for (int i = 5; i * i <= x; i += 6){
					if (x % i == 0 || x % (i + 2) == 0)
						return false;
				}
				return true;
			}

		}

		hash<KEY, 0> m_hash;
		const double A; //Arbitrary val used in hash function

		item<KEY, VALUE>** itemArray;

		size_t m_Capacity;
		size_t m_Size;
	//---------------------------------------------------------------------
	//Public members and functions
	//---------------------------------------------------------------------
	public:
		//typedef size_t size_t;

		//---------------------------------------------------------------------
		//Constructors and destructors
		//---------------------------------------------------------------------
		BMap(size_t M) : m_Capacity(M), m_hash(*this), A(0.123456789){
			while (!isPrime(m_Capacity)) //keep adjusting the capacity until it is prime, necesarry for hash2 to guarentee better distribution
				m_Capacity++;
			itemArray = new item<KEY, VALUE>*[m_Capacity];

			for (size_t i = 0; i != m_Capacity; ++i)
				itemArray[i] = new item<KEY, VALUE>();
			
			
			m_Size = 0;
		}

		BMap(const BMap& src) : A(src.A){
			if (&src == this) // check for self-assignment
				return;     // do nothing

			for (size_t i = 0; i != m_Capacity; ++i){
				item<KEY, VALUE>* current = itemArray[i];
				while (current != 0){
					item<KEY, VALUE>* toDelete = current;
					current = current->next;

					delete toDelete;
				}
			}

			delete this->itemArray;

			this->itemArray = src.itemArray;
			this->m_Capacity = src.m_Capacity;
			this->m_Size = src.m_Size;
		}

		~BMap(){
			for (size_t i = 0; i != m_Capacity; ++i){
				item<KEY, VALUE>* current = itemArray[i];
				while (current != 0){
					item<KEY, VALUE>* toDelete = current;
					current = current->next;

					delete toDelete;
				}
			}

			delete this->itemArray;
		}

		BMap& operator=(const BMap& src){
			if (&src == this) // check for self-assignment
				return *this;     // do nothing

			for (size_t i = 0; i != m_Capacity; ++i){
				item<KEY, VALUE>* current = itemArray[i];
				while (current != 0){
					item<KEY, VALUE>* toDelete = current;
					current = current->next;

					delete toDelete;
				}
			}

			delete this->itemArray;

			this->itemArray = src.itemArray;
			return *this;
		}

		//---------------------------------------------------------------------
		//Public functions
		//---------------------------------------------------------------------
		int insert(KEY key, VALUE value){
			int i = 0;
			size_t index = m_hash(key) % m_Capacity;
			item<KEY, VALUE>* current = itemArray[index];
			while (current->next != 0){
				//continues probing until
					//that specific key is found or
					//an empty link is found in the bucket
				if (current->key == key) break;
				else current = current->next;
				i++;
			}	
			if (current->key == key) current->val = value;
			else current->next = new item<KEY, VALUE>(key, value, true);
			
			m_Size++;
			return i;
		}

		int remove(KEY key, VALUE& value){
			int i = 0;
			size_t index = m_hash(key) % m_Capacity;
			item<KEY, VALUE>* current = itemArray[index];
			item<KEY, VALUE>* prev = current;
			while (current->next != 0){
				//continues probing until
					//that specific key is found
					//an empty link is found in the bucket
				if (current->key == key)
					break;
				else{
					prev = current;
					current = current->next;
					
				}
				i++;
			}
			//if the key is found, patch the links around it and remove it from the list
			if (current->key == key){ 
				value = current->val;
				prev->next = current->next;
				delete current;
				m_Size--;
				
				return i;
			}
			else return -1 * i;
		}

		int search(KEY key, VALUE& value){

			int i = 0;
			size_t index = m_hash(key) % m_Capacity;
			item<KEY, VALUE>* current = itemArray[index];
			item<KEY, VALUE>* prev = current;
			while (current->next != 0){
				//continues probing until
				//that specific key is found
				//an empty link is found in the bucket
				if (current->key == key)
					break;
				else{
					prev = current;
					current = current->next;

				}
				i++;
			}
			
			if (current->key == key){
				value = current->val;
				return i;
			}
			else return -1*i;
		}

		void clear(){
			for (size_t i = 0; i != m_Capacity; ++i){
				item<KEY, VALUE>* current = itemArray[i];
				while (current != 0){
					item<KEY, VALUE>* toDelete = current;
					current = current->next;

					delete toDelete;
				}
				itemArray[i] = new item<KEY, VALUE>();
			}
			m_Size = 0;
		}
			
		bool is_empty(){
			return (m_Size == 0);
		}

		std::size_t capacity(){
			return m_Capacity;
		}

		std::size_t size(){
			return m_Size;
		}

		double load(){
			int usedBuckets = 0;
			for (size_t i = 0; i != m_Capacity; ++i){
				if (itemArray[i]->next != 0) usedBuckets++;
			}

			double load = ((double)usedBuckets) / m_Capacity;
			return load;
		}

		std::ostream& print(std::ostream& out) const {
			out << "[ ";
			for (size_t i = 0; i != (m_Capacity - 1); ++i){
				item<KEY, VALUE>* current = itemArray[i]->next;
				if (current == 0){
					out << "-, ";
					continue;
				}
				while (current != 0){
					out << current->key;
					out << ", ";
					current = current->next;
				}
			}

			if (itemArray[m_Capacity - 1]->next != 0){
				item<KEY, VALUE>* current = itemArray[m_Capacity - 1]->next;
				while (current->next != 0){
					out << current->key;
					out << ", ";
					current = current->next;
				}
				out << current->key;
			}
			else
				out << "-";
			out << "]" << std::endl;

			return out;
		}

		int** cluster_distribution(){
			int maxBucketSize = 0;
			for (size_t i = 0; i != m_Capacity; ++i){
				if (itemArray[i]->next != 0){
					item<KEY, VALUE>* current = itemArray[i]->next;
					int currentBucketSize = 0;
					while (current != 0){
						current = current->next;
						currentBucketSize++;
					}
					maxBucketSize = (currentBucketSize < maxBucketSize) ? maxBucketSize : currentBucketSize;
				}
			}

			int** list = new int*[maxBucketSize];
			for (size_t i = 0; i != maxBucketSize; ++i){
				list[i] = new int[2];
				list[i][0] = i + 1;
				list[i][1] = 0;
			}

			for (size_t i = 0; i != m_Capacity; ++i){
				if (itemArray[i]->next != 0){
					item<KEY, VALUE>* current = itemArray[i]->next;
					int currentBucketSize = 0;
					while (current != 0){
						current = current->next;
						currentBucketSize++;
					}
					list[currentBucketSize-1][1]++;
				}
			}

			int totalClusterSizes = 0;
			for (size_t i = 0; i != maxBucketSize; ++i){
				if (list[i][1] != 0) totalClusterSizes++;
			}

			int** returnList = new int*[totalClusterSizes];
			int j = 0;
			for (size_t i = 0; i != maxBucketSize; ++i){
				if (list[i][1] != 0) {
					returnList[j] = new int[2];
					returnList[j][0] = list[i][0];
					returnList[j][1] = list[i][1];
					j++;
				}
			}

			delete list;
			return returnList;
		}
		
		KEY remove_random(){
			KEY k;
			VALUE v;
			std::srand(time(0));
			int index = std::rand() % m_Size;
			while (index == 0)
				index = std::rand() % m_Size;
			int i = 0;
			int j = 0;
			while (i != index){
				if (itemArray[j]->next != 0){
					item<KEY, VALUE>* current = itemArray[j];
					while (current->next != 0){
						current = current->next;
						i++;
						if (i == index) break;
					}
					if (i == index){
						k = current->key;
					}
					j++;
				}
				else j++;
			}
			
			this->remove(k, v);

			return k;
		}

	};//end class BMap
} //end namespace cop3530
#endif // _BMap_H_
