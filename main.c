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
		return;
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
			struct MapItem* temp = hash_table[index];

			//assign a dummy item at deleted position
			hash_table[index] = dummy;
			return;
		}

		//go to next cell
		++index;

		//wrap around the table
		index %= TABLESIZE;
	}

	return;
}

// END OF HASH TABLE
// END OF HASH TABLE
// END OF HASH TABLE



// Contiguous
void sequential_allocation(int files[], int content[], int filesLength, int* _startBlock, int* _endBlock)
{
	int flag = 0, p;
	int start, index, count = filesLength, entriesPerBlock = 5, i = 0, j = 0;
	int blocksNeeded = 0;
	int freeSpaceCount = 0;
	int currentBlockSpace = 0;

	start = 2;
	*_startBlock = start;

	//just nice multiples of 5 elements 
	if (count % 5 == 0)
	{
		blocksNeeded = count / 5;
	}
	//not nice 
	else
	{
		blocksNeeded = (count / 5) + 1;
	}

LOOP: 
	index = start * entriesPerBlock;
	if (files[index] == 0)
	{
		for (i = index; i < index + count; ++i)
		{
			if (files[i] == 0)
			{
				freeSpaceCount++;
				currentBlockSpace = freeSpaceCount / 5;

				if (freeSpaceCount == count)
				{
					for (p = 0; p < count; ++p)
					{
						files[index] = content[j];
						++index; ++j;
					}

					if (blocksNeeded <= 1)
					{
						*_startBlock = start;
						*_endBlock = *_startBlock;
					}
					else
					{
						*_startBlock = start;

						*_endBlock = *_startBlock + blocksNeeded - 1;
					}
					freeSpaceCount = 0;
					break;
				}

			}
			else
			{
				freeSpaceCount = 0;
				switch (i % 5)
				{
				case 0: i += 5; break;
				case 1: i += 4; break;
				case 2: i += 3; break;
				case 3: i += 2; break;
				case 4: i += 1; break;
				}
				continue;
			}
		}
	}
	else
	{
		freeSpaceCount = 0;
		start++;
		goto LOOP;
	}
}


// Linked
void linked_allocation(int files[], int content[], int filesLength, int* _startBlock, int* _endBlock)
{
	int start, currIndex, prevIndex, count = filesLength, entriesPerBlock = 5, i = 0, j = 0, totalBlockCount = 50;
	for (start = 2; start < totalBlockCount; ++start)
	{
		if (files[start * entriesPerBlock] == 0)
		{
			*_startBlock = start;
			break;
		}
	}
	prevIndex = start * entriesPerBlock;
LOOP: 
	currIndex = start * entriesPerBlock;
	if (files[currIndex] == 0)
	{
		for (i = currIndex; i < currIndex + entriesPerBlock - 1; ++i)
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
			++start;
			prevIndex = currIndex;
			files[prevIndex + entriesPerBlock - 1] = start;
			goto LOOP;
		}
		else
		{
			files[i] = -1;
			*_endBlock = start;
		}
	}
	else
	{
		++start;
		files[prevIndex + entriesPerBlock - 1] = start;
		goto LOOP;
	}
}

// Indexed
void indexed_allocation(int files[], int content[], int filesLength, int* _indexBlock, int* _startBlock, int* _endBlock)
{
	int start, index, indexblock = 0, count = filesLength, entriesPerBlock = 5, i = 0, j = 0, k = 0, counter = 5;

	// user decide where index block position
	//if (*_indexBlock == -1)
	//{
	//	//printf("start %d\n", *_indexBlock);
	//	printf("Enter index block: ");
	//	scanf("%d", &indexblock);
	//	*_indexBlock = indexblock;
	//	*_startBlock = indexblock;
	//	*_endBlock = indexblock;

	//	//printf("end %d\n", *_indexBlock);
	//}
	//else
	//{
	//	indexblock = *_indexBlock;
	//	*_startBlock = indexblock;
	//	*_endBlock = indexblock;
	//}

	printf("Enter index block: ");
	scanf("%d", &indexblock);
	*_indexBlock = indexblock;
	*_startBlock = indexblock;
	*_endBlock = indexblock;

LOOP: printf("Enter starting block: ");
	scanf("%d", &start);

	index = start * entriesPerBlock;
	if (files[index] == 0)
	{
		// insert block to insert block
		//for (i = indexblock; i < indexblock + entriesPerBlock - 1; ++i)
		//{
		//	if (files[*_indexBlock * 5] == 0)
		//	{
		//		files[*_indexBlock * 5] = start;
		//		//printf("indexblock %d\n", files[*_indexBlock]);

		//		*_endBlock = i;
		//		//printf("endblock %d\n", *_endBlock);
		//	}
		//}
		for (i = 0; i < 5; ++i)
		{
			if (files[*_indexBlock * 5 + i] == 0)
			{
				files[*_indexBlock * 5 + i] = start;
				break;
				//printf("indexblock %d\n", files[*_indexBlock]);
			}
		}


		//insert entries to block
		for (i = index; i < index + entriesPerBlock; ++i)
		{
			if (files[i] == 0 && count != 0 && counter > 0)
			{
				files[i] = content[j];
				//printf("content %d\n", content[j]);
				//printf("files %d\n", files[i]);
				++j;
				--count;
				--counter;
				if (counter == 0 && count > 0)
				{
					counter = 5;
					goto LOOP;
				}
			}
			else
			{
				//printf("fail");
			}
		}
	}
	else
	{
		// if block is taken repeat
		printf("Block %d has already been allocated.\n", start);
		goto LOOP;
	}
}



void disk_add(int fileName, int startBlock, int endBlock, int method)
{
	int i;
	int temp = 0;

	// check if file already has an entry
	if (hash_search(fileName) == NULL) // if new entry
	{
		// check if there's already 10 files
		for (i = 0; i < 10; ++i)
		{
			// if hard_disk has an empty spot
			if (hard_disk[i] == 0)
			{
				hash_insert(fileName, i); // insert into table

				//add new entry
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

				return;
			}
		}
	}
	else // if already exists
	{
		i = hash_search(fileName)->value;

		temp = fileName;
		temp = temp << 8;
		temp += startBlock;
		temp = temp << 8;
		temp += endBlock;
		temp = temp << 8;
		temp += method;

		hard_disk[i] = temp;
	}


}


void contiguous_read(int fileName)
{
	int counter = 0;
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
			printf("File content at disk index %d: %d \n" , i, hard_disk[i]);
			++counter;
		}
		else
		{
			continue;
		}
	}
	printf("Total number of printed content: %d \n", counter);

}

void linked_read(int fileName)
{
	int counter = 0;
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
			++counter;
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
				++counter;
			}
			else
			{
				break;
			}
		}
	}
	printf("Total number of printed content: %d \n", counter);

}

void indexed_read(int fileName)
{
	int counter = 0;
	struct MapItem* item = hash_search(fileName);

	int start_block = 0;
	int start_index = 0;

	start_block = hard_disk[item->value] & mask_read_2;
	start_block = start_block >> 16;

	// get the indexes that files are stored in
	int current_block = start_block;
	int i = 0; // iterate through the index block

	start_index = current_block * 5;
	int element = 0;
	for (i = start_index; i < start_index + 5; ++i)
	{
		if (hard_disk[i] != 0)
		{
			element = hard_disk[i] * 5;
			// iterate the actual elements
			for (int j = 0; j < 5; ++j)
			{
				if (hard_disk[element + j] > 0)
				{
					printf("File content at disk index %d: %d \n", element + j, hard_disk[element + j]);
					++counter;
				}
			}
		}
	}
	printf("Total number of printed content: %d \n", counter);
}


//void indexed_read(int fileName)
//{
//	struct MapItem* item = hash_search(fileName);
//	int start_block = 0;
//	int end_block = 0;
//	int arr[50] = { 0 };
//	start_block = hard_disk[item->value] & mask_read_2;
//	start_block = start_block >> 16;
//	start_block = hard_disk[item->value] & mask_read_3;
//	end_block = end_block >> 8;
//	// get the indexes that files are stored in
//	int current_block = start_block;
//	int i = 0;
//	while (current_block != end_block)
//	{
//		//start_index = current_block * 5;
//		/*for (i = start_index; i < start_index + 5; ++i)
//		{
//			printf("File content at disk index %d: %d \n", i, hard_disk[i]);
//		}
//		current_block = hard_disk[i];*/
//	}
//}


void contiguous_delete(int fileName)
{
	struct MapItem* item = hash_search(fileName);
	int disk_index = item->value;
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
		hard_disk[i] = 0;
	}

	hard_disk[disk_index] = 0;
	hash_delete(fileName);
	printf("File %d deleted. \n", fileName);
}


void linked_delete(int fileName)
{
	struct MapItem* item = hash_search(fileName);
	int disk_index = item->value;
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

	hard_disk[disk_index] = 0;
	hash_delete(fileName);
	printf("File %d deleted. \n", fileName);
}


void indexed_delete(int fileName)
{
	struct MapItem* item = hash_search(fileName);
	int disk_index = item->value;
	int start_block = 0;

	start_block = hard_disk[item->value] & mask_read_2;
	start_block = start_block >> 16;

	int start_index = 0;
	int element_index = 0;

	// get the indexes that files are stored in
	int current_block = start_block;
	int i = 0; // iterate through the index block

	start_index = current_block * 5;
	int element = 0;

	for (i = start_index; i < start_index + 5; ++i)
	{
		element_index = hard_disk[start_index];
		hard_disk[start_index] = 0;
		for (int j = 0; j < 5; ++j)
		{
			hard_disk[element_index * 5 + j] = 0;
		}
	}
	//for (i = start_index; i < start_index + 5; ++i)
	//{
	//	element = i;
	//	// iterate the actual elements
	//	for (int j = 0; j < 5; ++j)
	//	{
	//		hard_disk[element + j] = 0;
	//	}
	//}

	hard_disk[disk_index] = 0;
	hash_delete(fileName);
	printf("File %d deleted. \n", fileName);
}

void disk_map()
{
	int index = 0, blockNum = 0, start_block = 0, end_block = 0, file_name = 0;

	printf("Index\tBlock\tFile Data\n");
	for (index = 0; index < 500; ++index)
	{
		if (index != 0 && index % 5 == 0)
			++blockNum;

		if (blockNum >= 0 && blockNum < 2 && hard_disk[index] != 0)
		{
			file_name = hard_disk[index] & mask_read_1;
			file_name = file_name >> 24;
			start_block = hard_disk[index] & mask_read_2;
			start_block = start_block >> 16;
			end_block = hard_disk[index] & mask_read_3;
			end_block = end_block >> 8;
			printf("%d\t%d\t%d, %d, %d\n", index, blockNum, file_name, start_block, end_block);
		}
		else
			printf("%d\t%d\t%d\n", index, blockNum, hard_disk[index]);
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
	int _indexBlock = -1;


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
		
		if (token == NULL)
		{
			break;
		}
		
		command = token;
		token = strtok(NULL, ",");
		
		if (token == NULL)
		{
			break;
		}

		file_name = atoi(token);
		printf("command: %s \n", command);
		printf("file name: %d \n", file_name);

		token = strtok(NULL, ",");
		while (token != NULL)
		{
			if (strcmp(token, "\n") == 0 || strlen(token) == 0 || strcmp(token, "0") == 0)
			{
				break;
			}
			else if (atoi(token) == 0)
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
				method = hard_disk[item->value] & mask_read_4;

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
				indexed_read(file_name);
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
				contiguous_delete(file_name);
			}
			else if (method == 2) //linked
			{
				linked_delete(file_name);
			}
			else if (method == 3) // indexed
			{
				indexed_delete(file_name);
			}

		}
		else if (insertion_algo == 1)
		{
			//find start block of the file
			int _startBlock = 0, _endBlock = 0;
			sequential_allocation(hard_disk, file_content, i, &_startBlock, &_endBlock);
			disk_add(file_name, _startBlock, _endBlock, 1);

		}
		else if (insertion_algo == 2)
		{
			//linked
			int _startBlock = 0, _endBlock = 0;
			linked_allocation(hard_disk, file_content, i, &_startBlock, &_endBlock);
			disk_add(file_name, _startBlock, _endBlock, 2);
		}
		else
		{
			//indexed
			int _startBlock = 0, _endBlock = 0;

			indexed_allocation(hard_disk, file_content, i, &_indexBlock, &_startBlock, &_endBlock);
			printf("startblock: %d\n", _startBlock);
			printf("endblock: %d\n", _endBlock);
			disk_add(file_name, _startBlock, _startBlock, 3);
		}
		i = 0;
	}
	disk_map();

	return 0;
}