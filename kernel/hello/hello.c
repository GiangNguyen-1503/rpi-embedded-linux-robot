// Test:
//   make && sudo insmod hello.ko && dmesg | tail
//   lsmod | grep hello && modinfo hello.ko
//   sudo rmmod hello && dmesg | tail
// Interview questions to be able to answer afterwards:
//  - Why printk/pr_info and not printf?
//  - What happens if init returns a negative value?
//  - Why does MODULE_LICENSE matter

#include <linux/module.h>
#include <linux/init.h>

static int __init hello_init(void) {
	pr_info("hello loaded\n");
	return 0;
}

static void __exit hello_exit(void) {
	pr_info("hello UNloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Giang Nguyen");
MODULE_DESCRIPTION("Minimal hello kernel module");
