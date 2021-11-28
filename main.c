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
unsigned mask_read_4 = 0x000000FF; // file allocation algorithm

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
	int index = hash_code(key);

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
	//get the hash 
	int index = hash_code(key);

	struct MapItem* item = (struct MapItem*)malloc(sizeof(struct MapItem));
	if (item == NULL)
	{
		return NULL;
	}
	item->key = key;
	item->value = value;




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
	int index = hash_code(key);

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
void sequential_allocation(int files[], int content[], int startBlock, int filesLength)
{
	int maxBlocks = 100;
	int filledBlocks[100];
	int flag = 0, len, j, k, ch;
	int start, index, count = filesLength, entriesPerBlock = 5, i = 0, j = 0;
	int blocksNeeded = 0;
	int freeSpaceCount = 0;
	//Search thru the 100 blocks, j is index of current block
	//for (j = _startBlock; j < _endBlock; j++)
	//{
	//	//If there is free space in that block
	//	if (filledBlocks[j] == NULL)
	//	{
	//		for (k = 0; k < 5; k++)
	//		{
	//			hash_insert(j, i);
	//			//insert here!
	//		}
	//	}
	//}

	index = start * entriesPerBlock;

	blocksNeeded = count % 5;
	//just nice multiples of 5 elements 
	if (blocksNeeded == 0)
	{
		blocksNeeded /= 5;
	}
	//not nice 
	else
	{
		blocksNeeded /= 5 + 1;
	}

	if (files[index] == 0)
	{
		for (i = index; i < index + entriesPerBlock - 1; ++i)
		{
			if (files[i] == 0)
			{
				freeSpaceCount++;
				if (freeSpaceCount)
				{
					files[index] = content[j];
					freeSpaceCount = 0;
				}
			}
		}

		if (count != 0)
		{
			printf("Block full. Enter next starting block: ");
			scanf("%d", &start);
			files[index + entriesPerBlock - 1] = start;
			goto LOOP;
		}
	}
	else
	{
		printf("Block %d has already been allocated.\n", start);
	}
}


// Linked
void linked_allocation(int files[], int content[], int filesLength, int* _startBlock, int* _endBlock)
{
	int start, index, count = filesLength, entriesPerBlock = 5, i = 0, j = 0;
	printf("Enter starting block: ");
	scanf("%d", &start);
	*_startBlock = start;
	LOOP: index = start * entriesPerBlock;
	if (files[index] == 0)
	{
		for (i = index; i < index + entriesPerBlock - 1; ++i)
		{
			if (files[i] == 0)
			{
				if (count != 0)
				{
					files[i] = content[j];
					++j;
					--count;
				}
				else
				{
					files[i] = -1;
					*_endBlock = start;
					break;
				}
			}
		}

		if (count != 0)
		{
			printf("Block full. Enter next starting block: ");
			scanf("%d", &start);
			files[index + entriesPerBlock - 1] = start;
			goto LOOP;
		}
	}
	else
	{
		printf("Block %d has already been allocated.\n", start);
	}
}
// Indexed



void disk_add(int fileName, int startBlock, int endBlock, int method)
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
				hash_insert(fileName, i); // insert into table
			}
			// update start and end block
			// Bit shifting to store file name, start and end block
			temp = fileName;
			temp = temp << 8;
			temp += startBlock;
			temp = temp << 8;
			temp += endBlock;
			temp = temp << 8;
			temp += method;

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
			continue;
		}
	}
}


void contiguous_read(int fileName)
{
	struct MapItem* item = hash_search(fileName);

	int start_block = 0;
	int end_block = 0;

	start_block = hard_disk[item->value] & mask_read_2;
	start_block = start_block >> 16;
	end_block = hard_disk[item->value] & mask_read_3;
	end_block = end_block >> 8;

	int start_index = start_block * 5;
	int end_index = (end_block * 5) + 5;

	for (int i = start_index; i < end_index; ++i)
	{
		if (hard_disk[i] > 0)
		{
			printf("File content at disk index %d: %d", i, hard_disk[i]);
		}
		else
		{
			continue;
		}
	}


}

void linked_read(int fileName)
{
	struct MapItem* item = hash_search(fileName);

	int start_block = 0;
	int end_block = 0;

	start_block = hard_disk[item->value] & mask_read_2;
	start_block = start_block >> 16;
	end_block = hard_disk[item->value] & mask_read_3;
	end_block = end_block >> 8;


	int start_index = 0;
	int current_block = start_block;
	int i = 0;
	while (current_block != end_block)
	{
		start_index = current_block * 5;
		for (i = start_index; i < start_index + 4; ++i)
		{
			printf("File content at disk index %d: %d \n", i, hard_disk[i]);
		}
		current_block = hard_disk[i];
	}

	if (current_block == end_block)
	{
		start_index = current_block * 5;
		for (i = start_index; i < start_index + 5; ++i)
		{
			if (hard_disk[i] > 0)
			{
				printf("File content at disk index %d: %d \n", i, hard_disk[i]);
			}
			else
			{
				break;
			}
		}
	}


}

void indexed_read(int fileName)
{
	struct MapItem* item = hash_search(fileName);

	int start_block = 0;
	int end_block = 0;

	start_block = hard_disk[item->value] & mask_read_2;
	start_block = start_block >> 16;
	start_block = hard_disk[item->value] & mask_read_3;
	end_block = end_block >> 8;


}


void contiguous_delete(int fileName)
{

}


void linked_delete(int fileName)
{
	struct MapItem* item = hash_search(fileName);

	int start_block = 0;
	int end_block = 0;

	start_block = hard_disk[item->value] & mask_read_2;
	start_block = start_block >> 16;
	end_block = hard_disk[item->value] & mask_read_3;
	end_block = end_block >> 8;


	int start_index = 0;
	int current_block = start_block;
	int i = 0;
	while (current_block != end_block)
	{
		start_index = current_block * 5;
		for (i = start_index; i < start_index + 4; ++i)
		{
			hard_disk[i] = 0;
		}
		current_block = hard_disk[i];
		hard_disk[i] = 0;
	}

	if (current_block == end_block)
	{
		start_index = current_block * 5;
		for (i = start_index; i < start_index + 5; ++i)
		{
			hard_disk[i] = 0;
		}
	}

	printf("File %d deleted. \n", fileName);
}


void indexed_delete(int fileName)
{

}

void disk_map()
{

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


	// initializing disk
	for (int a = 0; a < 500; ++a)
	{
		hard_disk[a] = 0;
	}

	// initializing dummy entry for hash table
	dummy = (struct MapItem*)malloc(sizeof(struct MapItem));
	if (dummy == NULL)
	{
		return -1;
	}
	dummy->key = -1;
	dummy->value = -1;

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
			struct MapItem* item = hash_search(file_name);


			int method = 0;
			if (item != NULL)
			{
				
				//start_block = hard_disk[item->value] & mask_read_2;
				//start_block = start_block >> 16;

				//start_block = hard_disk[item->value] & mask_read_3;
				//end_block = end_block >> 8;
				
				method = hard_disk[item->value] & mask_read_4;


				//printf("start block: %d \n", start_block);
				//printf("end block: %d \n", end_block);
				printf("alloc method: %d \n", method);

			}
			else
			{
				break;
			}

			if (method == 1) // contiguous
			{
				contiguous_read(file_name);
			}
			else if (method == 2) //linked
			{
				linked_read(file_name);
			}
			else if (method == 3) // indexed
			{

			}

		}
		else if (strcmp(command, "delete") == 0)
		{
			struct MapItem* item = hash_search(file_name);

			int start_block = 0;
			int end_block = 0;
			int method = 0;
			if (item != NULL)
			{

				start_block = hard_disk[item->value] & mask_read_2;
				start_block = start_block >> 16;

				start_block = hard_disk[item->value] & mask_read_3;
				end_block = end_block >> 8;

				method = hard_disk[item->value] & mask_read_4;


				printf("start block: %d \n", start_block);
				printf("end block: %d \n", end_block);
				printf("alloc method: %d \n", method);

			}
			else
			{
				break;
			}

			if (method == 1) // contiguous
			{

			}
			else if (method == 2) //linked
			{
				linked_delete(file_name);
			}
			else if (method == 3) // indexed
			{

			}




		}
		else if (insertion_algo == 1)
		{
			//find start block of the file
			int _startBlock = 0, _endBlock = 0;
			contiguous_allocation(hard_disk, file_content, i, &_startBlock, &_endBlock);
			printf("startblock: %d\n", _startBlock);
			printf("endblock: %d\n", _endBlock);
			disk_add(file_name, _startBlock, _endBlock, 1);

		}
		else if (insertion_algo == 2)
		{
			//linked
			int _startBlock = 0, _endBlock = 0;
			linked_allocation(hard_disk, file_content, i, &_startBlock, &_endBlock);
			printf("startblock: %d\n", _startBlock);
			printf("endblock: %d\n", _endBlock);
			disk_add(file_name, _startBlock, _endBlock, 2);
		}
		else
		{
			//indexed

			//disk_add()
		}

		i = 0;
	}






	return -1;

}