#include <linux/kernel.h>
/*#include <sys/syscall.h>*/
#include <linux/syscalls.h>

/*
 * Definition for kernel_2d_memcpy system call 
 */
SYSCALL_DEFINE3(kernel_2d_memcpy, float*, src, float*, dest, int, len)
/*long kernel_2d_memcpy(float* src, float* dest, int len)*/
{
    /* Allocate a buffer of size len to hold the elements of array */
    float buffer[25]; 
    
    /* Copy from user copies the data from src to buffer of size len */
    if (__copy_from_user(buffer, src, sizeof(float)* (len)))
       return -EFAULT;

    /* Copy to user copies the data from buffer to dest of size len */
    if (__copy_to_user(dest, buffer, sizeof(float)*(len)))
       return -EFAULT;

    return 0;
}
