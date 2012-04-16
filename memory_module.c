#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Get the phiscal adress of processes");
MODULE_AUTHOR("Zhongqiu Wang");

static struct info * info_pointer;
static struct proc_dir_entry * proc_entry;
//struct task_struct * task = &init_task;这个错误很诡异,

int memory_read( char *page, char **start, off_t off,int count, int *eof, void *data )
{
	int len , i = 0;
	page[i] = '\0';
//	printk(KERN_INFO"Get all the processes phiscal adress\n");
	struct task_struct *task = current;
	do {
//	printk(KERN_INFO"current*** %s [%d]",current->comm,current->pid);
		printk( KERN_INFO "*** %s [%d]\n",task->comm, task->pid);
		for(;page[i] != '\0';i++)
		{
		}
		//在中间输入frame位置
		sprintf(&page[i],"|%d %s",task->pid,task->comm);	
           } while ( (task = next_task(task)) != current );
	for(len = 0;page[len] != '\0';len++){}
	return len;
}
int proc_init( void )
{
	printk(KERN_INFO"proc_init\n");
		proc_entry = create_proc_entry("memory",0644,NULL);
		if(proc_entry == NULL)
		{
			vfree(proc_entry);
			printk(KERN_INFO"memory: Coundn't create proc entry\n");
			return -ENOMEM;
		}
		else
		{
			proc_entry->read_proc = memory_read;
			//proc_entry->write_proc = memory_write;
			//proc_entry->owner = THIS_MODULE;
			printk(KERN_INFO"memory: Module loaded.\n");
			return 0;
		}
}

void proc_exit( void )
{
	//remove_proc_entry("memory",&proc_root);
	remove_proc_entry("memory",NULL);
	vfree(info_pointer);
	printk(KERN_INFO"memory: Module unloaded.\n");
}

module_init(proc_init);
module_exit(proc_exit);
