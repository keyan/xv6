#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  p = buf + strlen(p);
  *p = '\0';
  return buf;
}

void
process_path(char *path, char *search){
  int fd;
  struct stat st;
  char buf[512], *p;
  struct dirent de;

  if((fd = open(path, 0)) < 0){
    return;
  }

  if(fstat(fd, &st) < 0){
    close(fd);
    return;
  }

  if(strcmp(fmtname(path), search) == 0){
    printf("%s\n", path);
  }

  switch(st.type){
  case T_FILE:
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;

      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }

      // Don't recur again.
      if(strcmp(fmtname(buf), ".") == 0 || strcmp(fmtname(buf), "..") == 0){
        continue;
      }

      char param[strlen(buf)];
      strcpy(param, buf);
      process_path(param, search);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  char *path, *search;

  if(argc <= 2){
    printf("must provide path and search string\n");
    exit(1);
  }

  path = argv[1];
  search = argv[2];

  process_path(path, search);
  exit(0);
}
