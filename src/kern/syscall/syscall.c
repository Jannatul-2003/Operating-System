/*
 * Copyright (c) 2022
 * Computer Science and Engineering, University of Dhaka
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "syscall.h"
#include "syscall_def.h"
#include <string.h>
#include <errno.h>
#include <errmsg.h>
#include <types.h>
#include <kunistd.h>
#include <UsartRingBuffer.h>
#include <system_config.h>
#include <cm4.h>
#include <kmain.h>
#include <kstdio.h>


#define UART_TX_READY (1U << 7)
#define UART_RX_READY (1U << 5)
#define UART_BASE 0x40004400U   // USART2 base for many F4 variants (adjust if needed)
#define UART_SR   (*(volatile uint32_t*)(UART_BASE + 0x00))
#define UART_DR   (*(volatile uint32_t*)(UART_BASE + 0x04))
int32_t sys_read(uint32_t fd, uint8_t *buf, uint32_t len)

static char uart_recv_char(void)
{
    while (!(UART_SR & UART_RX_READY));  // Wait until data received
    return (char)(UART_DR & 0xFF);       // Read received byte
}
uint32_t sys_read(int fd, char *buf, uint32_t size)
{
    if (buf == NULL || size == 0)
    {
        return -1;
    }

    if (fd == STDIN_FILENO)
    {
        uint32_t bytes_read = 0;
        uint32_t max_size = (size > 256) ? 256 : size; // Limit to 256 bytes

        for (uint32_t i = 0; i < max_size; i++)
        {
            uint32_t c = uart_recv_char();
            if (c == -1)
            {
                break;
            }

            buf[i] = (char)c;
            bytes_read++;

            // Check for termination character
            if (c == '\n' || c == '\r' || c == '\0')
            {
                break;
            }
        }
        return (uint32_t)bytes_read;

    }

    return -1;
}
uint32_t sys_write(uint32_t fd, const uint8_t *buf, uint32_t len)
{
	if (fd != STDOUT_FILENO || buf == 0 || len == 0U)
	{
		return 0U;
	}
	for (uint32_t i = 0; i < len; i++)
		Uart_write(buf[i], __CONSOLE);

	return len;
}

/* Define the global current task id here (single definition). */
volatile uint32_t g_current_task_id = 0;

uint32_t syscall_dispatch(uint16_t callno, uint32_t a0, uint32_t a1,
						  uint32_t a2, uint32_t a3)
{
	switch (callno)
	{

	case SYS_read:
	{ // syscall #50
		int fd = a0;
		char *buf = (char *)a1;
		uint32_t len = a2;

		return sys_read(fd, buf, len);
	}

	case SYS_write:
	{
		// syscall #55
		uint32_t fd = a0;
		const uint8_t *buf = (const uint8_t *)a1;
		uint32_t len = a2;
		return sys_write(fd, buf, len);
	}

	case SYS_reboot:		  // syscall #119
		__NVIC_SystemReset(); // Trigger system reset
		return 0U;

	case SYS__exit:							// syscall #3
		SCB->ICSR = SCB_ICSR_PENDSVSET_Msk; // Trigger PendSV
		return 0U;

	case SYS_getpid: // syscall #5
		return (uint32_t)g_current_task_id;

	case SYS___time:		// syscall #113
		return __getTime(); // Get system time in ms

	case SYS_yield:							// syscall #120
		SCB->ICSR = SCB_ICSR_PENDSVSET_Msk; // Voluntary context switch
		return 0U;

	default:
		return (uint32_t)ENOSYS; // Error: Not implemented
	}
}