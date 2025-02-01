#include "sub.h"

void merge(Chunk ary[], int left, int mid, int right)
{
	int p1 = mid - left + 1, p2 = right - mid;
	Chunk* L = (Chunk*)malloc(p1 * sizeof(Chunk));
	Chunk* R = (Chunk*)malloc(p2 * sizeof(Chunk));
	for (int i = 0; i < p1; ++i)
	{
		L[i] = ary[left + i];
	}
	for (int i = 0; i < p2; ++i)
	{
		R[i] = ary[mid + 1 + i];
	}
	int i = 0, j = 0, k = left;
	while (i < p1 && j < p2)
	{
		if (L[i].start <= R[j].start)
		{
			ary[k++] = L[i++];
		}
		else
		{
			ary[k++] = R[j++];
		}
	}
	while (i < p1)
	{
		ary[k++] = L[i++];
	}
	while (j < p2)
	{
		ary[k++] = R[j++];
	}
	free(L);
	free(R);
}

void sort(Chunk ary[], int left, int right)
{
	if (left < right)
	{
		int mid = (left + right) / 2;
		sort(ary, left, mid);
		sort(ary, mid + 1, right);
		merge(ary, left, mid, right);
	}
}

Chunk* empty_merge(Heap* h, int* head) // true -> 弥措赛, false -> 弥家赛
{
	if (h->size == 0)
	{
		return NULL;
	}
	int idx = 0;
	Chunk* temp = (Chunk*)malloc(h->size * sizeof(Chunk));
	while (h->size)
	{
		temp[idx++] = min_pop(h);
	}
	sort(temp, 0, idx - 1);
	Chunk* ary = (Chunk*)malloc(total * sizeof(Chunk));
	int newidx = 0;
	ary[newidx] = temp[0];
	for (int i = 1; i < idx; ++i)
	{
		if (ary[newidx].end + 1 == temp[i].start)
		{
			ary[newidx].end = temp[i].end;
			ary[newidx].size += temp[i].size;
		}
		else
		{
			ary[++newidx] = temp[i];
		}
	}
	free(temp);
	*head = newidx + 1;
	for (int i = 0; i < *head; ++i)
	{
		min_push(h, ary[i]);
	}
	return ary;
}

void merge_sort(Node** node, Chunk** head, Heap* h) // true -> 弥措赛, false -> 弥家赛
{
	int top = 0, adtop = 0;
	Chunk* ary = empty_merge(h, &top);
	restore* adx = (restore*)malloc(sizeof(restore) * 10000);
	if (ary == NULL)
	{
		free(adx);
		return;
	}
	inorder(*node, adx, &adtop);
	Chunk* a = *head;
	do
	{
		Chunk* temp = a->right;
		if (a->used)
		{
			ary[top++] = *a;
		}
		if (a != NULL)
		{
			free(a);
		}
		a = temp;
	} while (a != *head);
	*head = NULL;
	sort(ary, 0, top - 1);
	Chunk* pre = NULL;
	for (int i = 0; i < top; ++i)
	{
		Chunk* newnode = (Chunk*)malloc(sizeof(Chunk));
		newnode->start = ary[i].start;
		newnode->end = ary[i].end;
		newnode->size = ary[i].size;
		newnode->used = ary[i].used;
		insert_chunk(head, newnode);
		pre = newnode;
		if (newnode->used)
		{
			for (int j = 0; j < adtop; ++j)
			{
				if (newnode->start == adx[j].start)
				{
					*node = insert(*node, adx[j].name, newnode);
					break;
				}
			}
		}
	}
	free(ary);
	free(adx);
	sorted = true;
}


