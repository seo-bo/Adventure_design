
#include <stdio.h>

int main(void) 
{
    int num = 0, div = 2;
    scanf("%d", &num);
    while (num != 1) 
	{
        if (num % div == 0) 
		{
            printf("%d\n", div);
            num = num / div;
        } 
		else
		{
            div++;
        }
    }
    return 0;
}
