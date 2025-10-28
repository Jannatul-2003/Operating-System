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
#include <syscall_def.h>
#include <errno.h>
#include <kstdio.h>
#include "syscall_kern.h"
#include "syscall.h" // our new header

void syscall(uint16_t callno, uint32_t *stack_frame)
{
	int retval = 0;
	uint32_t arg1 = stack_frame[0];
	uint32_t arg2 = stack_frame[1];
	uint32_t arg3 = stack_frame[2];

	switch (callno)
	{
	case SYS_read:
		retval = k_sys_read((int)arg1, (char *)arg2, (size_t)arg3);
		break;

	case SYS_write:
		retval = k_sys_write((int)arg1, (const char *)arg2, (size_t)arg3);
		break;

	case SYS_reboot:
		k_sys_reboot();
		retval = 0;
		break;

	case SYS_exit:
		k_sys_exit((int)arg1);
		retval = 0;
		break;

	case SYS_getpid:
		retval = k_sys_getpid();
		break;

	case SYS_time:
		retval = (int)k_sys_time_ms();
		break;

	case SYS_yield:
		k_sys_yield();
		retval = 0;
		break;

	default:
		retval = -ENOSYS;
		kprintf("syscall: unsupported call %d\n", callno);
		break;
	}

	// Return value goes to r0 (so user gets it)
	stack_frame[0] = (uint32_t)retval;
}
