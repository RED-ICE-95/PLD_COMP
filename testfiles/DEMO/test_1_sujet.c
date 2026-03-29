#include <stdio.h>

int alphabet(int n)
{
    int a = 'A';
    while (a < 'A' + n)
    {
        putchar(a);
        a = a + 1;
    }
    return a;
}

int main()
{
    int c;
    c = alphabet(15);
    return c;
}