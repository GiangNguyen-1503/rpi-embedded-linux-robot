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
#include <string.h>
#include <fcntl.h> //for open
#include <unistd.h> //for read write close
#include <errno.h>
#define DEV "/dev/pvm"

static int test_write_read(void);
static int test_eof(void);
static int test_too_large(void);

int main(void) {
	int fails = 0;
	fails += test_write_read();
	fails += test_eof();
	fails += test_too_large();	
	printf("%s (%d failed)\n", fails ? "FAILED" : "ALL PASS", fails);
	return fails ? 1 : 0;
}

static int test_write_read(void) {
	// du lieu ghi va doc phai giong nhau
	int fd;
	ssize_t n;
 	char buf[256];
 	const char *msg = "hello pvm";
	
	fd = open(DEV, O_WRONLY);
	if (fd < 0){
		perror("open");
		return 1;
	}

	n = write(fd, msg, strlen(msg));
	close(fd);
	if (n != (ssize_t)strlen(msg)) {
		printf("FAIL: write returned %zd\n", n);
		return 1;
	}

	fd = open(DEV, O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	n = read(fd, buf, sizeof(buf));
	close(fd);
	if ((n != (ssize_t)strlen(msg)) || (memcmp(buf, msg, n) != 0)) {
		printf("FAIL: read returned %zd\n", n);
		return 1;
	}

	printf("PASS: write/read\n");
	return 0;
}

static int test_eof(void) {
	// doc het du lieu thi lan read tiep theo phai return 0
	int fd;
	ssize_t n;
	char buf[256];
	const char *msg = "hello pvm";

	fd = open(DEV, O_RDWR);
	if (fd < 0) {
		perror("open");
		return 1;
	}
	n = write(fd, msg, strlen(msg));
	close(fd);
	if (n!=(ssize_t)strlen(msg)) {
		printf("FAIL write returned %zd\n", n);
		return 1;
	}
	
	fd = open(DEV, O_RDONLY);	
	if (fd < 0) {
		perror("open");
		return 1;
	}
	n = read(fd, buf, sizeof(buf));
	if (n <= 0) {
		close(fd);
		printf("FAIL: first read %zd\n", n);
		return 1;
	}

	n = read(fd, buf, sizeof(buf));
	close(fd);
	if (n!=0) {
		printf("FAIL: expected EOF, got %zd\n", n);
		return 1;
	}	
	printf("PASS: EOF\n");
	return 0;
}

static int test_too_large(void) {
	//ghi vuot 256 thi phia bi tu choi= ENOSPC
	int fd;
	ssize_t n;
	char big[300];

	fd = open(DEV, O_WRONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}	
	errno = 0;
	memset(big, 'X', sizeof(big));	
	n = write(fd, big, sizeof(big));
	int err = errno;
	close(fd);

	if (n!=-1 || err!= ENOSPC) {
		printf("FAIL: n=%zd errno=%d (%s)\n", n, err, strerror(err));
		return 1;	
	}
	printf("PASS: too large rejected\n");
	return 0;	
}
