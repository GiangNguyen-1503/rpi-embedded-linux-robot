// Kernel track switch the LED by writing GPIO registers
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
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define GPFSEL1 (0x04/4)
#define GPSET0 (0x1C/4)
#define GPCLR0 (0x28/4)
#define GPLEV0 (0x34/4)

int main(void) {
	printf("gpio_regs: TODO\n");
	int fd;
	uint32_t v;
	
	fd = open("/dev/gpiomem", O_RDWR |O_SYNC);
	if (fd<0) {
		perror("open");
		return 1;
	}
	
	volatile uint32_t *gpio; // o user space volatile pointer la duoc kernel driver thi ioremap
	gpio = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //ok, vi void * -> volatile uint32_t * ok;
	if (gpio == MAP_FAILED) {
		perror("mmap");
		close(fd);
		return 1;
	}
	
	
	gpio[GPCLR0] = 1u << 17; //writel(1u << 17, gpio + GPCLR0); //gpclr0
	
	//ghi output 001
	v = gpio[GPFSEL1]; //readl(gpio + GPFSEL1);
	v &= ~(7u << 21);
	v |= (1u << 21);
	gpio[GPFSEL1] = v; //writel(v, gpio + GPFSEL1);
	
	for (int i = 0; i < 5; i++) {
		printf("--- %d ---\n", i+1);	
		gpio[GPSET0] = 1u << 17; //writel(1u << 17, gpio + GPSET0);
		printf("GPLEV0 %d\n", (gpio[GPLEV0] >> 17) & 1);
		sleep(1);
		
		gpio[GPCLR0] = 1u << 17; //writel(1u << 17, gpio + GPCLR0);
		printf("GPLEV0 %d\n", (gpio[GPLEV0] >> 17) & 1);
		sleep(1);
	}
	
	v = gpio[GPFSEL1];
	v &= ~(7u << 21);
	gpio[GPFSEL1] = v; //writel(v, gpio + GPFSEL1);
	munmap((void *)gpio, 4096); // XXXmunmap(gpio, 4096) munmap/mmap nhan kieu void *, ma gpio la volatile uint32_t *;
	close(fd);	

	return 0;
}
