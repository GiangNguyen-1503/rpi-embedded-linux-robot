// User-space test for /dev/pvm (kernel/pvm_chardev).
//
// TODO:
//  - open("/dev/pvm", O_RDWR), check errors with perror()
//  - write a message, lseek back or reopen, read it back, compare
//  - close
// Then:
//   gdb ./build/tools/chardev_test/chardev_test   (break main, step, print)
//   valgrind --leak-check=full ./build/tools/chardev_test/chardev_test
#include <stdio.h>

int main(void) {
    printf("chardev_test: TODO\n");
    return 0;
}
