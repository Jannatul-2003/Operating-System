#ifndef _SYSCALL_KERN_H_
#define _SYSCALL_KERN_H_

#include <stdint.h>
#include <stddef.h>

/*
 * ======================================================
 * DUOS Kernel-Side System Call Interface
 * ------------------------------------------------------
 * This header declares all privileged (k_sys_*) functions
 * that are implemented inside the kernel and invoked
 * by syscall_dispatch().
 * ======================================================
 */

/* Kernel-side syscall handlers (privileged) */
int k_sys_write(int fd, const char *buf, size_t size);
int k_sys_read(int fd, char *buf, size_t size);
void k_sys_exit(int status);
int k_sys_getpid(void);
uint32_t k_sys_time_ms(void);
void k_sys_reboot(void);
void k_sys_yield(void);

#endif /* _SYSCALL_KERN_H_ */
