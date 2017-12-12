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




#define NCU_START_PAGE_ADDR (992)
#define NCU_END_PAGE_ADDR   (1023)
#define NCU_RANGE  (NCU_END_PAGE_ADDR-NCU_START_PAGE_ADDR)
asmlinkage int sys_csie_ncu_survey_TT(void){
	printk(KERN_INFO "By jerry@NCU %s %s %s\n",__func__ , __DATE__,__TIME__);


	unsigned long cr0 = 0L, cr2 = 0L, cr3 = 0L, cr4 = 0L;


	pgd_t *start =get_current()->mm->pgd;
#if 1 
	int i=0;
	int pgd_entry[NCU_RANGE];
	for(i=0;i< NCU_RANGE ;i++)
		pgd_entry[i]=0;

	//	printk(KERN_EMERG "PTRS_PER_PGD:%d\n",PTRS_PER_PGD);
	for(i =  NCU_START_PAGE_ADDR;i <= NCU_END_PAGE_ADDR;i++){
		pgdval_t e = pgd_val(*(start+i));
		pgd_entry[i]=(int)(((unsigned long)e)&0x00000001);
		printk(KERN_INFO "PAGE Entry[%d]=%lu  page_entry_value=%d\n",i,(unsigned long)e,pgd_entry[i]);
	}
	//	copy_to_user(result,testarr,1024*sizeof(int));
#endif
	return 1;
}

