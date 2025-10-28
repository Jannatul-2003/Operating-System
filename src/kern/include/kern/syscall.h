#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include <stdint.h>

/*
 * ======================================================
 * DUOS Kernel System Call Dispatcher Interface
 * ------------------------------------------------------
 * Used by SVC_Handler_C() to call the main dispatcher.
 * ======================================================
 */
void syscall(uint16_t callno, uint32_t *stack_frame);

#endif /* _SYSCALL_H_ */
