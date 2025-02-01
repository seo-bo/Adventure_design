#include "sub.h"


Heap* make_heap(void)
{
	Heap* h = (Heap*)malloc(sizeof(Heap));
	h->size = 0;
	return h;
}

void max_push(Heap* h, Chunk num)
{
	int i = ++(h->size);
	while ((i != 1) && num.size > h->heap[i / 2].size)
	{
		h->heap[i] = h->heap[i / 2];
		i /= 2;
	}
	h->heap[i] = num;
}

Chunk max_pop(Heap* h)
{
	if (!h->size)
	{
		Chunk empty = { 0, 0, 0, false, NULL };
		return empty;
	}
	int parent = 1, child = 2;
	Chunk num = h->heap[1], temp = h->heap[(h->size)--];
	while (child <= h->size)
	{
		if ((child < h->size) && h->heap[child + 1].size > h->heap[child].size)
		{
			child++;
		}
		if (h->heap[child].size <= temp.size)
		{
			break;
		}
		h->heap[parent] = h->heap[child];
		parent = child;
		child *= 2;
	}
	h->heap[parent] = temp;
	return num;
}