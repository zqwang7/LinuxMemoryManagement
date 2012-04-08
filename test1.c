#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Get the phiscal adress of processes");
MODULE_AUTHOR("Zhongqiu Wang");

#define MAX_PROCESS_NUMBER 10000;//最大进程数目，先设为数组，待定
#define MAX_FRAME_NUMBER 10000;//单个进程最大帧数，设为数组，待定
struct info
{
	//不用info也行，直接存到字符串里面，返回字符串，让用户解析
	//用于保存进程的id和祯号，frame可改为链表
	int pid;
	int frame[MAX_FRAME_NUMBER];
};
static info * info_pointer;
static struct proc_dir_entry * proc_entry;
int fortune_read( char *page, char **start, off_t off,int count, int *eof, void *data )
{
	int len;
	//将info组装成一个字符串，len为字符串的长度,将page返回给用户，用户解析该字符串并绘图
	return len;
}
ssize_t fortune_write( struct file *filp, const char __user *buff,unsigned long len, void *data )
{
	char * command;
	//获取用户传进来的命令
	if (command = copy_from_user( &cookie_pot[cookie_index], buff, len ))
	{
		return -EFAULT;
	}
	//解析命令，比如是某个进程还是部分进程还是全部进程
	//格式为“id1 id2 id3...”或者“all"
	if(traverseall)
	{
		/* Set up the anchor point */
		struct task_struct *task = &init_task;
		/* Walk through the task list, until we hit the init_task again */
		do {
			printk( KERN_INFO "*** %s [%d]\n",task->comm, task->pid);
			//下一步调用函数获得物理地址，存入到info中,也可不用结构体，直接构造返回字符串



		} while ( (task = next_task(task)) != &init_task );
	}
	else
	{
		//获得指定进程的id和物理地址，存入info中
	}
}
int proc_init( void )
{
	info_pointer = (info *)vmalloc(MAX_PROCESS_NUMBER);
	if(!info_pointer)
	{
		return -ENOMEM;
	}
	else
	{
		memset(cookie_pot,0,sizeof(info) * MAX_PROCESS_NUMBER);
		proc_entry = create_proc_entry("memory",0644,NULL);
		if(proc_entry == NULL)
		{
			return -ENOMEM;
			vfree(proc_entry);
			printk(KERN_INFO"memory: Coundn't create proc entry\n");
		}
		else
		{
			proc_entry->read_proc = memory_read;
			proc_entry->write_proc = memory_write;
			//proc_entry->owner = THIS_MODULE;
			printk(KERN_INFO"memory: Module loaded.\n");
			return 0;
		}
	}

	return 0;
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
