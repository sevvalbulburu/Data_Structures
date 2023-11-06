#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct bst
{
	int value;
	int power;
	struct bst *left;
	struct bst *right;
}BST;

typedef struct stack{
	int top;
	BST *items;
	int N;
}STACK;

// STACK 
STACK *initiliaze(int N)
{
	STACK *s = (STACK*)malloc(sizeof(STACK));
	if(s == NULL)
	{
		printf("Allocation error.\n");
		exit(0);
	}
	s->items = (BST*)malloc(N * sizeof(BST));
	if(s->items == NULL)
	{
		printf("Allocation error for array.\n");
		exit(0);
	}
	s->N = N;
	s->top = 0;
	return s;
}

int isEmpty(STACK *s)
{
	if(s->top == 0)
		return 1;
	return 0;
}

int isFull(STACK *s)
{
	if(s->top == s->N)
		return 1;
	return 0;
}

int pop(STACK *s, BST *item)
{
	if(isEmpty(s) == 1)
	{
		printf("Stack is empty.\n");
		return 0;
	}
	s->top--;
	*item = s->items[s->top];
	return 1;
}

int push(STACK *s, BST *item)
{
	if(isFull(s) == 1)
	{
		printf("Stack is full.\n");
		return 0;
	}
	s->items[s->top] = *item;
	s->top++;
	
//	printf("\nPush -> Apples: %d top: %d\n",s->items[s->top - 1].value, s->top );
	return 1;
}

// TREE
BST *newNode(int value)
{
	BST *ptr = (BST*)malloc(sizeof(BST));
	if(ptr == NULL)
	{
		printf("Allocation error.\n");
		exit(0);
	}
	ptr->left = NULL;
	ptr->right = NULL;
	ptr->value = value;
	ptr->power = value;
	return ptr;
}

// inserts given number to bst
BST *insertToBST(BST *root, int value)
{
	BST *ptr = newNode(value);
	BST *parent, *current;
	int stop = 0;
	
	if(root == NULL)
		root = ptr;
	else
	{
		current = root;
		while(!stop)
		{
			if(value < current->value)
			{
				parent = current;
				current = current->left;
				if(current == NULL)
				{
					parent->left = ptr;
					stop = 1;
				}
			}
			else
			{
				parent = current;
				current = current->right;
				if(current == NULL)
				{
					parent->right = ptr;
					stop = 1;
				}
			}
		}
	}
	return root;
}

BST *findMinNode(BST *subroot)
{
    BST *current = subroot;

    // loop down to find the leftmost leaf
    while (current && current->left != NULL)
        current = current->left;

    return current;
}

BST *deleteFromBST(BST *root, int value)
{
    // base case
    if (root == NULL) return root;

    // If the value to be deleted is smaller than the root's value then it lies in left subtree
    if (value < root->value)
        root->left = deleteFromBST(root->left, value);

    // If the value to be deleted is greater than the root's value then it lies in right subtree
    else if(value > root->value)
        root->right = deleteFromBST(root->right, value);

    else
    {
        // node with only one child or no child
        if(root->left == NULL)
        {
            BST *tmp = root;
            root = root->right;
            free(tmp);
            return root;
        }
        else if(root->right == NULL)
        {
            BST *tmp = root;
            root = root->left;
            free(tmp);
            return root;
        }

		//find min value in subtree if parent has two children
        BST *tmp = findMinNode(root->right);
        root->value = tmp->value;
        root->power = tmp->power;
        root->right = deleteFromBST(root->right, tmp->value);
    }
    return root;
}

BST *searchTree(BST *root, int data)
{
	BST *tmp = root;
	if(root == NULL)
	{
		printf("Tree is empty\n");
		return NULL;
	}
	
	while(tmp != NULL && data != tmp->value)
	{
		if(data < tmp->value)
			tmp = tmp->left;
		else
			tmp = tmp->right;
	}
	if(tmp == NULL)
		printf("Given value is not on the tree.\n");	
	return tmp;
	
}

void printGivenLevel(BST* root, int level) {
    if(root == NULL) 
        return;

    if(level == 1)
        printf("  %d(%d)\t", root->value, root->power);
    else if (level > 1)
	{
        printGivenLevel(root->left, level - 1);
        printGivenLevel(root->right, level - 1);
    }
}

// Compute the height of a tree
int height(BST* root)
{
	int lefth, righth;
    if(root == NULL)
        return 0;
    else 
	{
        // Compute the height of each subtree
        lefth = height(root->left);
        righth = height(root->right);
        // Use the larger one
        if(lefth > righth)
            return lefth + 1;
        else
            return righth + 1;
    }
}

// Function for print level order traversal of tree
void printTree(BST* root)
{
	int i;
    int h = height(root);
    for(i = 1; i <= h; i++)
	{
        printGivenLevel(root, i);
        printf("\n");
    }
}

BST *dropApple(BST *root, STACK *s, int p, int k)
{
	BST *apple = searchTree(root, k);
	int tmp;
	if(apple == NULL)
	{
		printf("Apple already dropped from tree or not on the tree.\n");
		return root;
	}
	// Drop the apple
	apple->power -= p;
	printf("Apple: %d Apple's Resedual Power: %d\n", apple->value, apple->power);
	if(apple->power <= 0)
	{
		tmp = apple->value;
		push(s, apple);
		root = deleteFromBST(root, tmp);
		printf("%d is dropped.\n", tmp);
	}

	return root;
}

int main()
{
	BST *root = NULL, *tmpApple;
	STACK *bucket1, *bucket2;
	int N, M, p, k, i, power, flag = 0;
	int *tmp;
    srand(time(0));
	while(flag == 0)
	{
		printf("\nIf you continue the game press '0'. Exit with any key.\n");
		scanf("%d", &flag);
		if(flag == 0)
		{
			
		    printf("Enter N and M respectively\n(M: Number of apples\nN: Maximum power of apples):\n");
		    scanf("%d %d", &N, &M);
			
			// Create random numbers for tree
			tmp = (int*)calloc(N, sizeof(int));
			if(tmp == NULL)
			{
				printf("Allocation error.\n");
				exit(0);
			}
			
			i = 0;
			while(i < M)
			{
				power = (rand() % N) + 1;
				if(tmp[power - 1] == 0)
				{
					root = insertToBST(root, power);
					tmp[power - 1] = 1;
					i++;
				}
			}
			
			printTree(root);
			// Create Stack for tree
			bucket1 = initiliaze(M);
			bucket2 = initiliaze(M);
					
			
			// Game must stop when all apples are dropped from tree -> when stack is full
			while(!isFull(bucket1))
			{
				printf("Enter the power and apple number respectively:\n");
				scanf("%d %d", &p, &k);
				
				root = dropApple(root, bucket1, p , k);
				printTree(root);
			}
			
			if(isFull(bucket1))
			{

				// Take all the apples respectively from first bucket and put them an empty bucket
				while(!isEmpty(bucket1))
				{
					tmpApple = (BST*)malloc(sizeof(BST));
					pop(bucket1, tmpApple);
					push(bucket2, tmpApple);
				}
				// Take apples from second bucket and print them 
				printf("List of the apples by their dropped order:\n");
				while(!isEmpty(bucket2))
				{
					pop(bucket2, tmpApple);
					printf(" %d ", tmpApple->value);
				}
			}
		}
		else
			flag = 1;
	}
	return 0;
}
