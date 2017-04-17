#ifndef VACC_H
#define  VACC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/mman.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#ifdef DEBUG
#define DBG(...) fprintf(stderr, __VA_ARGS__)
#else
#define DBG(...)
#endif

#if defined(__i386__)

#define ACCUM_OFFSET(x) offsetof(struct user, x.eax)
#define INSTR_OFFSET(x) offsetof(struct user, x.eip)

#elif defined(__x86_64__)

#define ACCUM_OFFSET(x) offsetof(struct user, x.rax)
#define INSTR_OFFSET(x) offsetof(struct user, x.rip)

#else
#pragma error("Not Supported")
#endif

typedef enum _status
{
    SUCCESS,
    FAILURE,
} status;

typedef struct _vacc
{
    pid_t pid;
    struct user_regs_struct state;
    uintptr_t addr;
} vacc;

void *vacc_memmove(void *dest, void *src, size_t size);
void *vacc_memmove(void *dest, void *src, size_t size);

status vaccinate(vacc *v, char *data, size_t size);

vacc vacc_create(pid_t pid);
status vacc_mmap_alloc(vacc *v, void *addr, size_t size, int prot, int flags);

status vacc_save(vacc *v);
status vacc_restore(vacc *v);

status vacc_attach(vacc *v);
status vacc_detach(vacc *v);
status vacc_continue(vacc *v);
status vacc_wait_stopped(vacc *v);
status vacc_wait_syscall(vacc *v);

#endif//VACC_H
