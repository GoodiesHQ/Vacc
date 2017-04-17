#include "vacc.h"

void *vacc_memmove(void *dst, void *src, size_t size)
{
    char *d = (char*)dst;
    char *s = (char*)src;

    for(size_t i = 0; i < size; ++i)
    {
        *d++ = *s++;
    }
    return dst;
}

void *vacc_memset(void *dst, char c, size_t n)
{
    char *d = (char*)dst;
    while(n--)
        *d++ = c;
    return dst;
}

status vacc_save(vacc *v)
{
    if(ptrace(PTRACE_GETREGS, v->pid, NULL, &v->state) == -1)
    {
        DBG("Could not retrieve registers.\n");
        return FAILURE;
    }
    return SUCCESS;
}

status vacc_restore(vacc *v)
{
    if(ptrace(PTRACE_SETREGS, v->pid, NULL, &v->state) == -1)
    {
        DBG("Could not restore registers.\n");
        return FAILURE;
    }
    return SUCCESS;
}

vacc vacc_create(pid_t pid)
{
    vacc v;
    v.pid = pid;
    v.addr = (uintptr_t)NULL; 

    return v;
}

status vacc_mmap_alloc(vacc *v, void *addr, size_t size, int prot, int flags)
{
    vacc_save(v);

    struct user_regs_struct regs_new = {0};
    vacc_memmove(&regs_new, &v->state, sizeof(struct user_regs_struct));

#if defined(__i386__) 
    regs_new.eax = 0x5a;
    regs_new.ebx = 0;
    regs_new.ecx = size;
    regs_new.edx = prot;    //PROT_READ | PROT_WRITE | PROT_EXEC;
    regs_new.esi = flags;   //MAP_ANON | MAP_PRIVATE;
    regs_new.edi = -1;
    regs_new.ebp = 0;
#elif defined(__x86_64__)
    regs_new.rax = 0x09;
    regs_new.rdi = 0;
    regs_new.rsi = size;
    regs_new.rdx = prot;    //PROT_READ | PROT_WRITE | PROT_EXEC;
    regs_new.rcx = flags;   //MAP_ANON | MAP_PRIVATE;
    regs_new.r8 = -1;
    regs_new.r9 = 0;
#endif

    if(vacc_continue(v) == FAILURE || vacc_wait_stopped(v) == FAILURE)
    {
        return FAILURE;
    }

}

status vacc_attach(vacc *v)
{
    if(ptrace(PTRACE_ATTACH, v->pid, NULL, NULL) == -1)
    {
        DBG("Could not attach to %d.\n", v->pid);
        return FAILURE;
    }
    return SUCCESS;
}

status vacc_detach(vacc *v)
{
    if(ptrace(PTRACE_DETACH, v->pid, NULL, NULL) == -1)
    {
        DBG("Could not detach from %d.\n", v->pid);
        return FAILURE;
    }
    return SUCCESS;
}

status vacc_continue(vacc *v)
{
    if(ptrace(PTRACE_CONT, v->pid, NULL, NULL) == -1)
    {
        DBG("Could not continue on %d\n", v->pid);
        return FAILURE;
    }
    return SUCCESS;
}

status vacc_wait_stopped(vacc *v)
{
    bool good = false;

    while(!good)
    {
        int status = 0;
        if(waitpid(v->pid, &status, 0) == -1)
        {
            DBG("Could not call waitpid on %d.\n", v->pid);
            return FAILURE;
        }
        if(WIFSTOPPED(status))
        {
            if(WSTOPSIG(status) != SIGTRAP)
            {
                good = true;
            }
        }
    }
    return SUCCESS;
}

status vacc_wait_syscall(vacc *v)
{
    struct user_regs_struct regs = {0};
    long accum;
    do {
        if(ptrace(PTRACE_SYSCALL, v->pid, NULL, NULL) == -1)
        {
            DBG("Could not ptrace syscall.\n");
            return FAILURE;
        }
        vacc_wait_stopped(v);
        accum = ptrace(PTRACE_PEEKUSER, v->pid, ACCUM_OFFSET(regs), NULL);
    } while(accum == -ENOSYS);
    return SUCCESS;
}

status vaccinate(vacc *v, char *data, size_t size)
{
    if(vacc_attach(v) == FAILURE)
        return FAILURE;

    if(vacc_wait_stopped(v) == FAILURE)
        goto detach_and_fail;

    sleep(3);

    vacc_detach(v);

    return SUCCESS;

detach_and_fail:
    vacc_detach(v);
    return FAILURE;
}
