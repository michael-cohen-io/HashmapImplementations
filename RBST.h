#ifndef _RBST_H_
#define _RBST_H_
#include <iostream>
#include <time.h>
#include <queue>

namespace cop3530 {

	template<typename KEY, typename VALUE>
	class RBST{
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
			int left;
			int right;
			int height;
			//Standard constructor, used to insert actual items
			item(signed int k, VALTYPE v, bool occupied){
				key = k;
				val = v;
				isOccupied = occupied;
				left = right = -1;
				height = 0;
			}

			//Default constructor, used to create nullItems
			item(){
				key = -1;
				val = 0;
				isOccupied = false;
				left = right = -1;
				height = 0;
			}

			//Comparison operators
			bool compareKeys(signed int k){
				return key == k;
			}
			bool operator==(const item<signed int, VALTYPE>& rhs) const { return (key == rhs.key); }
			bool operator<(const item<signed int, VALTYPE>& rhs) const { return key < rhs.key; }
			bool operator!=(const item<signed int, VALTYPE>& rhs) const { return (key != rhs.key); }
		};

		template<typename VALTYPE>
		struct item<double, VALTYPE> {
			double key;
			VALTYPE val;
			bool isOccupied;
			int left;
			int right;
			int height;
			//Standard constructor, used to insert actual items
			item(double k, VALTYPE v, bool occupied){
				key = k;
				val = v;
				isOccupied = occupied;
				left = right = -1;
				height = 0;
			}

			//Default constructor, used to create nullItems
			item(){
				key = -1.0;
				val = 0;
				isOccupied = false;
				left = right = -1;
				height = 0;
			}

			//Comparison operators
			bool compareKeys(double k){
				return key == k;
			}
			bool operator==(const item<double, VALTYPE>& rhs) const { return (key == rhs.key); }
			bool operator<(const item<double, VALTYPE>& rhs) const { return key < rhs.key; }
			bool operator!=(const item<double, VALTYPE>& rhs) const { return (key != rhs.key); }
		};

		template<typename VALTYPE>
		struct item<char*, VALTYPE> {
			char* key;
			VALTYPE val;
			bool isOccupied;
			int left;
			int right;
			int height;
			//Standard constructor, used to insert actual items
			item(char* k, VALTYPE v, bool occupied){
				key = k;
				val = v;
				isOccupied = occupied;
				left = right = -1;
				height = 0;
			}

			//Default constructor, used to create nullItems
			item(){
				key = "0";
				val = 0;
				isOccupied = false;
				left = right = -1;
				height = 0;
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
			bool lessThan(char* k){
				if (key == "" || k == "") return false;
				char* tmp = key;
				char* tmp2 = k;
				while (*tmp && *tmp2){
					if (*tmp++ >= *tmp2++)
						return false;
				}

				return true;
			}

			bool operator==(const item<char*, VALTYPE>& rhs) const { return (compareKeys(rhs.key)); }
			bool operator<(const item<char*, VALTYPE>& rhs) const { return lessThan(rhs.key); }
			bool operator!=(const item<char*, VALTYPE>& rhs) const { return (!compareKeys(rhs.key)); }
		};

		template<typename VALTYPE>
		struct item<std::string, VALTYPE> {
			std::string key;
			VALTYPE val;
			bool isOccupied;
			int left;
			int right;
			int height;
			//Standard constructor, used to insert actual items
			item(std::string k, VALTYPE v, bool occupied){
				key = k;
				val = v;
				isOccupied = occupied;
				left = right = -1;
				height = 0;
			}

			//Default constructor, used to create nullItems
			item(){
				key = "";
				val = 0;
				isOccupied = false;
				left = right = -1;
				height = 0;
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

			bool lessThan(std::string k){
				if (key == "" || k == "") return false;
				int i, j;
				for (i = j = 0; i != key.size() && j != k.size(); ++i, ++j){
					if (key[i] >= k[j])
						return false;
				}
				return true;
			}
			bool operator==(const item<std::string, VALTYPE>& rhs) const { return compareKeys(rhs.key); }
			bool operator<(const item<std::string, VALTYPE>& rhs) const { return lessThan(rhs.key); }
			bool operator!=(const item<std::string, VALTYPE>& rhs) const { return !compareKeys(rhs.key); }
		};

		//---------------------------------------------------------------------
		//Insertion methods & helpers
		//---------------------------------------------------------------------

		int getNode(KEY k, VALUE v){
			if (free != -1){
				int index = free;
				free = itemArray[free]->left;
				itemArray[index] = new item<KEY, VALUE>(k, v, true);

				m_Size++;
				return index;
			}
			else return -1;
		}

		void rRotate(int& root){
			item<KEY, VALUE>* tmpRoot = itemArray[root];
			int ogLeft = tmpRoot->left;
			tmpRoot->left = itemArray[ogLeft]->right;
			itemArray[ogLeft]->right = root;

			root = ogLeft; //to be made the (sub)root outside the method
		}

		void lRotate(int& root){
			item<KEY, VALUE>* tmpRoot = itemArray[root];
			int ogRight = tmpRoot->right;
			tmpRoot->right = itemArray[ogRight]->left;
			itemArray[ogRight]->left = root;

			root = ogRight; //to be made the (sub)root outside the method
		}


		//NOTE: Had to use the reverse of the standard convention for key comparison because the key comparison method was stored
		//	within the item class is it had to be the lhs of the operand
		void insert_at_root(int& root, KEY k, VALUE v, int& nodesVisited){
			
			nodesVisited++;
			
			if (root == -1){ //base case
				root = getNode(k, v);
				if (root == -1) nodesVisited *= -1;
				return;
			}

			if (itemArray[root]->key == k){ //same key case
				itemArray[root]->val = v;
				return;
			}

			if (itemArray[root]->key < k) { //recursive case #1
				insert_at_root(itemArray[root]->right, k, v, nodesVisited);
				lRotate(root);
			}
			else { //recursive case #2
				insert_at_root(itemArray[root]->left, k, v, nodesVisited);
				rRotate(root);
			}
		}


		//NOTE: Had to use the reverse of the standard convention for key comparison because the key comparison method was stored
		//	within the item class is it had to be the lhs of the operand
		void insert_at_leaf(int& root, KEY k, VALUE v, int& nodesVisited){
			nodesVisited++;

			if (root == -1){ //first insertion case
				root = getNode(k, v);
				if (root == -1) nodesVisited *= -1;
				return;
			}

			if (itemArray[root]->key == k){ //same key case
				itemArray[root]->val = v;
				return;
			}

			if (itemArray[root]->key < k){
				if (itemArray[root]->right != -1)
					insert_at_leaf(itemArray[root]->right, k, v, nodesVisited);
				else {
					itemArray[root]->right = getNode(k, v);
					if (itemArray[root]->right == -1) nodesVisited *= -1;
					return;
				}
			}
			else {
				if (itemArray[root]->left != -1)
					insert_at_leaf(itemArray[root]->left, k, v, nodesVisited);
				else {
					itemArray[root]->left = getNode(k, v);
					if (itemArray[root]->left == -1) nodesVisited *= -1;
					return;
				}
			}

		}


		//---------------------------------------------------------------------
		//Removal methods & helpers
		//---------------------------------------------------------------------

		void returnNode(int root){
			delete itemArray[root];

			itemArray[root] = new item<KEY, VALUE>();
			itemArray[root]->left = free;
			free = root;
			m_Size--;
		}
		int childCount(int root){
			if (root == -1) return -1;
			int count = 0;
			if (itemArray[root]->left != -1) count++;
			if (itemArray[root]->right != -1) count++;

			return count;
		}

		//NOTE: Had to use the reverse of the standard convention for key comparison because the key comparison method was stored
		//	within the item class is it had to be the lhs of the operand
		void remove(int& root, KEY k, VALUE& v, int& nodesVisited){
			nodesVisited++;
			
			//Key not found
			if (root == -1){ 
				nodesVisited *= -1;
				return;
			}
			
			//Key found
			if (itemArray[root]->key == k){
				v = itemArray[root]->val; //Return parameter

				int count = childCount(root);
				switch (count) {
				case 0: //Case 1: No Children

					returnNode(root);
					root = -1;
					break;
				case 1: //Case 2: One Child

							//Case 2A: Left Child
					if (itemArray[root]->left != -1){
						int tmpRoot = root;
						root = itemArray[root]->left;
						returnNode(tmpRoot);
					}

							//Case 2B: Right Child
					else {
						int tmpRoot = root;
						root = itemArray[root]->right;
						returnNode(tmpRoot);
					}
					break;
				case 2: //Case 3: Two Children

					//traverse inorder until min index in right subtree is reached
					int minIndex = itemArray[root]->right;
					while (itemArray[minIndex]->left != -1){
						minIndex = itemArray[minIndex]->left;
						nodesVisited++;
					}

					//Correct node to switch with current root found
					KEY newK = itemArray[minIndex]->key;
					VALUE newV = itemArray[minIndex]->val;
					itemArray[root]->key = newK;
					itemArray[root]->val = newV;

					//this will digress into a case in which zero or one children exist, Case 1 or 2 will apply
					remove(itemArray[root]->right, newK, newV, nodesVisited);
					break;
				} //end switch
				return;
			}//end if

			//Traversal 
			else if (itemArray[root]->key < k){ //go right
				remove(itemArray[root]->right, k, v, nodesVisited);
				return;
			}
			else { //go left
				remove(itemArray[root]->left, k, v, nodesVisited);
				return;
			}
		}

		//---------------------------------------------------------------------
		//Search methods & helpers
		//---------------------------------------------------------------------

		//NOTE: Had to use the reverse of the standard convention for key comparison because the key comparison method was stored
		//	within the item class is it had to be the lhs of the operand
		void search(int& root, KEY k, VALUE& v, int& nodesVisited){
			nodesVisited++;

			//Key not found
			if (root == -1){
				nodesVisited *= -1;
				return;
			}
			
			//Key found
			if (itemArray[root]->key == k){
				v = itemArray[root]->val;
			}

			//Traversal
			else if (itemArray[root]->key < k) { //go right
				search(itemArray[root]->right, k, v, nodesVisited);
			}
			else { //go left
				search(itemArray[root]->left, k, v, nodesVisited);
			}

			return;
		}

		//---------------------------------------------------------------------
		//Cluster & Remove Random methods & helpers
		//---------------------------------------------------------------------
		
		void setHeights(int node){//preorder traversal
			if (node == -1)
				return;
			int mLeft = itemArray[node]->left;
			int mRight = itemArray[node]->right;

			if (mLeft != -1) itemArray[mLeft]->height = itemArray[node]->height + 1;
			if (mRight != -1) itemArray[mRight]->height = itemArray[node]->height + 1;

			setHeights(mLeft);
			setHeights(mRight);
		}
		
		int inorder(int root, int& i){
			/*
			NOTE TO PROFESSOR: Your instructions asked us to perform an inorder traversal of the tree to 
			find the correct element to remove, but also asked us to begin with the root as node 1. 
			This does not make any sense as by nature in order traversals begin with the left-most element
			*/

			//Left
			if (itemArray[root]->left != -1) inorder(itemArray[root]->left, i);
			
			//Self
			if (i == 0)
				return root;
			else --i;
			
			//Right
			if (itemArray[root]->right != -1) inorder(itemArray[root]->right, i);


		}


		int root;
		int free;
		item<KEY, VALUE>** itemArray;

		size_t m_Capacity;
		size_t m_Size;

	//---------------------------------------------------------------------
	//Public members and functions
	//---------------------------------------------------------------------
	public:
		typedef size_t size_t;

		//---------------------------------------------------------------------
		//Constructors and destructors
		//---------------------------------------------------------------------
		RBST(int capacity) : m_Capacity(capacity){
			itemArray = new item<KEY, VALUE>*[m_Capacity];

			for (size_t i = 0; i != m_Capacity; ++i){
				itemArray[i] = new item<KEY, VALUE>();
				itemArray[i]->left = i + 1;
			}
			itemArray[m_Capacity - 1]->left = -1;

			root = -1;
			free = 0;

			m_Size = 0;
		}

		RBST(const RBST& src) : m_Capacity(rhs.m_Capacity) {
			if (&src == this) // check for self-assignment
				return *this;     // do nothing
			for (size_t i = 0; i != m_Capacity; ++i)
				delete this->itemArray[i];
			delete this->itemArray;

			this->itemArray = src.itemArray;
			this->free = src.free;
			this->m_Size = src.m_Size;
			this->root = src.root;

			
		}

		~RBST(){
			for (size_t i = 0; i != m_Capacity; ++i)
				delete this->itemArray[i];
			delete this->itemArray;
		}

		RBST& operator=(const RBST& src){
			if (&src == this) // check for self-assignment
				return *this;     // do nothing

			for (size_t i = 0; i != m_Capacity; ++i)
				delete this->itemArray[i];
			delete this->itemArray;

			this->itemArray = src.itemArray;
			this->free = src.free;
			this->m_Size = src.m_Size;
			this->root = src.root;

			return *this;
		}

		//---------------------------------------------------------------------
		//Public functions
		//---------------------------------------------------------------------
		int insert(KEY key, VALUE value){
			int visited = 0;
			
			if (m_Size == 0){ //first insertion case
				insert_at_root(this->root, key, value, visited);
				return visited;
			}

			std::srand(time(0));
			int rNum = rand() % m_Size;
			if (rNum == 0)
				insert_at_root(this->root, key, value, visited);
			else 
				insert_at_leaf(this->root, key, value, visited);

			return visited;
		}

		int remove(KEY key, VALUE& value){
			int visited = 0;
			remove(this->root, key, value, visited);
			return visited;
		}

		int search(KEY key, VALUE& value){
			int visited = 0;
			search(this->root, key, value, visited);
			return visited;
		}

		void clear(){
			for (int i = 0; i != m_Capacity; i++){
				returnNode(i);
			}
			this->root = -1;
		}

		bool is_empty(){
			return (m_Size == 0);
		}

		std::size_t capacity(){ return m_Capacity; }
		std::size_t size(){ return m_Size; }

		double load(){
			return ((double)m_Size) / m_Capacity;
		}

		std::ostream& print(std::ostream& out) const {
			out << "[ ";

			std::queue<int> q;
			q.push(root);
			while (!q.empty()){
				int i = q.front();
				q.pop();
				if (i != -1) out << itemArray[i]->key;
				else out << "-";
				if (i != m_Capacity-1) out << ", ";

				if (i != -1) q.push(itemArray[i]->left);
				if (i != -1) q.push(itemArray[i]->right);
			}
			out << "]" << std::endl;

			return out;
		}
		
		int** cluster_distribution(){
			setHeights(this->root);
			int maxNodeSize = 0;
			for (size_t i = 0; i != m_Capacity; ++i){
				if (childCount(i) == 0){
					int nHeight = itemArray[i]->height;
					maxNodeSize = (maxNodeSize > nHeight) ? maxNodeSize : nHeight;
				}
			}

			int** tmpList = new int*[maxNodeSize];
			for (size_t k = 0; k != maxNodeSize; ++k){
				tmpList[k] = new int[2];
				tmpList[k][0] = k + 1;
				tmpList[k][1] = 0;
			}

			for (size_t l = 0; l != m_Capacity; ++l){
				if (childCount(l) == 0){
					int nHeight = itemArray[l]->height;
					tmpList[nHeight - 1][1]++;
				}
			}

			int totalNodeHeights = 0;
			for (size_t m = 0; m != maxNodeSize; ++m){
				if (tmpList[m][1] != 0) totalNodeHeights++;
			}

			int** returnList = new int*[totalNodeHeights];
			int n = 0;
			for (size_t p = 0; p != maxNodeSize; ++p){
				if (tmpList[p][1] != 0){
					returnList[n] = new int[2];
					returnList[n][0] = tmpList[p][0];
					returnList[n][1] = tmpList[p][1];
					++n;
				}
			}
			
			delete tmpList;
			return returnList;
		}

		KEY remove_random(){
			std::srand(time(0));
			int index = rand() % m_Size;
			while (index == 0)
				index = rand() % m_Size;
			int nodeToRemove = inorder(this->root, index);

			KEY toReturn;
			if (nodeToRemove != -1) toReturn = itemArray[nodeToRemove]->key;
			returnNode(nodeToRemove);

			return toReturn;
		}

	};//end class RBST
}// end namespace cop3530
#endif // _RBST_H_