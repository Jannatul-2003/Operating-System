#include "types.h"

extern TCB_TypeDef *current_tcb;
extern TCB_TypeDef *task_list[];
extern int total_tasks;

void schedule_next(void)
{
    static int index = 0;
    index = (index + 1) % total_tasks;
    current_tcb = task_list[index];
}
