/*procfs.c - create a "file" in /proc */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>

struct proc_dir_entry *Our_Proc_File;

ssize_t procfile_read(char *buffer,char **buffer_location,off_t offset,int buffer_length, int *eof, void *data)
{
	printk(KERN_INFO "inside /proc/test : procfile_read\n");
	int len = 0;
	static int count = 1;
	if(offset > 0)
	{
		printk(KERN_INFO "offset %d : /proc/test : procfile_read, \wrote %d Bytes\n",(int)(offset),len);
		*eof = 1;
		return len;
	}
	len = sprintf(buffer,
			"For the %d%s time, go away!\n",count,
			(count%100 > 10 && count %100<14)?"th":
			(count%10 == 1)?"st":
			(count%10 == 2)?"nd":
			(count%10 == 3)?"rd":"th");
	count++;
	printk(KERN_INFO "leaving /proc/test : procfile_read, wrote %d Bytes\n", len);
	return len;
}

static int __init proc_init(void)
{
	int rv = 0;
	Our_Proc_File = create_proc_entry("test",0644,NULL);
	Our_Proc_File -> read_proc = procfile_read;
	//Our_Proc_File -> owner = THIS_MODULE;
	Our_Proc_File -> mode = S_IFREG | S_IRUGO;
	Our_Proc_File -> uid = 0;
	Our_Proc_File -> gid = 0;
	Our_Proc_File -> size = 37;
	
	printk(KERN_INFO "Trying to create /proc/test:\n");
	
	if(Our_Proc_File == NULL)
	{
		rv = -ENOMEM;
		//remove_proc_entry("test",&proc_root);
		printk(KERN_INFO "Error : Could not initialize /proc/test\n");
	}
	else
	{
		printk(KERN_INFO "Success!\n");
	}
	return rv;
}

static void __exit proc_exit(void)
{
	//remove_proc_entry("test", &proc_root);	
	printk(KERN_INFO "/proc/test removed \n");
}

module_init(proc_init);
module_exit(proc_exit);












