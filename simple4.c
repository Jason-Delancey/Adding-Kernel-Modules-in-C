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

	struct task_struct *task, *child;
	struct list_head *list;
	struct prinfo *buff;
	printk(KERN_INFO "Starting Module\n");
	buff = kmalloc(sizeof(*buff), GFP_KERNEL);
	buff->pid = 0;
	buff->state = 0;
	while(i < 16){
		buff->comm[i] = 0; i++;
	}

	/* traverse the linked list of tasks. DFS starting from init */
	task = &init_task;
	list_for_each(list, &task->children){
		/* points to next child in the list */
		child = list_entry(list, struct task_struct, sibling);

		buff->pid = child->pid;
		buff->state = child->state;
		i = 0;
		while(i < 16){
			c = child->comm[i];
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

