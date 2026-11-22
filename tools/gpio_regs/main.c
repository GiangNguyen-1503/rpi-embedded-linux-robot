// Kernel track day 3 (Tue 24): switch the LED by writing GPIO registers
// directly from user space, using /dev/gpiomem (no driver yet).
// Purpose: practise reading the BCM2711 datasheet and bit manipulation.
//
// TODO (find every value in the "BCM2711 ARM Peripherals" datasheet, GPIO chapter,
//       and note the page/section in docs/datasheet-notes.md):
//  - offset of the function-select register for your pin (GPFSELn)
//  - which 3 bits inside it select "output" for your pin, and the value to write
//  - offset of the set register (GPSETn) and clear register (GPCLRn)
//  - mmap /dev/gpiomem, use a volatile uint32_t* to access the registers
//  - read-modify-write GPFSEL with mask + shift; write 1 bits to GPSET/GPCLR
//
// Interview question: why does GPSET/GPCLR exist instead of one read-modify-write
// output register? (hint: atomicity)
#include <stdio.h>

int main(void) {
    printf("gpio_regs: TODO\n");
    return 0;
}
