#include "sub.h"

city create_city(int id, long long x, long long y)
{
	city c;
	c.id = id;
	c.x = x;
	c.y = y;
	return c;
}

long long calculate(city a, city b)
{
	long long dx = a.x - b.x;
	long long dy = a.y - b.y;
	return sqrt(dx * dx + dy * dy);
}

void init_mat()
{
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < count; j++)
		{
			if (i == j)
			{
				distance[i][j] = 0;
			}
			else
			{
				distance[i][j] = calculate(cities[i], cities[j]);
			}
		}
	}
}

int* make_permutation()
{
	for (int i = 0; i < count; i++)
	{
		permutation[i] = i;
	}
	return permutation;
}

void swap(int i, int j)
{
	int temp = current[i];
	current[i] = current[j];
	current[j] = temp;
}

void inversion(int pre, int next)
{
	while (pre < next) 
	{
		swap(pre, next);
		pre++;
		next--;
	}
}

bool next_permutation(int* begin, int* end)
{
	if (begin == end)
	{
		return false;
	}
	int* i = end - 1;
	while (i > begin && *(i - 1) >= *i)
	{
		--i;
	}
	if (i == begin)
	{
		for (int* left = begin, *right = end - 1; left < right; ++left, --right)
		{
			int temp = *left;
			*left = *right;
			*right = temp;
		}
		return false;
	}
	int* j = end - 1;
	while (*j <= *(i - 1))
	{
		--j;
	}
	int temp = *(i - 1);
	*(i - 1) = *j;
	*j = temp;
	for (int* left = i, *right = end - 1; left < right; ++left, --right)
	{
		temp = *left;
		*left = *right;
		*right = temp;
	}
	return true;
}
