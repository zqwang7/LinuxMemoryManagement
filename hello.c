#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");

static int __init MemMana_init(void)
{
	printk(KERN_ALERT "Hello,world!\n");
	return 0;
}

static void __exit MemMana_exit(void)
{
	printk(KERN_ALERT "Seeya!\n");
}

module_init(MemMana_init);
module_exit(MemMana_exit);
