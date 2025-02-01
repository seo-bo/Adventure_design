#include "sub.h"

void insert_chunk(Chunk** head, Chunk* newnode)// 공통으로 사용가능한 연결리스트 삽입 함수
{
	if (*head == NULL) // head가 존재하지않을때
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

void clear() // 화면을 비워주는 함수
{
	system("cls");
}

int MIN(int a, int b) // 최솟값 반환
{
	return (a > b) ? b : a;
}

int MAX(int a, int b) // 최댓값 반환
{
	return (a > b) ? a : b;
}

void Best_shrink(Chunk** head, Heap* h, int newsize) // 힙과 연결리스트의 head를 이용해서 전체 크기를 줄이는 함수
{
	// 조건 확인
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
	if (now->start <= newsize + 1 && newsize <= now->end) // 스택과 힙을 이용하여 크기 줄이기
	{
		bool flag = (now->start == newsize + 1);
		Chunk* temp = (Chunk*)malloc(h->size * sizeof(Chunk));
		int top = 0;
		while (h->size)
		{
			temp[top] = Best_pop(h);
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
			Best_push(h, temp[i]);
		}
		free(temp);
		total = newsize;
	}
	else
	{
		printf("줄일 수 없습니다.\n");
	}
}

void Best_expand(Chunk** head, Heap* h, int newsize) // 힙과 연결리스트의 head를 이용해서 전체 크기를 늘리는 함수
{
	// 조건 확인
	if (newsize <= total)
	{
		printf("늘릴 수 없습니다.\n");
		return;
	}
	total = newsize;
	if (*head == NULL) // head가 없는 경우 처리
	{
		Chunk* newnode = (Chunk*)malloc(sizeof(Chunk));
		newnode->start = 1;
		newnode->end = newsize;
		newnode->size = newsize;
		newnode->used = false;
		newnode->right = newnode;
		newnode->left = newnode;
		*head = newnode;
		Best_push(h, *newnode);
		return;
	}
	Chunk* now = *head;
	int endpoint = now->end;
	do // 연결리스트를 순회하면서 끝 위치의 노드를 찾음.
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
	Best_push(h, *newnode);
	sorted = false;
}