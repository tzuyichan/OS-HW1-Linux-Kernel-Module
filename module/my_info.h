#ifndef MY_INFO_H
#define MY_INFO_H


#include <linux/init.h>				// for the macros
#include <linux/module.h>  			// for all modules
#include <linux/kernel.h>  			// for pr_info()
#include <linux/proc_fs.h>			// for proc files
#include <generated/utsrelease.h>	// for version
#include <linux/seq_file.h>
#include <linux/processor.h>		// for cpu info
#include <linux/mm.h>				// for memory info
#include <linux/mmzone.h>			// for enums in memory info
#include <linux/vmstat.h>			// for global_node_page_state in memory info
#include <linux/time.h>
#include <linux/kernel_stat.h>

#define PROC_NAME "my_info"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("tzuyichan");
MODULE_DESCRIPTION("Create proc file to read system info.");

static int proc_init(void);
static void proc_exit(void);

static int my_open(struct inode *inode, struct file *file);

static void *my_seq_start(struct seq_file *s, loff_t *pos);
static void *my_seq_next(struct seq_file *s, void *v, loff_t *pos);
static void my_seq_stop(struct seq_file *s, void *v);
static int my_seq_show(struct seq_file *s, void *v);

static void my_version_show(struct seq_file *s);
static void my_cpu_show(struct seq_file *s, void *v);
static void my_mem_show(struct seq_file *s);
static void my_time_show(struct seq_file *s);

static const struct file_operations my_file_ops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = seq_release
};

static struct seq_operations my_seq_ops =
{
    .start = my_seq_start,
    .next = my_seq_next,
    .stop = my_seq_stop,
    .show = my_seq_show
};

static unsigned int cpu_printed;


#endif
