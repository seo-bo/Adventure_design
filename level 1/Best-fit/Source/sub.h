#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <memory.h>

typedef struct restore
{
	char* name;
	int start;
}restore;

typedef struct Chunk
{
	int start, end, size;
	bool used;
	struct Chunk* left;
	struct Chunk* right;
}Chunk;

typedef struct Node
{
	char* name;
	Chunk* address;
	struct Node* left;
	struct Node* right;
}Node;

typedef struct Heap
{
	Chunk heap[1000000];
	int size;
}Heap;

extern int total;
extern bool sorted;

void insert_chunk(Chunk** head, Chunk* newnode);
void clear();
int MIN(int a, int b);
int MAX(int a, int b);
void Best_shrink(Chunk** head, Heap* h, int newsize);
void Best_expand(Chunk** head, Heap* h, int newsize);
Heap* make_heap(void);
void Best_push(Heap* h, Chunk num);
Chunk Best_pop(Heap* h);
Chunk* Best_my_alloc(Chunk** head, Heap* h, int size);
void Best_my_free(Heap* h, Chunk* node);
void merge(Chunk ary[], int left, int mid, int right);
void sort(Chunk ary[], int left, int right);
Chunk* Best_empty_merge(Heap* h, int* head);
void Best_merge_sort(Node** node, Chunk** head, Heap* h);
Node* make_tree(char* str, Chunk* adr);
Node* search(Node* node, char* str);
Node* insert(Node* node, char* str, Chunk* adr);
Node* search_min(Node* node);
Node* del(Node* node, char* str);
void inorder(Node* node, restore* adx, int* idx);













