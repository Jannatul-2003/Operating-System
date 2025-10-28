#include "syscall_kern.h"
#include "types.h"
#include "kstdio.h"
#include "dev/usart.h"
#include "sys_init.h"

extern TCB_TypeDef *current_tcb;

int k_sys_write(int fd, const char *buf, size_t size)
{
    if (!buf)
        return -1;
    for (size_t i = 0; i < size; i++)
    {
        if (buf[i] == '\0')
            break;
        usart_write((uint8_t)buf[i]); // your UART driver function
    }
    return (int)size;
}

int k_sys_read(int fd, char *buf, size_t size)
{
    if (!buf)
        return -1;
    for (size_t i = 0; i < size; i++)
    {
        buf[i] = usart_read_blocking();
        if (buf[i] == '\n')
            break;
    }
    return (int)size;
}

void k_sys_exit(int status)
{
    if (current_tcb)
        current_tcb->status = 0; // terminated
    kprintf("Process exited with status %d\n", status);
    k_sys_yield();
    while (1)
        ;
}

int k_sys_getpid(void)
{
    return current_tcb ? current_tcb->task_id : -1;
}

uint32_t k_sys_time_ms(void)
{
    return get_systick_ms();
}

void k_sys_reboot(void)
{
    __disable_irq();
    SCB->AIRCR = (0x5FA << 16) | (1 << 2);
    while (1)
        ;
}

void k_sys_yield(void)
{
    SCB->ICSR |= (1 << 28); // PendSV set
}
