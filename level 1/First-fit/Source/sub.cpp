#include "sub.h"

int total = 0;
bool sorted = true;

Chunk*my_alloc(Chunk** head, Heap* h, int size)
{
	bool check = false;
	Chunk* ary = (Chunk*)malloc(sizeof(Chunk) * 10000);
	int idx = 0;
	Chunk pivot = { 0,0,0,false,NULL,NULL };
	while (h->size > 0)
	{
		ary[idx++] = min_pop(h);
		if (ary[idx - 1].size >= size && !check)
		{
			pivot = ary[idx - 1];
			check = true;
		}
	}
	if (!check)
	{
		for (int i = 0; i < idx; ++i)
		{
			min_push(h, ary[i]);
		}
		free(ary);
		return NULL;
	}
	int old = pivot.start;
	Chunk* now = *head;
	do
	{
		if (now->start == pivot.start)
		{
			now->start += size;
			now->size -= size;
			if (now->size == 0)
			{
				if (now == *head)
				{
					if (now->right == now)
					{
						*head = NULL;
					}
					else
					{
						*head = now->right;
						(*head)->left = now->left;
						now->left->right = *head;
					}
				}
				else
				{
					now->left->right = now->right;
					now->right->left = now->left;
				}
				free(now);
			}
			break;
		}
		now = now->right;
	} while (now != *head);
	Chunk* newnode = (Chunk*)malloc(sizeof(Chunk));
	newnode->size = size;
	newnode->end = old + size - 1;
	newnode->start = old;
	newnode->used = true;
	insert_chunk(head, newnode);
	now = *head;
	do
	{
		if (!now->used)
		{
			min_push(h, *now);
		}
		now = now->right;
	} while (now != *head);
	free(ary);
	return newnode;
}

void my_free(Heap* h, Chunk* node)
{
	if (node == NULL)
	{
		return;
	}
	node->used = false;
	min_push(h, *node);
}
