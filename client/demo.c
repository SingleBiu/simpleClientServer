#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char const *argv[])
{
    char buf[128] = {0};
    if (sizeof(argv[1]) > 127)
    {
        return 0;
    }
    strncpy(buf,argv[1],sizeof(argv[1]));
    printf("Get:|%s| argc:%d\n",buf,argc);
    return 0;
}
