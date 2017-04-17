#include <unistd.h>
#include <stdio.h>

int main()
{
    int i = 0;
    while(1)
    {
        printf("PID: %u - %d\n", getpid(), i++);
        sleep(1);
    }
    return 0;
}
