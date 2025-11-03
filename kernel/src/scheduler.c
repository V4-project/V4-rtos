#include "v4/internal/vm_internal.h"

#include <string.h>
#include <v4/errors.h>

void v4_scheduler_init(v4_scheduler_t *sched)
{
  if (!sched)
    return;

  memset(sched, 0, sizeof(v4_scheduler_t));

  /* Mark all tasks as DEAD initially */
  for (int i = 0; i < V4_MAX_TASKS; i++)
  {
    sched->tasks[i].state = V4_TASK_STATE_DEAD;
  }

  sched->current_task = 0;
  sched->task_count = 0;
  sched->tick_count = 0;
  sched->time_slice_ms = 10; /* 10ms default time slice */
  sched->context_switches = 0;
  sched->preemptions = 0;
  sched->critical_nesting = 0;
}

uint8_t v4_task_select_next(v4_rtos_vm_t *rtos_vm)
{
  if (!rtos_vm)
    return 0;

  v4_scheduler_t *sched = &rtos_vm->scheduler;
  v4_u32 current_tick = v4_platform_get_tick_ms();

  uint8_t highest_priority = 0;
  uint8_t selected_task = 0xFF;

  /* Priority-based scheduling with round-robin for equal priorities */
  for (uint8_t i = 0; i < V4_MAX_TASKS; i++)
  {
    v4_task_t *task = &sched->tasks[i];

    /* Skip DEAD tasks */
    if (task->state == V4_TASK_STATE_DEAD)
      continue;

    /* Wake up sleeping tasks if deadline passed */
    if (task->state == V4_TASK_STATE_BLOCKED)
    {
      if (current_tick >= task->sleep_until_tick)
      {
        task->state = V4_TASK_STATE_READY;
      }
      else
      {
        continue; /* Still sleeping */
      }
    }

    /* Select highest priority READY task */
    if (task->state == V4_TASK_STATE_READY || task->state == V4_TASK_STATE_RUNNING)
    {
      if (task->priority > highest_priority)
      {
        highest_priority = task->priority;
        selected_task = i;
      }
      else if (task->priority == highest_priority && selected_task != 0xFF)
      {
        /* Round-robin: prefer tasks after current task */
        if (i > sched->current_task && selected_task <= sched->current_task)
        {
          selected_task = i;
        }
      }
    }
  }

  /* If no task found, keep current task */
  if (selected_task == 0xFF)
  {
    selected_task = sched->current_task;
  }

  return selected_task;
}

static void v4_task_save_context(v4_rtos_vm_t *rtos_vm, v4_task_t *task)
{
  if (!rtos_vm || !task)
    return;

  /* Save stack depths */
  task->ds_depth = (uint8_t)vm_ds_depth_public(rtos_vm->vm);
  task->rs_depth = (uint8_t)vm_rs_depth_public(rtos_vm->vm);

  /* Copy stack contents to task's independent stacks */
  if (task->ds_base && task->ds_depth > 0)
  {
    vm_ds_copy_to_array(rtos_vm->vm, task->ds_base, task->ds_depth);
  }
  if (task->rs_base && task->rs_depth > 0)
  {
    vm_rs_copy_to_array(rtos_vm->vm, task->rs_base, task->rs_depth);
  }
}

static void v4_task_restore_context(v4_rtos_vm_t *rtos_vm, const v4_task_t *task)
{
  if (!rtos_vm || !task)
    return;

  /* Clear current stacks */
  vm_ds_clear(rtos_vm->vm);

  /* Restore stack contents from task's independent stacks */
  if (task->ds_base && task->ds_depth > 0)
  {
    for (int i = 0; i < task->ds_depth; i++)
    {
      vm_ds_push(rtos_vm->vm, task->ds_base[i]);
    }
  }

  /* Note: Return stack restoration would need vm_rs_* APIs which may not exist
   * For now we skip it as tasks typically don't share return stack state */
}

v4_err v4_schedule(v4_rtos_vm_t *rtos_vm)
{
  if (!rtos_vm)
    return V4_ERR_InvalidArg;

  v4_scheduler_t *sched = &rtos_vm->scheduler;

  /* Save current task context */
  v4_task_t *current = &sched->tasks[sched->current_task];
  if (current->state == V4_TASK_STATE_RUNNING)
  {
    v4_task_save_context(rtos_vm, current);
    current->state = V4_TASK_STATE_READY;
  }

  /* Select next task */
  uint8_t next_task_id = v4_task_select_next(rtos_vm);

  /* No switch needed */
  if (next_task_id == sched->current_task && current->state != V4_TASK_STATE_DEAD &&
      current->state != V4_TASK_STATE_BLOCKED)
  {
    current->state = V4_TASK_STATE_RUNNING;
    return V4_ERR_OK;
  }

  /* Context switch */
  v4_task_t *next = &sched->tasks[next_task_id];
  v4_task_restore_context(rtos_vm, next);
  next->state = V4_TASK_STATE_RUNNING;
  next->exec_count++;

  sched->current_task = next_task_id;
  sched->context_switches++;

  return V4_ERR_OK;
}
