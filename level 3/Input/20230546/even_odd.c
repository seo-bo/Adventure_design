
#include <stdio.h>

int main(void) 
{
    int num = 0, check = 0;
    scanf("%d", &num);
	if (num % 2 == 0)
	{
		check = 1;
	}
	else
	{
		check = 0;
	}
    printf("%d\n", check);
    return 0;
}
