#ifndef dll_h__
#define dll_h__
 
int async_write(int file, char* buffer, size_t size, off_t offset);
 
int async_read(int file, char* buffer, size_t size, off_t offset);
 
#endif