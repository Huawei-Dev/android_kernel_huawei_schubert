/*
 * Huawei double free check debugfs interface
 *
 * Copyright (c) 2017 Huawei.
 *
 * Authors:
 * chenli <chenli45@huawei.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/seq_file.h>
#include <linux/err.h>
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/ctype.h>
#include <linux/debugfs.h>
#include <linux/slub_def.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/mm_types.h>

#include <asm/memory.h>
#include <asm/module.h>
#include <asm/sections.h>


static struct dentry *slub_double_free_root;

#define SLUB_DEBUG_ENTRY(name) \
	static int slub_##name##_open(struct inode *inode, struct file *file) \
{ \
	    return single_open(file, slub_##name##_show, inode->i_private); \
} \
\
static const struct file_operations slub_##name##_fops = { \
	    .owner = THIS_MODULE, \
	    .open = slub_##name##_open, \
	    .read = seq_read, \
	    .llseek = seq_lseek, \
	    .release = single_release, \
}

static int __init hwslub_double_free_debugfs_init(void)
{
    /*create sys/kernel/debug/hwslub_double_free for debugfs*/
	slub_double_free_root = debugfs_create_dir("hwslub_double_free", NULL);
	if (!slub_double_free_root)
		return -ENOMEM;
	return 0;
}

static void __exit hwslub_double_free_debugfs_exit(void)
{
	debugfs_remove(slub_double_free_root);
}

module_init(hwslub_double_free_debugfs_init);
module_exit(hwslub_double_free_debugfs_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Huawei double free debugfs");
