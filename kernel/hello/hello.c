// Day 1 of the kernel track (Sun 22): the smallest possible module.
//
// TODO (write it yourself, ~15 lines):
//  - #include <linux/module.h>, <linux/init.h>
//  - an __init function that pr_info()s a message and returns 0
//  - an __exit function that pr_info()s a goodbye
//  - module_init(), module_exit()
//  - MODULE_LICENSE("GPL"), MODULE_AUTHOR(), MODULE_DESCRIPTION()
//
// Test:
//   make && sudo insmod hello.ko && dmesg | tail
//   lsmod | grep hello && modinfo hello.ko
//   sudo rmmod hello && dmesg | tail
//
// Interview questions to be able to answer afterwards:
//  - Why printk/pr_info and not printf?
//  - What happens if init returns a negative value?
//  - Why does MODULE_LICENSE matter?
