#include "sub.h"

void insert_chunk(Chunk** head, Chunk* newnode)// �������� ��밡���� ���Ḯ��Ʈ ���� �Լ�
{
	if (*head == NULL) // head�� ��������������
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

void clear() // ȭ���� ����ִ� �Լ�
{
	system("cls");
}

int MIN(int a, int b) // �ּڰ� ��ȯ
{
	return (a > b) ? b : a;
}

int MAX(int a, int b) // �ִ� ��ȯ
{
	return (a > b) ? a : b;
}

void Best_shrink(Chunk** head, Heap* h, int newsize) // ���� ���Ḯ��Ʈ�� head�� �̿��ؼ� ��ü ũ�⸦ ���̴� �Լ�
{
	// ���� Ȯ��
	if (newsize >= total || newsize < 1 || *head == NULL)
	{
		printf("���� �� �����ϴ�.\n");
		return;
	}
	Chunk* now = (*head)->left;
	if (now->used)
	{
		printf("���� �� �����ϴ�.\n");
		return;
	}
	if (now->start <= newsize + 1 && newsize <= now->end) // ���ð� ���� �̿��Ͽ� ũ�� ���̱�
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
		printf("���� �� �����ϴ�.\n");
	}
}

void Best_expand(Chunk** head, Heap* h, int newsize) // ���� ���Ḯ��Ʈ�� head�� �̿��ؼ� ��ü ũ�⸦ �ø��� �Լ�
{
	// ���� Ȯ��
	if (newsize <= total)
	{
		printf("�ø� �� �����ϴ�.\n");
		return;
	}
	total = newsize;
	if (*head == NULL) // head�� ���� ��� ó��
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
	do // ���Ḯ��Ʈ�� ��ȸ�ϸ鼭 �� ��ġ�� ��带 ã��.
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