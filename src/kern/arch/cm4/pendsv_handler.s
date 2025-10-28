.syntax unified
.thumb

.global PendSV_Handler
.type PendSV_Handler, %function

.extern current_tcb
.extern next_tcb
.extern schedule_next  // You’ll write this in C if not already present

PendSV_Handler:
    CPSID   I                   // Disable interrupts during context save
    MRS     R0, PSP             // Get process stack pointer (current task)
    CBZ     R0, no_save         // If first time, skip saving

    // Save callee-saved registers R4–R11
    STMDB   R0!, {R4-R11}
    LDR     R1, =current_tcb
    LDR     R1, [R1]
    STR     R0, [R1, #8]        // Store new PSP in TCB (offset 8)

no_save:
    // Choose next task
    BL      schedule_next

    // Restore next task context
    LDR     R1, =current_tcb
    LDR     R1, [R1]
    LDR     R0, [R1, #8]        // Load PSP from TCB
    LDMIA   R0!, {R4-R11}       // Restore registers
    MSR     PSP, R0

    CPSIE   I                   // Re-enable interrupts
    BX      LR
