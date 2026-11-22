// Kernel track day 4 (Wed 25): obstacle warning LED driver.
// A platform driver bound through the device tree (see pvm-led-overlay.dts).
// User space writes '1' or '0' to /dev/pvm_led to switch the LED.
//
// TODO (write it yourself):
//  - struct of_device_id with .compatible = "pvm,obstacle-led"
//  - platform_driver with .probe / .remove
//  - in probe: devm_gpiod_get(&pdev->dev, "led", GPIOD_OUT_LOW)
//    (gpio descriptor API: no hard-coded pin numbers in C, the pin comes from DT)
//  - register a misc device (misc_register) named "pvm_led" with a .write
//    that calls gpiod_set_value()
//  - module_platform_driver(), MODULE_DEVICE_TABLE(of, ...)
//
// Interview questions to be able to answer afterwards:
//  - How does the kernel match this driver to the hardware? (compatible string)
//  - Why use the gpiod API instead of writing GPIO registers directly?
//  - What does devm_ do for you?
