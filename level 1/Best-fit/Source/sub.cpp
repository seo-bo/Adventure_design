#include "sub.h"

int total = 0;
bool sorted = true;

Chunk* Best_my_alloc(Chunk** head, Heap* h, int size) // best-fit 기반 메모리 할당 함수
{
	bool check = false;
	Chunk* ary = (Chunk*)malloc(sizeof(Chunk) * 10000);
	int idx = 0;
	Chunk pivot = { 0,0,0,false,NULL,NULL };
	while (h->size > 0) // 힙에서 가용공간 빼서 체크하기
	{
		ary[idx++] = Best_pop(h);
		if (ary[idx - 1].size >= size && !check)
		{
			pivot = ary[idx - 1];
			check = true;
		}
	}
	if (!check) // 가용공간 없으면 다시 넣고 return
	{
		for (int i = 0; i < idx; ++i)
		{
			Best_push(h, ary[i]);
		}
		free(ary);
		return NULL;
	}
	int old = pivot.start;
	Chunk* now = *head;
	do // 가용공간 찾았으면 연결리스트 데이터 수정
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
	// 수정한 데이터 기반으로 연결리스트 삽입
	Chunk* newnode = (Chunk*)malloc(sizeof(Chunk));
	newnode->size = size;
	newnode->end = old + size - 1;
	newnode->start = old;
	newnode->used = true;
	insert_chunk(head, newnode);
	now = *head;
	do // 힙에 다시 가용공간 삽입
	{
		if (!now->used)
		{
			Best_push(h, *now);
		}
		now = now->right;
	} while (now != *head);
	free(ary);
	return newnode;
}

void Best_my_free(Heap* h, Chunk* node) // best-fit 기반 메모리 해제 함수
{
	if (node == NULL)
	{
		return;
	}
	node->used = false; // Chunk* 주소에 접근해서 used 상태 바꿔주기
	Best_push(h, *node);
}
