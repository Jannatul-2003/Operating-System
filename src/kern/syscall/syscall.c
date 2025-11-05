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

#include <syscall.h>
#include <syscall_def.h>
#include <errno.h>
#include <errmsg.h>
#include <stdint.h>
#include <kunistd.h>
#include <UsartRingBuffer.h>
#include <system_config.h>
#include <cm4.h>

// void syscall(uint16_t callno)
// {
// /* The SVC_Handler calls this function to evaluate and execute the actual function */
// /* Take care of return value or code */
// 	switch(callno)
// 	{
// 		/* Write your code to call actual function (kunistd.h/c or times.h/c and handle the return value(s) */
// 		case SYS_read:
// 			break;
// 		case SYS_write:
// 			break;
// 		case SYS_reboot:
// 			break;
// 		case SYS__exit:
// 			break;
// 		case SYS_getpid:
// 			break;
// 		case SYS___time:
// 			break;
// 		case SYS_yield:
// 			break;
// 		/* return error code see error.h and errmsg.h ENOSYS sys_errlist[ENOSYS]*/
// 		default: ;
// 	}
// /* Handle SVC return here */
// }

uint32_t sys_read(uint32_t fd, uint8_t *buf, uint32_t len)
{
	if (fd != STDIN_FILENO || buf == 0 || len == 0U)
	{
		return 0U;
	}
	if (len > 256U)
	{
		len = 256U; // Limit buffer size
	}

	// Read from UART with echo
	uint32_t count = 0U;
	while (count < len)
	{
		while (IsDataAvailable(__CONSOLE) == 0)
		{
			// Busy wait for data
		}
		int c = Uart_read(__CONSOLE);
		if (c < 0)
		{
			break;
		}
		
		// Echo the character back
		Uart_write((uint8_t)c, __CONSOLE);
		
		// Store the character
		buf[count++] = (uint8_t)c;
		
		// Handle backspace
		if ((uint8_t)c == 0x7F || (uint8_t)c == 0x08) {
			if (count >= 2) {
				count -= 2; // Remove backspace and previous char
			} else {
				count = 0;
			}
			continue;
		}
		
		// Break on Enter/Return
		if ((uint8_t)c == '\r' || (uint8_t)c == '\n')
		{
			Uart_write('\n', __CONSOLE); // Send newline
			break;
		}
	}
	
	if (count < len)
	{
		buf[count] = '\0'; // Null terminate
	}
	return count; // Return bytes read
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
		uint32_t fd = a0;
		uint8_t *buf = (uint8_t *)a1;
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