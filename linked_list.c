/* This function is used to:
 * Create a birthday struct
 * Create a linked list to traverse
 * Print out text for each element in the linked list
 * Delete the linked list after traversal
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h> /* defines doubly linked list */
#include <linux/types.h> /* defines list_head */
#include <linux/slab.h> /* defines kmalloc */

struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
};

/* This function is called when the module is loaded. */
int simple_init(void)
{
	static LIST_HEAD(birthday_list); /* defines and initializes birthday_list */
	struct birthday *ptr, *next;
	struct birthday *me, *you; /* create and initialize an instance of birthday */

	printk(KERN_INFO "Loading Module\n");
	me = kmalloc(sizeof(*me), GFP_KERNEL);
	me->day = 2;
	me->month = 8;
	me->year = 1995;
	INIT_LIST_HEAD(&me->list); /*initialize the list head member in struct birthday */
	you = kmalloc(sizeof(*you), GFP_KERNEL);
	you->day = 3;
	you->month = 9;
	you->year = 1996;
	INIT_LIST_HEAD(&you->list);

	list_add_tail(&me->list, &birthday_list); /* add the struct birthday object to the list tail */
	list_add_tail(&you->list, &birthday_list);

	/* traverse the linked list */
	list_for_each_entry(ptr, &birthday_list, list){
		printk(KERN_INFO "List entry\n");
	}

	/* delete the linked list */
	list_for_each_entry_safe(ptr, next, &birthday_list, list){
		list_del(&ptr->list);
		kfree(ptr);
	}

	//printk(KERN_INFO "Loading Module\n");

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

