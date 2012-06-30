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

MODULE_LICENSE("GPL");

static unsigned long transform1(unsigned vaddr)
{

unsigned long real_addr;

struct task_struct *cur_task = get_current();//获取当前进程控制块

struct mm_struct *mm = cur_task ->  mm;//进程虚拟空间

pgd_t *pgd;//描述页全局目录项

pmd_t *pmd;//描述页中间项

pte_t *pte;//页表项



pgd = pgd_offset(mm, addr);//找出所在目录

if (pgd_none(*pgd)){

        goto out;

}

pmd = pmd_offset(pgd, addr);//找出所在中间项



if (pmd_none(*pmd)){

    goto out;

}

pte = pte_offset(pmd, addr);//找出所在页面





if (pte_none(*pte)) {

    goto out;

}



//假设每页4KB

real_addr = vaddr & 0x00003fff; //取出页面偏移量

real_addr += pte;//内核空间访问的地址

real_addr -= PAGE_OFFSET;//真正物理地址()

return real_addr;



out:

printk("没有内存映射",real_addr);

}

unsigned long base;
__asm
{
    MOV   EAX,   CR3
    MOV   base,   EAX
}

static unsigned long transform2(unsigned long base,unsigned long vaddr)
{
unsigned long VAddress = (unsigned long)vaddr;

unsigned long PDE,PTE,PAddress;

PDE = Base[VAddress>>22];

if((PDE&1)==0)

return 0;

unsigned long PageFlage = PDE&0x00000080;

if (PageFlage != 0)

{

//PS位不为零，采用4MB分页方式

PAddress = (PDE&0xffc00000) + (VAddress&0x003fffff);

}

else

{

//PS位为零，采用4kb分页方式

PDE = (unsigned long)MapViewOfFile(hmp,4,0,PDE&0xfffff000,0x1000);//将一个文件映射对象映射到当前应用程序的地址空间

PTE = ((unsigned long)PDE)[(VAddress&0x003ff000)>>12];

if((PTE&1) == 0)

return 0;

PAddress = (PTE&0xfffff000)+(VAddress&0x00000fff);

UnmapViewOfFile((void)PDE);//在当前应用程序的内存地址空间解除对一个文件映射对象的映射

}

return (unsigned long)PAddress;
}

static int __init trans_init(void)
{
	//printk("KERN_ALERT Hello World!\n");
	return 0;
}

static void __exit trans_exit(void)
{
	//printk(KERN_ALERT "Goodbye World!\n");
}

module_init(trans_init);
module_exit(trans_exit);
