#include "sub.h"

Node* make_tree(char* str, Chunk* adr)
{
	Node* newnode = (Node*)malloc(sizeof(Node));
	newnode->name = (char*)malloc(strlen(str) + 1);
	memcpy(newnode->name, str, strlen(str) + 1);
	newnode->address = adr;
	newnode->left = newnode->right = NULL;
	return newnode;
}

Node* search(Node* node, char* str)
{
	if (node == NULL)
	{
		return NULL;
	}
	int pivot = strcmp(node->name, str);
	if (pivot == 0)
	{
		return node;
	}
	if (pivot > 0)
	{
		return search(node->left, str);
	}
	else
	{
		return search(node->right, str);
	}
}

Node* insert(Node* node, char* str, Chunk* adr)
{
	if (node == NULL)
	{
		return make_tree(str, adr);
	}
	int pivot = strcmp(node->name, str);
	if (pivot > 0)
	{
		node->left = insert(node->left, str, adr);
	}
	else if (pivot < 0)
	{
		node->right = insert(node->right, str, adr);
	}
	else
	{
		node->address = adr;
	}
	return node;
}

Node* search_min(Node* node)
{
	while (node->left != NULL)
	{
		node = node->left;
	}
	return node;
}

Node* del(Node* node, char* str)
{
	if (node == NULL)
	{
		return NULL;
	}
	int pivot = strcmp(node->name, str);
	if (pivot > 0)
	{
		node->left = del(node->left, str);
	}
	else if (pivot < 0)
	{
		node->right = del(node->right, str);
	}
	else
	{
		if (node->left == NULL)
		{
			Node* temp = node->right;
			free(node->name);
			free(node);
			return temp;
		}
		else if (node->right == NULL)
		{
			Node* temp = node->left;
			free(node->name);
			free(node);
			return temp;
		}
		Node* temp = search_min(node->right);
		node->name = temp->name;
		node->address = temp->address;
		node->right = del(node->right, temp->name);
	}
	return node;
}

void inorder(Node* node, restore* adx, int* idx)
{
	if (node == NULL)
	{
		return;
	}
	inorder(node->left, adx, idx);
	adx[*idx].name = (char*)malloc(strlen(node->name) + 1);
	strcpy(adx[*idx].name, node->name);
	adx[*idx].start = node->address->start;
	(*idx)++;
	inorder(node->right, adx, idx);
}