#include <stdint.h>
#include "syscall.h"
#include "syscall_def.h"

void SVC_Handler_C(uint32_t *stack_frame)
{
    // Syscall ID passed via R3
    uint16_t callno = (uint16_t)stack_frame[3];
    syscall(callno, stack_frame);
}
