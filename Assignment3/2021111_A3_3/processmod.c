#include <linux/module.h>     // included for all kernel modules
#include <linux/kernel.h>     // included for KERN_INFO
#include <linux/init.h>       // included for __init and __exit macros
#include <linux/sched.h>      // included for task_struct
#include <linux/moduleparam.h>
#include <linux/cred.h>
#include <linux/mm.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CSE-231-VIBHOR");
MODULE_DESCRIPTION("Process Detils Linux Kernel Module."); 

int PID = 0;

module_param(PID, int, S_IRUSR | S_IWUSR);

static int __init process_details_init(void)
{
    struct task_struct* taskp = get_pid_task(find_get_pid(PID),PIDTYPE_PID);
    char *pathname, *p;
    struct mm_struct* mm = taskp->mm;

    printk(KERN_INFO "Process '%d' Details ...", PID);
    printk(KERN_INFO "Process id: %d\n", taskp->pid);
    printk(KERN_INFO "Process user id: %d\n", taskp->real_cred->uid.val);
    printk(KERN_INFO "Process group id: %d\n", taskp->real_cred->gid.val);

    //printk(KERN_INFO "Process group id: %d\n", taskp->cred->uid.val);
    //printk(KERN_INFO "Process group id: %d\n", taskp->cred->gid.val);
    //printk(KERN_INFO "Process command: %s\n", taskp->comm);

    pathname = kmalloc(PATH_MAX, GFP_ATOMIC);
    p = d_path(&mm->exe_file->f_path, pathname, PATH_MAX);
    printk(KERN_INFO "Process command path: %s\n", p);

    return 0;
}

static void __exit process_details_cleanup(void)
{
}

module_init(process_details_init);      
module_exit(process_details_cleanup);
