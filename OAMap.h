#ifndef _OAMap_H_
#define _OAMap_H_
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>


namespace cop3530 {


	//PROBE == 0 -> Linear Probe
	//PROBE == 1 -> Quadratic Probe
	//PROBE == 2 -> Rehashing
	template <typename KEY, typename VALUE, int PROBE>
	class OAMap{

	//---------------------------------------------------------------------
	//Private members and functions
	//---------------------------------------------------------------------
	private:

		template <typename KEYTYPE, typename VALTYPE>
		struct item;
		
		template<typename VALTYPE>
		struct item<signed int, VALTYPE> {
			signed int key;
			VALTYPE val;
			bool isOccupied;
			//Standard constructor, used to insert actual items
			item(signed int k, VALTYPE v, bool occupied){
				key = k;
				val = v;
				isOccupied = occupied;
			}

			//Default constructor, used to create nullItems
			item(){
				key = -1;
				val = 0;
				isOccupied = false;
			}

			//Comparison operators
			bool compareKeys(signed int k){
				return key == k;
			}
			bool operator==(const item<signed int, VALTYPE>& rhs) const { return (key == rhs.key && val == rhs.val && isOccupied == rhs.isOccupied); }
			bool operator!=(const item<signed int, VALTYPE>& rhs) const { return (key != rhs.key && val != rhs.val && isOccupied != rhs.isOccupied); }
		};
		
		template<typename VALTYPE>
		struct item<double, VALTYPE> {
			double key;
			VALTYPE val;
			bool isOccupied;
			//Standard constructor, used to insert actual items
			item(double k, VALTYPE v, bool occupied){
				key = k;
				val = v;
				isOccupied = occupied;
			}

			//Default constructor, used to create nullItems
			item(){
				key = -1.0;
				val = 0;
				isOccupied = false;
			}

			//Comparison operators
			bool compareKeys(double k){
				return key == k;
			}
			bool operator==(const item<double, VALTYPE>& rhs) const { return (key == rhs.key && val == rhs.val && isOccupied == rhs.isOccupied); }
			bool operator!=(const item<double, VALTYPE>& rhs) const { return (key != rhs.key && val != rhs.val && isOccupied != rhs.isOccupied); }
		};

		template<typename VALTYPE>
		struct item<char*, VALTYPE> {
			char* key;
			VALTYPE val;
			bool isOccupied;
			//Standard constructor, used to insert actual items
			item(char* k, VALTYPE v, bool occupied){
				key = k;
				val = v;
				isOccupied = occupied;
			}

			//Default constructor, used to create nullItems
			item(){
				key = "0";
				val = 0;
				isOccupied = false;
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
			bool operator==(const item<char*, VALTYPE>& rhs) const { return (compareKeys(rhs.key) && val == rhs.val && isOccupied == rhs.isOccupied); }
			bool operator!=(const item<char*, VALTYPE>& rhs) const { return (!compareKeys(rhs.key) != rhs.val && isOccupied != rhs.isOccupied); }
		};

		template<typename VALTYPE>
		struct item<std::string, VALTYPE> {
			std::string key;
			VALTYPE val;
			bool isOccupied;
			//Standard constructor, used to insert actual items
			item(std::string k, VALTYPE v, bool occupied){
				key = k;
				val = v;
				isOccupied = occupied;
			}

			//Default constructor, used to create nullItems
			item(){
				key = "";
				val = 0;
				isOccupied = false;
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
			bool operator==(const item<std::string, VALTYPE>& rhs) const { return (compareKeys(rhs.key) && val == rhs.val && isOccupied == rhs.isOccupied); }
			bool operator!=(const item<std::string, VALTYPE>& rhs) const { return (!compareKeys(rhs.key) != rhs.val && isOccupied != rhs.isOccupied); }
		};

		template<typename KEYTYPE, int PROBETYPE>
		class hash;

		
		template<int PROBETYPE>
		class hash < signed int, PROBETYPE > {
		private:
			double probe(signed int key, int i){
				if (PROBETYPE == 0)
					return 1;
				else if (PROBETYPE == 1)
					return  (1 + i)/2.0;
				else if (PROBETYPE == 2)
					return rehash(key,i);
			}

			size_t rehash(signed int key, int i){
				size_t result = 1 + (key % (m_Parent.m_Capacity - 1));
				return result;
			}

			OAMap& m_Parent;
		public:
			hash(OAMap& parent) : m_Parent(parent) {}
			size_t operator()(signed int key, int i){
				double fractionalPart = (key * m_Parent.A) - floor(key * m_Parent.A);
				int index = (int)floor(fractionalPart * m_Parent.m_Capacity);

				size_t result = (int)(index + i*probe(key, i));
				return result;
			}
		};

		template<int PROBETYPE>
		class hash < double, PROBETYPE > {
		private:
			double probe(double key, int i){
				if (PROBETYPE == 0)
					return 1;
				else if (PROBETYPE == 1)
					return (1+i)/2.0;
				else if (PROBETYPE == 2)
					return rehash(key,i);
			}

			size_t rehash(double key, int i){
				size_t result = 1 + ((int)key % (m_Parent.m_Capacity - 1));
				return result;
			}
			OAMap& m_Parent;
		public:
			hash(OAMap& parent) : m_Parent(parent){}
			size_t operator()(double key, int i){
				double fractionalPart = (key * m_Parent.A) - floor(key * m_Parent.A);
				int index = (int)floor(fractionalPart * m_Parent.m_Capacity);

				size_t result = (int)(index + i*probe(key, i));
				return result;
			}
		};

		template<int PROBETYPE>
		class hash < char*, PROBETYPE > {
		private:
			double probe(char* key, int i){
				if (PROBETYPE == 0)
					return 1;
				else if (PROBETYPE == 1)
					return (1+i)/2.0;
				else if (PROBETYPE == 2)
					return rehash(key, i);
			}

			size_t rehash(char* key, int i){
				unsigned int index = 7; //prime numbers
				char* tmp = key;
				while (*tmp){
					index = 33 * index ^ *tmp++;
				}

				size_t result = 1 + (index % (m_Parent.m_Capacity - 1));
				return result;
			}
			OAMap& m_Parent;
		public:
			hash(OAMap& parent) : m_Parent(parent){}
			size_t operator()(char* key, int i){

				unsigned int index = 0;
				char* tmp = key;
				while (*tmp){
					if (index > 1000000){
						index /= *tmp;
					}
					index ^= (index << 5) + *tmp++;
				}

				size_t result = (int)(index + i*probe(key, i));
				return result;
			}
		};

		template<int PROBETYPE>
		class hash < std::string, PROBETYPE > {
		private:
			double probe(std::string key, int i){
				if (PROBETYPE == 0)
					return 1;
				else if (PROBETYPE == 1)
					return (1+i)/2.0;
				else if (PROBETYPE == 2)
					return rehash(key, i);
			}

			size_t rehash(std::string key, int i){
				int index = 7; //prime numbers
				for (size_t j = 0; j != key.size(); ++j){
					index = 33 * index ^ key[j];
					
				}

				size_t result = 1 + (index % (m_Parent.m_Capacity - 1));
				return result;
			}
			OAMap& m_Parent;
		public:
			hash(OAMap& parent) : m_Parent(parent){}
			size_t operator()(std::string key, int i){
				int index = 1;
				for (size_t j = 0; j != key.size(); ++j)
					index ^= (index << 5) + key[j];

				size_t result = (int)(index + i*probe(key, i));
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
		bool isPowerOf2(int x){
			return x > 0 && !(x & (x - 1));
		}
		
		hash<KEY, PROBE> m_hash;
		const double A; //Arbitrary val used in hash function

		item<KEY, VALUE>** itemArray;
		item<KEY, VALUE>* nullItem;
		
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

		OAMap(size_t M) : m_Capacity(M), m_hash(*this), A(0.123456789){
			if (PROBE == 1)	while (!isPowerOf2(m_Capacity))
					m_Capacity++;
			else while (!isPrime(m_Capacity)){ //keep adjusting the capacity until it is prime, necesarry for hash2 to guarentee better distribution
				m_Capacity++;
			}
			
			itemArray = new item<KEY, VALUE>*[m_Capacity];

			for (size_t i = 0; i != m_Capacity; ++i){
				itemArray[i] = new item<KEY, VALUE>();
			}
			
			nullItem = new item<KEY, VALUE>();
			nullItem->isOccupied = true;


			m_Size = 0;
		}

		OAMap(const OAMap& src) : A(src.A) {
			if (&src == this) // check for self-assignment
				return;     // do nothing
			//delete previous items in object before assigning new ones			
			for (size_t i = 0; i != m_Capacity; ++i)
				if (itemArray[i] != nullItem) delete this->itemArray[i];

			delete this->itemArray;
			delete this->nullItem;

			this->itemArray = src.itemArray;
			this->nullItem = src.nullItem;
			this->m_Capacity = src.m_Capacity;
			this->m_Size = src.m_Size;
		}

		~OAMap(){
			for (size_t i = 0; i != m_Capacity; ++i){
				if (itemArray[i] != nullItem) delete this->itemArray[i];
			}
				

			delete this->itemArray;
			delete this->nullItem;
		}

		OAMap& operator=(const OAMap& src) {
			if (&src == this) // check for self-assignment
				return *this;     // do nothing


			//delete previous items in object before assigning new ones
			for (size_t i = 0; i != m_Capacity; ++i)
				if (this->itemArray[i] != this->nullItem) delete this->itemArray[i];

			delete this->itemArray;
			delete this->nullItem;

			this->itemArray = src.itemArray;
			this->nullItem = src.nullItem;
			return *this;
		}

		//---------------------------------------------------------------------
		//Public functions
		//---------------------------------------------------------------------
		int insert(KEY key, VALUE value){
			
			if (m_Size == m_Capacity)
				return -1;

			//This variable is used in the case a nullItem is found. If it is, the first one to be found should have its index saved for possible insertion
			int nullItemIndex = -1;

			int i = 0;
			size_t index = m_hash(key, i) % m_Capacity;
			while (itemArray[index]->isOccupied && i != m_Capacity){ 
				//continues probing until:
					//the specific key to be replaced is found or
					//an empty slot is found
					//the probe has searched through each array slot and did not find an available slot.
						//this only occurs in the situation in which every item in the array is a nullItem

				if (itemArray[index]->compareKeys(key))
					break;
				else if (itemArray[index] == nullItem && nullItemIndex == -1){
					nullItemIndex = index;
				}
				else index = m_hash(key, ++i) % m_Capacity;
			}
			
			if (itemArray[index]->compareKeys(key)){
				itemArray[index]->val = value;
				return i;
			}
			else if (nullItemIndex != -1){
				itemArray[nullItemIndex] = new item<KEY, VALUE>(key, value, true);
			}
			else {
				if (itemArray[index] != nullItem) delete itemArray[index];
				itemArray[index] = new item<KEY, VALUE>(key, value, true);
			}

			m_Size++;
			return i;
		}

		int remove(KEY key, VALUE& value){
					
			int i = 0;
			size_t index = m_hash(key, i) % m_Capacity;

			while (itemArray[index]->isOccupied && i != m_Capacity){ 
				//continues probing until:
					//that specific key is found or 
					//an empty index is found or 
					//the probe has searched through each array slot and did not find an available slot.
						//this only occurs in the situation in which every item in the array is a nullItem

				if (itemArray[index]->compareKeys(key))
					break;
				else index = m_hash(key, ++i) % m_Capacity;
			}
			if (itemArray[index]->compareKeys(key)){
				value = itemArray[index]->val;
				delete itemArray[index];
				itemArray[index] = nullItem;

				m_Size--;
				return i;
			}
			else return -1 * i;
		}

		int search(KEY key, VALUE&  value){
			int i = 0;
			size_t index = m_hash(key, i) % m_Capacity;
			while (itemArray[index]->isOccupied && i != m_Capacity){
				if (itemArray[index]->compareKeys(key))
					break;
				else index = m_hash(key, ++i) % m_Capacity;
			}

			if (itemArray[index]->compareKeys(key)){
				value = itemArray[index]->val;
				return i;
			}
			else return -1 * i;
		}

		void clear(){
			for (size_t i = 0; i != m_Capacity; ++i){
				if (itemArray[i] != nullItem) delete itemArray[i];
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
			double load = ((double)m_Size) / m_Capacity;
			return load;
		}

		std::ostream& print(std::ostream& out) const {
			out << "[ ";
			for (size_t i = 0; i != (m_Capacity-1); ++i) {
				if (itemArray[i]->isOccupied && itemArray[i] != nullItem)
					out << itemArray[i]->key;
				else
					out << "-";
				out << ", ";
			}
			if (itemArray[m_Capacity - 1]->isOccupied && itemArray[m_Capacity - 1] != nullItem)
				out << itemArray[m_Capacity - 1]->key;
			else
				out << "-";
			out << "]" << std::endl;

			return out;
		}

		int** cluster_distribution(){
			if (this->load() == 1){
				int** toReturn = new int*[1];
				toReturn[0] = new int[2];
				toReturn[0][0] = m_Capacity;
				toReturn[0][1] = 1;
				return toReturn;
			}
			if (this->load() == 0){
				int** toReturn = new int*[1];
				toReturn[0] = new int[2];
				toReturn[0][0] = 0;
				toReturn[0][1] = 1;
				return toReturn;
			}
			
			int maxClusterSize = 0;
			
			size_t i = 0;
			for (i = 0; i != m_Capacity; ++i){
				if (!itemArray[i]->isOccupied){
					break;
				}
			}
			size_t j = i+1;
			while (j != i){
				if (itemArray[j]->isOccupied){
					int currentClusterSize = 0;
					while (itemArray[j]->isOccupied){
						currentClusterSize++;
						j = (j+1) % m_Capacity;
					}
					maxClusterSize = (maxClusterSize > currentClusterSize) ? maxClusterSize : currentClusterSize;
				}
				else j = (j + 1) % m_Capacity;
			}
			
			int** tmpList = new int*[maxClusterSize];
			for (size_t k = 0; k != maxClusterSize; ++k){
				tmpList[k] = new int[2];
				tmpList[k][0] = k + 1;
				tmpList[k][1] = 0;
			}

			j = i + 1;
			while (j != i){
				if (itemArray[j]->isOccupied){
					int currentClusterSize = 0;
					while (itemArray[j]->isOccupied){
						currentClusterSize++;
						j = (j + 1) % m_Capacity;
					}
					tmpList[currentClusterSize - 1][1]++;
				}
				else j = (j + 1) % m_Capacity;
			}

			int totalClusterSizes = 0;
			for (size_t l = 0; l != maxClusterSize; ++l){
				if (tmpList[l][1] != 0) totalClusterSizes++;
			}

			int** returnList = new int*[totalClusterSizes];
			j = 0;
			for (size_t m = 0; m != maxClusterSize; ++m){
				if (tmpList[m][1] != 0){
					returnList[j] = new int[2];
					returnList[j][0] = tmpList[m][0];
					returnList[j][1] = tmpList[m][1];
					++j;
				}
			}

			delete tmpList;
			return returnList;
		}

		KEY remove_random(){
			std::srand(time(0));
			int index = std::rand() % m_Size;
			while (index == 0)
				index = std::rand() % m_Size;
			int i = 0;
			int j = 0;
			while (i != index){
				if (itemArray[j]->isOccupied){
					i++;
				}
				j++;
			}
			KEY k = itemArray[j-1]->key;
			VALUE v;
			this->remove(k, v);

			return k;
		}
		

	};//end class OAMap

}// end namespace cop3530
#endif // _OAMap_H_
