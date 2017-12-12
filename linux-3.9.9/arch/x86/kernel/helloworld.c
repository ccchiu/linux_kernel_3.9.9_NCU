#include <linux/linkage.h>
#include <linux/kernel.h>
asmlinkage int sys_helloworld(void) {
	printk(KERN_INFO "By jerry@NCU %s %s\n", __DATE__,__TIME__);
	return 1;
}

