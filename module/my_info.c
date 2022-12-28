#include "my_info.h"

static int proc_init(void)
{
    umode_t file_permission = 0644;  // user r/w others r only
    proc_create(PROC_NAME, file_permission, NULL, &my_file_ops);

    cpu_printed = 0;

    return 0;
}

static void proc_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
}

static int my_open(struct inode *inode, struct file *file)
{
    return seq_open(file, &my_seq_ops);
}

static void *my_seq_start(struct seq_file *s, loff_t *pos)
{
    if ((*pos) < nr_cpu_ids)
        return &cpu_data(*pos);

    cpu_printed = 0;
    return NULL;
}

static void *my_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
    (*pos)++;
    return my_seq_start(s, pos);
}

static void my_seq_stop(struct seq_file *s, void *v)
{
}

static int my_seq_show(struct seq_file *s, void *v)
{
    if (!cpu_printed)
    {
        seq_puts(s, "\n=============Version=============\n");
        my_version_show(s);

        seq_puts(s, "\n=============CPU=============\n");
    }

    my_cpu_show(s, v);
    seq_puts(s, "\n");

    if (cpu_printed == nr_cpu_ids)
    {
        seq_puts(s, "=============Memory=============\n");
        my_mem_show(s);

        seq_puts(s, "\n=============Time=============\n");
        my_time_show(s);
        seq_puts(s, "\n");
    }

    return 0;
}

static void my_version_show(struct seq_file *s)
{
    seq_printf(s, "Linux version %s\n", UTS_RELEASE);
}

static void my_cpu_show(struct seq_file *s, void *v)
{
    struct cpuinfo_x86 *c = v;

    seq_printf(s, "processor\t: %u\n", c->cpu_index);
    seq_printf(s, "model name\t: %s\n", c->x86_model_id);
    seq_printf(s, "physical id\t: %d\n", c->phys_proc_id);
    seq_printf(s, "core id\t\t: %d\n", c->cpu_core_id);
    seq_printf(s, "cpu cores\t: %d\n", c->booted_cores);
    seq_printf(s, "cache size\t: %d KB\n", c->x86_cache_size);
    seq_printf(s, "clflush size\t: %u\n", c->x86_clflush_size);
    seq_printf(s, "cache_alignment\t: %d\n", c->x86_cache_alignment);
    seq_printf(s, "address sizes\t: %u bits physical, %u bits virtual\n", c->x86_phys_bits, c->x86_virt_bits);

    cpu_printed += 1;
}

static void my_mem_show(struct seq_file *s)
{
    struct sysinfo i;
    si_meminfo(&i);
    unsigned long pages[NR_LRU_LISTS];
    int lru;

    for (lru = LRU_BASE; lru < NR_LRU_LISTS; ++lru)
        pages[lru] = global_node_page_state(NR_LRU_BASE + lru);

    seq_printf(s, "MemTotal\t: %lu kB\n", i.totalram);
    seq_printf(s, "MemFree\t\t: %lu kB\n", i.freeram);
    seq_printf(s, "Buffers\t\t: %lu kB\n", i.bufferram);
    seq_printf(s, "Active\t\t: %lu kB\n", pages[LRU_ACTIVE_ANON] + pages[LRU_ACTIVE_FILE]);
    seq_printf(s, "Inactive\t: %lu kB\n", pages[LRU_INACTIVE_ANON] + pages[LRU_INACTIVE_FILE]);
    seq_printf(s, "Shmem\t\t: %lu kB\n", i.sharedram);
    seq_printf(s, "Dirty\t\t: %lu kB\n", global_node_page_state(NR_FILE_DIRTY));
    seq_printf(s, "Writeback\t: %lu kB\n", global_node_page_state(NR_WRITEBACK));
    seq_printf(s, "KernelStack\t: %lu kB\n", global_node_page_state(NR_KERNEL_STACK_KB));
    seq_printf(s, "PageTables\t: %lu kB\n", global_node_page_state(NR_PAGETABLE));
}

static void my_time_show(struct seq_file *s)
{
    struct timespec uptime;
    struct timespec idle;
    u64 nsec;
    u32 rem;
    int j;

    nsec = 0;
    for_each_possible_cpu(j)
    nsec += (__force u64) kcpustat_cpu(j).cpustat[CPUTIME_IDLE];

    get_monotonic_boottime(&uptime);
    idle.tv_sec = div_u64_rem(nsec, NSEC_PER_SEC, &rem);
    idle.tv_nsec = rem;

    seq_printf(s, "Uptime\t\t: %lu.%02lu (s)\nIdletime\t: %lu.%02lu (s)\n",
               (unsigned long) uptime.tv_sec,
               (uptime.tv_nsec / (NSEC_PER_SEC / 100)),
               (unsigned long) idle.tv_sec,
               (idle.tv_nsec / (NSEC_PER_SEC / 100)));
}

module_init(proc_init);
module_exit(proc_exit);
