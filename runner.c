#include <sys/mman.h>
#include <string.h>
//#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char **argv)
{

    const unsigned char shellcode[] ="";

    void *addr = mmap(NULL, sizeof(shellcode), PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if(addr == MAP_FAILED)
    {
        fprintf(stderr, "mmap returned NULL.\n");
        return -1;
    }
    memcpy(addr, shellcode, sizeof(shellcode));

    if(mprotect(addr, sizeof(shellcode), PROT_READ | PROT_EXEC))
    {
        fprintf(stderr, "mprotect failed (%d)\n", errno);
        return -1;
    }

    int (*sc)(void) = NULL;
    sc = addr;

    printf("Size: %d\n", sizeof(shellcode));

    int x = sc();

    return 0;
}
