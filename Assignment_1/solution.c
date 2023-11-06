#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
  int data;
  struct Node* next;
}NODE;

NODE *createNode()
{
	NODE *newNode;
	newNode = (NODE*)malloc(sizeof(NODE));
	if(newNode == NULL)
	{
		printf("Allocation Error\n");
		exit(0);
	}
	newNode->next = NULL;
	return newNode;
}

// add node to the front
NODE *insertAtFront(NODE *last, int data) {

	NODE* newNode = createNode();
	newNode->data = data;
	
	// check if the list is empty
	if (last == NULL) 
	{
		last = newNode;
		last->next = last;
	}
	else
	{
	 	newNode->next = last->next;
	    last->next = newNode;
	}

	return last;
}

void printList(NODE* last) {
  NODE *ptr;

  if (last == NULL) {
  printf("The list is empty");
  return;
  }

  ptr = last->next;

  do {
  printf("%d\t", ptr->data);
  ptr = ptr->next;

  } while (ptr != last->next);
  printf("\n");
}

NODE *createWheels(NODE* last, int N, int M, int commonNum, int* tmpArr, int* tmpIndx)
{
	int i, j, counter = 0, num, indx;
	srand(time(0));

	// wheelsArr is a temporary array for defining which numbers will be in one wheels
	int *wheelsArr = (int*)malloc(M * sizeof(int));
	wheelsArr[0] = commonNum;
	
	while(counter < M - 1)
	{
		num = (rand() % N) + 1;
		if(tmpArr[num - 1] == 0)
		{
			counter++;
			wheelsArr[counter] = num;
			tmpArr[num - 1] = 1;
		}
	}
	
	// Creates a random number for to place common number in each wheel. Check the random index with others.
	do
	{
		indx = rand() % (M - 1);
	}while(tmpIndx[0] == indx || tmpIndx[1] == indx || tmpIndx[2] == indx);
	
	i = 0;
	while(i < 3 && tmpIndx[i] != -1)
		i++;
	if(tmpIndx[i] == -1)
		tmpIndx[i] = indx;
		
	// Numbers added to wheels
	j = 1;
	i = 0;
	while(i < M && j < M)
	{
		if(i == indx)
			last = insertAtFront(last, wheelsArr[0]);
		else
		{
			last = insertAtFront(last, wheelsArr[j]);
			j++;
		}
		i++;
	}

	printList(last);
	free(wheelsArr);
	return last;
}

int findCommonNumber(NODE *last1, NODE *last2)
{
	NODE *tmp1 = last1->next;
	NODE *tmp2;
	int data, s = 0;
	
	do 
	{
		data = tmp1->data;
		tmp2 = last2->next;
		do
		{
			if(data == tmp2->data)
				s = 1;
			else 
				tmp2 = tmp2->next;
		}while(s == 0 && tmp2 != last2->next);
		
		tmp1 = tmp1->next;
		
  }while(s == 0 && tmp1 != last1->next);
  
  if(s == 0)
  {
  	printf("There is not any common number in the given lists.\n");
  	return(-1);
  }
	return tmp2->data;	
}

void findIndexOfCommonNumber(NODE *last1, NODE *last2, NODE *last3, int *indexes)
{
	int commonNum = findCommonNumber(last1, last2);
	int index = 1;
	NODE *tmp1 = last1->next;
	NODE *tmp2 = last2->next;
	NODE *tmp3 = last3->next;
	
	do
	{
		if(tmp1->data == commonNum)
			indexes[0] = index;
		if(tmp2->data == commonNum)
			indexes[1] = index;
		if(tmp3->data == commonNum)
			indexes[2] = index;
		tmp1 = tmp1->next;
		tmp2 = tmp2->next;
		tmp3 = tmp3->next;
		index++;
	}while(tmp1 != last1->next && tmp2 != last2->next && tmp3 != last3->next);
	
	printf("1. Wheel: %d\n2. Wheel: %d\n3. Wheel: %d\n", indexes[0], indexes[1], indexes[2]);
}

void findDirectionToSlide(int *indexes, int M, int **slideMtrx)
{
	// Right defines with 0 / Left defines with -1
	int target = indexes[0];
	int source, right, left, i = 1, j;
	
	while(i < 3)
	{
		source = indexes[i]; 	
		if(source <= target)
		{
			right = target - source;
			left = M - target + source;
		}
		else
		{
			left = source - target;
			right = M - source + target;
		}
		
		if(left <= right)
		{
			slideMtrx[0][i - 1] = -1;
			slideMtrx[1][i - 1] = left;
		}
		else
		{
			slideMtrx[0][i - 1] = 0;
			slideMtrx[1][i - 1] = right;
		}	
		i++;
	}
	// Print the directions and number of slides
	for(i = 0; i < 2; i++)
	{
		printf("\n%d. Wheel:\n", i + 2);
		if(slideMtrx[0][i] == -1)
			printf("Left : %d", slideMtrx[1][i]);
		else
			printf("Right : %d", slideMtrx[1][i]);
		printf("\n");
	}
}

NODE *slideOnWheel(NODE *last, int slideNum, int rightLeft, int M)
{
	int i, tmpSlide;
	// Right defines with 0 / Left defines with -1
	
	if(rightLeft == -1)
	{
		//slide to the left side for slideNum times
		for(i = 0; i < slideNum; i++)
			last = last->next;
	}
	else if(rightLeft == 0)
	{
		//slide to the right side for slideNum times
		tmpSlide = M - slideNum;
		for(i = 0; i < tmpSlide; i++)
			last = last->next;
	}

	return last;
}

void *printSlidedWheels(NODE *last1, NODE *last2, NODE *last3)
{
	printf("1. Wheel:\n");
	printList(last1);
	printf("2. Wheel:\n");
	printList(last2);
	printf("3. Wheel:\n");
	printList(last3);
}

int main()
{
	int N, M, i, commonNum, j, flag = 0, s;
	int *tmpArr, *indexes, *tmpIndx, **slideMtrx;
	
	srand(time(0));
	
	
	while(flag == 0)
	{
		NODE* last1 = NULL;
		NODE* last2 = NULL;
		NODE* last3 = NULL;
		// index array of common number for each wheel 
		indexes = (int*)malloc(3 * sizeof(int));
		if(indexes == NULL)
		{
		printf("Allocation Error\n");
		exit(0);
		}
		// results matrix for each wheel's number of slides and direction
		slideMtrx = (int**)malloc(2 * sizeof(int*));
			if(slideMtrx == NULL)
		{
		printf("Allocation Error\n");
		exit(0);
		}
		for(i = 0; i < 2; i++)
			slideMtrx[i] = (int*)malloc(2 * sizeof(int));
		// tmpIndx -> array for checking if common number is in the same index in different wheels
		tmpIndx = (int*)malloc(3 * sizeof(int));
		if(tmpIndx == NULL)
		{
		printf("Allocation Error\n");
		exit(0);
		}
	
		printf("New Wheels: '0' / Exit: 'Any Key'\n");
		scanf("%d", &s);
		if(s == 0)
		{
			for(i = 0; i < 3; i++)
				tmpIndx[i] = -1;
			
			do
			{
				printf("Enter the N and M respectively:\nN must be greater or equal to 3 * M - 2\n");
				printf("N:");
				scanf("%d", &N);
				printf("M:");
				scanf("%d", &M);
			}while(N < 3 * M - 2);
			
			// choose the common number
			commonNum = (rand() % N) + 1;
			
			// tmpArr -> used for creating wheels with different random numbers
			tmpArr = (int*)malloc(N * sizeof(int));
			if(tmpArr == NULL)
			{
				printf("Allocation Error\n");
				exit(0);
			}
			// initilize the tmpArr 
			for(i = 0; i < N; i++)
			{
				tmpArr[i] = 0;
			}
			tmpArr[commonNum - 1] = - 1;
			
			// Creating new random wheels
			printf("\n1. WHEEL:\n");
			last1 = createWheels(last1, N , M, commonNum, tmpArr, tmpIndx);
			printf("\n2. WHEEL:\n");
			last2 = createWheels(last2, N , M, commonNum, tmpArr, tmpIndx);
			printf("\n3. WHEEL:\n");
			last3 = createWheels(last3, N , M, commonNum, tmpArr, tmpIndx);
			
			//Finding common number
			printf("\nCOMMON NUMBER: %d\n", findCommonNumber(last1, last2));
			// Finding indexes of common number for each wheel
			findIndexOfCommonNumber(last1, last2, last3, indexes);
			// Finding directions and count must be slided
			findDirectionToSlide(indexes,  M, slideMtrx);
			// Sliding wheels
			last2 = slideOnWheel(last2,  slideMtrx[1][0], slideMtrx[0][0], M);
			last3 = slideOnWheel(last3,  slideMtrx[1][1], slideMtrx[0][1], M);
			
			printSlidedWheels(last1, last2, last3); 
		}
		else
			flag = 1;
			
	free(tmpArr);
	free(tmpIndx);
	free(indexes);
	for(i = 0; i < 2; i++)
		free(slideMtrx[i]);
	free(slideMtrx);
	free(last1);
	free(last2);
	free(last3);	
	}
	return 0;
}

