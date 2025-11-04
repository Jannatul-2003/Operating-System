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
 
#include <cm4.h>
#include <sys_clock.h>
#include <syscall.h>



volatile uint32_t ms_tick = 0;
volatile uint32_t sec_tick = 0;
volatile uint32_t min_tick = 0;
volatile uint32_t hour_tick = 0;


void __SysTick_init(uint32_t reload)
{
    reload =  (180 * reload);
    SYSTICK->CTRL &= ~(1 << 0);
    // Hardware counts LOAD→0 (inclusive), so we subtract 1 to get exact cycle count
    SYSTICK->LOAD = ((reload-1) & SysTick_LOAD_RELOAD_Msk);
    SYSTICK->VAL = 0;
    SYSTICK->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void SysTickIntDisable(void)
{
    SYSTICK->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void SysTickIntEnable(void)
{
    SYSTICK->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

void __SysTick_enable(void)
{
    SYSTICK->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void __SysTick_disable(void)
{
    SYSTICK->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

uint32_t __getSysTickCount(void)
{
    return ms_tick;
}

void __updateSysTick(uint32_t count)
{
    ms_tick = count;
    sec_tick = count / 1000;
    min_tick = sec_tick / 60;
    hour_tick = min_tick / 60;
}

uint32_t __getTime(void)
{
    return ms_tick;
}

uint32_t __get__Second(void)
{
    return sec_tick;
}

uint32_t __get__Minute(void)
{
    return min_tick;
}

uint32_t __get__Hour(void)
{
    return hour_tick;
}

// SysTick interrupt handler
void SysTick_Handler(void)
{
    ms_tick++;
    if (ms_tick % 1000 == 0) {
        sec_tick++;
        if (sec_tick % 60 == 0) {
            min_tick++;
            if (min_tick % 60 == 0) {
                hour_tick++;
            }
        }
    }
}

uint8_t ms_delay(uint32_t delay)
{
    uint32_t start = ms_tick;
    while ((ms_tick - start) < delay) {
        __NOP();
    }
    return 0;
}

uint32_t getmsTick(void)
{
    return ms_tick;
}

uint32_t wait_until(uint32_t delay)
{
    uint32_t start = ms_tick;
    while ((ms_tick - start) < delay) {
    }
    return ms_tick;
}

void __enable_fpu(void) {
    SCB->CPACR |= (0xF << 20); // enable CP10 and CP11 full access
    __DSB();
    __ISB();
}


void SYS_SLEEP_WFI(void)
{
    __WFI();
}