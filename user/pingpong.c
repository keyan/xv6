#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pp[2], cp[2];
  char buf[4];

  pipe(pp);
  pipe(cp);
  if(fork() == 0) {
    close(pp[0]);
    close(cp[1]);

    read(cp[0], buf, 4);
    printf("%d: received %s\n", getpid(), buf);

    strcpy(buf, "pong\0");
    write(pp[1], buf, 4);
  } else {
    close(pp[1]);
    close(cp[0]);

    strcpy(buf, "ping\0");
    write(cp[1], buf, 4);

    read(pp[0], buf, 4);
    printf("%d: received %s\n", getpid(), buf);
  }

  exit(0);
}
