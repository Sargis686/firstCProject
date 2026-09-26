#include <stdio.h>
int main()
{

    int numbers[3] = {60, 30, 40};
    int biggist = numbers[0];
    for (int i = 0; i < 3; i++)
    {
        if (biggist < numbers[i])
        {
            biggist = numbers[i];
        }
    }
    printf("%d\n", biggist);
    return 0;
};
