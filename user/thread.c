#include "user/thread.h"
#include "user/user.h"

int thread_create(void *(start_routine)(void*), void *arg) {
    int tid;
    int stack_size = 4096 * sizeof(void);
    void * stack = (void*)malloc(stack_size);
    tid = clone(stack);
    if (!tid) {
        (*start_routine) (arg);
        exit(0);
    }
    return 0;
}
void lock_init(struct lock_t* lock) {
    lock->locked = 0;
}
void lock_acquire(struct lock_t* lock) {
    while(__sync_lock_test_and_set(&lock->locked, 1) != 0)
    ;
    // Tell the C compiler and the processor to not move loads or stores
    // past this point, to ensure that the critical section's memory
    // references happen strictly after the lock is acquired.
    __sync_synchronize();
}
void lock_release(struct lock_t* lock) {
    // Tell the C compiler and the CPU to not move loads or stores
    // past this point, to ensure that all the stores in the critical
    // section are visible to other CPUs before the lock is released,
    // and that loads in the critical section occur strictly before
    // the lock is released.
    __sync_synchronize();

    // Release the lock, equivalent to lock->locked = 0.
    // This code doesn't use a C assignment, since the C standard
    // implies that an assignment might be implemented with
    // multiple store instructions.
    __sync_lock_release(&lock->locked);
}