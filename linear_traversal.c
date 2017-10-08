/* This program performs a linear traversal for each process in the kernel
 * In creates a prinfo buffer to store data from each process
 * and prints the data to the kernel log
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h> /* defines doubly linked list */
#include <linux/types.h> /* defines list_head */
#include <linux/slab.h> /* defines kmalloc */
#include <linux/sched.h> /* defines task_structs */


struct prinfo {
	pid_t parent_pid;
	pid_t pid;
	pid_t first_child_pid;
	pid_t next_sibling_pid;
	long state;
	long uid;
	char comm[64];
};

/* This function is called when the module is loaded. */
int simple_init(void)
{
	int i = 0;
	char c = 0;

	struct task_struct *task;
	struct prinfo *buff;
	printk(KERN_INFO "Loading Module\n");
	buff = kmalloc(sizeof(*buff), GFP_KERNEL);
	buff->pid = 0;
	buff->state = 0;
	while(i < 16){
		buff->comm[i] = 0; i++;
	}

	/* traverse the linked list of tasks. each iteration points to the next task */
	for_each_process(task){
		buff->pid = task->pid;
		buff->state = task->state;
		i = 0;
		while(i < 16){
			c = task->comm[i];
			buff->comm[i] = c;
			i++;
		}
		printk(KERN_INFO "Pid: %d\n", buff->pid);
		printk(KERN_INFO "State: %ld\n", buff->state);
		printk(KERN_INFO "Process name: %s\n", buff->comm);
	}

       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

