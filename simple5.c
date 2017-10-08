#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h> /* defines doubly linked list */
#include <linux/types.h> /* defines list_head */
#include <linux/slab.h> /* defines kmalloc */
#include <linux/sched.h> /* defines task_structs */
#include <linux/delay.h> /* defines delay function */

struct prinfo {
	pid_t parent_pid;
	pid_t pid;
	pid_t first_child_pid;
	pid_t next_sibling_pid;
	long state;
	long uid;
	char comm[64];
};

void search(struct task_struct *task, struct prinfo *buff);
void get_info(struct task_struct *task, struct prinfo *buff);

/* This function is called when the module is loaded. */
int simple_init(void){
	struct prinfo *buff;
	struct task_struct *task;
	printk(KERN_INFO "Starting Module\n");

	buff = kmalloc(sizeof(*buff), GFP_KERNEL);
	task = &init_task;
	search(task, buff);

	return 0;
}

void search(struct task_struct *task, struct prinfo *buff, int *counter){
		
	struct task_struct *child;
	struct list_head *list;

	/* traverse the linked list of tasks, recursive */
	list_for_each(list, &task->children){
		/* points to next child in the list */
		child = list_entry(list, struct task_struct, sibling);
		get_info(child, buff);
		search(child, buff);
	}
}

void get_info(struct task_struct *task, struct prinfo *buff){
	int i = 0;
	char c = 0;	
	buff->pid = 0; /* zero initialize buffer */
	buff->state = 0;
	while(i < 16){
		buff->comm[i] = 0; i++;
	}
	
	buff->pid = task->pid; /* get pid, state, name */
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

