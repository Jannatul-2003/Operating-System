.syntax unified
.thumb

.global SVC_Handler
.type SVC_Handler, %function
SVC_Handler:
    TST LR, #4
    ITE EQ
    MRSEQ R0, MSP
    MRSNE R0, PSP      // R0 = stack pointer (points to stacked R0–R3,R12,LR,PC,xPSR)
    BL  SVC_Handler_C  // call C function with (stack_frame, svc_number)
    BX  LR
