#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i, n;
  char *args[MAXARG];
  char buf[1];
  char cmd[512], *p;

  for (i = 1; i < argc; i++){
    args[i-1] = argv[i];
  }
  i--;

  p = cmd;
  for(;;){
    n = read(0, buf, 1);
    if(n == 0){
      break;
    }
    if(buf[0] == '\n'){
      *p = '\0';
      args[i] = cmd;
      if(fork() == 0){
        exec(args[0], args);
        break;
      } else {
        wait(0);
        p = cmd;
      }
    } else {
      *p++ = *buf;
    }
  }

  exit(0);
}
