// Kernel track obstacle warning LED driver.
// A platform driver bound through the device tree (see pvm-led-overlay.dts).
// User space writes '1' or '0' to /dev/pvm_led to switch the LED.
//
// TODO :
//  - struct of_device_id with .compatible = "pvm,pvm-led"
//  - platform_driver with .probe / .remove
//  - in probe: devm_gpiod_get(&pdev->dev, "led", GPIOD_OUT_LOW)
//    (gpio descriptor API: no hard-coded pin numbers in C, the pin comes from DT)
//  - register a misc device (misc_register) named "pvm_led" with a .write
//    that calls gpiod_set_value()
//  - module_platform_driver(), MODULE_DEVICE_TABLE(of, ...)

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/gpio/consumer.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

//device data
static struct gpio_desc *led;

static ssize_t led_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset) {
	char kbuf[4];
	size_t n; 
	
	if (len == 0)
		return 0;
	n = min(len, sizeof(kbuf) - 1);	
	if (copy_from_user(kbuf, buffer, n))
		return -EFAULT;
	kbuf[n] = '\0';

	if (kbuf[0] == '1')
		gpiod_set_value(led, 1);
	else if (kbuf[0] == '0')
		gpiod_set_value(led, 0);
	else
		return -EINVAL;

	return len;
}

//file_operations
static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = led_write, 
};

//struct miscdevice
static struct miscdevice misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "pvm_led",
	.fops = &fops,
};

//probe
static int led_probe(struct platform_device *pdev) {
	int ret;	
	pr_info("Led driver probed\n");
	
	led = devm_gpiod_get(&pdev->dev, "led", GPIOD_OUT_LOW);
	
	if (IS_ERR(led))
		return PTR_ERR(led);

	ret = misc_register(&misc_device);
	if (ret)
		return ret;
	dev_info(&pdev->dev, "probed\n");	
	return 0;	
}

//remove
static void led_remove(struct platform_device *pdev) {
	gpiod_set_value(led, 0);
	misc_deregister(&misc_device);
	pr_info("Led driver removed\n");
}

//matching
static const struct of_device_id led_of_match[] = {
	{ .compatible = "pvm,pvm-led" },
	{}
};

MODULE_DEVICE_TABLE(of, led_of_match);

//platform driver structure
static struct platform_driver led_driver = {
	.probe = led_probe,
	.remove = led_remove,
	.driver = {
		.name = "pvm-led",
		.of_match_table = led_of_match,	
	},
};

//modulr
module_platform_driver(led_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("giangNguyen");
MODULE_DESCRIPTION("Platform LED driver");

