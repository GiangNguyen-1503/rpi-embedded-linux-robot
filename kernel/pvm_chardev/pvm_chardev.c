// character device driver, no hardware needed.
// Creates /dev/pvm. Writing stores up to 64 bytes in a kernel buffer,
// reading returns them.
//
// TODO 
//  - alloc_chrdev_region() to get a major/minor number
//  - struct file_operations with .open .release .read .write
//  - cdev_init() + cdev_add()
//  - class_create() + device_create() so udev creates /dev/pvm automatically
//  - copy_from_user() in write, copy_to_user() in read
//    (NEVER dereference a user-space pointer directly: explain why in README)
//  - protect the buffer with a mutex (two processes may write at once)
//  - undo everything in reverse order in the exit function
//
// Test with tools/chardev_test (user space), then run it under gdb and valgrind

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;

//1.open
static int open(struct inode *inode, struct file *file) {
	pr_info("open\n");
	return 0;
}

//2.release
static int release(struct inode *inode, struct file *file) {
	pr_info("release\n");
	return 0;
}

//file-operations
static const struct file_operations fops = {
	.owner = THIS_MODULE, //keep module from rrmod;
	.open = open,
	.release = release,
};

//init
static int __init my_driver_init(void) {
	int ret;

	//1. allocate device number
	ret = alloc_chrdev_region(&dev_num, 0, 1, "pvm");
	if (ret < 0)
		return ret;

	//2. initialize cdev
	cdev_init(&my_cdev, &fops);

	//3. register cdev
	ret = cdev_add(&my_cdev, dev_num, 1);
	if (ret <0)
		goto err_unregister;

	//4. class create
	my_class = class_create("pvm");
	if (IS_ERR(my_class)) {
		ret = PTR_ERR(my_class);
		goto err_cdev;
	}

	//5. device create
	my_device = device_create(my_class, NULL, dev_num, NULL, "pvm");
	if (IS_ERR(my_device)) {
		ret = PTR_ERR(my_device);
		goto err_class;
	}

	pr_info("device registered\n");
	return 0;

err_class:
	class_destroy(my_class);
err_cdev:
	cdev_del(&my_cdev);
err_unregister:
	unregister_chrdev_region(dev_num, 1);
	return ret;
}

//exit
static void __exit my_driver_exit(void) {
	device_destroy(my_class, dev_num);
	class_destroy(my_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev_num, 1);
	
	pr_info("driver unregistered\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Giang Nguyen");
MODULE_DESCRIPTION("Character device driver\n"); 
