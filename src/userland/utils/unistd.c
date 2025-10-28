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
#include "unistd.h"
#include "syscall_def.h"

static inline int svc_call(uint16_t callno, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
    register uint32_t r0 __asm__("r0") = arg1;
    register uint32_t r1 __asm__("r1") = arg2;
    register uint32_t r2 __asm__("r2") = arg3;
    register uint32_t r3 __asm__("r3") = callno;
    __asm__ volatile("svc 0" : "+r"(r0) : "r"(r1), "r"(r2), "r"(r3));
    return (int)r0;
}

int write(int fd, const char *buf, size_t size)
{
    return svc_call(SYS_write, fd, (uint32_t)buf, size);
}

int read(int fd, char *buf, size_t size)
{
    return svc_call(SYS_read, fd, (uint32_t)buf, size);
}

int getpid(void)
{
    return svc_call(SYS_getpid, 0, 0, 0);
}

void exit(int code)
{
    svc_call(SYS__exit, code, 0, 0);
    while (1)
        ;
}

void yield(void)
{
    svc_call(SYS_yield, 0, 0, 0);
}

void reboot(void)
{
    svc_call(SYS_reboot, 0, 0, 0);
}
