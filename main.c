#include <stdio.h> // file
#include <stdlib.h> // atoi
#include <string.h> // strtok

#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#define MAXCHAR 1024	// How many char to read at a time
int hard_disk[500];


// Contiguous
void contiguous_allocation()
{



}

void recurse(int files[]) {
	int flag = 0, startBlock, len, j, k, ch;
	printf("Enter the starting block and the length of the files: ");
	scanf("%d%d", &startBlock, &len);
	for (j = startBlock; j < (startBlock + len); j++) {
		if (files[j] == 0)
			flag++;
	}
	if (len == flag) {
		for (int k = startBlock; k < (startBlock + len); k++) {
			if (files[k] == 0) {
				files[k] = 1;
				printf("%d\t%d\n", k, files[k]);
			}
		}
		if (k != (startBlock + len - 1))
			printf("The file is allocated to the disk\n");
	}
	else
		printf("The file is not allocated to the disk\n");
	printf("Do you want to enter more files?\n");
	printf("Press 1 for YES, 0 for NO: ");
	scanf("%d", &ch);
	if (ch == 1)
		recurse(files);
	else
		exit(0);
	return;
}


// Linked

// Indexed


int main(int argc, char** argv) {
	/* Make your program do whatever you want */
	int hard_disk[500];

	char row[MAXCHAR];

	char* token;
	char* command;
	int file_name;
	int file_content[100];
	int i = 0;

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
			//printf("token is: a%sa \n", token);
			if (strcmp(token, "\n") == 0 || strlen(token) == 0)
			{
				break;
			}
			file_content[i] = atoi(token);
			printf("file content at %d: %d \n", i, file_content[i]);
			++i;
			token = strtok(NULL, ",");
		}

		i = 0;
	}


	return -1;

}