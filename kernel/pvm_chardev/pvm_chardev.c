// Kernel track day 2 (Mon 23): character device driver, no hardware needed.
// Creates /dev/pvm. Writing stores up to 64 bytes in a kernel buffer,
// reading returns them.
//
// TODO (write it yourself):
//  - alloc_chrdev_region() to get a major/minor number
//  - struct file_operations with .open .release .read .write
//  - cdev_init() + cdev_add()
//  - class_create() + device_create() so udev creates /dev/pvm automatically
//  - copy_from_user() in write, copy_to_user() in read
//    (NEVER dereference a user-space pointer directly: explain why in README)
//  - protect the buffer with a mutex (two processes may write at once)
//  - undo everything in reverse order in the exit function
//
// Test with tools/chardev_test (user space), then run it under gdb and valgrind.
