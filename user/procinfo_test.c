#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

struct pinfo { 
  int ppid; 
  int syscall_count; 
  int page_usage; 
};

int main( int argc, char* argv[]) {
  struct pinfo param; 
  procinfo(&param);
  exit(0);
}
