#pragma once
#include <stdint.h>
#include <v4/task.h>
#include <v4/vm_api.h>

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @file vm_internal.h
   * @brief V4-RTOS kernel internal structures
   *
   * Extends V4 VM with RTOS features (scheduler, tasks, message queue).
   */

  /**
   * @brief RTOS extension for V4 VM
   *
   * This structure wraps the base V4 VM and adds RTOS-specific features.
   * The V4 VM handles bytecode execution, while this layer adds
   * preemptive multitasking and inter-task communication.
   */
  typedef struct v4_rtos_vm
  {
    struct Vm *vm;            /* V4 VM instance */
    v4_scheduler_t scheduler; /* Task scheduler */
    v4_msg_queue_t msg_queue; /* Message queue */
  } v4_rtos_vm_t;

  /* RTOS VM lifecycle */
  v4_rtos_vm_t *v4_rtos_vm_create(const VmConfig *cfg);
  void v4_rtos_vm_destroy(v4_rtos_vm_t *rtos_vm);

  /* Scheduler interface */
  void v4_scheduler_init(v4_scheduler_t *sched);
  v4_err v4_schedule(v4_rtos_vm_t *rtos_vm);
  uint8_t v4_task_select_next(v4_rtos_vm_t *rtos_vm);
  v4_err v4_task_spawn(v4_rtos_vm_t *rtos_vm, uint16_t word_idx, uint8_t priority,
                       uint8_t ds_size, uint8_t rs_size);
  v4_err v4_task_yield(v4_rtos_vm_t *rtos_vm);
  v4_err v4_task_sleep(v4_rtos_vm_t *rtos_vm, v4_u32 ms);
  v4_err v4_task_exit(v4_rtos_vm_t *rtos_vm);
  uint8_t v4_task_self(v4_rtos_vm_t *rtos_vm);
  uint8_t v4_task_count(v4_rtos_vm_t *rtos_vm);

  /* Message queue interface */
  void v4_msg_queue_init(v4_msg_queue_t *q);
  v4_err v4_msg_send(v4_rtos_vm_t *rtos_vm, uint8_t dst_task, uint8_t msg_type,
                     v4_i32 data);
  v4_err v4_msg_receive(v4_rtos_vm_t *rtos_vm, uint8_t msg_type, v4_i32 *data,
                        uint8_t *src_task, int blocking, v4_u32 timeout_ms);

  /* Platform time interface (must be provided by BSP) */
  v4_u32 v4_platform_get_tick_ms(void);
  v4_u32 v4_platform_get_tick_us(void);
  void v4_platform_delay_ms(v4_u32 ms);
  void v4_platform_delay_us(v4_u32 us);

#ifdef __cplusplus
} /* extern "C" */
#endif
