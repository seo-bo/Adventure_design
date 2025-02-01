#include "sub.h"

int total = 0;
bool sorted = true;

Chunk* my_alloc(Chunk** head, Heap* h, int size)
{
	if (!h->size || h->heap[1].size < size)
	{
		printf("할당할 수 없습니다.\n");
		return NULL;
	}
	Chunk pivot = max_pop(h);
	int old = pivot.start;
	Chunk* now = *head;
	do
	{
		if (now->start == pivot.start)
		{
			now->start += size;
			now->size -= size;
			if (now->size > 0)
			{
				max_push(h, *now);
			}
			else
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
	return newnode;
}

void my_free(Heap* h, Chunk* node)
{
	if (node == NULL)
	{
		return;
	}
	node->used = false;
	max_push(h, *node);
}