#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(){
    int ids[100];
    int i;
    getchar();
    getchar();
    getchar();
    do
        {
        scanf("%d", &ids[i]);
        i++;
        } while (getchar() != ']');

    for (int j = 0; j < i; j++)
     printf("%d", ids[j]);
    return 0;
}