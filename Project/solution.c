#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#define BUFFER_SIZE 256
#define SIZE 10  

// Tree node structure
typedef struct node{
    struct node *children[SIZE];
    int isEndOfWord;
    char value[SIZE];
}NODE;

// Create a node for tree
NODE *createNode() {
	int i;
    NODE *node = (NODE*)malloc(sizeof(NODE));
    node->isEndOfWord = 0;
    for (i = 0; i < SIZE; i++)
        node->children[i] = NULL;
    return node;
}

// finds associated number for letters
int getNumber(char let) {
    let = toupper(let); 

    if (let >= 'A' && let <= 'C')
        return (let - 'A') / 3 + 2;
    else if (let >= 'D' && let <= 'F')
        return (let - 'D') / 3 + 3;
    else if (let >= 'G' && let <= 'I')
        return (let - 'G') / 3 + 4;
    else if (let >= 'J' && let <= 'L')
        return (let - 'J') / 3 + 5;
    else if (let >= 'M' && let <= 'O')
        return (let - 'M') / 3 + 6;
    else if (let >= 'P' && let <= 'S')
        return (let - 'P') / 4 + 7;
    else if (let >= 'T' && let <= 'V')
        return (let - 'T') / 3 + 8;
    else if (let >= 'W' && let <= 'Z')
        return (let - 'W') / 4 + 9;
    else if(let == '#')
    	return 0;
    else
        return -1;
}


// Add words to tree
void insertTree(NODE *root, char *word) 
{
    NODE *curr = root;
    int i = 0, num, count = 0;
	int len = strlen(word);
    while(i < len) 
	{
    	// find the node that has not # 
        while (curr->children[0] != NULL)
            curr = curr->children[0];
            
        // get the number for each letter
        num = getNumber(word[i]);
        
		// if the node didnt create before, create it 
        if (curr->children[num] == NULL) 
		{
            curr->children[num] = createNode();
        	count++;
        }
        curr = curr->children[num];
        i++;
    }
    curr->isEndOfWord = 1;
    // count = 0 means that word must be added to # 
   if(count != 0)
    	strcpy(curr->value, word);
    else
    {
        while (curr->children[0] != NULL)
            curr = curr->children[0];
            
		if(curr->children[0] == NULL)
			curr->children[0] = createNode();
			
		curr->children[0]->isEndOfWord = 1;
		strcpy(curr->children[0]->value, word);
		curr = curr->children[0];
	}

}

void findNumber(NODE *root, int number)
{
	NODE *curr = root;
	int *digit;
	int s = 1, i = 0, dec = 0, index, k;
	char *word;
	digit = (int*)calloc(SIZE, sizeof(int));
	word = (char*)calloc(SIZE, sizeof(char));
	
	// get the numbers 
	while(number != 0)
	{
		digit[i] = number % 10;
		number /= 10;
		dec++;
		i++;
	}
	
	// find the word
	i = dec - 1;

	while(s && i >= 0)
	{    
		index = digit[i];
        if (curr->children[index] == NULL && curr->children[0] == NULL) 
     		s = 0;
     	else
		{
			//check if there is # 
			k = 1;
			while(k && curr->children[0] != NULL )
			{
				curr = curr->children[0];
					
				if(curr->children[index] != NULL)
				{
					k = 0;
					curr = curr->children[index];
					i--;
				}
				else if(curr->children[0] == NULL && curr->children[index] == NULL)
				{
					k = 0;
					s = 0;
				}
	
			}
			//if there isnt any # 
			if(k == 1)
			{
				if(curr->children[index] == NULL)
					s = 0;
				else
				{
     				curr = curr->children[index];
        			i--;
				}
			}					
	        
		}	
	} 
	if(s == 0 || curr->isEndOfWord == 0)
		printf("Kelime sozlukte bulunmamaktadir.\n");
	else if(i == -1 && curr->isEndOfWord == 1)
	{
		printf("Kelime: %s \n", curr->value);
		while(curr->children[0] != NULL)
		{
			curr = curr->children[0];
			printf("Kelime: %s \n", curr->value);
		}
	}
		
}

void freeTree(NODE* root) {
	int i;
    if (root == NULL) 
        return;
    for (i = 0; i < SIZE; i++) 
        freeTree(root->children[i]);

    free(root);
}

char** readDictionary(char* filename, int *lenght) 
{
    FILE* file = fopen(filename, "r");
    char buffer[BUFFER_SIZE];
    char line[BUFFER_SIZE];
    int len = 0, i;
    
    if (!file) {
        exit(1);
    }
    
     // Read each line and find the number of strings
    while (fgets(line, sizeof(line), file) != NULL) 
        len++;
    
    *lenght = len;
    // Allocation for dictionary
    char** stringList = (char**) malloc(len * sizeof(char*));
    if (!stringList) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
	
	//read file line by line, remove the \n and add them to the string array
    i = 0;
    fseek(file, 0, SEEK_SET);
    while (fgets(buffer, sizeof(buffer), file) && i < len) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove the trailing newline
        
        size_t length = strlen(buffer);
        stringList[i] = (char*) malloc((length + 1) * sizeof(char));  // Allocate memory for the string
        if (!stringList[i]) {
            printf("Memory allocation failed.\n");
            exit(1);
        }
        strcpy(stringList[i], buffer);  // Copy the line to the list
        i++;
    }

    fclose(file);

    return stringList;
}
int main() {
    NODE* root = createNode();  
    int i, dicLen, flag = 1;
	char s;
    char** dic = readDictionary("sample.txt", &dicLen);
    
	printf("Dictionary:\n");
	for(i = 0; i < dicLen; i++)
		printf("%s\n", dic[i]);
	
    for (i = 0; i < dicLen; i++)
        insertTree(root, dic[i]);
    
	while(flag)
	{
		printf("\nEnter '0' for quit or enter the number you want to found in the dictionary:\n");
		scanf("%d", &flag);
		if(flag != 0)
    		findNumber(root, flag);	
	}
	
    freeTree(root);
   

    return 0;
}
