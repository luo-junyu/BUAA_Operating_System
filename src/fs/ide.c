/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurred during read the IDE disk, panic. 
// 	
// Hint: use syscalls to access device registers and buffers
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
	char zero = 0;
	// set the IDE id

	while (offset_begin + offset < offset_end) {
        // Your code here
		syscall_write_dev(&diskno, 0x13000010, sizeof(int));
		int offset_sum = offset_begin + offset;
		syscall_write_dev(&offset_sum, 0x13000000, sizeof(int));
		syscall_write_dev(&zero, 0x13000020, sizeof(char));
		char status;
		syscall_read_dev(&status, 0x13000030, sizeof(char));

        // error occurred, then panic.
		if(status == 0) {
			user_panic("[DEBUG] ide_read: read failed!\n");
		}
		// else we can read
		syscall_read_dev(dst+offset, 0x13004000, 0x200);

		offset += 0x200;
	}
}


// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurred during read the IDE disk, panic.
//	
// Hint: use syscalls to access device registers and buffers
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
        // Your code here
	int offset_begin = secno * 0x200;
	int offset_end =  offset_begin + nsecs * 0x200;
	int offset = 0;
	char one = 1;
 	writef("diskno: %d\n", diskno);
	// set the IDE id
	while (offset_begin + offset < offset_end ) {
	    // copy data from source array to disk buffer.
		syscall_write_dev(&diskno, 0x13000010, sizeof(int));
		int offset_sum = offset_begin + offset;
		syscall_write_dev(&offset_sum, 0x13000000, sizeof(int));
		syscall_write_dev(src+offset, 0x13004000, 0x200);
		syscall_write_dev(&one, 0x13000020, sizeof(char));
        // if error occur, then panic.
		char status;
		syscall_read_dev(&status, 0x13000030, sizeof(char));
		if(status == 0) {
			user_panic("[DEBUG] ide_read: read failed!\n");
		}
		offset += 0x200;
	 }
}

