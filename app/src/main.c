#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/*
Task:
    Goal: Observe and fix a real race condition
    Tasks:
        - Create two equal-priority threads
        - Both increment a shared counter
        - Observe inconsistent results
        - Fix using mutex
        - Push tag: l2-task1
*/

LOG_MODULE_REGISTER(l2_task, LOG_LEVEL_DBG);

#define STACK_SIZE 1024

#define PRIO_MED  5
#define INCREMENTS 1000000

static volatile uint32_t shared_counter;
K_MUTEX_DEFINE(shared_mutex);


void thread_fn(void *p1, void *p2, void *p3)
{
    const char *name = k_thread_name_get(k_current_get());
    LOG_INF("[%s][Start] shared_counter=[%d]  tick=[%u]", name, shared_counter, k_uptime_get_32());
    // Try to lock the shared_mutex
    if (k_mutex_lock(&shared_mutex, K_FOREVER) != 0) {
        LOG_INF("[%s][MUTEX] error trying to lock mutextick=[%u]", name, k_uptime_get_32());
        return;
    }
    for (int i = 0; i < INCREMENTS; i ++) {        
        shared_counter ++;
    }
    // Release shared_mutex
    k_mutex_unlock(&shared_mutex);
    LOG_INF("[%s][Finish] shared_counter=[%d]  tick=[%u]", name, shared_counter, k_uptime_get_32());
}



K_THREAD_DEFINE(thread_a, STACK_SIZE, thread_fn,
                NULL, NULL, NULL, PRIO_MED, 0, 0);

K_THREAD_DEFINE(thread_b, STACK_SIZE, thread_fn,
                NULL, NULL, NULL, PRIO_MED, 0, 0);

                
int main(void)
{
    k_mutex_init(&shared_mutex);
    shared_counter = 0;
    LOG_INF("=== L2 TASK 1: Observe and fix a real race condition ===");
    return 0;
}