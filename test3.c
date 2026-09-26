#include <stdio.h>
int main()
{
    int a = 60;
    int b = 30;
    int c = 40;

    if (a > b && a > c) {
        printf("%d\n", a);
    } else if (b > a && b > c) {
        printf("%d\n", b);
    } else {
        printf("%d\n", c);
    }
    
    return 0;
    
};
