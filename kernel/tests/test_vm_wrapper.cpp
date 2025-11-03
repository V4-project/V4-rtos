#define DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cstring>

#include "v4/errors.hpp"
#include "v4/internal/vm.h"
#include "v4/internal/vm_internal.h"
#include "vendor/doctest/doctest.h"

// Helper: register a dummy word for task testing
static int register_dummy_word(Vm *vm)
{
  // Register a simple word (empty bytecode for testing)
  uint8_t dummy_code[] = {0x00};  // EXIT opcode
  return vm_register_word(vm, "dummy", dummy_code, sizeof(dummy_code));
}

TEST_CASE("VM create and destroy")
{
  uint8_t ram[256];
  VmConfig cfg;
  memset(&cfg, 0, sizeof(cfg));
  cfg.mem = ram;
  cfg.mem_size = sizeof(ram);
  cfg.mmio = nullptr;
  cfg.mmio_count = 0;

  SUBCASE("Create RTOS VM with valid config")
  {
    Vm *vm = v4_rtos_vm_create(&cfg);
    REQUIRE(vm != nullptr);

    // Verify scheduler initialization
    CHECK(vm->scheduler.current_task == 0);
    CHECK(vm->scheduler.task_count == 0);
    CHECK(vm->scheduler.time_slice_ms == 10);

    // Check all tasks are dead
    for (int i = 0; i < V4_MAX_TASKS; i++)
    {
      CHECK(vm->scheduler.tasks[i].state == V4_TASK_STATE_DEAD);
    }

    // Check message queue is initialized
    CHECK(vm->msg_queue.count == 0);
    CHECK(vm->msg_queue.read_idx == 0);
    CHECK(vm->msg_queue.write_idx == 0);

    v4_rtos_vm_destroy(vm);
  }

  SUBCASE("Create VM with NULL config")
  {
    Vm *vm = v4_rtos_vm_create(nullptr);
    CHECK(vm == nullptr);
  }

  SUBCASE("Destroy NULL VM")
  {
    // Should not crash
    v4_rtos_vm_destroy(nullptr);
    CHECK(true);
  }
}

TEST_CASE("Task spawn")
{
  uint8_t ram[1024];
  VmConfig cfg;
  memset(&cfg, 0, sizeof(cfg));
  cfg.mem = ram;
  cfg.mem_size = sizeof(ram);
  cfg.mmio = nullptr;
  cfg.mmio_count = 0;

  Vm *vm = v4_rtos_vm_create(&cfg);
  REQUIRE(vm != nullptr);

  int word_idx = register_dummy_word(vm);
  REQUIRE(word_idx >= 0);

  SUBCASE("Spawn single task")
  {
    int task_id = vm_task_spawn(vm, word_idx, 5, 256, 64);
    CHECK(task_id >= 0);
    CHECK(task_id < V4_MAX_TASKS);
    CHECK(vm->scheduler.task_count == 1);

    // Verify task state
    v4_task_t *task = &vm->scheduler.tasks[task_id];
    CHECK(task->state == V4_TASK_STATE_READY);
    CHECK(task->priority == 5);
    CHECK(task->word_idx == word_idx);
    CHECK(task->ds_base != nullptr);
    CHECK(task->rs_base != nullptr);
  }

  SUBCASE("Spawn multiple tasks")
  {
    int task_id1 = vm_task_spawn(vm, word_idx, 5, 256, 64);
    int task_id2 = vm_task_spawn(vm, word_idx, 3, 128, 32);
    CHECK(task_id1 >= 0);
    CHECK(task_id2 >= 0);
    CHECK(task_id1 != task_id2);
    CHECK(vm->scheduler.task_count == 2);
  }

  SUBCASE("Spawn maximum tasks")
  {
    // Spawn maximum number of tasks
    for (int i = 0; i < V4_MAX_TASKS; i++)
    {
      int task_id = vm_task_spawn(vm, word_idx, 1, 64, 32);
      CHECK(task_id >= 0);
    }
    CHECK(vm->scheduler.task_count == V4_MAX_TASKS);

    // Try to spawn one more (should fail)
    int task_id = vm_task_spawn(vm, word_idx, 1, 64, 32);
    CHECK(task_id < 0);
  }

  SUBCASE("Spawn with invalid word index")
  {
    int task_id = vm_task_spawn(vm, 9999, 1, 64, 32);
    CHECK(task_id < 0);
    CHECK(vm->scheduler.task_count == 0);
  }

  v4_rtos_vm_destroy(vm);
}

TEST_CASE("Task lifecycle")
{
  uint8_t ram[1024];
  VmConfig cfg;
  memset(&cfg, 0, sizeof(cfg));
  cfg.mem = ram;
  cfg.mem_size = sizeof(ram);
  cfg.mmio = nullptr;
  cfg.mmio_count = 0;

  Vm *vm = v4_rtos_vm_create(&cfg);
  REQUIRE(vm != nullptr);

  int word_idx = register_dummy_word(vm);
  REQUIRE(word_idx >= 0);

  SUBCASE("Get task info")
  {
    // Spawn a task with priority 7
    int task_id = vm_task_spawn(vm, word_idx, 7, 256, 64);
    REQUIRE(task_id >= 0);

    // Get task info
    v4_task_state_t state;
    uint8_t priority;
    v4_err err = vm_task_get_info(vm, task_id, &state, &priority);
    CHECK(err == V4_ERR(OK));
    CHECK(state == V4_TASK_STATE_READY);
    CHECK(priority == 7);

    // Test with invalid task ID
    err = vm_task_get_info(vm, 99, &state, &priority);
    CHECK(err != V4_ERR(OK));
  }

  SUBCASE("Get current task ID")
  {
    // Current task should be 0 (initial task)
    int task_id = vm_task_self(vm);
    CHECK(task_id == 0);
  }

  SUBCASE("Critical section nesting")
  {
    // Enter critical section
    v4_err err = vm_task_critical_enter(vm);
    CHECK(err == V4_ERR(OK));
    CHECK(vm->scheduler.critical_nesting == 1);

    // Nested critical section
    err = vm_task_critical_enter(vm);
    CHECK(err == V4_ERR(OK));
    CHECK(vm->scheduler.critical_nesting == 2);

    // Exit critical sections
    err = vm_task_critical_exit(vm);
    CHECK(err == V4_ERR(OK));
    CHECK(vm->scheduler.critical_nesting == 1);

    err = vm_task_critical_exit(vm);
    CHECK(err == V4_ERR(OK));
    CHECK(vm->scheduler.critical_nesting == 0);
  }

  v4_rtos_vm_destroy(vm);
}

TEST_CASE("Message passing")
{
  uint8_t ram[1024];
  VmConfig cfg;
  memset(&cfg, 0, sizeof(cfg));
  cfg.mem = ram;
  cfg.mem_size = sizeof(ram);
  cfg.mmio = nullptr;
  cfg.mmio_count = 0;

  Vm *vm = v4_rtos_vm_create(&cfg);
  REQUIRE(vm != nullptr);

  int word_idx = register_dummy_word(vm);
  REQUIRE(word_idx >= 0);

  SUBCASE("Send and receive message")
  {
    int task1 = vm_task_spawn(vm, word_idx, 1, 256, 64);
    REQUIRE(task1 >= 0);

    // Get current task ID
    int current = vm_task_self(vm);

    // Send message to current task
    v4_err err = vm_task_send(vm, current, 42, 12345);
    CHECK(err == V4_ERR(OK));
    CHECK(vm->msg_queue.count == 1);

    // Receive the message
    int32_t data;
    uint8_t src;
    int result = vm_task_receive(vm, 0, &data, &src);
    CHECK(result == 1);  // Message received
    CHECK(data == 12345);
    CHECK(src == current);
    CHECK(vm->msg_queue.count == 0);  // Message consumed
  }

  SUBCASE("Message queue full")
  {
    int task1 = vm_task_spawn(vm, word_idx, 1, 256, 64);
    REQUIRE(task1 >= 0);

    // Fill the message queue (V4_MSG_QUEUE_SIZE = 16)
    v4_err err;
    for (int i = 0; i < 16; i++)
    {
      err = vm_task_send(vm, task1, 1, i);
      CHECK(err == V4_ERR(OK));
    }
    CHECK(vm->msg_queue.count == 16);

    // Try to send one more (should fail with queue full error)
    err = vm_task_send(vm, task1, 1, 999);
    CHECK(err != V4_ERR(OK));  // Should fail (error code may vary)
  }

  SUBCASE("Broadcast message")
  {
    // Send broadcast message (target = 0xFF)
    v4_err err = vm_task_send(vm, 0xFF, 99, 777);
    CHECK(err == V4_ERR(OK));
    CHECK(vm->msg_queue.count == 1);

    // Current task should receive broadcast
    int32_t data;
    uint8_t src;
    int result = vm_task_receive(vm, 99, &data, &src);
    CHECK(result == 1);  // Broadcast received
    CHECK(data == 777);
  }

  v4_rtos_vm_destroy(vm);
}
