#include <sys/mman.h>
#include <string.h>
//#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char **argv)
{

    const unsigned char shellcode[] = \
                                      "\x55\x89\xe5\x31\xd2\xb6\x0a\xc1\xe2\x08\x66\xba\x65\x72\x52\x68\x6e\x69\x67\x67\x68\x20\x75\x70\x20\x68\x53\x68\x75\x74\x31\xc0\xb0\x04\x31\xdb\x43\x89\xe1\x31\xd2\xb2\x0f\xcd\x80\x89\xec\x5d\xc3";

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
