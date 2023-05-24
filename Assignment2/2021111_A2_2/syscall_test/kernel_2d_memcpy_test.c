#include <stdio.h>
#include <sys/syscall.h>
#include <linux/kernel.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <linux/kernel.h>
#include <syscall.h>

#define KERNEL_2D_MEMCPY_SYSCALL 451

void check_equal(float lhs[5][5], float rhs[5][5], int size, int colSize)
{
    int i;
    int j;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < colSize; j++)
        {
            if (lhs[i][j] != rhs[i][j])
            {
                printf("Error : LHS[%d][%d] != RHS[%d][%d]\n", i, j, i, j);
                return;
            }
        }
    }
    printf("Message : Success LHS = RHS \n");
}

int main()
{
    float src_arr[5][5];
    float dest_arr[5][5]; 

    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
          src_arr[i][j]=(5*i+j)/10;
        }
    }

    long sys_call_status;

    sys_call_status = syscall(KERNEL_2D_MEMCPY_SYSCALL, src_arr, dest_arr, 25);

    if (sys_call_status != EFAULT)
    {
        printf("Message : System Call %d successfuly invoked \n", KERNEL_2D_MEMCPY_SYSCALL);
        check_equal(src_arr, dest_arr, 5, 5);
    }

    return 0;
}

