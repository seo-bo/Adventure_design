
#include <stdio.h>

int main(void)
{
    int num1 = 0, num2 = 0, result = 0;
    scanf("%d", &num1);
    scanf("%d", &num2);
    result = num1 + num2;
    printf("%d\n", result);
    result = num1 - num2;
    printf("%d\n", result);
    result = num1 * num2;
    printf("%d\n", result);
    if (num2 != 0)
	{
        result = num1 / num2;
        printf("%d\n", result);
        result = num1 % num2;
        printf("%d\n", result);
    }
    return 0;
}
