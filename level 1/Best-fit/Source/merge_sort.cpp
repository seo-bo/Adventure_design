#include "sub.h"

void merge(Chunk ary[], int left, int mid, int right) // Chunk 기반 배열로 병합 정렬 수행
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

void sort(Chunk ary[], int left, int right) // 재귀함수로 병합 정렬 구현
{
	if (left < right)
	{
		int mid = (left + right) / 2;
		sort(ary, left, mid);
		sort(ary, mid + 1, right);
		merge(ary, left, mid, right);
	}
}

Chunk* Best_empty_merge(Heap* h, int* head)  // 빈 가용공간 먼저 병합 정렬 수행 및 병합
{
	if (h->size == 0)
	{
		return NULL;
	}
	int idx = 0;
	Chunk* temp = (Chunk*)malloc(h->size * sizeof(Chunk));
	while (h->size) // 힙에서 데이터를 제거
	{
		temp[idx++] = Best_pop(h);
	}
	sort(temp, 0, idx - 1);
	Chunk* ary = (Chunk*)malloc(total * sizeof(Chunk));
	int newidx = 0;
	ary[newidx] = temp[0];
	for (int i = 1; i < idx; ++i) // 가용 공간 병합
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
	for (int i = 0; i < *head; ++i) // 다시 힙에 넣어줌
	{
		Best_push(h, ary[i]);
	}
	return ary;
}

void Best_merge_sort(Node** node, Chunk** head, Heap* h)  // 전체적인 병합 정렬 수행
{
	int top = 0, adtop = 0;
	Chunk* ary = Best_empty_merge(h, &top);
	restore* adx = (restore*)malloc(sizeof(restore) * 10000);
	if (ary == NULL)
	{
		free(adx);
		return;
	}
	inorder(*node, adx, &adtop);  // 중위 순회 하면서 이름과 시작주소 수집
	Chunk* a = *head;
	do // 연결리스트 순회하면서 노드 수집(free도 같이)
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
	for (int i = 0; i < top; ++i) // 새로운 연결리스트 생성 및 이진 탐색 트리 주소 덮어씌우기
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

