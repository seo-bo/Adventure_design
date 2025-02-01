#include "sub.h"

void insert_chunk(Chunk** head, Chunk* newnode)// 공통으로 사용 가능
{
	if (*head == NULL)
	{
		newnode->right = newnode;
		newnode->left = newnode;
		*head = newnode;
	}
	else
	{
		newnode->left = (*head)->left;
		newnode->right = *head;
		(*head)->left->right = newnode;
		(*head)->left = newnode;
	}
}

void clear()
{
	system("cls");
}

int MIN(int a, int b)
{
	return (a > b) ? b : a;
}

int MAX(int a, int b)
{
	return (a > b) ? a : b;
}

void shrink(Chunk** head, Heap* h, int newsize) 
{
	if (!sorted)
	{
		printf("먼저 정렬 및 합병을 해주세요.\n");
		return;
	}
	if (newsize >= total || newsize < 1 || *head == NULL)
	{
		printf("줄일 수 없습니다.\n");
		return;
	}
	Chunk* now = (*head)->left;
	if (now->used)
	{
		printf("줄일 수 없습니다.\n");
		return;
	}
	if (now->start <= newsize + 1 && newsize <= now->end)
	{
		bool flag = (now->start == newsize + 1);
		Chunk* temp = (Chunk*)malloc(h->size * sizeof(Chunk));
		int top = 0;
		while (h->size)
		{
			temp[top] = max_pop(h);
			if (temp[top].start == now->start)
			{
				if (!flag)
				{
					temp[top].end = newsize;
					temp[top].size = temp[top].end - temp[top].start + 1;
					now->end = newsize;
					now->size = now->end - now->start + 1;
					top++;
				}
				else
				{
					now->left->right = now->right;
					now->right->left = now->left;
					if (now == *head)
					{
						*head = now->right;
					}
					free(now);
					top--;
				}
				break;
			}
			top++;
		}
		for (int i = 0; i < top; ++i)
		{
			max_push(h, temp[i]);
		}
		free(temp);
		total = newsize;
	}
	else
	{
		printf("줄일 수 없습니다.\n");
	}
}

void expand(Chunk** head, Heap* h, int newsize)
{
	if (newsize <= total)
	{
		printf("늘릴 수 없습니다.\n");
		return;
	}
	total = newsize;
	if (*head == NULL)
	{
		Chunk* newnode = (Chunk*)malloc(sizeof(Chunk));
		newnode->start = 1;
		newnode->end = newsize;
		newnode->size = newsize;
		newnode->used = false;
		newnode->right = newnode;
		newnode->left = newnode;
		*head = newnode;
		max_push(h, *newnode);
		return;
	}
	Chunk* now = *head;
	int endpoint = now->end;
	do
	{
		endpoint = MAX(endpoint, now->end);
		now = now->right;
	} while (now != *head);
	Chunk* newnode = (Chunk*)malloc(sizeof(Chunk));
	printf("%d", endpoint);
	newnode->start = endpoint + 1;
	newnode->end = newsize;
	newnode->size = newsize - newnode->start + 1;
	newnode->used = false;
	newnode->right = *head;
	newnode->left = (*head)->left;
	(*head)->left->right = newnode;
	(*head)->left = newnode;
	max_push(h, *newnode);
	sorted = false;
}
