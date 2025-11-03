#include <stdlib.h>

#include "v4/internal/vm_internal.h"

/**
 * @file vm_wrapper.c
 * @brief V4-RTOS VM wrapper
 *
 * Wraps the V4 VM and adds RTOS features (scheduler, message queue).
 * The V4 VM handles bytecode execution, while this layer provides
 * preemptive multitasking.
 */

v4_rtos_vm_t *v4_rtos_vm_create(const VmConfig *cfg)
{
  if (!cfg)
    return NULL;

  v4_rtos_vm_t *rtos_vm = (v4_rtos_vm_t *)calloc(1, sizeof(v4_rtos_vm_t));
  if (!rtos_vm)
    return NULL;

  /* Create V4 VM instance */
  rtos_vm->vm = vm_create(cfg);
  if (!rtos_vm->vm)
  {
    free(rtos_vm);
    return NULL;
  }

  /* Initialize RTOS components */
  v4_scheduler_init(&rtos_vm->scheduler);
  v4_msg_queue_init(&rtos_vm->msg_queue);

  return rtos_vm;
}

void v4_rtos_vm_destroy(v4_rtos_vm_t *rtos_vm)
{
  if (!rtos_vm)
    return;

  /* Destroy V4 VM */
  if (rtos_vm->vm)
  {
    vm_destroy(rtos_vm->vm);
  }

  free(rtos_vm);
}
