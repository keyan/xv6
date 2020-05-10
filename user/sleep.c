#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int ticks;

  if(argc <= 1){
    printf("must provide an integer argument\n");
    exit(1);
  }

  ticks = atoi(argv[1]);
  int ret = sleep(ticks);

  if(ret == 0) {
    exit(0);
  }
  exit(1);
}
