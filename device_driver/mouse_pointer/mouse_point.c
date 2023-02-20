#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>
#include <linux/input.h>
MODULE_LICENSE("GPL");
struct input_poll_dev * dev;
int init_module(void)
{
	printk(KERN_INFO "Mouse Point device driver loaded \n");
    dev=input_allocate_device();
    if(!dev)
    {
        printk(KERN_INFO "input allocate fail\n");
        return -1;
    }
    dev->
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
}