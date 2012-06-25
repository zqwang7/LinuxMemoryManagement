#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <asm/page.h>
#include <asm/pgtable.h>

MODULE_AUTHOR("Zhongqiu Wang");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A module to get all the physical address...");

static unsigned long v2p(struct mm_struct *mm, unsigned long addr)
{
	unsigned long ret = 1;
	pgd_t *pgd;
        pud_t *pud;
        pmd_t *pmd;
        pte_t *pte;
        spinlock_t *ptl;
        pgd = pgd_offset(mm, addr);
        if (pgd_none(*pgd) || unlikely(pgd_bad(*pgd)))
	{
        	goto out;
//		printk("pgd not ok\n");
        }
        pud = pud_offset(pgd, addr);
        if (pud_none(*pud) || unlikely(pud_bad(*pud)))
	{
//j		printk("pud not ok\n");
                goto out;
	}
        pmd = pmd_offset(pud, addr);
        if (pmd_none(*pmd) || unlikely(pmd_bad(*pmd)))
	{
//		printk("pmd not ok\n");
               	goto out;
       	}
        pte = pte_offset_map_lock(mm, pmd, addr, &ptl);
        if (!pte)
	{
//		printk("pte not ok\n");
                goto out;
	}
        if (!pte_present(*pte))
	{
//		printk("pte_present not ok\n");
            	goto unlock;
	}
	ret = (pte_val(*pte) & PAGE_MASK) | (addr & ~PAGE_MASK); 
unlock:
        pte_unmap_unlock(pte, ptl);
out:
        return ret;
}

int flag = 0;
int pids[2048];
int number_of_process = 0;
int count = 0;
int count2 = 0;

static void *ps_seq_start(struct seq_file *s,loff_t *pos)
{
	printk("start pos is %d\n",*pos);
	if(*pos == 0)
	{
		printk("************Let's Begin**************\n");
		struct task_struct *temp_task = &init_task;
		number_of_process = 0;
		do
		{
			printk("%d\n",temp_task->pid);
			pids[number_of_process++] = temp_task->pid;	
		}while((temp_task = next_task(temp_task)) != &init_task);
		printk("number of process %d\n",number_of_process);
	}
	if(*pos > number_of_process)
	{
		return NULL;
	}
	else
	{
		struct task_struct *p;
		struct pid *k;
		do
		{
			if(*pos >= number_of_process)
				return NULL;
			k = find_vpid(pids[(*pos)++]);
			p = pid_task(k,PIDTYPE_PID);
			if(p == NULL)
			{
				printk("task->pid = %d not found\n",pids[*pos - 1]);
			}
		}while(p == NULL);
		return p;
	}
}

static void *ps_seq_next(struct seq_file *s,void *v,loff_t *pos)
{
	printk("next\n");
	//struct task_struct *task=(struct task_struct *)v;
	struct task_struct *p;
	struct pid *k;
	if(*pos > number_of_process)
	{
		return NULL;
	}
	do
	{
		if(*pos >= number_of_process)
			return NULL;
		k = find_vpid(pids[(*pos)++]);
       		p = pid_task(k,PIDTYPE_PID);
		if(p == NULL)
	        {
	             printk("task->pid = %d not found\n",pids[*pos - 1]);
	        }

	}while(p == NULL);
	return p;
}

static void ps_seq_stop(struct seq_file *s,void *v)
{
	printk("stop\n");
}

static int ps_seq_show(struct seq_file *s,void *v)
{
	rwlock_t lock = RW_LOCK_UNLOCKED;
	struct task_struct *task=(struct task_struct *)v;
	struct vm_area_struct * temp = NULL; 		
	printk("show\n");
	if(task == NULL)
	{
		printk("task is null\n");
		return 0;
	}
	if(!task->mm)
	{
		printk("task->pid = %d don't have mm\n",task->pid);
		return 0;
	}
	else
	{
		temp = task->mm->mmap;
	}
	read_lock(&lock);
	seq_printf(s,"%d %d %s#",task->pid,task->parent->pid,task->comm);
	printk("task->pid = %d OOOOOOOO\n",task->pid);
	while(temp)
	{
		unsigned long first = ((temp->vm_start) >> 12),second = ((temp->vm_end) >> 12),count,num;
		for(count = first;count < second;count++)
		{
			//获得对应的祯	
			num = v2p(task->mm,count << 12);
			if(num == 1)
				continue;
			num = (num >> 12);
			seq_printf(s,"%lu,",num);
		}
		temp = temp->vm_next;
	}
	seq_printf(s,"|");
	read_unlock(&lock);
	return 0;
}

static struct seq_operations ps_seq_ops = {
	.start = ps_seq_start,
	.next = ps_seq_next,
	.stop = ps_seq_stop,
	.show = ps_seq_show
};

static int ps_open(struct inode *inode,struct file *file)
{
	struct seq_file *p = file->private_data;/*p为seq_file结构实例*/  
	if (!p)
	{  
		p = kmalloc(sizeof(*p) , GFP_KERNEL);  
		if (!p)  
			return -ENOMEM;  
		file->private_data = p;/*放到file的private_data中*/  
	}  
	memset(p, 0, sizeof(*p));  
	mutex_init(&p->lock);  
	p->op = &ps_seq_ops;/*设置seq_file的operation为op*/
	file->f_version = 0;  
	file->f_mode &= ~FMODE_PWRITE;  
	return 0;
}

static struct file_operations ps_file_ops = {
	.owner = THIS_MODULE,
	.open = ps_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release= seq_release
};
static int __init ps_init(void)
{
	struct proc_dir_entry *entry;
	entry = create_proc_entry("mymemory",0,NULL);
	if(entry)
	entry->proc_fops = &ps_file_ops;
	return 0;
}

static void __exit ps_exit(void)
{
	remove_proc_entry("mymemory",NULL);
}

module_init(ps_init);
module_exit(ps_exit);
