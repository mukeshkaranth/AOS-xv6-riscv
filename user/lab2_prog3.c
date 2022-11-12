#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main (int argc, char * argv[]) {
    set_tickets(10);
    int i,k;
    const int loop = 250000;
    for (i = 0; i< loop; i++) {
        asm("nop"); //prevents kernel from optimising the for loop.
        for (k = 0; k < loop; k++) {
            asm("nop");
        }
    }
    scheduler_statistics();
    exit(0);
}