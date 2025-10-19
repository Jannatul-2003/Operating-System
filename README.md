# Operating Systems - Comprehensive Study Notes

## Part 1: Computer Hardware Fundamentals

### Basic Components & Architecture

**Q: What are the basic elements of a computer system?**

A: A computer system consists of:
- **CPU**: Performs computations and loads/stores data via system bus
- **Memory Controller**: Refreshes dynamic RAM and arbitrates access between devices and CPU
- **Device Controllers**: Control specific devices, operate in parallel with CPU, can access memory directly (DMA), and signal CPU with interrupts
- **System Bus**: Connects all components for data transfer

**Q: What is the fetch-execute cycle?**

A: The CPU repeatedly:
1. Loads instruction from memory at address in Program Counter (PC)
2. Decodes the instruction
3. Executes the instruction (generates control signals)
4. Increments PC
5. Repeats

This can be interrupted by external events (interrupts).

### CPU Registers & Modes

**Q: What are the main CPU registers and their purposes?**

A: 
- **Program Counter (PC)**: Points to next instruction
- **Stack Pointer (SP)**: Points to top of stack
- **Status Register**: Contains condition codes (positive, zero, negative results)
- **General Purpose Registers (R0-R12)**: Hold operands, minimize memory access
- **Link Register (LR/R14)**: Stores return address for subroutines

**Q: What's the difference between user mode and kernel mode?**

A:
- **User Mode (Unprivileged)**:
  - Uses 'safe' subset of instructions
  - Limited register access
  - Cannot disable interrupts
  - Cannot directly access hardware
  
- **Kernel Mode (Privileged/Supervisor)**:
  - All instructions available
  - Full register access
  - Can modify system configuration
  - Direct hardware access

**Q: Why do we need two modes?**

A: To protect the OS from application interference. Applications can only affect their own state and cannot:
- Interfere with the OS
- Interfere with other applications
- Violate OS security policies

### Interrupts

**Q: What is an interrupt and why is it important?**

A: An interrupt is a suspension of normal execution caused by an external event, performed so processing can be resumed later. Benefits:
- Improves CPU efficiency
- CPU can execute other instructions during I/O operations
- Avoids wasteful polling

**Q: What are the two main classes of interrupts?**

A:
1. **Synchronous (Program Exceptions)**:
   - Arithmetic overflow
   - Division by zero
   - Illegal instructions
   - Invalid memory access
   
2. **Asynchronous (External Events)**:
   - Timer interrupts
   - I/O device signals
   - Hardware failures

**Q: How does the interrupt cycle work?**

A:
1. Processor checks for pending interrupts
2. If none: fetch next instruction
3. If interrupt pending:
   - Save current state (PC, SP, registers)
   - Switch to kernel mode
   - Jump to interrupt handler
   - Execute handler
   - Restore saved state
   - Return to interrupted program

**Q: What's the difference between simple and nested interrupts?**

A:
- **Sequential Interrupts**: New interrupts remain pending until current one completes
- **Nested/Prioritized Interrupts**: Higher priority interrupts can interrupt lower priority handlers

### I/O Operations

**Q: What are the three methods of I/O operations?**

A:

1. **Programmed I/O (Polling)**:
   - CPU continuously checks device status
   - Wastes CPU cycles
   - Simple but inefficient

2. **Interrupt-Driven I/O**:
   - Device signals CPU when ready
   - CPU free for other work
   - Every byte passes through CPU (still inefficient for bulk transfers)

3. **Direct Memory Access (DMA)**:
   - I/O controller transfers data directly to/from memory
   - CPU only involved at start and completion
   - Most efficient for large transfers
   - CPU interrupted only when transfer complete

### Memory Hierarchy

**Q: What is the memory hierarchy and why does it exist?**

A: Memory is organized in layers from fastest/smallest to slowest/largest:

```
Registers (fastest, smallest)
    ↓
Cache (L1, L2, L3)
    ↓
Main Memory (RAM)
    ↓
Solid State Disk
    ↓
Hard Disk (slowest, largest)
```

**Properties as you go down:**
- Decreasing cost per bit
- Increasing capacity
- Increasing access time
- Decreasing access frequency (hopefully, due to locality!)

**Q: What is cache memory and how does it work?**

A: Cache is fast memory between CPU and RAM:
- Access time: 1-few cycles vs tens-hundreds for RAM
- Holds recently used data/instructions
- Hardware-maintained, mostly transparent to software
- Multiple levels (L1, L2, L3)
- Exploits principle of locality (recently accessed data likely to be accessed again)

**Q: How do you calculate effective memory access time?**

A: Formula: `T_eff = H × T1 + (1 - H) × (T1 + T2)`

Where:
- H = hit rate in faster memory
- T1 = access time of faster memory
- T2 = access time of slower memory

**Example**: Cache 1ns, RAM 10ns, hit rate 95%
- T_eff = 0.95 × 1 + 0.05 × (1 + 10) = 1.5ns

### Multiprogramming

**Q: What is multiprogramming/multitasking?**

A: CPU has multiple programs to execute:
- Some tasks waiting for I/O
- Some ready to run but not currently running
- Interrupt handler switches tasks when they become runnable
- Timer interrupts enable time-sharing
- Improves CPU utilization

---

## Part 2: Operating System Basics

### OS Components

**Q: What is an Operating System?**

A: System software that:
- Exploits available hardware
- Provides high-level services representing/implemented by hardware
- Manages hardware reliably and efficiently
- Acts as interface between applications and hardware (the "extended machine")
- Acts as resource manager

**Q: What are the main OS components?**

A:

1. **Process Management**:
   - Process: program in execution
   - Has text (code), data, and stack segments
   - Execution context (all info OS needs to manage it)
   - Modern systems separate execution: threads

2. **Memory Management**:
   - Process isolation
   - Automatic allocation
   - Support for modular programming
   - Protection and access control
   - Virtual memory

3. **File System**:
   - Long-term storage
   - Information stored in named objects (files)
   - Hierarchical organization

4. **I/O Management**:
   - Device drivers
   - Interrupt handling
   - Buffering

5. **Security & Protection**:
   - Access control (regulate user access)
   - Information flow control
   - Authentication

### Process Details

**Q: What are the three segments of a process?**

A:
1. **Text**: Contains the code (instructions)
2. **Data**: Global variables (can grow dynamically upward)
3. **Stack**: Activation records, local variables (can grow dynamically downward)

**Q: What concurrency issues can arise with multiple processes?**

A: **Deadlock**: Two or more processes waiting for each other to release resources, creating a circular wait condition where no process can proceed.

### Virtual Memory

**Q: What is virtual memory and why is it useful?**

A: Virtual memory:
- Gives each process illusion of having RAM to themselves
- Logical addresses independent of other processes
- Provides process isolation
- Allows overlapping execution while swapping processes in/out
- Enables programs larger than physical RAM

**Q: What is paging?**

A: Memory management scheme where:
- Process divided into fixed-size blocks (pages)
- Virtual address = page number + offset
- Pages can be located anywhere in physical memory
- Pages may exist only on disk (swapped out)
- Operating system maintains page tables for translation

---

## Part 3: OS Structure & System Calls

### OS Structures

**Q: What are the main OS architectural approaches?**

A:

**1. Layered Approach**:
- Layer a: Processor allocation, multiprogramming
- Layer b: Memory management
- Layer c: Devices
- Layer d: File system
- Layer e: Users
- Each layer depends on inner layers
- In practice, many inter-dependencies exist

**2. Monolithic Kernel**:
- Everything in kernel (Linux, Windows)
- Also called "spaghetti nest"
- User services and kernel services in same address space
- **Pros**: Faster (no mode switching overhead)
- **Cons**: Less modular, harder to maintain, less secure

**3. Microkernel**:
- Minimal kernel (address spaces, IPC, basic scheduling, minimal hardware abstraction)
- Other services as user-level servers
- System calls become IPC requests
- Examples: Minix, L4
- **Pros**: More modular, more secure, easier to maintain
- **Cons**: Performance overhead from IPC

### System Calls

**Q: What is a system call?**

A: A controlled entry point into the kernel allowing applications to request OS services. It:
- Can be viewed as special procedure calls
- Provides controlled kernel entry
- Performs privileged operations while in kernel
- Returns result to caller
- Represents the abstract machine provided by OS

**Q: What are the main categories of system calls?**

A:

1. **Process Management**:
   - `fork()`: Create child process
   - `exec()`: Load new program
   - `exit()`: Terminate process
   - `wait()`: Wait for child

2. **File Management**:
   - `open()`, `close()`: Open/close files
   - `read()`, `write()`: Read/write data
   - `lseek()`: Change file position

3. **Directory Management**:
   - `mkdir()`, `rmdir()`: Create/remove directories
   - `link()`, `unlink()`: Create/remove links

4. **Miscellaneous**:
   - `chmod()`: Change file permissions
   - `kill()`: Send signal to process
   - `time()`: Get system time

**Q: Why not just use function calls to access kernel?**

A: Function calls don't:
- Change from user mode to kernel mode and back
- Restrict entry points to secure locations
- Provide security and protection mechanisms

---

## Part 4: ARM Cortex-M4 Specifics

### Processor Architecture

**Q: What are the main components of ARM Cortex-M4?**

A:
- **Cortex-M4 Core**: Main processor
- **NVIC**: Nested Vector Interrupt Controller
- **MPU**: Memory Protection Unit (optional)
- **FPU**: Floating Point Unit (optional)
- **FPBP**: Flash Patch Breakpoint (debugging)
- **DWT**: Data Watchpoint and Trace
- **SysTick**: System tick timer
- **ITM**: Instrumentation Trace Microcell

### Execution Modes

**Q: What are the two execution modes in Cortex-M4?**

A:

**1. Thread Mode**:
- Executes application software
- Entered after reset
- Can be privileged or unprivileged (controlled by CONTROL register)

**2. Handler Mode**:
- Handles exceptions/interrupts
- Always privileged
- Returns to thread mode after handling

**Q: What's the difference between privileged and unprivileged levels?**

A:

**Unprivileged**:
- Limited instruction access (no MSR/MRS to special registers)
- No access to CPS instruction
- Can't access SysTick, NVIC, SCB directly
- Restricted memory/peripheral access
- Must use SVC to access privileged resources

**Privileged**:
- Full instruction and resource access
- Can modify CONTROL register
- Direct hardware access

### Core Registers

**Q: What are the important Cortex-M4 registers?**

A:
- **R0-R12**: General purpose registers
- **R13 (SP)**: Stack pointer
  - MSP: Main Stack Pointer (kernel/handler mode)
  - PSP: Process Stack Pointer (user tasks)
  - Controlled by CONTROL register bit[1]
- **R14 (LR)**: Link Register (return address)
- **R15 (PC)**: Program Counter

**Special Registers**:
- **PRIMASK**: Masks all configurable priority exceptions
- **FAULTMASK**: Masks all except NMI
- **BASEPRI**: Masks exceptions below priority threshold
- **CONTROL**: Controls stack selection and privilege level

### Stack Operation

**Q: How does the dual stack system work?**

A: Cortex-M4 has two stacks:

**Main Stack (MSP)**:
- Used by OS kernel
- Used in handler mode
- Initialized on reset from address 0x00000000

**Process Stack (PSP)**:
- Used by application tasks
- Controlled by CONTROL register bit[1]
- OS assigns separate PSP to each task

**Benefits**:
- If application stack corrupts, kernel stack intact
- Better isolation and reliability
- Enables stack overflow detection via MPU

### Memory Model

**Q: What is the Cortex-M4 memory map?**

A: Fixed 4GB address space with regions:
- **Code**: Program memory (Flash)
- **SRAM**: Data memory
- **Peripheral**: Memory-mapped I/O
- **External RAM**: Off-chip memory
- **External Device**: Off-chip peripherals
- **System**: Control registers (NVIC, MPU, etc.)

Features:
- Memory-mapped I/O
- Bit-banding (atomic bit operations)
- Memory Protection Unit for access control

### SysTick Timer

**Q: What is SysTick and why is it important?**

A: SysTick is a 24-bit countdown timer integrated in the processor:

**Purpose**:
- Generate periodic interrupts for OS
- Enable time-slicing for multitasking
- Portable (part of processor, not board-specific)

**Registers**:
- **CTRL**: Control and status
- **LOAD**: Reload value (24-bit)
- **VAL**: Current value
- **CALIB**: Calibration value

**Configuration Steps**:
1. Disable timer
2. Set reload value (desired count - 1)
3. Clear current value
4. Select clock source
5. Enable interrupt
6. Enable timer

---

## Part 5: Interrupts & Exceptions (ARM Cortex-M4)

### NVIC (Nested Vector Interrupt Controller)

**Q: What types of exceptions exist in Cortex-M4?**

A:

**System Exceptions (1-15)**:
- Fixed priority (not alterable):
  - Reset (-3)
  - NMI (-2)
  - HardFault (-1)
- Programmable priority:
  - MemManage, BusFault, UsageFault
  - SVCall (system call)
  - PendSV (pending service)
  - SysTick

**External Interrupts (16+)**:
- All have programmable priority
- Peripheral interrupts (UART, GPIO, timers, etc.)
- Up to 240 interrupts (chip-dependent)

### Interrupt Priority

**Q: How does interrupt priority work?**

A:
- 8-bit priority field per interrupt
- STM32F4 implements 4 MSB (16 priority levels): 0x00, 0x10, 0x20, ..., 0xF0
- **Lower number = higher priority**
- Priority divided into:
  - **Pre-empt priority**: Can interrupt lower priority handlers
  - **Sub-priority**: Order among same pre-empt priority

**Special Priorities**:
- Reset: -3 (highest)
- NMI: -2
- HardFault: -1
- Configurable: 0-255 (or 0-15 for 4-bit implementation)

### Interrupt Management

**Q: What are the steps to enable and use an interrupt?**

A:
1. Set interrupt priority: `NVIC_SetPriority(IRQn, priority)`
2. Enable interrupt in peripheral (set interrupt enable bit)
3. Enable interrupt in NVIC: `NVIC_EnableIRQ(IRQn)`
4. Implement ISR (Interrupt Service Routine)
5. Clear interrupt flag in ISR

**Q: What are the key NVIC registers?**

A:
- **ISER**: Interrupt Set Enable (enable interrupts)
- **ICER**: Interrupt Clear Enable (disable interrupts)
- **ISPR**: Interrupt Set Pending (manually pend)
- **ICPR**: Interrupt Clear Pending
- **IABR**: Interrupt Active Bit Register (read-only status)
- **IPR**: Interrupt Priority Registers

### Interrupt States

**Q: What are the possible interrupt states?**

A:
1. **Inactive**: Not active or pending
2. **Pending**: Waiting for higher priority interrupt to complete
3. **Active**: Currently being serviced
4. **Active and Pending**: Being serviced AND another from same source pending

**Transitions**:
- Interrupt occurs → Pending
- Processor starts ISR → Active (Pending cleared)
- ISR completes → Inactive
- If interrupt occurs during ISR → Active and Pending

### Interrupt Masking

**Q: How can interrupts be disabled/masked?**

A: Three special registers:

**1. PRIMASK (bit 0)**:
- Set: Masks all configurable priority interrupts
- Clear: Unmasks
- Doesn't affect HardFault, NMI
- Functions: `__disable_irq()`, `__enable_irq()`

**2. FAULTMASK (bit 0)**:
- Set: Masks all except NMI
- More powerful than PRIMASK
- Functions: `__disable_fault_irq()`, `__enable_fault_irq()`

**3. BASEPRI (8-bit value)**:
- Masks interrupts with priority ≥ value
- Write 0 to disable masking
- More selective than PRIMASK/FAULTMASK
- Example: Write 0x20 masks priorities 0x20-0xFF

---

## Part 6: System Calls in ARM Cortex-M4

### SVC (Supervisor Call)

**Q: What is SVC and how does it work?**

A: SVC is the system call mechanism in ARM:

**Characteristics**:
- Exception number 11
- Programmable priority
- Triggered by SVC instruction: `SVC #n` (n = service number)
- **Precise**: Handler runs immediately after SVC instruction (unless higher priority interrupt)

**Why SVC?**:
- Applications run in unprivileged mode
- Need privileged mode for hardware access
- SVC provides safe transition to kernel

**Q: How do you implement an SVC handler?**

A: Two-part implementation:

**Part 1: Assembly wrapper** (determine which stack was used):
```assembly
SVC_Handler:
    TST LR, #4              ; Test bit 2 of EXC_RETURN
    ITE EQ                   ; If-Then-Else
    MRSEQ R0, MSP           ; If 0: MSP used, copy to R0
    MRSNE R0, PSP           ; If 1: PSP used, copy to R0
    B SVC_Handler_C         ; Call C handler with stack frame address
```

**Part 2: C handler** (process the service):
```c
void SVC_Handler_C(unsigned int *svc_args) {
    uint8_t svc_number;
    // Get SVC number from instruction before stacked PC
    svc_number = ((char *)svc_args[6])[-2];
    
    // Extract parameters from stack frame
    uint32_t r0 = svc_args[0];
    uint32_t r1 = svc_args[1];
    
    // Process service based on number
    switch(svc_number) {
        case 0: /* Service 0 */ break;
        case 1: /* Service 1 */ break;
    }
    
    // Return result via stack frame
    svc_args[0] = result;
}
```

**Q: What's in the stack frame during SVC?**

A: Automatically stacked by hardware:
- `svc_args[0]`: R0
- `svc_args[1]`: R1
- `svc_args[2]`: R2
- `svc_args[3]`: R3
- `svc_args[4]`: R12
- `svc_args[5]`: LR
- `svc_args[6]`: PC (return address)
- `svc_args[7]`: xPSR

If FPU active: S0-S15, FPSCR also stacked

### PendSV (Pending Service)

**Q: What is PendSV and why is it crucial for OS?**

A: PendSV is a software-triggered exception used for context switching:

**Characteristics**:
- Exception number 14
- Set by writing to ICSR register bit 28
- Not precise (can be deferred)
- Usually set to lowest priority

**Why PendSV?**:
- Set in high-priority ISR
- Executes after all interrupts complete
- Perfect for context switching (no interruption during switch)

**Q: How does context switching work with PendSV?**

A: Context switch in PendSV handler:

```assembly
PendSV_Handler:
    ; ---- Save current context ----
    MRS R0, PSP                    ; Get current PSP
    STMDB R0!, {R4-R11}           ; Save R4-R11 (R0-R3,R12,LR,PC,xPSR auto-saved)
    
    LDR R1, =curr_task
    LDR R2, [R1]                   ; Get current task ID
    LDR R3, =PSP_array
    STR R0, [R3, R2, LSL #2]      ; Save PSP to array
    
    ; ---- Load next context ----
    LDR R4, =next_task
    LDR R4, [R4]                   ; Get next task ID
    STR R4, [R1]                   ; curr_task = next_task
    
    LDR R0, [R3, R4, LSL #2]      ; Load new PSP from array
    LDMIA R0!, {R4-R11}           ; Restore R4-R11
    MSR PSP, R0                    ; Set new PSP
    
    BX LR                          ; Return (auto-restores other registers)
```

### Task Initialization

**Q: How do you create the initial stack frame for a new task?**

A: Set up fake stack frame as if task was interrupted:

```c
// Calculate stack frame location (top - 16 words)
PSP_array[task_id] = stack_top - 16*4;

// Set initial PC (entry point)
HW32_REG(PSP_array[task_id] + 14*4) = (uint32_t)task_function;

// Set initial xPSR (Thumb bit set)
HW32_REG(PSP_array[task_id] + 15*4) = 0x01000000;

// Other registers can be initialized to 0 or specific values
```

**Stack frame layout** (from bottom to top):
- R4-R11 (saved manually in PendSV)
- R0-R3, R12 (auto-saved)
- LR (auto-saved)
- PC (auto-saved) ← Entry point
- xPSR (auto-saved)

---

## Part 7: Process Management

### Process Concepts

**Q: What's the difference between a process and a thread?**

A:

**Process (Task/Job)**:
- Execution of individual program
- Owner of resources (memory, files, etc.)
- Has own address space
- Contains one or more threads
- Higher overhead to create/switch

**Thread**:
- Unit of execution within a process
- Shares process resources
- Has own stack and execution context
- Lighter weight than process
- Faster to create/switch

**Q: What are the possible process/thread states?**

A:
1. **New**: Being created
2. **Ready**: Waiting for CPU assignment
3. **Running**: Currently executing
4. **Blocked (Waiting)**: Waiting for event (I/O, resource)
5. **Terminated**: Finished execution

**State Transitions**:
- New → Ready: Process created and admitted
- Ready → Running: Dispatcher selects process
- Running → Ready: Time slice expired, yield()
- Running → Blocked: Wait for I/O, resource
- Blocked → Ready: Event occurred (I/O complete)
- Running → Terminated: Exit

### Process Creation & Termination

**Q: What causes process creation?**

A:
1. System initialization (boot time)
   - Foreground processes (interactive)
   - Background processes/daemons (services)
2. System call by running process (fork(), exec())
3. User request (double-click, command)
4. Batch job initiation

**Q: What causes process termination?**

A:
1. **Normal exit** (voluntary): Program completes
2. **Error exit** (voluntary): Program detects error, exits gracefully
3. **Fatal error** (involuntary): Illegal instruction, invalid memory access
4. **Killed** (involuntary): Another process kills it

### Process Implementation

**Q: What is a Process Control Block (PCB)?**

A: Data structure containing all process information:

**Process Management**:
- Process state (new, ready, running, etc.)
- Process ID
- Program counter
- CPU registers
- Stack pointer
- Priority

**Memory Management**:
- Base/limit registers
- Page tables
- Segment tables

**File Management**:
- Open file descriptors
- Current directory

**I/O Management**:
- List of I/O devices
- Pending I/O operations

**Accounting**:
- CPU time used
- Time limits
- Account numbers

### Scheduling Queues

**Q: How does the OS organize processes?**

A:

**Ready Queue**:
- Contains all processes ready to run
- Dispatcher selects from this queue
- Avoids scanning all processes

**Blocked Queues**:
- Separate queue for each event type
- I/O device queues
- Timer queues
- Resource queues
- When event occurs, move process to ready queue

**Benefits**:
- Fast access to ready processes
- Efficient event handling
- No need to scan all processes

### Thread Model

**Q: What do threads share vs. what is private?**

A:

**Shared by all threads in process**:
- Address space (code, data)
- Global variables
- Open files
- Child processes
- Signals
- Accounting information

**Private to each thread**:
- Program counter
- Registers
- Stack
- Thread state
- Priority (in some systems)

**Q: What's the Thread Control Block (TCB)?**

A: Lightweight version of PCB containing:
- Thread ID
- Thread state
- Program counter
- Stack pointer
- Register values
- Pointer to PCB
- Priority

### Context Switching

**Q: What happens during a context switch?**

A:

**Save current context**:
1. Save CPU registers to stack/TCB
2. Save program counter
3. Save stack pointer
4. Update process state to ready/blocked

**Load next context**:
1. Select next thread (scheduling algorithm)
2. Load its stack pointer
3. Restore its registers
4. Load its program counter
5. Update state to running

**When does context switch occur?**:
- System call (especially if blocking)
- Exception/fault
- Interrupt (especially timer)
- Can happen between any two instructions!

**Q: Why is context switching overhead important?**

A:
- Happens frequently (milliseconds)
- Saving/restoring registers takes time
- Cache must be reloaded (cache pollution)
- TLB must be flushed (virtual memory overhead)
- Faster switches = better system responsiveness

### User-Level vs Kernel-Level Threads

**Q: What are user-level threads?**

A: Threads implemented entirely in user space:

**Pros**:
- Very fast switching (no kernel involvement)
- Can support massive numbers of threads
- Custom scheduling algorithms
- Works on any OS

**Cons**:
- Blocking system call blocks all threads
- Can't utilize multiple CPUs
- No true parallelism
- Cooperative multitasking (threads must yield)
- Page fault blocks entire process

**Q: What are kernel-level threads?**

A: Threads implemented in the kernel:

**Pros**:
- True preemptive multitasking
- Can utilize multiple CPUs
- Blocking call blocks only one thread
- Better for I/O-bound applications

**Cons**:
- Slower (system call overhead)
- Limited number of threads
- More resource-intensive

---

## Part 8: RTOS-Specific Concepts

### RTOS vs General-Purpose OS

**Q: What makes an RTOS different?**

A:

**Real-Time OS characteristics**:
- **Deterministic**: Predictable response times
- **Responsiveness**: Fast interrupt handling
- **User control**: Developer controls priorities
- **Reliability**: High availability
- **Fail-soft**: Graceful degradation

**vs. General-Purpose OS**:
- RTOS: Minimize latency, predictable behavior
- GP-OS: Maximize throughput, fairness

### Hard vs Soft Real-Time

**Q: What's the difference between hard and soft real-time?**

A:

**Hard Real-Time**:
- **Must** meet deadline
- Failure = system failure
- Examples: Airbag deployment, nuclear reactor control
- Typically verified formally
- No virtual memory (unpredictable delays)

**Soft Real-Time**:
- **Should** meet deadline
- Missing deadline degrades performance but not catastrophic
- Examples: Video streaming, audio playback
- More flexible scheduling
- Can use more OS features

### Task Priorities in RTOS

**Q: How does priority-based scheduling work?**

A:
- Each task assigned priority (number)
- Highest priority ready task runs
- Lower priority tasks preempted immediately
- Can lead to priority inversion problem

**Priority Inversion Solution**:
- Priority inheritance: Low-priority task temporarily inherits high priority
- Priority ceiling: Prevent medium priority from running

### Scheduling Algorithms

**Q: What are common scheduling algorithms?**

A:

**1. Round Robin (Time Slicing)**:
- Each task gets fixed time quantum
- Fair but not responsive
- Good for time-sharing

**2. Priority-Based**:
- Highest priority runs first
- Good for real-time
- Risk of starvation

**3. Rate Monotonic (RM)**:
- Static priority assignment
- Shorter period = higher priority
- Optimal for periodic tasks

**4. Earliest Deadline First (EDF)**:
- Dynamic priority
- Task with nearest deadline runs
- Optimal utilization

---

## Important Formulas & Calculations

### Effective Memory Access Time
```
T_eff = H × T1 + (1-H) × (T1 + T2)

Where:
H = Hit rate
T1 = Fast memory access time
T2 = Slow memory access time
```

### Interrupt Priority Calculation
```
Register Number = IRQ_number / 32  (or IRQ_number >> 5)
Bit Position = IRQ_number % 32     (or IRQ_number & 0x1F)
```

### Stack Frame Size
- Cortex-M3: 8 words (R0-R3, R12, LR, PC, xPSR)
- Cortex-M4 without FPU: 8 words
- Cortex-M4 with FPU: 26 words (+ S0-S15, FPSCR, reserved)

---

## Key Takeaways

### Hardware-Software Interface
1. OS must understand hardware (interrupts, memory, I/O)
2. Hardware provides mechanisms (modes, protection)
3. OS provides policies (scheduling, allocation)

### Protection Mechanisms
1. **User/Kernel modes**: Prevent unauthorized access
2. **Virtual memory**: Process isolation
3. **Interrupts**: Safe hardware notification
4. **System calls**: Controlled kernel entry

### Efficiency Trade-offs
1. **User-level threads**: Fast but limited
2. **Kernel-level threads**: Flexible but slower
3. **Monolithic kernel**: Fast but less secure
4. **Microkernel**: Secure but overhead

### Real-Time Constraints
1. **Predictability > Performance**: In RTOS
2. **Priority inheritance**: Solve priority inversion
3. **Interrupt latency**: Critical metric
4. **Deterministic behavior**: Must be guaranteed

### Context Switching
1. Happens frequently (every few ms)
2. Must save/restore all state
3. Hardware assists (automatic stacking)
4. OS manages (queue, priority, scheduling)

# Operating Systems - Comprehensive Study Notes (Continued)

## Part 9: Advanced Process Synchronization

### Critical Section Problem

**Q: What is a critical section?**

A: A critical section is a code segment where:
- Shared resources are accessed
- Only one process/thread should execute at a time
- Must be protected from concurrent access

**Example scenarios**:
- Updating shared variables
- Accessing shared data structures
- Writing to shared files
- Modifying hardware registers

**Q: What are the requirements for a critical section solution?**

A: Must satisfy three conditions:

1. **Mutual Exclusion**: 
   - Only one process in critical section at a time
   - No two processes execute critical section simultaneously

2. **Progress**: 
   - If no process in critical section, selection cannot be postponed indefinitely
   - Only processes not in remainder section can participate in decision
   - Selection must be made in finite time

3. **Bounded Waiting**: 
   - Limit on number of times other processes enter critical section
   - After process requests entry, there's a bound on waiting
   - Prevents starvation

### Synchronization Mechanisms

**Q: What is a semaphore?**

A: A semaphore is a synchronization primitive with:
- Integer value (count)
- Two atomic operations: wait() and signal()

**Types**:

**1. Binary Semaphore (Mutex)**:
```c
semaphore mutex = 1;  // Initially 1

// Process enters critical section
wait(mutex);          // Decrements to 0
    // Critical section
signal(mutex);        // Increments to 1
```

**2. Counting Semaphore**:
```c
semaphore resources = N;  // N available resources

wait(resources);     // Allocate resource
    // Use resource
signal(resources);   // Release resource
```

**Q: How do wait() and signal() work?**

A:

**wait(S)** (also called P(), down(), acquire()):
```c
wait(S) {
    while (S <= 0)
        ; // Busy wait
    S--;
}
```

**signal(S)** (also called V(), up(), release()):
```c
signal(S) {
    S++;
}
```

**With blocking (no busy wait)**:
```c
wait(S) {
    S--;
    if (S < 0) {
        // Add this process to S's waiting queue
        block();
    }
}

signal(S) {
    S++;
    if (S <= 0) {
        // Remove a process from waiting queue
        wakeup(process);
    }
}
```

### Deadlock

**Q: What is deadlock?**

A: A situation where processes are permanently blocked, each waiting for a resource held by another process in the set.

**Four Necessary Conditions** (Coffman conditions):

1. **Mutual Exclusion**: 
   - Resources cannot be shared
   - Only one process can use at a time

2. **Hold and Wait**: 
   - Process holds resources while waiting for others
   - Already allocated resources not released

3. **No Preemption**: 
   - Resources cannot be forcibly taken
   - Only process can release voluntarily

4. **Circular Wait**: 
   - Circular chain of processes
   - Each waiting for resource held by next

**All four must be present for deadlock!**

**Q: How can we handle deadlock?**

A: Four strategies:

**1. Prevention**: Ensure at least one condition cannot occur
- **Mutual Exclusion**: Make resources shareable (not always possible)
- **Hold and Wait**: Require all resources at once, or release before requesting more
- **No Preemption**: Allow resource preemption
- **Circular Wait**: Order resources, request in increasing order

**2. Avoidance**: Dynamically check if allocation is safe
- **Banker's Algorithm**: Check if granting request leaves system in safe state
- Requires advance knowledge of max resource needs
- Conservative but flexible

**3. Detection and Recovery**: 
- Let deadlock occur
- Detect using resource allocation graphs
- Recover by:
  - Process termination (abort one or all)
  - Resource preemption (rollback)

**4. Ignore**: 
- "Ostrich algorithm"
- Used by many systems (Unix, Windows)
- Assumption: rare enough to ignore
- Manual recovery (reboot)

### Producer-Consumer Problem

**Q: What is the producer-consumer problem?**

A: Classic synchronization problem where:
- **Producer**: Creates data, puts in buffer
- **Consumer**: Takes data from buffer
- **Buffer**: Shared, bounded size

**Issues**:
- Producer must wait if buffer full
- Consumer must wait if buffer empty
- Mutual exclusion on buffer access

**Q: How do you solve it with semaphores?**

A:
```c
#define BUFFER_SIZE 10

semaphore mutex = 1;          // Mutual exclusion
semaphore empty = BUFFER_SIZE; // Count empty slots
semaphore full = 0;            // Count full slots

// Producer
while (true) {
    // Produce item
    wait(empty);        // Wait for empty slot
    wait(mutex);        // Enter critical section
    
    // Add item to buffer
    
    signal(mutex);      // Leave critical section
    signal(full);       // Increment full count
}

// Consumer
while (true) {
    wait(full);         // Wait for full slot
    wait(mutex);        // Enter critical section
    
    // Remove item from buffer
    
    signal(mutex);      // Leave critical section
    signal(empty);      // Increment empty count
    // Consume item
}
```

**Key points**:
- Order matters! Wait on count semaphore first
- `wait(empty)` before `wait(mutex)` prevents deadlock
- Three semaphores needed: 1 mutex + 2 counting

### Readers-Writers Problem

**Q: What is the readers-writers problem?**

A: Database access problem:
- **Readers**: Read data only, can read simultaneously
- **Writers**: Modify data, need exclusive access
- **Constraint**: No reader when writer active, no other writer when writer active

**First Readers-Writers Solution** (readers priority):
```c
semaphore mutex = 1;      // Protects read_count
semaphore write = 1;      // Writers' mutual exclusion
int read_count = 0;       // Number of readers

// Reader
wait(mutex);
read_count++;
if (read_count == 1)
    wait(write);          // First reader blocks writers
signal(mutex);

// Read data

wait(mutex);
read_count--;
if (read_count == 0)
    signal(write);        // Last reader unblocks writers
signal(mutex);

// Writer
wait(write);
// Write data
signal(write);
```

**Problem**: Writers may starve if continuous readers

**Second Solution** (writers priority):
- Use additional semaphores
- Writers get priority
- Problem: Readers may starve

### Dining Philosophers Problem

**Q: What is the dining philosophers problem?**

A: Classic deadlock scenario:
- 5 philosophers sit at round table
- 5 forks (one between each pair)
- Philosopher needs 2 forks to eat
- Each can: think, pick up forks, eat, put down forks

**Deadlock scenario**:
```c
// Each philosopher does:
wait(fork[i]);          // Pick up left fork
wait(fork[(i+1)%5]);    // Pick up right fork
// Eat
signal(fork[(i+1)%5]);  // Put down right fork
signal(fork[i]);        // Put down left fork
```

**Deadlock**: All pick up left fork simultaneously!

**Q: What are solutions to dining philosophers?**

A: Several approaches:

**1. Allow only 4 to sit** (one less than resources):
```c
semaphore room = 4;
// Philosopher:
wait(room);
wait(fork[i]);
wait(fork[(i+1)%5]);
// Eat
signal(fork[(i+1)%5]);
signal(fork[i]);
signal(room);
```

**2. Asymmetric solution** (odd philosophers pick left first, even pick right first):
```c
if (i % 2 == 0) {
    wait(fork[i]);
    wait(fork[(i+1)%5]);
} else {
    wait(fork[(i+1)%5]);
    wait(fork[i]);
}
```
Breaks circular wait!

**3. Pick both forks atomically** (using additional mutex):
```c
semaphore mutex = 1;

wait(mutex);
wait(fork[i]);
wait(fork[(i+1)%5]);
signal(mutex);
// Eat
signal(fork[(i+1)%5]);
signal(fork[i]);
```

---

## Part 10: ARM Cortex-M4 Synchronization Primitives

### Exclusive Access Instructions

**Q: Why do we need special instructions for synchronization?**

A: Simple read-modify-write isn't atomic:
```c
// NOT ATOMIC! Can be interrupted
count++;
// Compiles to:
// LDR R0, [count]    ; Read
// ADD R0, R0, #1     ; Modify
// STR R0, [count]    ; Write
// <- Interrupt can occur between any instructions!
```

**Q: What are LDREX and STREX?**

A: Exclusive load and store instructions for atomic operations:

**LDREX** (Load Exclusive):
- Loads value from memory
- Tags memory location for exclusive access
- Returns value

**STREX** (Store Exclusive):
- Attempts to store value
- Succeeds only if no other write to tagged location
- Returns status: 0 = success, 1 = failure
- Must clear exclusive tag

**Q: How do you implement a mutex with LDREX/STREX?**

A:

```assembly
; Acquire mutex
mutex_lock:
    LDREX   R1, [R0]        ; Load mutex value exclusively
    CMP     R1, #0          ; Check if unlocked (0 = unlocked)
    BNE     mutex_lock      ; If locked, retry
    MOV     R1, #1          ; Prepare locked value
    STREX   R2, R1, [R0]    ; Try to store (lock it)
    CMP     R2, #0          ; Check if succeeded
    BNE     mutex_lock      ; If failed, retry
    DMB                     ; Data memory barrier
    BX      LR              ; Return

; Release mutex
mutex_unlock:
    DMB                     ; Data memory barrier
    MOV     R1, #0          ; Unlock value
    STR     R1, [R0]        ; Store (unlock)
    DSB                     ; Data synchronization barrier
    BX      LR              ; Return
```

**Key points**:
- Retry loop until successful (spinlock)
- Memory barriers ensure ordering
- Lightweight for short critical sections

### Memory Barriers

**Q: What are memory barriers and why do we need them?**

A: Processor can reorder instructions for optimization, but this breaks synchronization!

**Three types in ARM Cortex-M4**:

**1. DMB (Data Memory Barrier)**:
```assembly
DMB    ; Wait for all memory accesses to complete
```
- Ensures memory accesses before complete before those after
- Used around critical sections

**2. DSB (Data Synchronization Barrier)**:
```assembly
DSB    ; Wait for all memory accesses AND instructions to complete
```
- Stronger than DMB
- Ensures side effects visible
- Used before interrupt enabling

**3. ISB (Instruction Synchronization Barrier)**:
```assembly
ISB    ; Flush pipeline, re-fetch instructions
```
- Used after changing:
  - CONTROL register
  - Vector table
  - MPU settings

**Typical usage**:
```assembly
DMB                    ; Ensure prior accesses complete
; Critical section
DMB                    ; Ensure critical section complete

; After changing CONTROL:
MSR CONTROL, R0
ISB                    ; Required by architecture
```

### Implementing Semaphores in RTOS

**Q: How does an RTOS typically implement semaphores?**

A: Using a data structure and LDREX/STREX:

```c
typedef struct {
    int count;                  // Semaphore count
    task_t *waiting_queue;      // Queue of waiting tasks
    // Spinlock for atomic operations
} semaphore_t;

void sem_wait(semaphore_t *sem) {
    disable_interrupts();       // Or use LDREX/STREX
    
    sem->count--;
    if (sem->count < 0) {
        // Block current task
        current_task->state = BLOCKED;
        enqueue(&sem->waiting_queue, current_task);
        
        // Context switch to another task
        schedule();             // Calls PendSV
    }
    
    enable_interrupts();
}

void sem_signal(semaphore_t *sem) {
    disable_interrupts();
    
    sem->count++;
    if (sem->count <= 0) {
        // Wake up a waiting task
        task_t *task = dequeue(&sem->waiting_queue);
        task->state = READY;
        enqueue(&ready_queue, task);
        
        // May trigger context switch
        if (task->priority > current_task->priority)
            schedule();
    }
    
    enable_interrupts();
}
```

**Q: What's the difference between spinlock and blocking lock?**

A:

**Spinlock** (busy-wait):
```c
void spinlock_acquire(volatile int *lock) {
    while (1) {
        int val;
        do {
            val = __LDREXW(lock);
        } while (val != 0);
        
        if (__STREXW(1, lock) == 0)
            break;  // Success!
    }
    __DMB();
}
```
- **Pros**: Very fast for short critical sections, no context switch
- **Cons**: Wastes CPU, bad for long waits
- **Use when**: Critical section < context switch time (~microseconds)

**Blocking lock** (sleep-wait):
```c
void mutex_acquire(mutex_t *mutex) {
    if (/* try to acquire fails */) {
        block_current_task();
        schedule();  // Context switch
    }
}
```
- **Pros**: CPU available for other tasks
- **Cons**: Context switch overhead
- **Use when**: Critical section > context switch time

---

## Part 11: Real-Time Scheduling

### Rate Monotonic Scheduling (RMS)

**Q: What is Rate Monotonic Scheduling?**

A: Static priority scheduling algorithm for periodic tasks:

**Rules**:
- Each task has fixed period (T) and execution time (C)
- Priority = 1/Period (shorter period = higher priority)
- Preemptive: higher priority can interrupt lower

**Example**:
```
Task 1: Period = 50ms, Execution = 20ms, Priority = 3 (highest)
Task 2: Period = 100ms, Execution = 35ms, Priority = 2
Task 3: Period = 200ms, Execution = 50ms, Priority = 1 (lowest)
```

**Q: How do you check if tasks are schedulable with RMS?**

A: **Utilization test**:

```
U = Σ(C_i / T_i) ≤ n(2^(1/n) - 1)

Where:
C_i = Execution time of task i
T_i = Period of task i
n = Number of tasks
```

**Bounds**:
- n=1: U ≤ 1.000 (100%)
- n=2: U ≤ 0.828 (82.8%)
- n=3: U ≤ 0.779 (77.9%)
- n→∞: U ≤ 0.693 (69.3%)

**Example check**:
```
U = 20/50 + 35/100 + 50/200
  = 0.4 + 0.35 + 0.25
  = 1.0

For 3 tasks: bound = 0.779
1.0 > 0.779 → NOT guaranteed schedulable by this test
(But might still be schedulable - test is sufficient but not necessary)
```

**Q: What are RMS pros and cons?**

A:

**Pros**:
- Simple to implement
- Optimal among static priority algorithms
- Predictable behavior
- Well-studied mathematical properties

**Cons**:
- Not optimal overall (EDF better)
- Utilization bound ~69% (wastes CPU)
- Poor for aperiodic tasks
- Priority inversion possible

### Earliest Deadline First (EDF)

**Q: What is Earliest Deadline First scheduling?**

A: Dynamic priority algorithm:

**Rules**:
- Task with earliest absolute deadline gets highest priority
- Priorities change dynamically
- Preemptive

**Example timeline**:
```
Time 0: Task 1 arrives (deadline=50), Task 2 arrives (deadline=100)
        → Run Task 1 (earlier deadline)
Time 30: Task 3 arrives (deadline=40)
         → Preempt Task 1, run Task 3 (earliest deadline)
Time 40: Task 3 completes
         → Resume Task 1
```

**Q: How do you check EDF schedulability?**

A: **Simple utilization test**:

```
U = Σ(C_i / T_i) ≤ 1.0

If U ≤ 1.0, ALL deadlines will be met!
```

**Much better than RMS!** Can use up to 100% CPU.

**Example**:
```
Same tasks as before:
U = 20/50 + 35/100 + 50/200 = 1.0

With EDF: Schedulable! ✓
With RMS: Not guaranteed ✗
```

**Q: What are EDF pros and cons?**

A:

**Pros**:
- Optimal for single processor
- Can utilize 100% CPU
- Better than RMS
- Flexible

**Cons**:
- More complex implementation
- Harder to analyze
- During overload, behavior unpredictable
- Not commonly used in practice (implementation complexity)

### Priority Inversion

**Q: What is priority inversion?**

A: High-priority task blocked by low-priority task:

**Scenario**:
```
Time 0: Low priority Task L acquires Mutex M
Time 1: High priority Task H arrives, waits for Mutex M
        → Task L has priority, but H is waiting!
Time 2: Medium priority Task M becomes ready
        → Task M runs (higher priority than L)
        → Task H still waiting (priority inversion!)
```

**Famous example**: Mars Pathfinder mission (1997)
- Information bus task (high priority) blocked
- Meteorological task (low priority) held lock
- Communication task (medium priority) ran
- System watchdog reset spacecraft!

**Q: What are solutions to priority inversion?**

A:

**1. Priority Inheritance Protocol**:
```
When high-priority task blocks on resource:
→ Low-priority holder temporarily inherits high priority
→ Runs to completion faster
→ Returns to original priority
```

**Example**:
```
Time 0: Task L (priority 1) locks Mutex M
Time 1: Task H (priority 10) waits for Mutex M
        → Task L priority becomes 10 (inheritance)
Time 2: Task M (priority 5) becomes ready
        → Task L still runs (now priority 10 > 5)
Time 3: Task L releases Mutex M
        → Task L returns to priority 1
        → Task H acquires mutex and runs
```

**2. Priority Ceiling Protocol**:
```
Each resource assigned ceiling = max priority of users
Task holding resource inherits ceiling priority
Prevents medium tasks from interfering
```

**3. Disable Preemption** (simple but crude):
```
lock(mutex) {
    disable_interrupts();
    // Critical section
    enable_interrupts();
}
```
- Only for very short critical sections
- Increases interrupt latency

### Interrupt Latency

**Q: What is interrupt latency?**

A: Time from interrupt signal to first instruction of ISR:

**Components**:
1. **Interrupt recognition**: Hardware detects interrupt
2. **Current instruction completion**: Finish current instruction
3. **Context save**: Push registers to stack (automatic)
4. **Vector fetch**: Get ISR address from vector table
5. **Pipeline fill**: Load first ISR instructions

**Cortex-M4**: Typically 12-15 cycles (best case)

**Q: What affects interrupt latency?**

A:

**Increases latency**:
- Disabled interrupts (PRIMASK, FAULTMASK, BASEPRI)
- Lower priority (higher priority ISR running)
- Long instructions (division, multi-load/store)
- Cache miss
- Bus contention
- Active FPU (more state to save)

**Reduces latency**:
- Tail-chaining (consecutive interrupts)
- Late arriving (higher priority interrupts)
- Lazy stacking (FPU state)

**Q: How do you minimize interrupt latency?**

A:

**1. Keep ISRs short**:
```c
void UART_IRQHandler(void) {
    char c = UART->DR;           // Read data
    buffer[head++] = c;          // Store in buffer
    if (head >= BUFFER_SIZE)
        head = 0;
    // Signal task (don't process here!)
    set_event(UART_EVENT);
}
```

**2. Defer processing**:
```c
// ISR
void IRQHandler(void) {
    // Minimal work
    set_flag();
    trigger_task();  // Wake processing task
}

// Task
void processing_task(void) {
    while(1) {
        wait_for_flag();
        // Do heavy processing here
    }
}
```

**3. Use appropriate priorities**:
- Critical ISRs: High priority
- Background ISRs: Low priority

**4. Avoid disabling interrupts**:
```c
// BAD:
disable_interrupts();
long_operation();
enable_interrupts();

// BETTER:
use_mutex();  // Only blocks conflicting code
```

---

## Part 12: Memory Management Details

### Virtual Memory Deep Dive

**Q: How does virtual-to-physical address translation work?**

A: Through **page tables**:

**Simple paging**:
```
Virtual Address = | Page Number | Offset |
                   ↓ (lookup in page table)
Physical Address = | Frame Number | Offset |
```

**Example** (32-bit, 4KB pages):
```
Virtual: 0x12345678
         ↓
Page number = 0x12345 (bits 31-12)
Offset = 0x678 (bits 11-0)
         ↓ (lookup page table)
Frame number = 0xABCDE
         ↓
Physical: 0xABCDE678
```

**Q: What is a Translation Lookaside Buffer (TLB)?**

A: Hardware cache for page table entries:

**Without TLB**:
```
Memory access → Page table lookup (memory access) → Actual access
= 2 memory accesses per instruction!
```

**With TLB**:
```
Memory access → TLB lookup (fast!) → Actual access
= 1 memory access (if TLB hit)
```

**TLB characteristics**:
- Small (64-256 entries typical)
- Fast (1 cycle)
- High hit rate (95-99%)
- Hardware-managed (in ARM Cortex-A)
- Flushed on context switch (invalidate all/some entries)

**Q: What is the Multi-Level Page Table?**

A: Hierarchical page table structure to save memory:

**Problem with single-level**:
```
32-bit address, 4KB pages
→ 2^20 pages possible
→ Page table = 2^20 × 4 bytes = 4MB per process!
```

**Two-level solution**:
```
Virtual Address = | Level-1 Index | Level-2 Index | Offset |
                         ↓               ↓
                   Level-1 table → Level-2 table → Frame
```

**Benefits**:
- Only allocate L2 tables for used memory
- Sparse address spaces handled efficiently
- Example: 4MB for full table vs. few KB for typical process

**ARM Cortex-M4** (no MMU):
- Uses Memory Protection Unit (MPU) instead
- Region-based protection (not paging)
- Simpler, faster, but less flexible

### MPU (Memory Protection Unit)

**Q: How does MPU differ from MMU?**

A:

**MMU (Memory Management Unit)**:
- Full virtual memory
- Page-based (4KB pages)
- Translation: virtual → physical
- Complex, higher overhead
- Used in Cortex-A (application processors)

**MPU (Memory Protection Unit)**:
- No virtual memory (direct physical addressing)
- Region-based (programmable sizes)
- Protection only (no translation)
- Simple, low overhead
- Used in Cortex-M (microcontrollers)

**Q: How do you configure MPU regions?**

A: Define regions with attributes:

```c
typedef struct {
    uint32_t base_address;    // Region start (must be aligned)
    uint32_t size;            // Region size (power of 2)
    uint32_t attributes;      // Access permissions, memory type
} mpu_region_t;

// Example: Configure region for task stack
void setup_task_stack_region(void *stack_base, uint32_t size) {
    MPU->RBAR = (uint32_t)stack_base | MPU_REGION_NUMBER;
    MPU->RASR = MPU_REGION_ENABLE |
                MPU_REGION_SIZE |
                MPU_AP_RW_RW |     // Read-write for privileged & user
                MPU_XN;             // Execute Never
}
```

**Region attributes**:
- **Access permissions**:
  - No access
  - Privileged read-write, user no access
  - Privileged read-write, user read-only
  - Full access
- **Memory type**:
  - Normal (cacheable, bufferable)
  - Device (strict ordering)
  - Strongly-ordered (strictest)
- **Execute Never (XN)**: Prevent code execution

**Q: What happens on MPU violation?**

A: MemManage fault exception:
```c
void MemManage_Handler(void) {
    uint32_t fault_addr = SCB->MMFAR;  // Faulting address
    
    // Check fault type
    if (SCB->CFSR & SCB_CFSR_IACCVIOL_Msk) {
        // Instruction access violation
    }
    if (SCB->CFSR & SCB_CFSR_DACCVIOL_Msk) {
        // Data access violation
    }
    
    // Typically: terminate offending task
    terminate_task(current_task);
}
```

### Stack Management

**Q: How does stack overflow detection work?**

A: Multiple approaches:

**1. MPU-based** (hardware):
```c
// Configure guard region below stack
void setup_stack_guard(void *stack_base, uint32_t stack_size) {
    // Stack grows down, guard at bottom
    void *guard_base = (char*)stack_base - GUARD_SIZE;
    
    MPU->RBAR = (uint32_t)guard_base | REGION_NUM;
    MPU->RASR = MPU_REGION_ENABLE |
                SIZE_TO_MPU(GUARD_SIZE) |
                MPU_AP_NO_ACCESS;  // No access!
}
```
- Automatic detection
- Zero overhead at runtime
- Hardware fault on overflow

**2. Software check** (periodic):
```c
void check_stack_overflow(void) {
    uint32_t current_sp = __get_PSP();
    task_t *task = current_task;
    
    if (current_sp < task->stack_limit) {
        // Overflow detected!
        handle_stack_overflow(task);
    }
}

// Call from SysTick or before context switch
```
- Software overhead
- May miss overflow between checks

**3. Canary value** (guard pattern):
```c
#define STACK_CANARY 0xDEADBEEF

void init_task_stack(task_t *task) {
    // Put canary at stack bottom
    *(uint32_t*)(task->stack_base) = STACK_CANARY;
}

void check_canary(task_t *task) {
    if (*(uint32_t*)(task->stack_base) != STACK_CANARY) {
        // Stack overflow corrupted canary!
        handle_overflow(task);
    }
}
```
- Simple
- Only detects after overflow
- Can be bypassed if overflow skips canary

**Q: How much stack does a task need?**

A: Calculate worst-case:

```
Stack size = Context_save_size + 
             Max_call_depth × Frame_size +
             Max_local_variables +
             Safety_margin

Where:
Context_save = 8 words (M3) or 26 words (M4 with FPU)
Frame_size = Return address + saved registers + locals
```

**Example**:
```c
void task_function(void) {
    char buffer[512];        // 512 bytes locals
    process_data(buffer);    // Nested call
}

void process_data(char *data) {
    int temp[64];            // 256 bytes locals
    // ...
}

Minimum stack = 26×4 + 512 + 256 + safety
              = 104 + 512 + 256 + 256
              = 1128 bytes
→ Allocate 2KB to be safe
```

**Tips**:
- Avoid large local arrays (use dynamic allocation)
- Limit recursion depth
- Monitor actual usage during development
- Add margin (25-50%)

---

## Part 13: RTOS Task Management

### Task States in Detail

**Q: What are all possible RTOS task states?**

A: Typical RTOS has these states:

```
DORMANT → READY ⇄ RUNNING ⇄ SUSPENDED
            ↕         ↕
         BLOCKED   WAITING
                     ↓
                 TERMINATED
```

**State descriptions**:

1. **DORMANT**: 
   - Task created but not started
   - Resources allocated but not activated

2. **READY**: 
   - Waiting for CPU
   - Can run immediately
   - In ready queue

3. **RUNNING**: 
   - Currently executing
   - Only one per CPU

4. **BLOCKED**: 
   - Waiting for event (I/O, semaphore)
   - Not in ready queue
   - Will be moved to READY when event occurs

5. **WAITING**: 
   - Sleeping for specified time
   - Timeout-based blocking

6. **SUSPENDED**: 
   - Explicitly suspended by another task
   - Not schedulable even if ready
   - Must be explicitly resumed

7. **TERMINATED**: 
   - Finished execution
   - Resources being cleaned up

### Task Control Block Details

**Q: What's in a complete TCB?**

A: Comprehensive structure:

```c
typedef struct task_control_block {
    // Execution context
    void *stack_pointer;          // Current SP
    void *stack_base;             // Stack bottom
    uint32_t stack_size;          // Total size
    
    // Identification
    uint32_t task_id;
    char name[MAX_NAME_LEN];
    
    // Scheduling
    uint8_t priority;             // Task priority
    uint8_t original_priority;    // For priority inheritance
    uint32_t state;               // Current state
    uint32_t time_slice;          // Remaining quantum
    
    // Synchronization
    void *waiting_on;             // Resource waiting for
    uint32_t timeout;             // Timeout value
    uint32_t wakeup_time;         // Absolute wakeup time
    
    // Statistics
    uint32_t total_runtime;       // Total CPU time
    uint32_t context_switches;    // Number of switches
    uint32_t max_stack_usage;     // Peak stack usage
    
    // Queue links
    struct task_control_block *next;
    struct task_control_block *prev;
    
    // Cleanup
    void (*cleanup_func)(void);   // Called on termination
    void *cleanup_arg;
} TCB_t;
```

# Operating Systems - Comprehensive Study Notes (Continued)

## Part 13: RTOS Task Management (Continued)

### Task Creation (Continued)

**Q: What happens during task creation?**

A: Step-by-step process:

```c
task_handle_t create_task(
    void (*task_func)(void*),    // Task function
    void *param,                  // Parameter to pass
    uint32_t stack_size,         // Stack size in bytes
    uint8_t priority,            // Task priority
    char *name                   // Task name
) {
    // 1. Allocate TCB
    TCB_t *tcb = malloc(sizeof(TCB_t));
    if (!tcb) return NULL;
    
    // 2. Allocate stack
    void *stack = malloc(stack_size);
    if (!stack) {
        free(tcb);
        return NULL;
    }
    
    // 3. Initialize TCB
    tcb->stack_base = stack;
    tcb->stack_size = stack_size;
    tcb->priority = priority;
    tcb->state = READY;
    tcb->task_id = next_task_id++;
    strncpy(tcb->name, name, MAX_NAME_LEN);
    
    // 4. Initialize stack frame
    // Stack grows downward, so start at top
    uint32_t *stack_top = (uint32_t*)((char*)stack + stack_size);
    
    // Create initial stack frame (as if task was interrupted)
    stack_top -= 16;  // Make room for stack frame
    
    // Hardware-stacked registers (auto-saved on exception)
    stack_top[15] = 0x01000000;           // xPSR (Thumb bit set)
    stack_top[14] = (uint32_t)task_func;  // PC (entry point)
    stack_top[13] = (uint32_t)task_exit;  // LR (return = exit task)
    stack_top[12] = 0;                     // R12
    stack_top[11] = 0;                     // R3
    stack_top[10] = 0;                     // R2
    stack_top[9] = 0;                      // R1
    stack_top[8] = (uint32_t)param;       // R0 (parameter)
    
    // Software-stacked registers (manually saved in PendSV)
    stack_top[7] = 0;                      // R11
    stack_top[6] = 0;                      // R10
    stack_top[5] = 0;                      // R9
    stack_top[4] = 0;                      // R8
    stack_top[3] = 0;                      // R7
    stack_top[2] = 0;                      // R6
    stack_top[1] = 0;                      // R5
    stack_top[0] = 0;                      // R4
    
    tcb->stack_pointer = (void*)stack_top;
    
    // 5. Add to ready queue
    add_to_ready_queue(tcb);
    
    // 6. If higher priority than current, trigger context switch
    if (priority > current_task->priority) {
        trigger_context_switch();
    }
    
    return (task_handle_t)tcb;
}
```

**Q: What is the task_exit function?**

A: Cleanup when task returns:

```c
void task_exit(void) {
    // This is called if task function returns
    // Task should never return, but if it does:
    
    disable_interrupts();
    
    // Remove from all queues
    remove_from_ready_queue(current_task);
    
    // Mark as terminated
    current_task->state = TERMINATED;
    
    // Call cleanup function if registered
    if (current_task->cleanup_func) {
        current_task->cleanup_func(current_task->cleanup_arg);
    }
    
    // Add to terminated list (for later resource cleanup)
    add_to_terminated_list(current_task);
    
    // Force context switch to another task
    current_task = NULL;  // No longer valid
    schedule();           // Never returns
    
    // Should never reach here
    while(1);
}
```

### Task Deletion

**Q: How do you safely delete a task?**

A: Multiple approaches depending on who deletes:

**1. Self-deletion** (task deletes itself):
```c
void task_delete_self(void) {
    disable_interrupts();
    
    current_task->state = TERMINATED;
    remove_from_ready_queue(current_task);
    
    // Defer resource cleanup (can't free own stack!)
    add_to_cleanup_list(current_task);
    
    // Switch to another task
    schedule();  // Never returns
}
```

**2. Delete another task**:
```c
void task_delete(task_handle_t handle) {
    TCB_t *tcb = (TCB_t*)handle;
    
    disable_interrupts();
    
    // Can't delete running task this way
    if (tcb == current_task) {
        enable_interrupts();
        task_delete_self();
        return;
    }
    
    // Remove from all queues
    remove_from_all_queues(tcb);
    
    // Mark as terminated
    tcb->state = TERMINATED;
    
    // Safe to free resources (not currently running)
    free(tcb->stack_base);
    free(tcb);
    
    enable_interrupts();
}
```

**3. Cleanup task** (idle task cleanup):
```c
void idle_task(void *param) {
    while (1) {
        // Clean up terminated tasks
        TCB_t *tcb;
        while ((tcb = get_terminated_task()) != NULL) {
            free(tcb->stack_base);
            free(tcb);
        }
        
        // Enter low-power mode
        __WFI();  // Wait For Interrupt
    }
}
```

### Task Scheduling Algorithms

**Q: What scheduling algorithms do RTOSs use?**

A: Common algorithms:

**1. Priority-Based Preemptive** (most common):
```c
TCB_t* priority_schedule(void) {
    // Find highest priority ready task
    TCB_t *highest = NULL;
    uint8_t max_priority = 0;
    
    for (int i = 0; i < num_tasks; i++) {
        TCB_t *task = &task_list[i];
        if (task->state == READY && task->priority > max_priority) {
            max_priority = task->priority;
            highest = task;
        }
    }
    
    return highest;
}
```

**Optimization with priority bitmap**:
```c
// Fast priority lookup using bit operations
uint32_t ready_bitmap = 0;  // Bit per priority level

void add_to_ready(TCB_t *task) {
    ready_bitmap |= (1 << task->priority);
    // Add to priority queue...
}

TCB_t* fast_priority_schedule(void) {
    // Find highest set bit (highest priority)
    int highest_priority = 31 - __CLZ(ready_bitmap);  // Count Leading Zeros
    
    // Get task from that priority queue
    return get_task_at_priority(highest_priority);
}
```

**2. Round-Robin** (time-sliced):
```c
TCB_t* round_robin_schedule(void) {
    static int current_index = 0;
    
    // Find next ready task in circular order
    int start = current_index;
    do {
        current_index = (current_index + 1) % num_tasks;
        TCB_t *task = &task_list[current_index];
        
        if (task->state == READY) {
            task->time_slice = DEFAULT_QUANTUM;
            return task;
        }
    } while (current_index != start);
    
    return NULL;  // No ready tasks
}
```

**3. Priority + Round-Robin** (hybrid):
```c
// Each priority level has round-robin queue
typedef struct {
    TCB_t *queue[MAX_TASKS_PER_PRIORITY];
    int head, tail, count;
} priority_queue_t;

priority_queue_t ready_queues[NUM_PRIORITIES];

TCB_t* hybrid_schedule(void) {
    // Find highest priority with ready tasks
    for (int pri = NUM_PRIORITIES - 1; pri >= 0; pri--) {
        if (ready_queues[pri].count > 0) {
            // Round-robin within this priority
            TCB_t *task = dequeue(&ready_queues[pri]);
            task->time_slice = DEFAULT_QUANTUM;
            return task;
        }
    }
    return NULL;
}

// On time slice expiration
void time_slice_expired(void) {
    // Move current task to back of its priority queue
    enqueue(&ready_queues[current_task->priority], current_task);
    // Trigger context switch
    trigger_context_switch();
}
```

**Q: How does the scheduler get invoked?**

A: Multiple trigger points:

**1. Timer interrupt** (time-slicing):
```c
void SysTick_Handler(void) {
    systick_count++;
    
    // Check if time slice expired
    if (current_task->time_slice > 0) {
        current_task->time_slice--;
    }
    
    if (current_task->time_slice == 0) {
        // Time slice expired, trigger context switch
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    }
    
    // Check sleeping tasks
    check_sleeping_tasks();
}
```

**2. Task blocks** (wait, sleep):
```c
void task_wait(semaphore_t *sem) {
    disable_interrupts();
    
    sem->count--;
    if (sem->count < 0) {
        current_task->state = BLOCKED;
        current_task->waiting_on = sem;
        add_to_wait_queue(&sem->waiting_queue, current_task);
        
        // Trigger context switch
        trigger_context_switch();
    }
    
    enable_interrupts();
}
```

**3. Higher priority task becomes ready**:
```c
void task_signal(semaphore_t *sem) {
    disable_interrupts();
    
    sem->count++;
    if (sem->count <= 0) {
        TCB_t *task = dequeue(&sem->waiting_queue);
        task->state = READY;
        task->waiting_on = NULL;
        add_to_ready_queue(task);
        
        // If higher priority, preempt current task
        if (task->priority > current_task->priority) {
            trigger_context_switch();
        }
    }
    
    enable_interrupts();
}
```

### Context Switch Optimization

**Q: How can context switches be optimized?**

A: Several techniques:

**1. Lazy FPU context save**:
```c
void PendSV_Handler(void) {
    // Check if FPU was used
    if (current_task->fpu_used) {
        // Save FPU registers (S0-S15, FPSCR)
        asm("VSTMDB R0!, {S16-S31}");
    }
    
    // Normal context save
    // ...
    
    // Load next task
    if (next_task->fpu_used) {
        asm("VLDMIA R0!, {S16-S31}");
    }
}
```

**2. Tail-chaining**:
- Hardware optimization in Cortex-M
- Consecutive interrupts skip unstacking/stacking
- Automatic, no software needed
- Saves ~12 cycles

**3. Minimize critical sections**:
```c
// BAD: Long critical section
void schedule(void) {
    disable_interrupts();
    
    // Long scheduling algorithm
    next_task = complex_scheduling_logic();
    
    // Update queues
    update_all_queues();
    
    enable_interrupts();
}

// BETTER: Short critical section
void schedule(void) {
    // Do work outside critical section
    TCB_t *next = find_next_task();
    
    // Only protect actual switch
    disable_interrupts();
    swap_tasks(current_task, next);
    enable_interrupts();
}
```

**4. Batch operations**:
```c
void wake_multiple_tasks(TCB_t **tasks, int count) {
    disable_interrupts();
    
    bool need_switch = false;
    uint8_t highest_priority = current_task->priority;
    
    // Wake all tasks in one critical section
    for (int i = 0; i < count; i++) {
        tasks[i]->state = READY;
        add_to_ready_queue(tasks[i]);
        
        if (tasks[i]->priority > highest_priority) {
            need_switch = true;
            highest_priority = tasks[i]->priority;
        }
    }
    
    // Single context switch for all
    if (need_switch) {
        trigger_context_switch();
    }
    
    enable_interrupts();
}
```

---

## Part 14: Inter-Task Communication

### Message Queues

**Q: What is a message queue?**

A: FIFO buffer for inter-task communication:

```c
typedef struct {
    void *buffer;              // Message storage
    uint32_t msg_size;         // Size of each message
    uint32_t capacity;         // Max messages
    uint32_t count;            // Current count
    uint32_t head, tail;       // Queue pointers
    
    semaphore_t free_slots;    // Count free slots
    semaphore_t used_slots;    // Count used slots
    mutex_t mutex;             // Protect queue
    
    TCB_t *waiting_senders;    // Tasks waiting to send
    TCB_t *waiting_receivers;  // Tasks waiting to receive
} message_queue_t;

// Create queue
message_queue_t* mq_create(uint32_t msg_size, uint32_t capacity) {
    message_queue_t *mq = malloc(sizeof(message_queue_t));
    
    mq->buffer = malloc(msg_size * capacity);
    mq->msg_size = msg_size;
    mq->capacity = capacity;
    mq->count = 0;
    mq->head = 0;
    mq->tail = 0;
    
    sem_init(&mq->free_slots, capacity);
    sem_init(&mq->used_slots, 0);
    mutex_init(&mq->mutex);
    
    return mq;
}

// Send message (blocking)
void mq_send(message_queue_t *mq, void *msg) {
    sem_wait(&mq->free_slots);  // Wait for free slot
    mutex_lock(&mq->mutex);
    
    // Copy message to queue
    void *dest = (char*)mq->buffer + (mq->tail * mq->msg_size);
    memcpy(dest, msg, mq->msg_size);
    
    mq->tail = (mq->tail + 1) % mq->capacity;
    mq->count++;
    
    mutex_unlock(&mq->mutex);
    sem_signal(&mq->used_slots);  // Signal message available
}

// Receive message (blocking)
void mq_receive(message_queue_t *mq, void *msg) {
    sem_wait(&mq->used_slots);  // Wait for message
    mutex_lock(&mq->mutex);
    
    // Copy message from queue
    void *src = (char*)mq->buffer + (mq->head * mq->msg_size);
    memcpy(msg, src, mq->msg_size);
    
    mq->head = (mq->head + 1) % mq->capacity;
    mq->count--;
    
    mutex_unlock(&mq->mutex);
    sem_signal(&mq->free_slots);  // Signal free slot
}
```

**Q: What are message queue variants?**

A:

**1. With timeout**:
```c
bool mq_send_timeout(message_queue_t *mq, void *msg, uint32_t timeout_ms) {
    if (!sem_wait_timeout(&mq->free_slots, timeout_ms)) {
        return false;  // Timeout
    }
    
    mutex_lock(&mq->mutex);
    // ... copy message ...
    mutex_unlock(&mq->mutex);
    sem_signal(&mq->used_slots);
    
    return true;  // Success
}
```

**2. Non-blocking**:
```c
bool mq_send_nonblock(message_queue_t *mq, void *msg) {
    if (!sem_try_wait(&mq->free_slots)) {
        return false;  // Queue full
    }
    
    mutex_lock(&mq->mutex);
    // ... copy message ...
    mutex_unlock(&mq->mutex);
    sem_signal(&mq->used_slots);
    
    return true;
}
```

**3. Priority queue**:
```c
typedef struct {
    void *message;
    uint8_t priority;
} priority_msg_t;

void mq_send_priority(message_queue_t *mq, void *msg, uint8_t priority) {
    sem_wait(&mq->free_slots);
    mutex_lock(&mq->mutex);
    
    // Insert in priority order
    int insert_pos = find_insert_position(mq, priority);
    insert_at_position(mq, msg, priority, insert_pos);
    
    mutex_unlock(&mq->mutex);
    sem_signal(&mq->used_slots);
}
```

### Event Flags

**Q: What are event flags?**

A: Bit-based synchronization mechanism:

```c
typedef struct {
    uint32_t flags;            // Event bits
    mutex_t mutex;             // Protect flags
    TCB_t *waiting_tasks;      // Tasks waiting for events
} event_group_t;

#define EVENT_FLAG_0  (1 << 0)
#define EVENT_FLAG_1  (1 << 1)
#define EVENT_FLAG_2  (1 << 2)
// ... up to 32 flags

// Set event flags
void event_set(event_group_t *eg, uint32_t flags) {
    mutex_lock(&eg->mutex);
    
    eg->flags |= flags;
    
    // Wake tasks waiting for these flags
    wake_waiting_tasks(eg);
    
    mutex_unlock(&eg->mutex);
}

// Clear event flags
void event_clear(event_group_t *eg, uint32_t flags) {
    mutex_lock(&eg->mutex);
    eg->flags &= ~flags;
    mutex_unlock(&eg->mutex);
}

// Wait for ANY flags (OR)
uint32_t event_wait_any(event_group_t *eg, uint32_t flags, uint32_t timeout) {
    mutex_lock(&eg->mutex);
    
    while ((eg->flags & flags) == 0) {
        // No matching flags yet
        add_to_wait_list(eg, current_task, flags, WAIT_ANY);
        
        mutex_unlock(&eg->mutex);
        task_block_timeout(timeout);
        mutex_lock(&eg->mutex);
        
        if (timeout_expired()) {
            mutex_unlock(&eg->mutex);
            return 0;  // Timeout
        }
    }
    
    uint32_t matched = eg->flags & flags;
    mutex_unlock(&eg->mutex);
    return matched;
}

// Wait for ALL flags (AND)
uint32_t event_wait_all(event_group_t *eg, uint32_t flags, uint32_t timeout) {
    mutex_lock(&eg->mutex);
    
    while ((eg->flags & flags) != flags) {
        // Not all flags set yet
        add_to_wait_list(eg, current_task, flags, WAIT_ALL);
        
        mutex_unlock(&eg->mutex);
        task_block_timeout(timeout);
        mutex_lock(&eg->mutex);
        
        if (timeout_expired()) {
            mutex_unlock(&eg->mutex);
            return 0;
        }
    }
    
    mutex_unlock(&eg->mutex);
    return flags;
}
```

**Example usage**:
```c
event_group_t system_events;

// Producer tasks
void sensor_task(void) {
    while (1) {
        read_sensor();
        event_set(&system_events, SENSOR_DATA_READY);
        delay(100);
    }
}

void network_task(void) {
    while (1) {
        if (network_data_received()) {
            event_set(&system_events, NETWORK_DATA_READY);
        }
    }
}

// Consumer task
void processing_task(void) {
    while (1) {
        // Wait for either sensor OR network data
        uint32_t events = event_wait_any(&system_events, 
                                         SENSOR_DATA_READY | NETWORK_DATA_READY,
                                         INFINITE_TIMEOUT);
        
        if (events & SENSOR_DATA_READY) {
            process_sensor_data();
            event_clear(&system_events, SENSOR_DATA_READY);
        }
        
        if (events & NETWORK_DATA_READY) {
            process_network_data();
            event_clear(&system_events, NETWORK_DATA_READY);
        }
    }
}
```

### Mailboxes

**Q: What is a mailbox?**

A: Single-message buffer (message queue with capacity=1):

```c
typedef struct {
    void *message;             // Single message slot
    bool full;                 // Is slot occupied?
    mutex_t mutex;
    semaphore_t data_available;
    TCB_t *waiting_sender;
    TCB_t *waiting_receiver;
} mailbox_t;

// Create mailbox
mailbox_t* mailbox_create(void) {
    mailbox_t *mb = malloc(sizeof(mailbox_t));
    mb->message = NULL;
    mb->full = false;
    mutex_init(&mb->mutex);
    sem_init(&mb->data_available, 0);
    return mb;
}

// Post message (blocking if full)
void mailbox_post(mailbox_t *mb, void *msg) {
    mutex_lock(&mb->mutex);
    
    while (mb->full) {
        // Wait for mailbox to be empty
        mb->waiting_sender = current_task;
        mutex_unlock(&mb->mutex);
        task_block();
        mutex_lock(&mb->mutex);
    }
    
    mb->message = msg;
    mb->full = true;
    
    // Wake waiting receiver
    if (mb->waiting_receiver) {
        task_wake(mb->waiting_receiver);
        mb->waiting_receiver = NULL;
    }
    
    mutex_unlock(&mb->mutex);
    sem_signal(&mb->data_available);
}

// Fetch message (blocking if empty)
void* mailbox_fetch(mailbox_t *mb) {
    sem_wait(&mb->data_available);
    mutex_lock(&mb->mutex);
    
    void *msg = mb->message;
    mb->message = NULL;
    mb->full = false;
    
    // Wake waiting sender
    if (mb->waiting_sender) {
        task_wake(mb->waiting_sender);
        mb->waiting_sender = NULL;
    }
    
    mutex_unlock(&mb->mutex);
    return msg;
}
```

**Use case**: Simple one-to-one communication
```c
mailbox_t *cmd_mailbox;

// Command sender
void control_task(void) {
    while (1) {
        command_t *cmd = get_next_command();
        mailbox_post(cmd_mailbox, cmd);
    }
}

// Command processor
void worker_task(void) {
    while (1) {
        command_t *cmd = mailbox_fetch(cmd_mailbox);
        process_command(cmd);
        free(cmd);
    }
}
```

### Memory Pools

**Q: What are memory pools and why use them?**

A: Pre-allocated fixed-size memory blocks:

**Problems with malloc/free in RTOS**:
- Non-deterministic time
- Fragmentation
- Not real-time safe
- May fail unpredictably

**Memory pool solution**:
```c
typedef struct {
    void *pool_base;           // Start of pool
    uint32_t block_size;       // Size of each block
    uint32_t num_blocks;       // Total blocks
    uint32_t free_blocks;      // Available blocks
    void **free_list;          // List of free blocks
    mutex_t mutex;
} memory_pool_t;

// Create pool
memory_pool_t* pool_create(uint32_t block_size, uint32_t num_blocks) {
    memory_pool_t *pool = malloc(sizeof(memory_pool_t));
    
    // Allocate pool memory
    pool->pool_base = malloc(block_size * num_blocks);
    pool->block_size = block_size;
    pool->num_blocks = num_blocks;
    pool->free_blocks = num_blocks;
    
    // Initialize free list
    pool->free_list = malloc(sizeof(void*) * num_blocks);
    for (int i = 0; i < num_blocks; i++) {
        pool->free_list[i] = (char*)pool->pool_base + (i * block_size);
    }
    
    mutex_init(&pool->mutex);
    return pool;
}

// Allocate block (O(1) time!)
void* pool_alloc(memory_pool_t *pool) {
    mutex_lock(&pool->mutex);
    
    if (pool->free_blocks == 0) {
        mutex_unlock(&pool->mutex);
        return NULL;  // Pool exhausted
    }
    
    void *block = pool->free_list[--pool->free_blocks];
    
    mutex_unlock(&pool->mutex);
    return block;
}

// Free block (O(1) time!)
void pool_free(memory_pool_t *pool, void *block) {
    mutex_lock(&pool->mutex);
    
    pool->free_list[pool->free_blocks++] = block;
    
    mutex_unlock(&pool->mutex);
}
```

**Usage example**:
```c
// Application setup
memory_pool_t *msg_pool;

void app_init(void) {
    // Create pool for messages
    msg_pool = pool_create(sizeof(message_t), 50);
}

// Fast, deterministic allocation
void send_message(void) {
    message_t *msg = pool_alloc(msg_pool);
    if (msg) {
        msg->type = CMD_START;
        msg->data = 42;
        mq_send(command_queue, msg);
    }
}

void receive_message(void) {
    message_t *msg;
    mq_receive(command_queue, &msg);
    process_message(msg);
    pool_free(msg_pool, msg);  // Return to pool
}
```

---

## Part 15: Advanced Topics

### Software Timers

**Q: What are software timers in an RTOS?**

A: User-space timers managed by OS:

```c
typedef struct software_timer {
    uint32_t period;           // Timer period (ticks)
    uint32_t remaining;        // Ticks until expiry
    bool auto_reload;          // Restart after expiry?
    bool active;               // Is timer running?
    void (*callback)(void*);   // Function to call
    void *param;               // Parameter for callback
    struct software_timer *next;
} sw_timer_t;

sw_timer_t *timer_list = NULL;

// Create timer
sw_timer_t* timer_create(uint32_t period_ms, 
                         bool auto_reload,
                         void (*callback)(void*),
                         void *param) {
    sw_timer_t *timer = malloc(sizeof(sw_timer_t));
    timer->period = MS_TO_TICKS(period_ms);
    timer->remaining = timer->period;
    timer->auto_reload = auto_reload;
    timer->active = false;
    timer->callback = callback;
    timer->param = param;
    
    // Add to timer list
    disable_interrupts();
    timer->next = timer_list;
    timer_list = timer;
    enable_interrupts();
    
    return timer;
}

// Start timer
void timer_start(sw_timer_t *timer) {
    disable_interrupts();
    timer->remaining = timer->period;
    timer->active = true;
    enable_interrupts();
}

// Stop timer
void timer_stop(sw_timer_t *timer) {
    disable_interrupts();
    timer->active = false;
    enable_interrupts();
}

// Timer tick (called from SysTick)
void timer_tick(void) {
    sw_timer_t *timer = timer_list;
    
    while (timer) {
        if (timer->active) {
            if (--timer->remaining == 0) {
                // Timer expired
                if (timer->auto_reload) {
                    timer->remaining = timer->period;
                } else {
                    timer->active = false;
                }
                
                // Execute callback
                if (timer->callback) {
                    timer->callback(timer->param);
                }
            }
        }
        timer = timer->next;
    }
}
```

**Usage**:
```c
void timeout_handler(void *param) {
    printf("Timer expired!\n");
    // Handle timeout
}

void app_main(void) {
    // One-shot timer (500ms)
    sw_timer_t *one_shot = timer_create(500, false, timeout_handler, NULL);
    timer_start(one_shot);
    
    // Periodic timer (1000ms)
    sw_timer_t *periodic = timer_create(1000, true, timeout_handler, NULL);
    timer_start(periodic);
}
```

### Watchdog Integration

**Q: How do you integrate a watchdog timer with RTOS?**

A: Monitor system health:

```c
typedef struct {
    uint32_t timeout_ms;
    bool enabled;
    sw_timer_t *feed_timer;
} watchdog_t;

watchdog_t system_watchdog;

// Initialize watchdog
void watchdog_init(uint32_t timeout_ms) {
    // Configure hardware watchdog
    IWDG->KR = 0xCCCC;  // Start watchdog
    IWDG->KR = 0x5555;  // Enable register access
    IWDG->PR = calculate_prescaler(timeout_ms);
    IWDG->RLR = calculate_reload(timeout_ms);
    IWDG->KR = 0xAAAA;  // Refresh watchdog
    
    system_watchdog.timeout_ms = timeout_ms;
    system_watchdog.enabled = true;
    
    // Create timer to feed watchdog periodically
    system_watchdog.feed_timer = timer_create(
        timeout_ms / 2,  // Feed at half timeout period
        true,            // Auto-reload
        watchdog_feed,
        NULL
    );
    timer_start(system_watchdog.feed_timer);
}

// Feed watchdog
void watchdog_feed(void *param) {
    if (system_watchdog.enabled && system_healthy()) {
        IWDG->KR = 0xAAAA;  // Refresh watchdog
    }
    // If system not healthy, let watchdog reset system
}

// Health check
bool system_healthy(void) {
    // Check critical tasks are running
    if (!is_task_alive(critical_task_1)) return false;
    if (!is_task_alive(critical_task_2)) return false;
    
    // Check no deadlock
    if (detect_deadlock()) return false;
    
    
    // Check stack usage
    for (each task) {
        if (task_stack_usage(task) > 90%) return false;
    }
    
    // Check heap usage
    if (heap_usage() > 95%) return false;
    
    // All checks passed
    return true;
}

// Task liveness tracking
typedef struct {
    TCB_t *task;
    uint32_t last_heartbeat;
    uint32_t timeout_ms;
} task_monitor_t;

task_monitor_t monitored_tasks[MAX_MONITORED];

void task_heartbeat(TCB_t *task) {
    // Task calls this periodically to prove it's alive
    for (int i = 0; i < MAX_MONITORED; i++) {
        if (monitored_tasks[i].task == task) {
            monitored_tasks[i].last_heartbeat = get_tick_count();
            break;
        }
    }
}

bool is_task_alive(TCB_t *task) {
    uint32_t now = get_tick_count();
    
    for (int i = 0; i < MAX_MONITORED; i++) {
        if (monitored_tasks[i].task == task) {
            uint32_t elapsed = now - monitored_tasks[i].last_heartbeat;
            return (elapsed < monitored_tasks[i].timeout_ms);
        }
    }
    return false;  // Not monitored
}
```

### Power Management

**Q: How does an RTOS implement power management?**

A: Multiple techniques:

**1. Idle task low-power mode**:
```c
void idle_task(void *param) {
    while (1) {
        // Clean up terminated tasks
        cleanup_terminated_tasks();
        
        // Enter low-power mode
        __disable_irq();
        
        // Check if we can sleep
        if (no_immediate_work()) {
            // Configure wake-up sources
            configure_wakeup_interrupts();
            
            // Enter sleep mode
            __WFI();  // Wait For Interrupt
            // CPU stops, peripherals run
            // Wakes on any interrupt
        }
        
        __enable_irq();
    }
}
```

**2. Tickless idle**:
```c
// Normal: SysTick every 1ms (wastes power)
// Tickless: Stop SysTick during idle, program next wake time

void tickless_idle(void) {
    __disable_irq();
    
    if (can_enter_tickless()) {
        // Find next scheduled event
        uint32_t next_task_time = find_next_task_wakeup();
        uint32_t next_timer_time = find_next_timer_expiry();
        uint32_t sleep_time = MIN(next_task_time, next_timer_time);
        
        if (sleep_time > MIN_TICKLESS_PERIOD) {
            // Stop SysTick
            SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
            
            // Program RTC wake-up
            configure_rtc_wakeup(sleep_time);
            
            // Compensate tick count
            uint32_t before = get_rtc_count();
            
            // Sleep
            __WFI();
            
            // Calculate slept time
            uint32_t after = get_rtc_count();
            uint32_t slept = after - before;
            
            // Update system tick
            systick_count += slept;
            
            // Restart SysTick
            SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
        }
    }
    
    __enable_irq();
}
```

**3. Dynamic voltage/frequency scaling**:
```c
typedef enum {
    POWER_MODE_HIGH,      // Full speed
    POWER_MODE_MEDIUM,    // Reduced speed
    POWER_MODE_LOW        // Minimum speed
} power_mode_t;

void set_power_mode(power_mode_t mode) {
    switch (mode) {
        case POWER_MODE_HIGH:
            // 180 MHz, high voltage
            set_system_clock(180000000);
            set_voltage_scale(PWR_SCALE1);
            break;
            
        case POWER_MODE_MEDIUM:
            // 84 MHz, medium voltage
            set_system_clock(84000000);
            set_voltage_scale(PWR_SCALE2);
            break;
            
        case POWER_MODE_LOW:
            // 42 MHz, low voltage
            set_system_clock(42000000);
            set_voltage_scale(PWR_SCALE3);
            break;
    }
    
    // Adjust SysTick for new frequency
    reconfigure_systick();
}

// Adaptive power management
void power_monitor_task(void *param) {
    while (1) {
        uint32_t cpu_usage = calculate_cpu_usage();
        
        if (cpu_usage > 80%) {
            set_power_mode(POWER_MODE_HIGH);
        } else if (cpu_usage > 40%) {
            set_power_mode(POWER_MODE_MEDIUM);
        } else {
            set_power_mode(POWER_MODE_LOW);
        }
        
        delay(1000);  // Check every second
    }
}
```

### CPU Usage Measurement

**Q: How do you measure CPU usage in an RTOS?**

A: Track idle task execution:

```c
volatile uint32_t idle_counter = 0;
volatile uint32_t total_counter = 0;

// Idle task
void idle_task(void *param) {
    while (1) {
        idle_counter++;  // Count idle loops
        
        // Optional: WFI if no work
        if (no_ready_tasks()) {
            __WFI();
        }
    }
}

// Measurement task
void cpu_monitor_task(void *param) {
    uint32_t last_idle = 0;
    uint32_t last_total = 0;
    
    while (1) {
        delay(1000);  // Measure every second
        
        uint32_t idle_delta = idle_counter - last_idle;
        uint32_t total_delta = total_counter - last_total;
        
        last_idle = idle_counter;
        last_total = total_counter;
        
        // CPU usage = (1 - idle_time/total_time) * 100
        uint32_t cpu_usage = 100 - ((idle_delta * 100) / total_delta);
        
        printf("CPU Usage: %d%%\n", cpu_usage);
    }
}

// In SysTick or main loop
void count_tick(void) {
    total_counter++;
}
```

**Alternative: Per-task CPU usage**:
```c
typedef struct {
    TCB_t *task;
    uint32_t runtime;        // Total execution time
    uint32_t last_start;     // When task started running
} task_stats_t;

task_stats_t task_statistics[MAX_TASKS];

// Called on context switch OUT
void track_task_end(TCB_t *task) {
    uint32_t now = get_cycle_count();  // CPU cycle counter
    
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_statistics[i].task == task) {
            task_statistics[i].runtime += (now - task_statistics[i].last_start);
            break;
        }
    }
}

// Called on context switch IN
void track_task_start(TCB_t *task) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_statistics[i].task == task) {
            task_statistics[i].last_start = get_cycle_count();
            break;
        }
    }
}

// Calculate per-task usage
void print_task_usage(void) {
    uint32_t total_runtime = 0;
    
    // Calculate total
    for (int i = 0; i < MAX_TASKS; i++) {
        total_runtime += task_statistics[i].runtime;
    }
    
    // Print each task
    for (int i = 0; i < MAX_TASKS; i++) {
        uint32_t usage = (task_statistics[i].runtime * 100) / total_runtime;
        printf("%s: %d%%\n", task_statistics[i].task->name, usage);
    }
}
```

### Stack Overflow Detection (Advanced)

**Q: What advanced techniques exist for stack overflow detection?**

A: Beyond basic canaries:

**1. Periodic stack checking**:
```c
void check_all_stacks(void) {
    for (int i = 0; i < num_tasks; i++) {
        TCB_t *task = &task_list[i];
        
        // Calculate stack usage
        uint32_t *stack_bottom = (uint32_t*)task->stack_base;
        uint32_t *stack_top = (uint32_t*)((char*)task->stack_base + task->stack_size);
        
        // Find highest used address (lowest address with non-pattern)
        uint32_t *ptr = stack_bottom;
        while (ptr < stack_top && *ptr == STACK_FILL_PATTERN) {
            ptr++;
        }
        
        uint32_t used = (char*)stack_top - (char*)ptr;
        uint32_t percent = (used * 100) / task->stack_size;
        
        // Update maximum
        if (used > task->max_stack_usage) {
            task->max_stack_usage = used;
        }
        
        // Warn if getting full
        if (percent > 80) {
            printf("WARNING: Task %s stack %d%% full\n", task->name, percent);
        }
        
        // Error if overflow
        if (used > task->stack_size) {
            printf("ERROR: Task %s stack overflow!\n", task->name);
            handle_stack_overflow(task);
        }
    }
}
```

**2. Stack painting**:
```c
#define STACK_FILL_PATTERN 0xA5A5A5A5

void init_task_stack_painted(TCB_t *task) {
    // Fill entire stack with pattern
    uint32_t *ptr = (uint32_t*)task->stack_base;
    uint32_t words = task->stack_size / 4;
    
    for (uint32_t i = 0; i < words; i++) {
        ptr[i] = STACK_FILL_PATTERN;
    }
    
    // Then set up stack frame at top
    setup_stack_frame(task);
}

// Later: scan for pattern to find high-water mark
uint32_t measure_stack_usage(TCB_t *task) {
    uint32_t *ptr = (uint32_t*)task->stack_base;
    uint32_t words = task->stack_size / 4;
    uint32_t unused_words = 0;
    
    // Count unused words from bottom
    for (uint32_t i = 0; i < words; i++) {
        if (ptr[i] != STACK_FILL_PATTERN) {
            break;
        }
        unused_words++;
    }
    
    return task->stack_size - (unused_words * 4);
}
```

**3. MPU-based stack guard**:
```c
void setup_task_mpu(TCB_t *task) {
    // Guard region below stack
    configure_mpu_region(
        MPU_REGION_STACK_GUARD,
        (uint32_t)task->stack_base - GUARD_SIZE,
        GUARD_SIZE,
        MPU_AP_NONE,  // No access
        MPU_XN        // Execute never
    );
    
    // Stack region
    configure_mpu_region(
        MPU_REGION_STACK,
        (uint32_t)task->stack_base,
        task->stack_size,
        MPU_AP_RW_RW,  // Read-write
        MPU_XN         // Execute never (stack shouldn't execute)
    );
    
    // Any access to guard region → MemManage fault
}
```

### Debug and Trace

**Q: How do you implement debug tracing in an RTOS?**

A: Multiple levels:

**1. Simple event logging**:
```c
#define MAX_TRACE_EVENTS 1000

typedef struct {
    uint32_t timestamp;
    uint8_t event_type;
    void *task;
    uint32_t param;
} trace_event_t;

trace_event_t trace_buffer[MAX_TRACE_EVENTS];
volatile uint32_t trace_index = 0;

#define EVENT_TASK_SWITCH    1
#define EVENT_TASK_CREATE    2
#define EVENT_TASK_DELETE    3
#define EVENT_SEMAPHORE_WAIT 4
#define EVENT_SEMAPHORE_POST 5

void trace_event(uint8_t type, void *task, uint32_t param) {
    if (trace_index < MAX_TRACE_EVENTS) {
        trace_buffer[trace_index].timestamp = get_tick_count();
        trace_buffer[trace_index].event_type = type;
        trace_buffer[trace_index].task = task;
        trace_buffer[trace_index].param = param;
        trace_index++;
    }
}

// Use in context switch
void context_switch(TCB_t *from, TCB_t *to) {
    trace_event(EVENT_TASK_SWITCH, to, from->task_id);
    // ... perform switch ...
}

// Dump trace
void dump_trace(void) {
    for (uint32_t i = 0; i < trace_index; i++) {
        printf("[%d] Event %d, Task %p, Param %d\n",
               trace_buffer[i].timestamp,
               trace_buffer[i].event_type,
               trace_buffer[i].task,
               trace_buffer[i].param);
    }
}
```

**2. Circular buffer (continuous tracing)**:
```c
typedef struct {
    trace_event_t *buffer;
    uint32_t size;
    uint32_t write_index;
    uint32_t read_index;
    bool wrapped;
} circular_trace_t;

circular_trace_t system_trace;

void trace_init(uint32_t size) {
    system_trace.buffer = malloc(sizeof(trace_event_t) * size);
    system_trace.size = size;
    system_trace.write_index = 0;
    system_trace.read_index = 0;
    system_trace.wrapped = false;
}

void trace_add(uint8_t type, void *task, uint32_t param) {
    trace_event_t *event = &system_trace.buffer[system_trace.write_index];
    
    event->timestamp = get_tick_count();
    event->event_type = type;
    event->task = task;
    event->param = param;
    
    system_trace.write_index++;
    if (system_trace.write_index >= system_trace.size) {
        system_trace.write_index = 0;
        system_trace.wrapped = true;
    }
}

// Retrieve events (newest to oldest)
bool trace_get_next(trace_event_t *event) {
    if (!system_trace.wrapped && system_trace.write_index == 0) {
        return false;  // Empty
    }
    
    uint32_t idx = system_trace.write_index;
    if (idx == 0) {
        idx = system_trace.size - 1;
    } else {
        idx--;
    }
    
    *event = system_trace.buffer[idx];
    system_trace.write_index = idx;
    
    return true;
}
```

**3. ITM (Instrumentation Trace Macrocell) - ARM Cortex-M**:
```c
// Hardware trace - very fast, minimal overhead
#define ITM_Port32(n) (*((volatile uint32_t *)(0xE0000000 + 4*n)))

void ITM_SendChar(char ch) {
    while (ITM_Port32(0) == 0);  // Wait if busy
    ITM_Port32(0) = ch;           // Send character
}

void ITM_SendString(const char *str) {
    while (*str) {
        ITM_SendChar(*str++);
    }
}

// Use for printf redirection
int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        ITM_SendChar(ptr[i]);
    }
    return len;
}

// Trace events with minimal overhead
void trace_task_switch(TCB_t *from, TCB_t *to) {
    ITM_Port32(1) = (uint32_t)from;
    ITM_Port32(2) = (uint32_t)to;
    ITM_Port32(3) = get_tick_count();
}
```

### Assert and Error Handling

**Q: How should an RTOS handle errors and assertions?**

A: Structured error handling:

```c
// Error codes
typedef enum {
    ERR_NONE = 0,
    ERR_NULL_POINTER,
    ERR_INVALID_PARAM,
    ERR_OUT_OF_MEMORY,
    ERR_TIMEOUT,
    ERR_QUEUE_FULL,
    ERR_QUEUE_EMPTY,
    ERR_DEADLOCK_DETECTED,
    ERR_STACK_OVERFLOW,
    ERR_PRIORITY_INVALID
} error_code_t;

// Error handler
void error_handler(error_code_t error, 
                  const char *file, 
                  int line,
                  const char *func) {
    // Log error
    printf("ERROR %d in %s:%d (%s)\n", error, file, line, func);
    
    // Trace current state
    dump_system_state();
    
    // Attempt recovery or halt
    switch (error) {
        case ERR_STACK_OVERFLOW:
            // Terminate offending task
            terminate_current_task();
            break;
            
        case ERR_DEADLOCK_DETECTED:
            // Break deadlock (implementation-specific)
            break_deadlock();
            break;
            
        case ERR_OUT_OF_MEMORY:
            // Try to free some memory
            emergency_cleanup();
            break;
            
        default:
            // Unrecoverable - halt system
            disable_interrupts();
            while(1) {
                // Blink error LED or wait for watchdog reset
            }
    }
}

// Assert macro
#define ASSERT(condition) \
    do { \
        if (!(condition)) { \
            error_handler(ERR_INVALID_PARAM, __FILE__, __LINE__, __func__); \
        } \
    } while(0)

// Parameter validation
void* mq_send(message_queue_t *mq, void *msg) {
    ASSERT(mq != NULL);
    ASSERT(msg != NULL);
    ASSERT(mq->capacity > 0);
    
    // ... implementation ...
}
```

### System State Dump

**Q: What information should a system dump include?**

A: Comprehensive state snapshot:

```c
void dump_system_state(void) {
    printf("\n=== SYSTEM STATE DUMP ===\n");
    
    // Current time
    printf("Tick Count: %lu\n", get_tick_count());
    
    // CPU usage
    printf("CPU Usage: %d%%\n", calculate_cpu_usage());
    
    // Memory status
    printf("\n--- Memory ---\n");
    printf("Heap Used: %d / %d bytes\n", heap_used(), heap_total());
    printf("Heap Free: %d bytes\n", heap_free());
    
    // Task information
    printf("\n--- Tasks ---\n");
    for (int i = 0; i < num_tasks; i++) {
        TCB_t *task = &task_list[i];
        
        printf("Task %d: %s\n", task->task_id, task->name);
        printf("  State: %s\n", state_to_string(task->state));
        printf("  Priority: %d\n", task->priority);
        printf("  Stack: %d / %d bytes (%d%%)\n",
               task->max_stack_usage,
               task->stack_size,
               (task->max_stack_usage * 100) / task->stack_size);
        printf("  CPU Time: %lu ms\n", ticks_to_ms(task->total_runtime));
        
        if (task->state == BLOCKED) {
            printf("  Waiting on: %p\n", task->waiting_on);
        }
        
        // Stack backtrace if possible
        if (task != current_task) {
            printf("  Stack trace:\n");
            print_stack_trace(task);
        }
    }
    
    // Queue status
    printf("\n--- Queues ---\n");
    dump_all_queues();
    
    // Semaphore status
    printf("\n--- Semaphores ---\n");
    dump_all_semaphores();
    
    // Recent trace events
    printf("\n--- Recent Events ---\n");
    dump_recent_trace(20);  // Last 20 events
    
    printf("\n=== END DUMP ===\n\n");
}

// Stack backtrace
void print_stack_trace(TCB_t *task) {
    uint32_t *sp = (uint32_t*)task->stack_pointer;
    uint32_t *stack_top = (uint32_t*)((char*)task->stack_base + task->stack_size);
    
    // Look for return addresses (aligned, in code region)
    for (int i = 0; i < 10 && sp < stack_top; sp++) {
        uint32_t addr = *sp;
        if (is_code_address(addr)) {
            printf("    0x%08X", addr);
            
            // Try to find function name (if symbol table available)
            const char *func = address_to_function(addr);
            if (func) {
                printf(" (%s)", func);
            }
            printf("\n");
        }
    }
}
```

---

## Part 16: Practical Patterns and Best Practices

### Producer-Consumer Pattern (Practical)

**Q: What's a practical producer-consumer implementation?**

A: Real-world example with error handling:

```c
#define BUFFER_SIZE 10

typedef struct {
    int data[BUFFER_SIZE];
    int head, tail, count;
    mutex_t mutex;
    semaphore_t empty;  // Count of empty slots
    semaphore_t full;   // Count of full slots
    bool shutdown;      // For graceful shutdown
} prod_cons_queue_t;

prod_cons_queue_t pq;

void pq_init(void) {
    pq.head = 0;
    pq.tail = 0;
    pq.count = 0;
    pq.shutdown = false;
    
    mutex_init(&pq.mutex);
    sem_init(&pq.empty, BUFFER_SIZE);
    sem_init(&pq.full, 0);
}

// Producer
bool pq_produce(int value, uint32_t timeout_ms) {
    // Wait for empty slot
    if (!sem_wait_timeout(&pq.empty, timeout_ms)) {
        return false;  // Timeout
    }
    
    mutex_lock(&pq.mutex);
    
    // Check for shutdown
    if (pq.shutdown) {
        mutex_unlock(&pq.mutex);
        sem_signal(&pq.empty);  // Return the slot
        return false;
    }
    
    // Add to buffer
    pq.data[pq.tail] = value;
    pq.tail = (pq.tail + 1) % BUFFER_SIZE;
    pq.count++;
    
    mutex_unlock(&pq.mutex);
    
    // Signal consumer
    sem_signal(&pq.full);
    
    return true;
}

// Consumer
bool pq_consume(int *value, uint32_t timeout_ms) {
    // Wait for full slot
    if (!sem_wait_timeout(&pq.full, timeout_ms)) {
        return false;  // Timeout
    }
    
    mutex_lock(&pq.mutex);
    
    // Check for shutdown
    if (pq.shutdown && pq.count == 0) {
        mutex_unlock(&pq.mutex);
        return false;
    }
    
    // Remove from buffer
    *value = pq.data[pq.head];
    pq.head = (pq.head + 1) % BUFFER_SIZE;
    pq.count--;
    
    mutex_unlock(&pq.mutex);
    
    // Signal producer
    sem_signal(&pq.empty);
    
    return true;
}

// Graceful shutdown
void pq_shutdown(void) {
    mutex_lock(&pq.mutex);
    pq.shutdown = true;
    mutex_unlock(&pq.mutex);
    
    // Wake all waiting tasks
    for (int i = 0; i < BUFFER_SIZE; i++) {
        sem_signal(&pq.full);
        sem_signal(&pq.empty);
    }
}

// Producer task
void producer_task(void *param) {
    int value = 0;
    
    while (1) {
        value++;
        
        if (!pq_produce(value, 1000)) {
            // Timeout or shutdown
            if (pq.shutdown) {
                printf("Producer exiting\n");
                break;
            }
        } else {
            printf("Produced: %d\n", value);
        }
        
        delay(100);  // Simulate work
    }
}

// Consumer task
void consumer_task(void *param) {
    int value;
    
    while (1) {
        if (!pq_consume(&value, 1000)) {
            // Timeout or shutdown
            if (pq.shutdown) {
                printf("Consumer exiting\n");
                break;
            }
        } else {
            printf("Consumed: %d\n", value);
            // Process value
        }
    }
}
```

### Worker Pool Pattern

**Q: How do you implement a worker pool for task distribution?**

A:

```c
typedef struct {
    void (*func)(void*);
    void *param;
} work_item_t;

#define WORK_QUEUE_SIZE 50
#define NUM_WORKERS 4

message_queue_t *work_queue;
TCB_t *worker_tasks[NUM_WORKERS];

void worker_pool_init(void) {
    // Create work queue
    work_queue = mq_create(sizeof(work_item_t), WORK_QUEUE_SIZE);
    
    // Create worker tasks
    for (int i = 0; i < NUM_WORKERS; i++) {
        char name[16];
        sprintf(name, "Worker%d", i);
        worker_tasks[i] = create_task(worker_function, 
                                      (void*)(intptr_t)i,
                                      2048, 
                                      PRIORITY_NORMAL,
                                      name);
    }
}

void worker_function(void *param) {
    int worker_id = (int)(intptr_t)param;
    work_item_t work;
    
    printf("Worker %d started\n", worker_id);
    
    while (1) {
        // Wait for work
        mq_receive(work_queue, &work);
        
        printf("Worker %d processing task\n", worker_id);
        
        // Execute work
        if (work.func) {
            work.func(work.param);
        }
        
        printf("Worker %d completed task\n", worker_id);
    }
}

// Submit work to pool
bool submit_work(void (*func)(void*), void *param) {
    work_item_t work = {
        .func = func,
        .param = param
    };
    
    return mq_send_nonblock(work_queue, &work);
}

// Example usage
void long_computation(void *param) {
    int *data = (int*)param;
    // Do expensive work
    *data *= 2;
    printf("Computation result: %d\n", *data);
}

void app_main(void) {
    worker_pool_init();
    
    // Submit multiple tasks
    int data1 = 10, data2 = 20, data3 = 30;
    submit_work(long_computation, &data1);
    submit_work(long_computation, &data2);
    submit_work(long_computation, &data3);
    
    // Workers process in parallel
}
```

### State Machine Pattern

**Q: How do you implement a state machine in an RTOS task?**

A:

```c
typedef enum {
    STATE_IDLE,
    STATE_CONNECTING,
    STATE_CONNECTED,
    STATE_TRANSMITTING,
    STATE_ERROR
} connection_state_t;

typedef struct {
    connection_state_t state;
    event_group_t *events;
    int retry_count;
    uint32_t last_activity;
} state_machine_t;

#define EVENT_CONNECT_REQ   (1 << 0)
#define EVENT_CONNECTED     (1 << 1)
#define EVENT_DATA_READY    (1 << 2)
#define EVENT_DISCONNECT    (1 << 3)
#define EVENT_ERROR         (1 << 4)
#define EVENT_TIMEOUT       (1 << 5)

void state_machine_task(void *param) {
    state_machine_t sm;
    sm.state = STATE_IDLE;
    sm.events = event_group_create();
    sm.retry_count = 0;
    
    while (1) {
        uint32_t events;
        
        switch (sm.state) {
            case STATE_IDLE:
                // Wait for connect request
                events = event_wait_any(sm.events, 
                                       EVENT_CONNECT_REQ,
                                       INFINITE_TIMEOUT);
                
                if (events & EVENT_CONNECT_REQ) {
                    printf("Starting connection...\n");
                    initiate_connection();
                    sm.state = STATE_CONNECTING;
                    sm.retry_count = 0;
                }
                break;
                
            case STATE_CONNECTING:
                // Wait for connected or timeout
                events = event_wait_any(sm.events,
                                       EVENT_CONNECTED | EVENT_TIMEOUT | EVENT_ERROR,
                                       5000);  // 5 second timeout
                
                if (events & EVENT_CONNECTED) {
                    printf("Connected!\n");
                    sm.state = STATE_CONNECTED;
                    sm.last_activity = get_tick_count();
                    
                } else if (events & (EVENT_TIMEOUT | EVENT_ERROR)) {
                    sm.retry_count++;
                    
                    if (sm.retry_count < MAX_RETRIES) {
                        printf("Retry %d...\n", sm.retry_count);
                        initiate_connection();
                        // Stay in CONNECTING state
                    } else {
                        printf("Connection failed\n");
                        sm.state = STATE_ERROR;
                    }
                }
                break;
                
            case STATE_CONNECTED:
                // Wait for data or disconnect
                events = event_wait_any(sm.events,
                                       EVENT_DATA_READY | EVENT_DISCONNECT | EVENT_ERROR,
                                       10000);  // 10 second timeout
                
                if (events & EVENT_DATA_READY) {
                    printf("Transmitting data...\n");
                    sm.state = STATE_TRANSMITTING;
                    sm.last_activity = get_tick_count();
                    
                } else if (events & EVENT_DISCONNECT) {
                    printf("Disconnecting...\n");
                    close_connection();
                    sm.state = STATE_IDLE;
                    
                } else if (events & EVENT_ERROR) {
                    printf("Connection error\n");
                    sm.state = STATE_ERROR;
                    
                } else {
                    // Timeout - check keepalive
                    uint32_t elapsed = get_tick_count() - sm.last_activity;
                    if (elapsed > KEEPALIVE_TIMEOUT) {
                        printf("Keepalive timeout\n");
                        sm.state = STATE_ERROR;
                    }
                }
                break;
                
            case STATE_TRANSMITTING:
                // Transmit data
                if (transmit_data()) {
                    printf("Transmission complete\n");
                    sm.state = STATE_CONNECTED;
                    sm.last_activity = get_tick_count();
                } else {
                    printf("Transmission failed\n");
                    sm.state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                // Cleanup and retry or give up
                printf("In error state, cleaning up...\n");
                cleanup_connection();
                
                // Wait a bit before retry
                delay(1000);
                
                // Return to idle
                sm.state = STATE_IDLE;
                sm.retry_count = 0;
                break;
        }
    }
}

// External triggers
void request_connect(state_machine_t *sm) {
    event_set(sm->events, EVENT_CONNECT_REQ);
}

void notify_connected(state_machine_t *sm) {
    event_set(sm->events, EVENT_CONNECTED);
}

void notify_data_ready(state_machine_t *sm) {
    event_set(sm->events, EVENT_DATA_READY);
}
```

### Deferred Interrupt Processing

**Q: What's the proper way to defer interrupt processing?**

A: Use ISR + Task pattern:

```c
// Fast ISR - minimal work
volatile bool data_ready = false;
event_group_t *uart_events;

void UART_IRQHandler(void) {
    if (UART->SR & UART_SR_RXNE) {
        // Read data quickly
        char data = UART->DR;
        
        // Store in buffer
        if (uart_rx_buffer_space_available()) {
            uart_rx_buffer_push(data);
            
            // Signal processing task
            event_set_from_isr(uart_events, EVENT_DATA_RECEIVED);
        } else {
            // Buffer overflow - count error
            uart_rx_overflow_count++;
        }
        
        // Clear interrupt flag (if needed)
        UART->SR &= ~UART_SR_RXNE;
    }
}

// Processing task - does heavy work
void uart_processing_task(void *param) {
    uint8_t buffer[256];
    
    while (1) {
        // Wait for data
        event_wait_any(uart_events, EVENT_DATA_RECEIVED, INFINITE_TIMEOUT);
        event_clear(uart_events, EVENT_DATA_RECEIVED);
        
        // Read all available data
        int len = uart_rx_buffer_read(buffer, sizeof(buffer));
        
        if (len > 0) {
            // Process data (can take time)
            process_uart_data(buffer, len);
            
            // Update statistics
            total_bytes_received += len;
            
            // Maybe forward to other tasks
            if (is_command(buffer, len)) {
                handle_command(buffer, len);
            }
        }
    }
}

// Safe ISR-to-task communication
void event_set_from_isr(event_group_t *eg, uint32_t flags) {
    // No mutex needed - atomic operation
    eg->flags |= flags;
    
    // If higher priority task waiting, trigger context switch
    if (should_yield_from_isr(eg)) {
        trigger_context_switch_from_isr();
    }
}
```

### Interrupt-Safe Data Structures

**Q: How do you make data structures interrupt-safe?**

A: Multiple techniques:

**1. Lock-free ring buffer**:
```c
typedef struct {
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile uint32_t head;  // Written by producer (ISR)
    volatile uint32_t tail;  // Written by consumer (task)
    uint32_t size;
} lockfree_ringbuf_t;

// Called from ISR - no locks needed
bool ringbuf_push_isr(lockfree_ringbuf_t *rb, uint8_t data) {
    uint32_t next_head = (rb->head + 1) % rb->size;
    
    if (next_head == rb->tail) {
        return false;  // Full
    }
    
    rb->buffer[rb->head] = data;
    
    // Memory barrier ensures write completes before updating head
    __DMB();
    
    rb->head = next_head;
    return true;
}

// Called from task
bool ringbuf_pop(lockfree_ringbuf_t *rb, uint8_t *data) {
    if (rb->head == rb->tail) {
        return false;  // Empty
    }
    
    *data = rb->buffer[rb->tail];
    
    __DMB();
    
    rb->tail = (rb->tail + 1) % rb->size;
    return true;
}

// Multi-byte read (batch operation)
int ringbuf_read(lockfree_ringbuf_t *rb, uint8_t *buffer, int max_len) {
    int count = 0;
    
    while (count < max_len && rb->head != rb->tail) {
        buffer[count++] = rb->buffer[rb->tail];
        __DMB();
        rb->tail = (rb->tail + 1) % rb->size;
    }
    
    return count;
}
```

**2. Double buffering**:
```c
typedef struct {
    uint8_t buffer_a[BUFFER_SIZE];
    uint8_t buffer_b[BUFFER_SIZE];
    uint8_t *write_buffer;   // ISR writes here
    uint8_t *read_buffer;    // Task reads here
    volatile int write_count;
    volatile bool swap_pending;
} double_buffer_t;

double_buffer_t adc_buffer;

void adc_buffer_init(void) {
    adc_buffer.write_buffer = adc_buffer.buffer_a;
    adc_buffer.read_buffer = adc_buffer.buffer_b;
    adc_buffer.write_count = 0;
    adc_buffer.swap_pending = false;
}

// ISR fills write buffer
void ADC_IRQHandler(void) {
    uint16_t value = ADC->DR;
    
    if (adc_buffer.write_count < BUFFER_SIZE) {
        adc_buffer.write_buffer[adc_buffer.write_count++] = value;
        
        if (adc_buffer.write_count >= BUFFER_SIZE) {
            // Buffer full - signal swap
            adc_buffer.swap_pending = true;
            event_set_from_isr(adc_events, EVENT_BUFFER_READY);
        }
    }
}

// Task processes read buffer
void adc_processing_task(void *param) {
    while (1) {
        event_wait_any(adc_events, EVENT_BUFFER_READY, INFINITE_TIMEOUT);
        
        if (adc_buffer.swap_pending) {
            // Swap buffers (quick operation)
            disable_interrupts();
            
            uint8_t *temp = adc_buffer.write_buffer;
            adc_buffer.write_buffer = adc_buffer.read_buffer;
            adc_buffer.read_buffer = temp;
            
            int count = adc_buffer.write_count;
            adc_buffer.write_count = 0;
            adc_buffer.swap_pending = false;
            
            enable_interrupts();
            
            // Process data (can take time, ISR writing to other buffer)
            process_adc_samples(adc_buffer.read_buffer, count);
        }
    }
}
```

### Critical Section Best Practices

**Q: What are best practices for critical sections?**

A:

```c
// ❌ BAD: Long critical section
void bad_example(void) {
    disable_interrupts();
    
    // Long operations
    process_data();           // Takes time
    update_database();        // Takes time
    send_to_network();        // Takes time
    
    enable_interrupts();
}

// ✅ GOOD: Minimal critical section
void good_example(void) {
    // Prepare outside critical section
    data_t processed = process_data();
    
    // Only protect shared state
    disable_interrupts();
    update_shared_state(processed);
    enable_interrupts();
    
    // Continue without locks
    update_database(processed);
    send_to_network(processed);
}

// ✅ GOOD: Use appropriate granularity
typedef struct {
    int count;
    mutex_t mutex;
} counter_t;

void increment_counter(counter_t *c) {
    mutex_lock(&c->mutex);
    c->count++;
    mutex_unlock(&c->mutex);
    // Fast, appropriate for non-ISR code
}

// ✅ BETTER: For simple operations, use atomic instructions
void atomic_increment(volatile int *counter) {
    // ARM exclusive access
    int value, status;
    do {
        value = __LDREXW(counter);
        status = __STREXW(value + 1, counter);
    } while (status != 0);
    
    __DMB();
}

// ✅ BEST: Use lock-free algorithms when possible
```

### Resource Management Pattern

**Q: How do you properly manage limited resources?**

A: Resource pool with tracking:

```c
typedef struct {
    void *resource;
    bool in_use;
    TCB_t *owner;
    uint32_t acquire_time;
} resource_entry_t;

typedef struct {
    resource_entry_t *entries;
    int count;
    semaphore_t available;
    mutex_t mutex;
} resource_pool_t;

resource_pool_t* resource_pool_create(int count, 
                                      void* (*create_func)(void)) {
    resource_pool_t *pool = malloc(sizeof(resource_pool_t));
    
    pool->entries = malloc(sizeof(resource_entry_t) * count);
    pool->count = count;
    
    // Create resources
    for (int i = 0; i < count; i++) {
        pool->entries[i].resource = create_func();
        pool->entries[i].in_use = false;
        pool->entries[i].owner = NULL;
    }
    
    sem_init(&pool->available, count);
    mutex_init(&pool->mutex);
    
    return pool;
}

void* resource_acquire(resource_pool_t *pool, uint32_t timeout_ms) {
    // Wait for availability
    if (!sem_wait_timeout(&pool->available, timeout_ms)) {
        return NULL;  // Timeout
    }
    
    mutex_lock(&pool->mutex);
    
    // Find free resource
    for (int i = 0; i < pool->count; i++) {
        if (!pool->entries[i].in_use) {
            pool->entries[i].in_use = true;
            pool->entries[i].owner = current_task;
            pool->entries[i].acquire_time = get_tick_count();
            
            void *resource = pool->entries[i].resource;
            mutex_unlock(&pool->mutex);
            return resource;
        }
    }
    
    // Should never happen (semaphore prevents)
    mutex_unlock(&pool->mutex);
    ASSERT(false);
    return NULL;
}

void resource_release(resource_pool_t *pool, void *resource) {
    mutex_lock(&pool->mutex);
    
    // Find resource
    for (int i = 0; i < pool->count; i++) {
        if (pool->entries[i].resource == resource) {
            ASSERT(pool->entries[i].in_use);
            ASSERT(pool->entries[i].owner == current_task);
            
            pool->entries[i].in_use = false;
            pool->entries[i].owner = NULL;
            
            mutex_unlock(&pool->mutex);
            sem_signal(&pool->available);
            return;
        }
    }
    
    mutex_unlock(&pool->mutex);
    ASSERT(false);  // Resource not in pool!
}

// Automatic release using RAII-style pattern
typedef struct {
    resource_pool_t *pool;
    void *resource;
} scoped_resource_t;

scoped_resource_t resource_acquire_scoped(resource_pool_t *pool, 
                                         uint32_t timeout_ms) {
    scoped_resource_t sr;
    sr.pool = pool;
    sr.resource = resource_acquire(pool, timeout_ms);
    return sr;
}

void resource_release_scoped(scoped_resource_t *sr) {
    if (sr->resource) {
        resource_release(sr->pool, sr->resource);
        sr->resource = NULL;
    }
}

// Usage
void task_function(void) {
    scoped_resource_t sr = resource_acquire_scoped(my_pool, 1000);
    
    if (sr.resource) {
        // Use resource
        use_resource(sr.resource);
        
        // Automatic cleanup even if early return
        if (error_occurred()) {
            resource_release_scoped(&sr);
            return;
        }
        
        // More work...
        
        resource_release_scoped(&sr);
    }
}
```

### Timeout Handling Pattern

**Q: How do you implement robust timeout handling?**

A:

```c
typedef struct {
    uint32_t start_time;
    uint32_t timeout_ms;
    bool expired;
} timeout_t;

void timeout_start(timeout_t *t, uint32_t timeout_ms) {
    t->start_time = get_tick_count();
    t->timeout_ms = timeout_ms;
    t->expired = false;
}

bool timeout_check(timeout_t *t) {
    if (t->expired) {
        return true;
    }
    
    uint32_t elapsed = get_tick_count() - t->start_time;
    if (elapsed >= t->timeout_ms) {
        t->expired = true;
        return true;
    }
    
    return false;
}

uint32_t timeout_remaining(timeout_t *t) {
    if (t->expired) {
        return 0;
    }
    
    uint32_t elapsed = get_tick_count() - t->start_time;
    if (elapsed >= t->timeout_ms) {
        t->expired = true;
        return 0;
    }
    
    return t->timeout_ms - elapsed;
}

// Usage in complex operations
bool complex_operation_with_timeout(uint32_t total_timeout_ms) {
    timeout_t timeout;
    timeout_start(&timeout, total_timeout_ms);
    
    // Step 1 - with remaining timeout
    if (!step1_with_timeout(timeout_remaining(&timeout))) {
        return false;
    }
    
    if (timeout_check(&timeout)) {
        return false;  // Timeout
    }
    
    // Step 2 - with remaining timeout
    if (!step2_with_timeout(timeout_remaining(&timeout))) {
        return false;
    }
    
    if (timeout_check(&timeout)) {
        return false;  // Timeout
    }
    
    // Step 3 - final step
    return step3_with_timeout(timeout_remaining(&timeout));
}

// Retry with exponential backoff
bool retry_with_backoff(bool (*operation)(void), 
                       int max_retries,
                       uint32_t initial_delay_ms) {
    uint32_t delay = initial_delay_ms;
    
    for (int retry = 0; retry < max_retries; retry++) {
        if (operation()) {
            return true;  // Success
        }
        
        // Wait before retry
        delay_ms(delay);
        
        // Exponential backoff (cap at reasonable maximum)
        delay *= 2;
        if (delay > 5000) {
            delay = 5000;
        }
        
        printf("Retry %d/%d after %dms\n", retry + 1, max_retries, delay);
    }
    
    return false;  // All retries failed
}
```

---

## Part 17: Performance Optimization

### Context Switch Optimization Deep Dive

**Q: How do you minimize context switch overhead?**

A: Multiple optimizations:

**1. Reduce register save/restore**:
```c
// Only save registers that aren't automatically saved
void __attribute__((naked)) optimized_context_switch(void) {
    // Hardware auto-saves: R0-R3, R12, LR, PC, xPSR
    // We only need to save: R4-R11 (and FPU if used)
    
    asm volatile(
        "MRS    R0, PSP                 \n"  // Get stack pointer
        
        // Check if FPU used (saves time if not)
        "TST    LR, #0x10               \n"  // Test FPU bit in LR
        "IT     EQ                      \n"
        "VSTMDBEQ R0!, {S16-S31}        \n"  // Save FPU only if used
        
        "STMDB  R0!, {R4-R11}           \n"  // Save remaining regs
        
        // Save SP for current task
        "LDR    R1, =current_task       \n"
        "LDR    R1, [R1]                \n"
        "STR    R0, [R1]                \n"
        
        // Load SP for next task
        "LDR    R0, =next_task          \n"
        "LDR    R0, [R0]                \n"
        "LDR    R0, [R0]                \n"
        
        // Restore registers
        "LDMIA  R0!, {R4-R11}           \n"
        
        "TST    LR, #0x10               \n"
        "IT     EQ                      \n"
        "VLDMIAEQ R0!, {S16-S31}        \n"
        
        "MSR    PSP, R0                 \n"
        "BX     LR                      \n"  // Return
    );
}
```

**2. Fast scheduling with O(1) lookup**:
```c
// Priority-based with bitmap for O(1) selection
#define NUM_PRIORITIES 32

typedef struct {
    uint32_t ready_bitmap;                    // One bit per priority
    TCB_t *ready_queues[NUM_PRIORITIES];      // Queue per priority
} fast_scheduler_t;

fast_scheduler_t scheduler;

void scheduler_add_task(TCB_t *task) {
    // Set bit for this priority
    scheduler.ready_bitmap |= (1 << task->priority);
    
    // Add to queue (circular linked list)
    if (scheduler.ready_queues[task->priority] == NULL) {
        // First task at this priority
        scheduler.ready_queues[task->priority] = task;
        task->next = task;
        task->prev = task;
    } else {
        // Add to end of queue
        TCB_t *head = scheduler.ready_queues[task->priority];
        task->next = head;
        task->prev = head->prev;
        head->prev->next = task;
        head->prev = task;
    }
}

TCB_t* scheduler_select_next(void) {
    if (scheduler.ready_bitmap == 0) {
        return idle_task;  // Nothing ready
    }
    
    // Find highest priority (O(1) using CLZ instruction)
    int highest_priority = 31 - __CLZ(scheduler.ready_bitmap);
    
    // Get task from that queue
    TCB_t *task = scheduler.ready_queues[highest_priority];
    
    // Round-robin within priority: move to next task
    scheduler.ready_queues[highest_priority] = task->next;
    
    return task;
}

void scheduler_remove_task(TCB_t *task) {
    if (task->next == task) {
        // Last task at this priority
        scheduler.ready_queues[task->priority] = NULL;
        scheduler.ready_bitmap &= ~(1 << task->priority);
    } else {
        // Remove from circular list
        task->prev->next = task->next;
        task->next->prev = task->prev;
        
        // Update head if needed
        if (scheduler.ready_queues[task->priority] == task) {
            scheduler.ready_queues[task->priority] = task->next;
        }
    }
}
```

**3. Lazy FPU context save**:
```c
// Don't save FPU until another task uses it
volatile TCB_t *fpu_owner = NULL;

void fpu_lazy_init(void) {
    // Enable lazy stacking
    FPU->FPCCR |= FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk;
}

void on_fpu_exception(void) {
    // FPU exception triggered on FPU use
    
    if (fpu_owner != current_task) {
        if (fpu_owner != NULL) {
            // Save previous owner's FPU context
            save_fpu_context(fpu_owner);
        }
        
        if (current_task->fpu_context_valid) {
            // Restore current task's FPU context
            restore_fpu_context(current_task);
        } else {
            // First use - initialize
            initialize_fpu_context(current_task);
            current_task->fpu_context_valid = true;
        }
        
        fpu_owner = current_task;
    }
}
```

### Memory Allocation Optimization

**Q: How do you optimize dynamic memory allocation for RTOS?**

A: Custom allocators:

**1. Fixed-size block allocator** (fastest):
```c
// O(1) allocation and deallocation
typedef struct block {
    struct block *next;
} block_t;

typedef struct {
    void *memory;
    block_t *free_list;
    size_t block_size;
    size_t num_blocks;
} fixed_allocator_t;

void fixed_alloc_init(fixed_allocator_t *alloc, 
                     size_t block_size,
                     size_t num_blocks) {
    alloc->block_size = block_size;
    alloc->num_blocks = num_blocks;
    alloc->memory = malloc(block_size * num_blocks);
    
    // Build free list
    alloc->free_list = (block_t*)alloc->memory;
    block_t *block = alloc->free_list;
    
    for (size_t i = 0; i < num_blocks - 1; i++) {
        block->next = (block_t*)((char*)block + block_size);
        block = block->next;
    }
    block->next = NULL;
}

void* fixed_alloc(fixed_allocator_t *alloc) {
    if (alloc->free_list == NULL) {
        return NULL;  // Out of blocks
    }
    
    block_t *block = alloc->free_list;
    alloc->free_list = block->next;
    
    return block;
}

void fixed_free(fixed_allocator_t *alloc, void *ptr) {
    block_t *block = (block_t*)ptr;
    block->next = alloc->free_list;
    alloc->free_list = block;
}
```

**2. Slab allocator** (for common sizes):
```c
typedef struct {
    fixed_allocator_t allocators[NUM_SLABS];
    size_t sizes[NUM_SLABS];
} slab_allocator_t;

void slab_init(slab_allocator_t *slab) {
    // Common sizes: 16, 32, 64, 128, 256, 512, 1024 bytes
    size_t sizes[] = {16, 32, 64, 128, 256, 512, 1024};
    size_t counts[] = {100, 80, 60, 40, 20, 10, 5};
    
    for (int i = 0; i < NUM_SLABS; i++) {
        slab->sizes[i] = sizes[i];
        fixed_alloc_init(&slab->allocators[i], sizes[i], counts[i]);
    }
}

void* slab_alloc(slab_allocator_t *slab, size_t size) {
    // Find appropriate slab
    for (int i = 0; i < NUM_SLABS; i++) {
        if (size <= slab->sizes[i]) {
            return fixed_alloc(&slab->allocators[i]);
        }
    }
    
    // Too big for slabs - use general allocator
    return malloc(size);
}

void slab_free(slab_allocator_t *slab, void *ptr, size_t size) {
    // Find slab
    for (int i = 0; i < NUM_SLABS; i++) {
        if (size <= slab->sizes[i]) {
            fixed_free(&slab->allocators[i], ptr);
            return;
        }
    }
    
    // Was from general allocator
    free(ptr);
}
```

### Cache Optimization

**Q: How do you optimize for cache performance?**

A:

**1. Data structure layout**:
```c
// ❌ BAD: Poor cache locality
typedef struct {
    int id;
    char name[256];        // Large, rarely used
    int priority;          // Frequently accessed
    uint32_t statistics[100];  // Large
    int state;             // Frequently accessed
} bad_tcb_t;

// ✅ GOOD: Hot data together
typedef struct {
    // Frequently accessed (hot) data - fits in one cache line
    int id;
    int priority;
    int state;
    void *stack_pointer;
    struct tcb *next;
    // ... other hot fields ...
    
    // Cold data
    char *name;            // Pointer to separate storage
    statistics_t *stats;   // Pointer to separate storage
} good_tcb_t;
```

**2. Align critical structures**:
```c
// Align to cache line (typically 32 or 64 bytes)
typedef struct __attribute__((aligned(32))) {
    volatile uint32_t head;
    uint8_t pad1[28];      // Padding to separate cache lines
    
    volatile uint32_t tail;
    uint8_t pad2[28];
    
    uint8_t buffer[BUFFER_SIZE];
} cache_aligned_ringbuf_t;

// Prevents false sharing between producer and consumer
```

**3. Prefetching**:
```c
void process_task_list(TCB_t *list) {
    TCB_t *current = list;
    TCB_t *next = current->next;
    
    while (current) {
        // Prefetch next task while processing current
        if (next) {
            __builtin_prefetch(next, 0, 3);  // GCC builtin
        }
        
        // Process current task
        process_task(current);
        
        current = next;
        if (current) {
            next = current->next;
        }
    }
}
```

---

## Key Takeaways and Summary

### Essential Concepts

**1. Interrupts are fundamental**:
- Hardware mechanism for asynchronous events
- Must be fast (deferred processing)
- Priority-based handling
- Context automatically saved/restored

**2. Synchronization is critical**:
- Mutex: Mutual exclusion
- Semaphore: Counting/signaling
- Event flags: Multiple condition waiting
- Must prevent deadlock

**3. Real-time means predictable**:
- Deterministic response times
- Priority-based scheduling
- Bounded blocking times
- Worst-case analysis

**4. Context switching is expensive**:
- Minimize frequency
- Reduce saved state
- Use fast scheduling algorithms
- Consider cache effects

**5. Memory management matters**:
- Avoid fragmentation
- Use pools for predictability
- Stack overflow detection
- Careful with dynamic allocation

### Common Pitfalls

**1. Priority inversion** - Use priority inheritance
**2. Deadlock** - Careful lock ordering
**3. Starvation** - Fair scheduling, watchdogs
**4. Stack overflow** - Proper sizing, detection
**5. Race conditions** - Proper synchronization
**6. Long ISRs** - Defer processing
**7. Forgetting memory barriers** - Use DMB/DSB/ISB

### Best Practices Summary

- **ISRs**: Keep short, defer work
- **Critical sections**: Minimize duration
- **Locks**: Consistent ordering, avoid nesting
- **Timeouts**: Always use them
- **Resources**: Pool management, RAII pattern
- **Debugging**: Comprehensive tracing
- **Testing**: Stress testing, worst-case analysis
- **Documentation**: Thread-safety, requirements

This completes the comprehensive operating systems study notes covering hardware fundamentals through advanced RTOS concepts and practical implementation patterns!
