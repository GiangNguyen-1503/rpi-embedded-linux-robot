rpi-embedded-linux-robot

"Linux kernel drivers" and "user-space tools" in C on a Raspberry Pi 4B from a first kernel module to a device-tree-bound platform driver, built as the "embedded Linux layer" for my autonomous obstacle-avoidance car (graduation capstone).

All code in this repository is written by me, in C.


Hardware & environment
Raspberry Pi 4 Model B, Raspberry Pi OS 64-bit
Kernel 6.18.60+rpt-rpi-v8, headers from linux-headers-rpi-v8
Red LED on GPIO17 (header pin 11) → 1 kΩ resistor → GND (header pin 9)


Status
Part ----- Component ----- What it shows -----	Status
A – Kernel	kernel/hello	module lifecycle (init/exit), Kbuild	✅
A – Kernel	kernel/pvm_chardev + tools/chardev_test	character device, read/write, locking, tests	✅
A – Kernel	tools/gpio_regs + docs/datasheet-notes.md	register-level GPIO from the BCM2711 datasheet	✅
A – Kernel	kernel/pvm_led	platform driver + device tree overlay + gpiod	✅
B – User space	capstone algorithm in C, services, IPC, UART	middleware	(((in progress)))


Repository layout
kernel/         kernel modules (built with Kbuild, not CMake)
    hello/          minimal loadable module
    pvm_chardev/    character device /dev/pvm
    pvm_led/        platform driver + DT overlay, /dev/pvm_led
tools/          user-space programs
    chardev_test/   automated tests for /dev/pvm
    gpio_regs/      GPIO17 via BCM2711 registers (mmap /dev/gpiomem)
docs/           datasheet notes
services/       Part B (in progress)


Part A – Kernel
/kernel/hello
Minimal module: module_init/module_exit, __init/__exit, out-of-tree Kbuild Makefile.

  cd kernel/hello && make
  sudo insmod hello.ko && sudo rmmod hello
  dmesg | tail -n 2
  
/kernel/pvm_chardev - character device /dev/pvm
A 256-byte in-kernel buffer exposed as /dev/pvm.
- Registration in four steps: alloc_chrdev_region
                           -> cdev_init/cdev_add
                           -> class_create
                           -> device_create (udev creates /dev/pvm); torn down in reverse order
- Error handling in init with goto unwinding; IS_ERR/PTR_ERR for pointer-returning APIs
- read/write use copy_to_user/copy_from_user (never dereference __user pointers)
- read tracks *offset and returns 0 at end of data, so cat terminates
- write replaces the buffer contents; writes larger than 256 bytes are rejected with -ENOSPC
- A mutex protects the buffer and its length; mutex_lock_interruptible so a waiting process can be interrupted

  cd kernel/pvm_chardev && make && sudo insmod pvm_chardev.ko
  echo hello | sudo tee /dev/pvm
  sudo cat /dev/pvm                 # hello
  sudo rmmod pvm_chardev


Tests: tools/chardev_test
  cd tools/chardev_test
  gcc -Wall -Wextra -g -O0 -o chardev_test main.c
  sudo ./chardev_test
result:
    PASS: write/read
    PASS: EOF
    PASS: too large rejected
    ALL PASS (0 failed)

  sudo valgrind --leak-check=full --track-fds=yes --error-exitcode=1 ./chardev_test
Result: ERROR SUMMARY: 0 errors, all heap blocks freed, only the 3 standard file descriptors open at exit.

Concurrency check: two writer processes (30-byte and 2-byte strings) writing in a loop while 2000 single-read calls run every read returned one complete string, never a mix.


/tools/gpio_regs — GPIO from the datasheet
User-space program that drives GPIO17 by writing BCM2711 registers directly.
- Register offsets taken from the BCM2711 ARM Peripherals datasheet. see docs/datasheet-notes.md
- mmap of /dev/gpiomem through a volatile uint32_t *
- GPFSEL1 bits 23:21 = 001 (output) with read-modify-write; GPSET0/GPCLR0 bit 17 written directly (write-only, writing 0 has no effect); state read back from GPLEV0
- The pin is cleared before switching to output, and returned to input on exit

  cd tools/gpio_regs
  gcc -Wall -Wextra -g -O0 -o gpio_regs main.c
  ./gpio_regs                      # in a second shell: watch -n 0.5 pinctrl get 17


/kernel/pvm_led — platform driver + device tree
The LED is described in a device tree overlay instead of being hard-coded in the driver.
- Overlay node with compatible = "pvm,pvm-led" and led-gpios = <&gpio 17 0> (active-high)
- of_device_id table matches the node, so the kernel calls probe
- probe: devm_gpiod_get(dev, "led", GPIOD_OUT_LOW) + misc_register → /dev/pvm_led (misc major 10, dynamic minor)
- write: '1' turns the LED on, '0' off, anything else returns -EINVAL
- remove: turns the LED off and deregisters the misc device; the GPIO is released by devm

  cd kernel/pvm_led
  dtc -@ -I dts -O dtb -o pvm-led.dtbo pvm-led-overlay.dts
  sudo dtoverlay -d . pvm-led
  make && sudo insmod pvm_led.ko
  dmesg | tail -n 1                # pvm-led pvm-led: probed
  echo 1 | sudo tee /dev/pvm_led   # LED on  (pinctrl get 17 -> hi)
  echo 0 | sudo tee /dev/pvm_led   # LED off (pinctrl get 17 -> lo)


Debugging notes
1. A compiler warning that was really a leak. make reported "label defined but not used" in pvm_chardev's init. The cause: after a failed cdev_add I returned directly instead of jumping to the unwind label, so the allocated device number was never released.

2. LED logic inverted. echo 1 drove the pin low. Reading the live device tree with xxd /proc/device-tree/pvm-led/led-gpios showed the GPIO flag cell was 1 (GPIO_ACTIVE_LOW), so gpiod inverted every value. Fixed in the overlay; (((the driver did not change.)))

3. A failing test that was a test bug. chardev_test reported "expected EOF, got 0". Stepping through with GDB showed n == 0 (correct EOF); the condition reused n after it had been overwritten by the second read.


Part B – User space (((in progress...)))
-->> Rewrite of the capstone's obstacle-avoidance logic in C as a small middleware stack:       camera-based free-space detection (Canny + column scan, left/front/right zones),
     a CRUISE/AVOIDING/STUCK state machine,
     services connected by Unix domain sockets (SOCK_SEQPACKET) with pthreads, 
     w/a/s/d/x motor commands to the Arduino over UART,
     and the obstacle indicator driven through /dev/pvm_led.
