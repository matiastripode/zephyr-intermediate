#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(l1_task, LOG_LEVEL_DBG);

#define STACK_SIZE 1024

#define PRIO_COOP (-1)
#define PRIO_HIGH 3
#define PRIO_MED  5
#define PRIO_LOW  7


void thread_low_fn(void *p1, void *p2, void *p3)
{
    int step = 0;

    for (int i = 0; i < 5; i ++) {
        LOG_INF("T_LOW running %d  tick=%u", step++, k_uptime_get_32());
        // k_sleep(K_MSEC(300));
        k_busy_wait(300000);
    }
}

void thread_med_fn(void *p1, void *p2, void *p3)
{
    int step = 0;

    for (int i = 0; i < 5; i ++) {
        LOG_INF("T_MED running %d  tick=%u", step++, k_uptime_get_32());
        // k_sleep(K_MSEC(200));
        k_busy_wait(200000);
    }
}

void thread_high_fn(void *p1, void *p2, void *p3)
{
    int step = 0;

    for (int i = 0; i < 5; i ++) {
        LOG_INF("T_HIGH running %d  tick=%u", step++, k_uptime_get_32());
        // k_sleep(K_MSEC(100));
        k_busy_wait(100000);
    }
}

void thread_cooperative_fn(void *p1, void *p2, void *p3)
{
    int step = 0;

    for (int i = 0; i < 5; i ++) {
        LOG_INF("T_COOPERATIVE running %d  tick=%u", step++, k_uptime_get_32());
        // k_sleep(K_MSEC(100));
        k_busy_wait(100000);
    }
}


K_THREAD_DEFINE(thread_low, STACK_SIZE, thread_low_fn,
                NULL, NULL, NULL, PRIO_LOW, 0, 0);
K_THREAD_DEFINE(thread_med, STACK_SIZE, thread_med_fn,
                NULL, NULL, NULL, PRIO_MED, 0, 0);
K_THREAD_DEFINE(thread_high, STACK_SIZE, thread_high_fn,
                NULL, NULL, NULL, PRIO_HIGH, 0, 0);
K_THREAD_DEFINE(thread_cooperative, STACK_SIZE, thread_cooperative_fn,
                NULL, NULL, NULL, PRIO_COOP, 0, 0);

                
int main(void)
{
    LOG_INF("=== L1 TASK 1: Thread Interleaving ===");
    LOG_INF("Thread LOW: priority %d, sleeps 300ms", PRIO_LOW);
    LOG_INF("Thread MED: priority %d, sleeps 200ms", PRIO_MED);
    LOG_INF("Thread HIGH: priority %d, sleeps 100ms", PRIO_HIGH);
    LOG_INF("Thread COOPERATIVE: priority %d, sleeps 100ms", PRIO_COOP);
    return 0;
}