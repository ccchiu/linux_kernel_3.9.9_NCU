#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/export.h>
#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/aio.h>
#include <linux/capability.h>
#include <linux/kernel_stat.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <linux/mman.h>
#include <linux/pagemap.h>
#include <linux/file.h>
#include <linux/uio.h>
#include <linux/hash.h>
#include <linux/writeback.h>
#include <linux/backing-dev.h>
#include <linux/pagevec.h>
#include <linux/blkdev.h>
#include <linux/security.h>
#include <linux/cpuset.h>
#include <linux/hardirq.h> /* for BUG_ON(!in_atomic()) only */
#include <linux/memcontrol.h>
#include <linux/cleancache.h>

#include <linux/module.h>    
#include <linux/proc_fs.h>    
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <asm/desc.h>
#include <asm/pgtable.h>
#include <asm/desc.h>




static int get_reg_info()
{

	unsigned long pgd_paddr,cr4;
        pgd_paddr = read_cr3();
	cr4=read_cr4();
	#if 0
	len += sprintf( buf+len, "cr4=%08X  ", cr4 );
	len += sprintf( buf+len, "PSE=%X  ", (cr4>>4)&1 );
	len += sprintf( buf+len, "PAE=%X  ", (cr4>>5)&1 );
	len += sprintf( buf+len, "\n" );
	len += sprintf( buf+len, "cr3=%08X cr0=%08X\n",cr3,cr0);
	len += sprintf( buf+len, "pgd:0x%08X\n",(unsigned int)mm->pgd);
	len += sprintf( buf+len, "gdtr address:%lX, limit:%X\n", gdtr.address,gdtr.limit);
	#endif 
	printk(KERN_INFO "cr3=%08X \n",pgd_paddr);
	printk(KERN_INFO "cr4=%08X  ", cr4 );
	printk(KERN_INFO "PAE=%X  ", (cr4>>5)&1);
	printk(KERN_INFO "PSE=%X  ", (cr4>>4)&1);
	
	
	return	1;
}



#define NCU_START_PAGE_ADDR (0)
#define NCU_END_PAGE_ADDR   (1023)
#define NCU_RANGE  (NCU_END_PAGE_ADDR-NCU_START_PAGE_ADDR)+1
/* Page Size FLAG in page entry */
#define PAGE_SIZE_FLAG (7)
asmlinkage int sys_csie_ncu_survey_TT(void){
	printk(KERN_INFO "By jerry@NCU %s %s %s\n",__func__ , __DATE__,__TIME__);

	get_reg_info();

	pgd_t *start =get_current()->mm->pgd;
#if 1 
	int i=0;
	pgdval_t pgd_entry[NCU_RANGE];
	for(i=0;i< NCU_RANGE ;i++)
		pgd_entry[i]=0;

	for(i =  NCU_START_PAGE_ADDR;i <= NCU_END_PAGE_ADDR;i++){
		//pgdval_t e = pgd_val(*(start+i));
		pgd_entry[i]=pgd_val(*(start+i));
		printk(KERN_INFO "PAGE Entry[%d]=%lu(%lx) PS=%d \n",i,
			        (unsigned long)pgd_entry[i],
				(unsigned long)pgd_entry[i],
				(pgd_entry[i]>>PAGE_SIZE_FLAG) & 0x00000001);
		//pgd_entry[i]=(int)(((unsigned long)e)&0x00000001);
		//printk(KERN_INFO "PAGE Entry[%d]=%lu  page_entry_value=%d\n",i,(unsigned long)e,pgd_entry[i]);
	}
	//	copy_to_user(result,testarr,1024*sizeof(int));
#endif
	return 1;
}


