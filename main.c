#include <stdio.h> // file
#include <stdlib.h> // atoi
#include <string.h> // strtok

#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#define MAXCHAR 1024	// How many char to read at a time
#define TABLESIZE 10


int hard_disk[500]; // disk

unsigned mask_read_1 = 0xFF000000; // file name
unsigned mask_read_2 = 0x00FF0000; // start block
unsigned mask_read_3 = 0x0000FF00; // end block

unsigned mask_clear_1 = 0x00FFFFFF; // file name
unsigned mask_clear_2 = 0xFF00FFFF; // start block
unsigned mask_clear_3 = 0xFFFF00FF; // end block


/// <summary>
/// Hash Table 
/// 
/// Key is the file name
/// Value is the address in the disk storing the file information
/// </summary>

struct MapItem
{
	int key; 
	int value; 
};

struct MapItem* hash_table[TABLESIZE];
struct MapItem* dummy;



int hash_code(int key) {
	return key % TABLESIZE;
}

struct MapItem* hash_search(int key)
{
	int index = hashCode(key);

	//move in array until an empty 
	while (hash_table[index] != NULL) 
	{

		if (hash_table[index]->key == key)
			return hash_table[index];

		//go to next cell
		++index;

		//wrap around the table
		index %= TABLESIZE;
	}

	return NULL;
}

void hash_insert(int key, int value)
{
	struct MapItem* item = (struct MapItem*)malloc(sizeof(struct MapItem));

	item->key = key;
	item->value = value;

	//get the hash 
	int index = hash_code(key);

	//move in array until an empty or deleted cell
	while (hash_table[index] != NULL && hash_table[index]->key != -1) {
		//go to next cell
		++index;

		//wrap around the table
		index %= TABLESIZE;
	}

	hash_table[index] = item;
}

void hash_delete(int key)
{
	int index = hashCode(key);

	//move in array until an empty
	while (hash_table[index] != NULL)
	{

		if (hash_table[index]->key == key)
		{
			struct DataItem* temp = hash_table[index];

			//assign a dummy item at deleted position
			hash_table[index] = dummy;
			return temp;
		}

		//go to next cell
		++index;

		//wrap around the table
		index %= TABLESIZE;
	}

	return NULL;
}

// END OF HASH TABLE
// END OF HASH TABLE
// END OF HASH TABLE



// Contiguous

void contiguous_allocation(int files[], int content[], int startBlock, int filesLength) 
{
	int flag = 0, j, k, ch;
	int start = startBlock;
	int total_size = startBlock + filesLength;
	printf("Enter the starting block and the length of the files: ");
	//scanf("%d%d", &startBlock, &len);
	for (j = start; j < total_size; ++j) 
	{
		if (files[j] == 0)
			flag++;
	}
	if (filesLength == flag) {
		for (k = startBlock; k < total_size; ++k) 
		{
			if (files[k] == 0) {
				files[k] = 1;
				printf("%d\t%d\n", k, files[k]);
			}
		}
		if (k != (total_size - 1))
			printf("The file is allocated to the disk\n");
	}
	else
		printf("The file is not allocated to the disk\n");
	//printf("Do you want to enter more files?\n");
	//printf("Press 1 for YES, 0 for NO: ");
	//scanf("%d", &ch);
	//if (ch == 1)
	//	contiguous_allocation(files);
	//else
	//	exit(0);
	return;
}


// Linked

// Indexed



void disk_add(int fileName, int startBlock, int endBlock)
{
	int i;
	int temp = 0; 

	// check if there's already 10 files
	for (i = 0; i < 10; ++i)
	{
		if (hard_disk[i] == 0)
		{
			//add new entry

			// check if file already has an entry
			if (hash_search(fileName) == NULL) // if new entry
			{
				insert(fileName, hard_disk + i); // insert into table
			}
			// update start and end block
			// Bit shifting to store file name, start and end block
			temp = fileName;
			temp = temp << 8;
			temp += startBlock;
			temp = temp << 8;
			temp += endBlock;
			temp = temp << 8;

			hard_disk[i] = temp;
			// TEST
			//printf("binary: %d \n", temp);
			//a = test & mask_1;
			//a = a >> 24;
			//printf("test a: %d \n", a);

			//a = test & mask_2;
			//a = a >> 16;
			//printf("test a: %d \n", a);

			//a = test & mask_3;
			//a = a >> 8;
			//printf("test a: %d \n", a);
			// TEST
			return NULL;
		}
		else
		{
			exit(-2);
		}
	}
}

int main(int argc, char** argv) {
	/* Make your program do whatever you want */

	char row[MAXCHAR];

	char* token;
	char* command;
	int insertion_algo = 0; // 1 contiguous, 2 linked, 3 index
	int file_name;
	int file_content[100];
	int i = 0;

	//tesst
	int test = 0;
	int a = 0;
	//test

	// initializing disk
	for (int a = 0; a < 500; ++a)
	{
		hard_disk[a] = 0;
	}

	// initializing dummy entry for hash table
	dummy = (struct MapItem*)malloc(sizeof(struct MapItem));
	dummy->value = -1;
	dummy->key = -1;

	// choosing insertion algo for user
	printf("Choose your insertion algorithm. 1 - Contiguous, 2 - Linked, 3 - Index \n");
	scanf("%d", &insertion_algo);

	if (insertion_algo < 1 || insertion_algo > 3)
	{
		printf("Invalid algorithm selected. \n");
		exit(-1);
	}


	// reading csv
	FILE* fstream = fopen("input.csv", "r");

	while (!feof(fstream))
	{
		fgets(row, MAXCHAR, fstream);

		token = strtok(row, ",");
		command = token;
		token = strtok(NULL, ",");
		file_name = atoi(token);
		printf("command: %s \n", command);
		printf("file name: %d \n", file_name);

		token = strtok(NULL, ",");
		while (token != NULL)
		{
			if (strcmp(token, "\n") == 0 || strlen(token) == 0)
			{
				break;
			}
			file_content[i] = atoi(token);
			printf("file content at %d: %d \n", i, file_content[i]);
			++i;
			token = strtok(NULL, ",");
		}

		

		if (strcmp(command, "read") == 0)
		{

		}
		else if (strcmp(command, "delete") == 0)
		{

		}
		else if (insertion_algo == 1)
		{
			//find start block of the file
			
			//contiguous_allocation(hard_disk, file_content, startBlock, i)
		}
		else if (insertion_algo == 2)
		{

		}
		else
		{

		}

		i = 0;
	}






	return -1;

}