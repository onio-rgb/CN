#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <linux/vmalloc.h>
MODULE_LICENSE("GPL");
static struct input_dev *dev;
static dev_t chdev_t;
static struct cdev *cdev;
struct class *mychardev_class;
struct coord
{
    int step_count;
    int rel_x;
    int rel_y;
};
struct coord *buffk;
void move_mouse_pointer(struct coord *move_by)
{
    int increment_x = move_by->rel_x / move_by->step_count;
    int increment_y = move_by->rel_y / move_by->step_count;
    for (int i = 0; i < move_by->step_count; i++)
    {
        input_report_rel(dev, REL_X, increment_x);
        input_sync(dev);
        input_report_rel(dev, REL_Y, increment_y);
        input_sync(dev);
        msleep(10);
    }
}
static int opendev(struct inode *inode, struct file *filep)
{
    printk(KERN_INFO "Device mouse_point opened succesfully\n");
    buffk = vmalloc(sizeof(struct coord));
    return 0;
}
static ssize_t write(struct file *filep, const char __user *buf, size_t count, loff_t *f_pos)
{
    copy_from_user(buffk, buf, count);
    printk(KERN_INFO "Read coord from user %d , %d\n", buffk->rel_x, buffk->rel_y);
    move_mouse_pointer(buffk);
    return count;
}
static ssize_t read(struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
    count = sizeof("Your mouse was moved by the driver succesfully\n");
    copy_to_user(buf, "Your mouse was moved by the driver succesfully\n", count);
    return count;
}
static int release(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "Freeing the buffer device\n");
    vfree(buffk);
    return 0;
}
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = opendev,
    .release = release,
    .read = read,
    .write = write};
int init_module(void)
{
    // initialize char device driver
    mychardev_class = vmalloc(sizeof(struct class));
    cdev = vmalloc(sizeof(struct cdev));

    if (alloc_chrdev_region(&chdev_t, 0, 1, "mouse_point") != 0)
    {
        printk(KERN_ERR "alloc_chrdev_region error\n");
        return 1;
    }
    mychardev_class = class_create(THIS_MODULE, "mouse_point");
    cdev_init(cdev, &fops);
    cdev->owner = THIS_MODULE;
    cdev_add(cdev, chdev_t, 1);
    device_create(mychardev_class, NULL, chdev_t, NULL, "mouse_point0");
    printk(KERN_INFO "Mouse Point device driver loaded \n");

    // initialize input device driver
    dev = input_allocate_device();
    if (!dev)
    {
        printk(KERN_INFO "input allocate fail\n");
        return -1;
    }
    dev->name = "test_pointer";
    dev->phys = "usb-0000:05:00.3-2/input2:1";
    dev->id.vendor = 0x046d;
    dev->id.product = 0xc53f; // dont really need this here because not polling the USB_BUS for raw bits
    set_bit(EV_REL, dev->evbit);
    set_bit(EV_KEY, dev->evbit);
    set_bit(BTN_LEFT, dev->keybit); // BTN_LEFT imp for Xorg to register this device as a mouse.
    set_bit(BTN_RIGHT, dev->keybit);// BTN_RIGHT imp for Xorg to register this device as a mouse.
    set_bit(REL_X, dev->relbit);
    set_bit(REL_Y, dev->relbit);
    int error = input_register_device(dev);
    if (error > 0)
    {
        printk(KERN_ERR "input register device fail\n");
        return 1;
    }
    return 0;
}

void cleanup_module(void)
{
    input_unregister_device(dev);
    device_destroy(mychardev_class, chdev_t);
    class_unregister(mychardev_class);
    class_destroy(mychardev_class);
    unregister_chrdev_region(chdev_t, 1);
    printk(KERN_INFO "Unloading mouse point device driver .\n");
}