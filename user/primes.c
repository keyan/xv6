// Concurrent prime sieve based on idea presented in:
//	 https://swtch.com/~rsc/thread/
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
worker(int p[])
{
  int s, n, nread;

  // We never write to the left neighbor.
  close(p[1]);

  nread = read(p[0], &s, sizeof(s));
  if(nread == 0) {
    exit(0);
  }
  printf("prime %d\n", s);

  int child_p[2];
  pipe(child_p);
  if(fork() == 0) {
    worker(child_p);
  } else {
    close(child_p[0]);
  }

  while(1) {
    nread = read(p[0], &n, sizeof(n));
    // The write end has been closed, pass along the message by doing the same.
    if(nread == 0) {
      close(child_p[1]);
      wait(0);
      exit(0);
    }

    // Not prime.
    if(n % s == 0) {
      continue;
    }

    write(child_p[1], &n, sizeof(n));
  }
}

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);
  if(fork() == 0) {
    worker(p);
    exit(0);
  }
  close(p[0]);

  for(int n = 2; n < 35; n++) {
    write(p[1], &n, sizeof(n));
  }
  close(p[1]);

  wait(0);
  exit(0);
}
