#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x92997ed8, "_printk" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x999e8297, "vfree" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x48f9fbf7, "input_event" },
	{ 0xf9a482f9, "msleep" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xaee657ee, "__class_create" },
	{ 0x4800abdd, "cdev_init" },
	{ 0x58c36c14, "cdev_add" },
	{ 0x3818fa5c, "device_create" },
	{ 0x6e5e3851, "input_allocate_device" },
	{ 0x3a26ba6b, "input_register_device" },
	{ 0x5c65e38b, "input_unregister_device" },
	{ 0xeabef977, "device_destroy" },
	{ 0x7377b2e, "class_unregister" },
	{ 0x645620c0, "class_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x541a6db8, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "F8B445366E39C31E725F247");
