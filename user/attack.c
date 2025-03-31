#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

#define sec_start "my very "
int main(int argc, char *argv[])
{
  char *end = sbrk(PGSIZE * 32);
  end += 16 * PGSIZE;
  write(2, end + 32, 8);
  exit(0);
}
