#include "port/sdk_threads.h"
#include "common.h"
#include "libco.h"

#include <eekernel.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Emulate PS2's cooperative multithreading model using libco

#define THREAD_MAX_PRIORITY 127
#define READY_QUEUE_SIZE THREAD_MAX_PRIORITY + 1
#define MAIN_THREAD_ID 1
#define MAX_THREADS 256 - 3
#define ONE_MB 1024 * 1024

typedef struct Thread {
    cothread_t cothread;
    int id;
    int state;
    int init_priority;
    int current_priority;
    int wakeup_request_count;
    struct Thread* prev;
    struct Thread* next;
} Thread;

static Thread threads[MAX_THREADS];
static Thread* current_thread = NULL;
static Thread* ready_queue[READY_QUEUE_SIZE];
static int thread_count = 0;
static bool interrupt = false;
static bool logging_enabled = false;

static void log(const char* fmt, ...) {
#if defined(DEBUG)
    if (!logging_enabled) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    fprintf(stdout, "\n");
    va_end(args);
#endif
}

static void switch_to_current_thread() {
    log("🧵 Switching to [%d]\n", current_thread->id);
    co_switch(current_thread->cothread);
}

static int highest_priority() {
    for (int priority = 1; priority < READY_QUEUE_SIZE; priority++) {
        if (ready_queue[priority] != NULL) {
            return priority;
        }
    }

    return 127;
}

void begin_interrupt() {
    if (interrupt) {
        fatal_error("Already in an interrupt");
    }

    interrupt = true;
}

void end_interrupt() {
    if (!interrupt) {
        fatal_error("Not in an interrupt");
    }

    interrupt = false;
    switch_to_current_thread();
}

static void initialize_if_needed() {
    if (thread_count > 0) {
        return;
    }

    Thread* main_thread = &threads[0];
    main_thread->cothread = co_active();
    main_thread->id = MAIN_THREAD_ID;
    main_thread->state = THS_RUN;
    main_thread->init_priority = 1;
    main_thread->current_priority = 1;
    main_thread->wakeup_request_count = 0;
    main_thread->prev = NULL;
    main_thread->next = NULL;

    current_thread = main_thread;
    ready_queue[1] = main_thread;
    thread_count = 1;
}

static Thread* find_tail(Thread* node) {
    if (node == NULL) {
        return NULL;
    }

    Thread* tail = node;

    while (tail->next != NULL) {
        tail = tail->next;
    }

    return tail;
}

static void append_to_ready_queue(Thread* thread) {
    if (thread == NULL) {
        return;
    }

    Thread* tail = find_tail(ready_queue[thread->current_priority]);

    if (tail == NULL) {
        // Queue is empty. Let's put the thread at the head
        ready_queue[thread->current_priority] = thread;
    } else {
        // Queue has some threads. Let's append the thread
        tail->next = thread;
        thread->prev = tail;
    }
}

static void remove_from_ready_queue(Thread* thread) {
    if (thread == NULL) {
        return;
    }

    Thread* head = ready_queue[thread->current_priority];
    Thread* prev = thread->prev;
    Thread* next = thread->next;

    if (prev != NULL) {
        prev->next = next;
    }

    if (next != NULL) {
        next->prev = prev;
    }

    // If we remove the head we must adjust the ready queue too
    if (thread->id == head->id) {
        ready_queue[thread->current_priority] = next;
    }
}

void reschedule() {
    for (int i = 1; i < READY_QUEUE_SIZE; i++) {
        Thread* thread = ready_queue[i];

        if (thread == NULL) {
            continue;
        }

        thread->state = THS_RUN;
        current_thread = thread;

        if (interrupt) {
            // Switching is going to occur after the interrupt handler "returns"
        } else {
            // Switch immediately if not inside an interrupt handler
            switch_to_current_thread();
        }

        return;
    }

    fatal_error("No threads to switch to");
}

int GetThreadId(void) {
    initialize_if_needed();

    return current_thread->id;
}

int CreateThread(struct ThreadParam* param) {
    initialize_if_needed();

    if (thread_count == MAX_THREADS) {
        printf("Can't create more than %d threads\n", MAX_THREADS);
        return -1;
    }

    if ((param->initPriority < 1) || (param->initPriority > THREAD_MAX_PRIORITY)) {
        printf("Invalid priority: %d\n", param->initPriority);
        return -1;
    }

    Thread* new_thread = &threads[thread_count];
    new_thread->cothread = co_create(ONE_MB, param->entry);
    new_thread->id = thread_count + 1;
    new_thread->state = THS_DORMANT;
    new_thread->init_priority = param->initPriority;
    new_thread->current_priority = param->initPriority;
    new_thread->wakeup_request_count = 0;
    new_thread->prev = NULL;
    new_thread->next = NULL;

    thread_count += 1;

    log("🧵 Created [%d]\n", new_thread->id);

    return new_thread->id;
}

int StartThread(int tid, void* arg) {
    initialize_if_needed();

    if (tid > thread_count) {
        printf("Can't start a non-existent thread %d\n", tid);
        return -1;
    }

    Thread* thread = &threads[tid - 1];

    if (thread->state != THS_DORMANT) {
        printf("Can't start a thread that is not in DORMANT state\n");
        return -1;
    }

    thread->state = THS_READY;
    append_to_ready_queue(thread);

    log("🧵 Started [%d]\n", thread->id);

    return thread->id;
}

int SleepThread(void) {
    initialize_if_needed();

    log("🧵 Sleeping [%d]\n", current_thread->id);

    if (current_thread->wakeup_request_count > 0) {
        current_thread->wakeup_request_count -= 1;
        return current_thread->id;
    }

    current_thread->state = THS_WAIT;
    remove_from_ready_queue(current_thread);

    reschedule();

    return current_thread->id;
}

static void check_irq(bool irq) {
    if (irq && !interrupt) {
        fatal_error("Can't call an interrupt API from a thread");
    }

    if (!irq && interrupt) {
        fatal_error("Can't call a thread API from an interrupt");
    }
}

static int wakeup_thread(int tid, bool irq) {
    initialize_if_needed();
    check_irq(irq);

    if (tid > thread_count) {
        printf("Can't wakeup a non-existent thread %d\n", tid);
        return -1;
    }

    Thread* thread = &threads[tid - 1];

    log("🧵 Waking up [%d]\n", thread->id);

    switch (thread->state) {
    case THS_WAIT:
        thread->state = THS_READY;
        append_to_ready_queue(thread);

        if (irq && (thread->current_priority < current_thread->current_priority)) {
            current_thread->state = THS_READY;
            reschedule();
        }

        break;

    case THS_WAITSUSPEND:
        thread->state = THS_SUSPEND;
        break;

    case THS_READY:
    case THS_RUN:
    case THS_SUSPEND:
    case THS_DORMANT:
        thread->wakeup_request_count += 1;
        break;
    }

    return tid;
}

int WakeupThread(int tid) {
    return wakeup_thread(tid, false);
}

int iWakeupThread(int tid) {
    return wakeup_thread(tid, true);
}

int SuspendThread(int tid) {
    initialize_if_needed();

    if (tid > thread_count) {
        printf("Can't suspend a non-existent thread %d\n", tid);
        return -1;
    }

    if (tid == current_thread->id) {
        printf("Can't suspend current thread\n");
        return -1;
    }

    Thread* thread = &threads[tid - 1];

    log("🧵 Suspending [%d]\n", thread->id);

    switch (thread->state) {
    case THS_WAIT:
        thread->state = THS_WAITSUSPEND;
        break;

    case THS_WAITSUSPEND:
    case THS_SUSPEND:
        // The thread is already suspended
        break;

    case THS_DORMANT:
        // Suspending a dormant thread doesn't make much sense
        break;

    case THS_READY:
        remove_from_ready_queue(thread);
        thread->state = THS_SUSPEND;
        break;

    case THS_RUN:
        // If we're here something has gone wrong
        fatal_error("Implementation error");
        break;
    }

    return tid;
}

int ResumeThread(int tid) {
    initialize_if_needed();

    if (tid > thread_count) {
        printf("Can't resume a non-existent thread %d\n", tid);
        return -1;
    }

    if (tid == current_thread->id) {
        printf("Can't resume current thread\n");
        return -1;
    }

    Thread* thread = &threads[tid - 1];

    log("🧵 Resuming thread %d\n", thread->id);

    switch (thread->state) {
    case THS_SUSPEND:
        thread->state = THS_READY;
        append_to_ready_queue(thread);
        break;

    case THS_WAITSUSPEND:
        thread->state = THS_WAIT;
        break;

    case THS_DORMANT:
    case THS_READY:
    case THS_WAIT:
        // Resuming a thread which is dormant, ready or waiting doesn't make much sense
        break;

    case THS_RUN:
        // If we're here something has gone wrong
        fatal_error("Implementation error");
        break;
    }

    return tid;
}

void ExitDeleteThread(void) {
    initialize_if_needed();

    if (current_thread->id == MAIN_THREAD_ID) {
        fatal_error("Can't exit main thread");
    }

    thread_count -= 1;
    co_delete(current_thread->cothread);
    remove_from_ready_queue(current_thread);
    memset(current_thread, 0, sizeof(Thread));
    reschedule();
}

int ChangeThreadPriority(int tid, int prio) {
    initialize_if_needed();

    if (tid > thread_count) {
        printf("Can't change priority of a non-existent thread %d\n", tid);
        return -1;
    }

    if ((prio < 1) || (prio > THREAD_MAX_PRIORITY)) {
        printf("Invalid priority: %d\n", prio);
        return -1;
    }

    Thread* thread = &threads[tid - 1];
    int old_priority = thread->current_priority;
    bool reschedule_needed = false;
    const int current_highest_priority = highest_priority();

    log("🧵 Changing priority of [%d] from %d to %d\n", thread->id, old_priority, prio);

    switch (thread->state) {
    case THS_RUN:
        reschedule_needed = prio > current_highest_priority;
        break;

    case THS_READY:
        reschedule_needed = prio < current_highest_priority;
        break;

    default:
        thread->current_priority = prio;
        return old_priority;
    }

    // This puts the thread at the bottom of the ready queue for its priority
    remove_from_ready_queue(thread);
    thread->current_priority = prio;
    append_to_ready_queue(thread);

    if (reschedule_needed) {
        reschedule();
    }

    return old_priority;
}

static int refer_thread_status(int tid, struct ThreadParam* info, bool irq) {
    initialize_if_needed();
    check_irq(irq);

    if (tid > thread_count) {
        printf("Can't get status of a non-existent thread %d\n", tid);
        return -1;
    }

    if (info == NULL) {
        return tid;
    }

    Thread* thread = &threads[tid - 1];
    info->status = thread->state;
    info->currentPriority = thread->current_priority;

    return tid;
}

int ReferThreadStatus(int tid, struct ThreadParam* info) {
    return refer_thread_status(tid, info, false);
}

int iReferThreadStatus(int tid, struct ThreadParam* info) {
    return refer_thread_status(tid, info, true);
}

int DelayThread(u_int) {
    not_implemented(__func__);
}
