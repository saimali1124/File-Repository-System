#include <iostream>
#include <vector>
#include <List>
#include <fstream>
using namespace std;

// template for generic type
template<class priorityKeyType>
class PriorityQueue_Node //node class
{
public:
	//data members of node class
	priorityKeyType key;
	int User_ID;
	char operation_Type; //e for empty, R for read, W for write

	PriorityQueue_Node() //default constructor of node class
	{
		this->key = NULL;
		this->User_ID = -1;
		this->operation_Type = 'e';
	}
};

template<class priorityKeyType>
class PriorityQueue //queue class
{
private:

	void Heapify(int i) //function that checks if maxheap is maintained for a specific node 
	{
		if (i >= 0 && MaxHeap.empty() == 0)
		{
			int largest = i;
			int left = 2 * i + 1; //following the formula of index when first index has the first key
			int right = 2 * i + 2;

			if ((left <= MaxHeap.size() - 1) && (MaxHeap[largest].key < MaxHeap[left].key))
				largest = left;
			if (right <= MaxHeap.size() - 1 && (MaxHeap[largest].key < MaxHeap[right].key))
				largest = right;

			if (largest != i)
			{
				PriorityQueue_Node<priorityKeyType> temp = MaxHeap[i];
				MaxHeap[i] = MaxHeap[largest];
				MaxHeap[largest] = temp;
				Heapify(largest); //recursively calling function
			}
		}
	}

public:
	//data members of queue class
	vector< PriorityQueue_Node<priorityKeyType>> MaxHeap; //array that forms the priority queue
	priorityKeyType min_Key; //stores minimum value of the queue

	PriorityQueue() //default constructor of the queue class
	{
		min_Key = 0;
	}

	PriorityQueue(const PriorityQueue& obj) //copy constructor of queue class
	{
		min_Key = obj.min_Key;
	}

	void BuildQueue() //fully sorts an unsorted array into a priority queue
	{
		int i = (MaxHeap.size() - 1) / 2;

		while (i >= 1)
		{
			Heapify(i);
			i--;
		}
	}

	PriorityQueue_Node<priorityKeyType> ExtractMax() //function that removes highest key valued node then sorts the array
	{
		if (MaxHeap.empty() == 0)
		{
			PriorityQueue_Node<priorityKeyType> temp = MaxHeap[MaxHeap.size() - 1];
			MaxHeap[MaxHeap.size() - 1] = MaxHeap[0];
			MaxHeap[0] = temp;
			temp = MaxHeap[MaxHeap.size() - 1];
			MaxHeap.pop_back();
			Heapify(0);
			return temp;
		}
	}

	priorityKeyType FindMax() //returns the max value of the queue(its first index key)
	{
		if (this->MaxHeap.size() > 0)
		{
			return MaxHeap[0].key;
		}
		else
		{
			return 99;
		}
	}

	priorityKeyType FindMin() //returns the min value of the queue
	{
		if (MaxHeap.size() > 0)
		{
			return min_Key;
		}
		else
		{
			return 1;
		}
	}

	void Insert(priorityKeyType key, int id, char operation) //inserts a node into the queue and sorts it to a priority queue
	{
		if (MaxHeap.size() == 0)
			this->min_Key = key;
		if (key <= this->min_Key)
			this->min_Key = key;

		PriorityQueue_Node<priorityKeyType > temp;
		temp.key = key;
		temp.User_ID = id;
		temp.operation_Type = operation;
		this->MaxHeap.push_back(temp);
		this->BuildQueue();
		this->Heapify(0); //call heapify
	}

	int Size() //returns size of the queue
	{
		return this->MaxHeap.size();
	}

	bool Empty() //returns true if the queue is empty else returns false
	{
		return this->MaxHeap.empty();
	}

	~PriorityQueue() //destructor of queue class
	{
		this->MaxHeap.clear();
	}
};

// template for generic type
template <typename priorityKeyType>
class HashItem { //hashitem class 
public:
	//data members of hashitem class
	int file_ID; //the file ID of a file
	PriorityQueue<priorityKeyType>* waiting_queue; //holds track of user IDs that are waiting in queue for access
	PriorityQueue_Node<priorityKeyType>* access; //holds track of user IDs that currently have access to a certain file

	HashItem() //default constructor of hashitem class
	{
		file_ID = 0;
		waiting_queue = nullptr;
		access = nullptr;
	}

	void print() //print function for hashitem class
	{
		cout << this->file_ID << "	";
	}
};

// template for generic type
template<typename priorityKeyType>
class HashTable //hashtable class
{
private:
	//data members of hashtable class
	list<HashItem<priorityKeyType>>* Hash_List; //array which has linked list on each index
	int num_bucks; //the number of indexes of the array

	int calcHash(int key) //function that calculates index number where a certain file needs to be placed
	{
		return key % this->num_bucks;
	}

	bool searchInst(int key) //returns true if a key value is found in a certain queue
	{
		int hashitem = this->calcHash(key);
		if (hashitem >= 0) {
			for (auto it = Hash_List[hashitem].begin(); it != Hash_List[hashitem].end(); ++it)
			{
				if (it->file_ID == key)
					return true;
			}
		}
		return false;
	}

public:

	HashTable() //default constructor
	{
		this->num_bucks = 10;
		Hash_List = new list<HashItem<priorityKeyType>>[this->num_bucks];
	}

	HashTable(int n) //parametrized constructor 
	{
		this->num_bucks = n;
		this->Hash_List = new list<HashItem<priorityKeyType>>[this->num_bucks];
	}

	bool insertFile(int key) //insert file in the linked list in a certain index of array
	{
		int hashitem = this->calcHash(key);
		bool search = this->searchInst(key);
		if (search == 0)
		{
			HashItem<priorityKeyType>* temp = new HashItem<priorityKeyType>;
			temp->file_ID = key;
			Hash_List[hashitem].push_back(*temp); //pushes file in a linked list in array
			return true;
		}
		else
		{
			cout << "File already Exists.\n";
		}
		return false;
	}

	//----------------------------------------------------function to load data from file----------------------------------------------------//
	void fileInput()
	{
		//variables required to load data from file
		int fid = 0, uid = 0, priority = 0;
		char f_acc = NULL;
		//variable for hash index
		int hashitem = 0;
		//flag variables
		bool insert = false, exists = false;

		ifstream fin;
		fin.open("data.txt");

		if (!fin.is_open())
		{
			cout << "Unable to open file.";
		}
		else
		{
			while (!fin.eof())
			{
				//input from file: File ID, User ID, Priority, Operation
				fin >> fid;
				fin.ignore();
				fin >> uid;
				fin.ignore();
				fin >> priority;
				fin.ignore();
				fin >> f_acc;
				fin.ignore();

				//calculate hash index
				hashitem = this->calcHash(fid);
				//check if the values meet boundary conditions
				if ((fid >= 5000 && fid <= 99999) && (uid >= 0 && uid <= 10000) && (priority >= 0 && priority <= 100) && (f_acc == 'R' || f_acc == 'W'))
				{
					//if boundary conditions are met, flag is set
					insert = true;
					//search that if file already exists then flag is set so that file is not added multiple times
					for (auto it = Hash_List[hashitem].begin(); it != Hash_List[hashitem].end(); ++it)
					{
						if (it->file_ID == fid)
						{
							exists = true;
							break;
						}
					}
				}
				else
				{
					insert = false;
				}
				//if file is not added before than add the file in repository and give access to users
				if (insert && !exists)
				{
					this->insertFile(fid);
					this->RequestFileAccess(uid, fid, f_acc, priority);
					exists = false;
				}
				else if (!insert)
				{
					//if boundary conditions are not met then output some error
					cout << "Either File ID or User ID or Priority are out of declared range or the requested operation is false.\n";
				}
				else if (exists)
				{
					//if file already exists then only give access to users
					this->RequestFileAccess(uid, fid, f_acc, priority);
					exists = false;
				}
			}
			cout << "Data load completed.\n";
		}
	}


	void RequestFileAccess(int user_ID, int file_ID, char operation, priorityKeyType priority) //to request access for a certain file
	{
		bool exists = false;
		int hashitem = this->calcHash(file_ID);

		for (auto it = Hash_List[hashitem].begin(); it != Hash_List[hashitem].end(); ++it)
		{
			if (it->file_ID == file_ID)
			{
				exists = true; //make exists variable true if file exists
				break;
			}
		}

		if (!exists) 
		{
			cout << "Error: File ID does not exist" << endl;
		}
		else //if file does exist
		{
			for (auto it = Hash_List[hashitem].begin(); it != Hash_List[hashitem].end(); ++it)
			{
				if (it->file_ID == file_ID)
				{
					if (it->access == nullptr) //if no one has access to a certain file
					{
						it->access = new PriorityQueue_Node<priorityKeyType>[10];

						//initializing each key index to -1
						for (int i = 1; i < 10; i++)
						{
							it->access[i].key = -1;
						}
						//give access to a user
						it->access[0].key = priority;
						it->access[0].User_ID = user_ID;
						it->access[0].operation_Type = operation;
					}
					else if (it->access[0].operation_Type == 'R' && operation == 'R') //if access is with read users then give access to other read users as well
					{
						for (int i = 0; i < 10; i++)
						{
							if (it->access[i].User_ID == -1)
							{
								it->access[i].key = priority;
								it->access[i].User_ID = user_ID;
								it->access[i].operation_Type = operation;
								break;
							}
						}
					}
					else if (it->waiting_queue == nullptr) //if no one is in waiting queue then start filling it with users
					{
						it->waiting_queue = new PriorityQueue<priorityKeyType>;
						it->waiting_queue->Insert(priority, user_ID, operation);
					}
					else
					{
						it->waiting_queue->Insert(priority, user_ID, operation); //inserting users in waiting queue
					}
					break;
				}
			}
		}
	}

	//----------------------------------------------------------function to release file----------------------------------------------------------//
	void ReleaseFile(int user_ID, int file_ID)
	{
		//flag variable to be used in certain operations
		bool exists = false;

		//hash index being calculated
		int hashitem = this->calcHash(file_ID);

		//checking that whether the file exists or not
		for (auto it = Hash_List[hashitem].begin(); it != Hash_List[hashitem].end(); ++it)
		{
			if (it->file_ID == file_ID)
			{
				//if the file exists the flag is set
				exists = true;
				break;
			}
		}

		if (!exists)
		{
			cout << "Error: File ID does not exist" << endl; //output error if file doesn't exists
		}
		else
		{
			for (auto it = Hash_List[hashitem].begin(); it != Hash_List[hashitem].end(); ++it)
			{
				if (it->file_ID == file_ID)
				{
					//if the access array of file is empty then it means that no user is currently accessing the file
					if (it->access == nullptr)
					{
						break;
					}
					else
					{
						//finding the user in access array
						for (int j = 0; j != 10; j++)
						{
							//if the user exists then swapping the user id with '-1'
							if (it->access[j].User_ID == user_ID)
							{
								it->access[j].User_ID = -1;
								it->access[j].operation_Type = NULL;
								it->access[j].key = NULL;
								exists = false;
								j = 0; //checking the access array from start that if any other user is currently accessing the file
							}

							else if (it->access[j].User_ID != -1)
							{
								//if user exists then flag is set
								exists = true;
							}
						}

						//if no user is currently accessing the file then delete the access array
						if (!exists)
						{
							delete[] it->access;
							it->access = nullptr;
						}

						//well if the no user if accessing the file then give access to the next user in waiting queue
						if (!exists && it->waiting_queue != nullptr)
						{
							if (it->waiting_queue->Size() > 0) //check whether anyone is in the waiting queue or not
							{
								it->access = new PriorityQueue_Node<priorityKeyType>[20];
								it->access[0] = it->waiting_queue->ExtractMax();

								//if the first user in waiting queue
								//then give access to all users with read operation until a user with write operation comes
								if (it->access[0].operation_Type == 'R')
								{
									int k = 0, a = 1;
									for (int k = 0, a = 1; k <= it->waiting_queue->Size(); k++) //iterator through whole waiting queue
									{
										if (it->waiting_queue->MaxHeap[0].operation_Type == 'R')
										{
											it->access[a] = it->waiting_queue->ExtractMax();
											a++;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//function that displays the files and users associated with them
	void print()
	{
		for (int i = 0; i < num_bucks; i++)
		{
			for (auto it = Hash_List[i].begin(); it != Hash_List[i].end(); ++it)
			{
				cout << "H" << i << " -> File ";
				cout << it->file_ID << " ... ";
				cout << "Access granted to ";
				if (it->access == nullptr) //if no one has access
				{
					cout << "none" << endl << "Next none" << endl << "Waiting none" << endl;
				}
				else //if access is given to atleast one user
				{
					for (int j = 0; j < 10; j++)
					{
						if (it->access[j].User_ID != -1)
							cout << "User " << it->access[j].User_ID << " , ";
					}
					char RoW; //read or write
					for (int j = 0; j < 10; j++)
					{
						if (it->access[j].User_ID != -1)
							RoW = it->access[j].operation_Type;
					}
					if (RoW == 'R')
					{
						cout << "read" << endl;
					}
					else if (RoW == 'W')
					{
						cout << "write" << endl;
					}

					cout << "Next "; //for displaying next user in waiting queue
					if (it->waiting_queue == nullptr || it->waiting_queue->Size() <= 0)
					{
						cout << "none" << endl;
					}
					else if (it->waiting_queue->Size() > 0)
					{
						cout << "User " << it->waiting_queue->MaxHeap[0].User_ID << " , ";
						if (it->waiting_queue->MaxHeap[0].operation_Type == 'R')
						{
							cout << "read" << endl;
						}
						else if (it->waiting_queue->MaxHeap[0].operation_Type == 'W')
						{
							cout << "write" << endl;
						}
					}

					cout << "Waiting "; //for displaying number of users who are in waiting queue
					if (it->waiting_queue == nullptr || it->waiting_queue->Size() <= 0)
					{
						cout << "none" << endl;
					}
					else
					{
						cout << it->waiting_queue->Size() << " users" << endl;
					}
				}
			}
		}
	}

	void hashDisplay() //function to display files 
	{
		for (int i = 0; i < num_bucks; i++)
		{
			for (auto it = Hash_List[i].begin(); it != Hash_List[i].end(); ++it)
			{
				cout << it->file_ID << "	->	";
			}
			cout << endl;
		}
	}

	void deleteFile(int key)
	{
		int hashitem = this->calcHash(key);
		bool done = false, exists = false; //flag variables

		if (hashitem >= 0) {
			//iterate through the repository to find the file
			for (auto it = Hash_List[hashitem].begin(); it != Hash_List[hashitem].end(); ++it)
			{
				if (it->file_ID == key) //if file is found then check that the file currently being is accessed or not
				{
					if (it->access != nullptr)
					{
						for (int j = 0; j != 10; j++)
						{
							if (it->access[j].User_ID != -1)
							{
								//if file is currently accessed some user than flag is set
								exists = true;
							}
						}
					}
					if (!exists) //if file is not accessed then we will delete it
					{
						Hash_List[hashitem].erase(it);
						cout << "File deleted.\n";
						done = true;
						break;
					}
					else //else output some error
					{
						cout << "File is accessed by some user/s.\n";
						done = true;
					}
				}
			}
			if (!done)
			{
				//if file is not found then ouput error
				cout << "File does not exists.\n";
			}
		}
	}

	~HashTable() //destructor of HashTable class
	{
		for (int i = 0; i < 10; i++)
		{
			Hash_List[i].clear();
		}
		delete[] Hash_List;
	}

	//--------------------------------------------------------------MENU--------------------------------------------------------------//
	void menu()
	{
		//give user some options that what functions can be performed
		int choice;
		cout << "Enter the number corresponding to the task that needs to be performed : " << endl;
		cout << " 1. Print Hash Table" << endl;
		cout << " 2. Load Data" << endl;
		cout << " 3. Insert File" << endl;
		cout << " 4. Delete a File" << endl;
		cout << " 5. Request access to a File" << endl;
		cout << " 6. Release the file" << endl;
		cout << " 7. EXIT" << endl << endl;
		cin >> choice;
		while (choice > 0 && choice < 8) //some conditional statements to check which operation user is trying to perform
		{
			//decide on the chosen operation
			if (choice == 1) //to print 
			{
				cout << "---------------------------------------------------------------------" << endl;
				cout << endl;
				print();
			}
			else if (choice == 2) //to load a file from txt file
			{
				cout << "---------------------------------------------------------------------" << endl;
				this->fileInput();
			}
			else if (choice == 3) //to insert a file
			{
				cout << "---------------------------------------------------------------------" << endl;
				int fil;
				cout << " Enter File ID" << endl;
				cin >> fil;
				insertFile(fil);
			}
			else if (choice == 4) //to delete a file
			{
				cout << "---------------------------------------------------------------------" << endl;
				int fid;
				cout << " Enter File ID" << endl;
				cin >> fid;
				deleteFile(fid);
			}
			else if (choice == 5) //to request for file access
			{
				cout << "---------------------------------------------------------------------" << endl;
				int fid, uid;
				char op;
				cout << " Enter File ID" << endl;
				cin >> fid;
				//checking boundary conditions on input values, if not met output some error
				while (fid < 5000 && fid > 99999)
				{
					cout << "Incorrect input. Enter value between 5000 and 99999" << endl;
					cin >> fid;
				}
				cout << " Enter User ID" << endl;
				cin >> uid;
				while (uid < 0 && fid > 10000)
				{
					cout << "Incorrect input. Enter value between 0 and 10000" << endl;
					cin >> uid;
				}
				cout << "Enter 'r' for read / 'w' for write" << endl;
				cin >> op;
				while (op != 'r' && op != 'w')
				{
					cout << "Incorrect input. Enter either r or w" << endl;
					cin >> op;
				}
				if (op == 'r')
				{
					op = 'R';
				}
				if (op == 'w')
				{
					op = 'W';
				}
				int choice2;
				cout << " Press 1 to enter a number for priority" << endl;
				cout << " Press 2 to set priority as highest or lowest" << endl;
				cin >> choice2;
				if (choice2 == 1) //if user wants to set priority himself
				{
					priorityKeyType number;
					cout << " Enter number : " << endl;
					cin >> number;
					while (number < 0 && number > 100)
					{
						cout << "Incorrect input. Enter value between 0 and 100" << endl;
						cin >> number;
					}
					RequestFileAccess(uid, fid, op, number);
				}
				else if (choice2 == 2) //else if he wants to just set the highest or lowest priority
				{
					int choice3;
					priorityKeyType number;

					//asking that whether user wants to set highest or lowest priority
					cout << " Enter 1 to set highest priority" << endl;
					cout << " Enter 2 to set lowest priority" << endl;
					cin >> choice3;
					int hashitem = calcHash(fid);
					auto it2 = Hash_List[hashitem].end();
					--it2;
					//we will check the highest priority and assign a priority larger than it
					if (choice3 == 1)
					{
						for (auto it = Hash_List[hashitem].begin(); it != Hash_List[hashitem].end(); ++it)
						{
							if (it->file_ID == fid)
							{
								if (it->waiting_queue != nullptr)
								{
									number = it->waiting_queue->FindMax();
									number += 1;
									RequestFileAccess(uid, fid, op, number);
									break;
								}
								else
								{
									number = 100;
									RequestFileAccess(uid, fid, op, number);
									break;
								}
							}
							if (it == it2)
							{
								cout << "File Not Found!\n";
							}
						}
					}
					//we will check the lowest priority and assign a priority smaller than it
					else if (choice3 == 2)
					{
						for (auto it = Hash_List[hashitem].begin(); it != Hash_List[hashitem].end(); ++it)
						{
							if (it->file_ID == fid)
							{
								if (it->waiting_queue != nullptr)
								{
									number = it->waiting_queue->FindMin();
									number -= 1;
									RequestFileAccess(uid, fid, op, number);
									break;
								}
								else
								{
									number = 0;
									RequestFileAccess(uid, fid, op, number);
									break;
								}
								if (it == it2)
								{
									cout << "File Not Found!\n";
								}
							}
						}
					}
					else
					{
						cout << " Error : Wrong Input" << endl;
					}
				}
				else
				{
					cout << "Error : Wrong Input" << endl;
				}
			}
			else if (choice == 6) //to release file
			{
				cout << "---------------------------------------------------------------------" << endl;
				int fid, uid;
				cout << " Enter File ID" << endl;
				cin >> fid;
				cout << " Enter User ID" << endl;
				cin >> uid;

				ReleaseFile(uid, fid);
			}
			else if (choice == 7) //to end program
			{
				cout << "---------------------------------------------------------------------" << endl;
				break;
			}
			cout << "---------------------------------------------------------------------" << endl;
			cout << "Enter the number corresponding to the task that needs to be performed : " << endl;
			cout << " 1. Print Hash Table" << endl;
			cout << " 2. Load Data" << endl;
			cout << " 3. Insert File" << endl;
			cout << " 4. Delete a File" << endl;
			cout << " 5. Request access to a File" << endl;
			cout << " 6. Release the file" << endl;
			cout << " 7. EXIT" << endl << endl;
			cin >> choice;
		}
	}
};


int main() //main of the program
{
	HashTable<int> MyHashTable;
	
	MyHashTable.menu(); //calling the main function of the HashTable class
}


